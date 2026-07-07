/**
 * @file gf2x.c
 * @brief GF(2) polynomial multiplication mod (X^n - 1) via 256-point NTT.
 *
 * Each operand is split into 128-coefficient (BLOCK) blocks. The GF(2)
 * product of two blocks (degree <= 254) is obtained through the verified
 * OP_ntt256 / OP_cwm / OP_intt256 operators (a negacyclic 256-point NTT,
 * q = 8380417); since the product degree is < 256 the negacyclic result
 * equals the exact integer convolution. Coefficients are taken mod 2 and
 * XOR-accumulated (overlap-add) with a cyclic offset, which performs the
 * reduction mod (X^n - 1).
 *
 * The twiddle table and the scalar operator constants are precomputed
 * offline (see below), so no runtime setup buffers are needed. INTT output
 * is already the true integer convolution value because NTT_INV_N folds the
 * R^2 domain factor (CWM and the INTT final step each contribute R^{-1}).
 *
 * RAM: two persistent 256 x int32 buffers (2 KB). The twiddle table is
 *      const (.rodata / flash). Hardware acceleration: swap OP_ntt256 /
 *      OP_cwm / OP_intt256 for silicon — no other code changes needed.
 */

#include "gf2x.h"
#include <stdint.h>
#include <string.h>
#include "operator_interface.h"
#include "parameters.h"

/* ---- NTT constants ---- */
#define BLOCK       128           /* coefficients per data block      */
#define NTT_N       256           /* NTT size (2 * BLOCK)             */
#define OMEGA_COUNT 2             /* 1 fwd + 1 inv twiddle set        */

/*
 * Precomputed operator constants (offline, q = 8380417, R = 2^32,
 * zeta = 1753 a primitive 512-th root of unity):
 *   NTT_QINV   = q^{-1} mod 2^32
 *   NTT_INV_N  = 256^{-1} * R^2 mod q   (folds the R^2 fixup into INTT)
 *   NTT_TWIDDLE[0..255]   = zeta^{ brv8(k)} in Montgomery domain (forward)
 *   NTT_TWIDDLE[256..511] = zeta^{-brv8(k)} in Montgomery domain (inverse)
 * These match the OP_ntt256/OP_intt256 (Dilithium-style) butterfly layout.
 */
#define NTT_Q       8380417
#define NTT_QINV    ((int32_t)0x03802001U)   /* q^{-1} mod 2^32 */
#define NTT_INV_N   41978          /* 256^{-1} * R^2 mod q  (R=2^32) */

