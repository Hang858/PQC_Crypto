#undef LAC_LIGHT
#undef LAC128
#undef LAC256
#define LAC128
#include "polarlac_param.h"
#include "lac128_params.h"
#include "operator_interface.h"
#ifdef NTT_HARDWARE_IMPL
#include "ntt_drv.h"
#endif

#define info_nodes lac128_info_nodes
#define lambda_offset lac128_lambda_offset
#define llr_layer_vec lac128_llr_layer_vec
#define bit_layer_vec lac128_bit_layer_vec
#define polar lac128_polar
#include "polar128.h"
#include "lac128_llr.h"
#undef info_nodes
#undef lambda_offset
#undef llr_layer_vec
#undef bit_layer_vec
#undef polar

const polarlac_params_t lac128_params = {
    .level = LAC128_LEVEL,
    .name = LAC128_NAME,
    .dim_n = LAC128_DIM_N,
    .dim_n_2 = LAC128_DIM_N_2,
    .dim_n_4 = LAC128_DIM_N_4,
    .dim_n_8 = LAC128_DIM_N_8,
    .seed_len = LAC128_SEED_LEN,
    .pk_len = LAC128_PK_LEN,
    .sk_part_len = LAC128_SK_PART_LEN,
    .sk_len = LAC128_SK_LEN,
    .message_len = LAC128_MESSAGE_LEN,
    .cipher_len = LAC128_CIPHER_LEN,
    .c2_vec_num = LAC128_C2_VEC_NUM,
    .num_one = LAC128_NUM_ONE,
    .hash_type = LAC128_HASH_TYPE,
    .sample_len = LAC128_SAMPLE_LEN,
    .data_len = LAC128_DATA_LEN,
    .code_len = LAC128_CODE_LEN,
    .polar_n = LAC128_POLAR_N,
    .polar_log_n = LAC128_POLAR_LOG_N,
    .polar_k = LAC128_POLAR_K,
    .polar_ecc_bytes = LAC128_POLAR_ECC_BYTES,
    .polar_info_nodes = lac128_info_nodes,
    .polar_lambda_offset = lac128_lambda_offset,
    .polar_llr_layer_vec = lac128_llr_layer_vec,
    .polar_bit_layer_vec = lac128_bit_layer_vec,
    .llr_table = lac128_llr_table,
    .secret_key_bytes = LAC128_SECRETKEYBYTES,
    .public_key_bytes = LAC128_PUBLICKEYBYTES,
    .message_bytes = LAC128_BYTES,
    .session_bytes = LAC128_SESSIONBYTES,
    .ciphertext_bytes = LAC128_CIPHERTEXTBYTES,
    .seed_bytes = LAC128_SEEDBYTES,
    .ntt_q = LAC128_NTTQ,
    .ntt_inv_q = LAC128_NTT_INV_Q,
    .ntt_inv_n = LAC128_NTT_INV_N,
    .ntt_k = LAC128_NTT_K,
    .bitlen_q = LAC128_BITLEN_Q,
    .b_q = LAC128_B_Q,
    .inverse_n = LAC128_INVERSE_N,
    .inverse_n_beta = LAC128_INVERSE_N_BETA,
    .ntt_omega_count = LAC128_NTT_OMEGA_COUNT,
    .ntt_omega = LAC128_NTT_OMEGA,
#ifdef NTT_HARDWARE_IMPL
    .ntt_hw_config = NTTMR_COENUM_512 | NTTMR_LOOP_8 | NTTMR_SEL_32BIT | NTTMR_PARAM_D_13
#endif
};