static const int32_t NTT_TWIDDLE[2 * 256] = {
    /* forward: zeta^{brv8(k)} in Montgomery domain */
    4193792, 25847, 5771523, 7861508, 237124, 7602457, 7504169, 466468,
    1826347, 2353451, 8021166, 6288512, 3119733, 5495562, 3111497, 2680103,
    2725464, 1024112, 7300517, 3585928, 7830929, 7260833, 2619752, 6271868,
    6262231, 4520680, 6980856, 5102745, 1757237, 8360995, 4010497, 280005,
    2706023, 95776, 3077325, 3530437, 6718724, 4788269, 5842901, 3915439,
    4519302, 5336701, 3574422, 5512770, 3539968, 8079950, 2348700, 7841118,
    6681150, 6736599, 3505694, 4558682, 3507263, 6239768, 6779997, 3699596,
    811944, 531354, 954230, 3881043, 3900724, 5823537, 2071892, 5582638,
    4450022, 6851714, 4702672, 5339162, 6927966, 3475950, 2176455, 6795196,
    7122806, 1939314, 4296819, 7380215, 5190273, 5223087, 4747489, 126922,
    3412210, 7396998, 2147896, 2715295, 5412772, 4686924, 7969390, 5903370,
    7709315, 7151892, 8357436, 7072248, 7998430, 1349076, 1852771, 6949987,
    5037034, 264944, 508951, 3097992, 44288, 7280319, 904516, 3958618,
    4656075, 8371839, 1653064, 5130689, 2389356, 8169440, 759969, 7063561,
    189548, 4827145, 3159746, 6529015, 5971092, 8202977, 1315589, 1341330,
    1285669, 6795489, 7567685, 6940675, 5361315, 4499357, 4751448, 3839961,
    2091667, 3407706, 2316500, 3817976, 5037939, 2244091, 5933984, 4817955,
    266997, 2434439, 7144689, 3513181, 4860065, 4621053, 7183191, 5187039,
    900702, 1859098, 909542, 819034, 495491, 6767243, 8337157, 7857917,
    7725090, 5257975, 2031748, 3207046, 4823422, 7855319, 7611795, 4784579,
    342297, 286988, 5942594, 4108315, 3437287, 5038140, 1735879, 203044,
    2842341, 2691481, 5790267, 1265009, 4055324, 1247620, 2486353, 1595974,
    4613401, 1250494, 2635921, 4832145, 5386378, 1869119, 1903435, 7329447,
    7047359, 1237275, 5062207, 6950192, 7929317, 1312455, 3306115, 6417775,
    7100756, 1917081, 5834105, 7005614, 1500165, 777191, 2235880, 3406031,
    7838005, 5548557, 6709241, 6533464, 5796124, 4656147, 594136, 4603424,
    6366809, 2432395, 2454455, 8215696, 1957272, 3369112, 185531, 7173032,
    5196991, 162844, 1616392, 3014001, 810149, 1652634, 4686184, 6581310,
    5341501, 3523897, 3866901, 269760, 2213111, 7404533, 1717735, 472078,
    7953734, 1723600, 6577327, 1910376, 6712985, 7276084, 8119771, 4546524,
    5441381, 6144432, 7959518, 6094090, 183443, 7403526, 1612842, 4834730,
    7826001, 3919660, 8332111, 7018208, 3937738, 1400424, 7534263, 1976782,
    /* inverse: zeta^{-brv8(k)} in Montgomery domain */
    4193792, 8354570, 518909, 2608894, 7913949, 876248, 777960, 8143293,
    5700314, 5268920, 2884855, 5260684, 2091905, 359251, 6026966, 6554070,
    8100412, 4369920, 19422, 6623180, 3277672, 1399561, 3859737, 2118186,
    2108549, 5760665, 1119584, 549488, 4794489, 1079900, 7356305, 5654953,
    2797779, 6308525, 2556880, 4479693, 4499374, 7426187, 7849063, 7568473,
    4680821, 1600420, 2140649, 4873154, 3821735, 4874723, 1643818, 1699267,
    539299, 6031717, 300467, 4840449, 2867647, 4805995, 3043716, 3861115,
    4464978, 2537516, 3592148, 1661693, 4849980, 5303092, 8284641, 5674394,
    4540456, 3628969, 3881060, 3019102, 1439742, 812732, 1584928, 7094748,
    7039087, 7064828, 177440, 2409325, 1851402, 5220671, 3553272, 8190869,
    1316856, 7620448, 210977, 5991061, 3249728, 6727353, 8578, 3724342,
    4421799, 7475901, 1100098, 8336129, 5282425, 7871466, 8115473, 3343383,
    1430430, 6527646, 7031341, 381987, 1308169, 22981, 1228525, 671102,
    2477047, 411027, 3693493, 2967645, 5665122, 6232521, 983419, 4968207,
    8253495, 3632928, 3157330, 3190144, 1000202, 4083598, 6441103, 1257611,
    1585221, 6203962, 4904467, 1452451, 3041255, 3677745, 1528703, 3930395,
    6403635, 846154, 6979993, 4442679, 1362209, 48306, 4460757, 554416,
    3545687, 6767575, 976891, 8196974, 2286327, 420899, 2235985, 2939036,
    3833893, 260646, 1104333, 1667432, 6470041, 1803090, 6656817, 426683,
    7908339, 6662682, 975884, 6167306, 8110657, 4513516, 4856520, 3038916,
    1799107, 3694233, 6727783, 7570268, 5366416, 6764025, 8217573, 3183426,
    1207385, 8194886, 5011305, 6423145, 164721, 5925962, 5948022, 2013608,
    3776993, 7786281, 3724270, 2584293, 1846953, 1671176, 2831860, 542412,
    4974386, 6144537, 7603226, 6880252, 1374803, 2546312, 6463336, 1279661,
    1962642, 5074302, 7067962, 451100, 1430225, 3318210, 7143142, 1333058,
    1050970, 6476982, 6511298, 2994039, 3548272, 5744496, 7129923, 3767016,
    6784443, 5894064, 7132797, 4325093, 7115408, 2590150, 5688936, 5538076,
    8177373, 6644538, 3342277, 4943130, 4272102, 2437823, 8093429, 8038120,
    3595838, 768622, 525098, 3556995, 5173371, 6348669, 3122442, 655327,
    522500, 43260, 1613174, 7884926, 7561383, 7470875, 6521319, 7479715,
    3193378, 1197226, 3759364, 3520352, 4867236, 1235728, 5945978, 8113420,
    3562462, 2446433, 6136326, 3342478, 4562441, 6063917, 4972711, 6288750,
};

/* ---- persistent NTT buffers (2 KB) ---- */
static int32_t ntt_buf_a[NTT_N];  /* 1024 B: dense-block / product      */
static int32_t ntt_buf_b[NTT_N];  /* 1024 B: current sparse-block NTT   */
static int     ntt_ready = 0;

/* ------------------------------------------------------------------ */
/*  one-time NTT operator initialisation                               */
/* ------------------------------------------------------------------ */
static void init_ntt(void)
{
    if (OP_ntt256_init(NTT_TWIDDLE, OMEGA_COUNT, NTT_Q,
                       NTT_QINV, NTT_INV_N, 0) == OP_SUCCESS)
        ntt_ready = 1;
}

/* ------------------------------------------------------------------ */
/*  Test whether a BLOCK-sized slice of v is all-zero (skip if so).     */
/* ------------------------------------------------------------------ */
static int block_is_zero(const uint64_t *v, int block_idx, int n)
{
    int start = block_idx * BLOCK;
    int end   = start + BLOCK;
    if (end > n) end = n;

    for (int pos = start; pos < end; pos++) {
        int w = pos >> 6;
        int b = pos & 63;
        if ((v[w] >> b) & 1ULL)
            return 0;   /* non-zero */
    }
    return 1;           /* all zero — can skip */
}

/* ------------------------------------------------------------------ */
/*  Unpack up to BLOCK bits into buf (zero-padded to NTT_N).            */
/* ------------------------------------------------------------------ */
static void unpack_block(int32_t *buf, const uint64_t *v, int block_idx, int n)
{
    int start = block_idx * BLOCK;
    int end   = start + BLOCK;
    if (end > n) end = n;

    memset(buf, 0, NTT_N * sizeof(int32_t));
    for (int pos = start; pos < end; pos++) {
        int w = pos >> 6;
        int b = pos & 63;
        buf[pos - start] = (int32_t)((v[w] >> b) & 1ULL);
    }
}

/* ------------------------------------------------------------------ */
/*  XOR a 256-coefficient block product into the output bit-vector.     */
/*  offset = starting global position; wraps modulo n (mod X^n - 1).    */
/* ------------------------------------------------------------------ */
static void xor_result(uint64_t *o, const int32_t *res, int offset, int n)
{
    for (int k = 0; k < NTT_N; k++) {
        if (!(res[k] & 1)) continue;
        int pos = offset + k;
        if (pos >= n) pos -= n;   /* single wrap suffices: offset < n,
                                     k < 256, n > 256                   */
        int w = pos >> 6;
        int b = pos & 63;
        o[w] ^= (1ULL << b);
    }
}

/* ================================================================== */
/*  Public API — carry-less multiplication mod (X^PARAM_N - 1).        */
/*  a1 is the sparse operand (HQC always passes y / r2 first).         */
/* ================================================================== */
void vect_mul(uint64_t *o, const uint64_t *a1, const uint64_t *a2)
{
    if (!ntt_ready)
        init_ntt();

    int n        = PARAM_N;
    int n_blocks = (n + BLOCK - 1) / BLOCK;

    memset(o, 0, (size_t)VEC_N_SIZE_64 * sizeof(uint64_t));

    /* Iterate non-zero blocks of the sparse operand a1 (outer) against
       all blocks of the dense operand a2 (inner). */
    for (int ib = 0; ib < n_blocks; ib++) {
        if (block_is_zero(a1, ib, n))
            continue;

        unpack_block(ntt_buf_b, a1, ib, n);
        OP_ntt256(ntt_buf_b, ntt_buf_b, 0);        /* fwd twiddle set 0 */

        for (int jb = 0; jb < n_blocks; jb++) {
            unpack_block(ntt_buf_a, a2, jb, n);
            OP_ntt256(ntt_buf_a, ntt_buf_a, 0);

            /* pointwise multiply and inverse transform, in place;
               INTT output is already the true integer convolution
               (NTT_INV_N folds the R^2 domain factor). */
            OP_cwm(ntt_buf_a, ntt_buf_a, ntt_buf_b, NTT_N, NTT_Q, 0);
            OP_intt256(ntt_buf_a, ntt_buf_a, 1);   /* inv twiddle set 1 */

            int offset = (ib + jb) * BLOCK;
            if (offset >= n) offset -= n;
            xor_result(o, ntt_buf_a, offset, n);
        }
    }
}
