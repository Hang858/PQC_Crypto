#undef LAC_LIGHT
#undef LAC128
#undef LAC256
#define LAC256
#include "polarlac_param.h"
#include "lac256_params.h"
#include "operator_interface.h"
#ifdef NTT_HARDWARE_IMPL
#include "ntt_drv.h"
#endif

#define info_nodes lac256_info_nodes
#define lambda_offset lac256_lambda_offset
#define llr_layer_vec lac256_llr_layer_vec
#define bit_layer_vec lac256_bit_layer_vec
#define polar lac256_polar
#include "polar256.h"
#include "lac256_llr.h"
#undef info_nodes
#undef lambda_offset
#undef llr_layer_vec
#undef bit_layer_vec
#undef polar

const polarlac_params_t lac256_params = {
    .level = LAC256_LEVEL,
    .name = LAC256_NAME,
    .dim_n = LAC256_DIM_N,
    .dim_n_2 = LAC256_DIM_N_2,
    .dim_n_4 = LAC256_DIM_N_4,
    .dim_n_8 = LAC256_DIM_N_8,
    .seed_len = LAC256_SEED_LEN,
    .pk_len = LAC256_PK_LEN,
    .sk_part_len = LAC256_SK_PART_LEN,
    .sk_len = LAC256_SK_LEN,
    .message_len = LAC256_MESSAGE_LEN,
    .cipher_len = LAC256_CIPHER_LEN,
    .c2_vec_num = LAC256_C2_VEC_NUM,
    .num_one = LAC256_NUM_ONE,
    .hash_type = LAC256_HASH_TYPE,
    .sample_len = LAC256_SAMPLE_LEN,
    .data_len = LAC256_DATA_LEN,
    .code_len = LAC256_CODE_LEN,
    .polar_n = LAC256_POLAR_N,
    .polar_log_n = LAC256_POLAR_LOG_N,
    .polar_k = LAC256_POLAR_K,
    .polar_ecc_bytes = LAC256_POLAR_ECC_BYTES,
    .polar_info_nodes = lac256_info_nodes,
    .polar_lambda_offset = lac256_lambda_offset,
    .polar_llr_layer_vec = lac256_llr_layer_vec,
    .polar_bit_layer_vec = lac256_bit_layer_vec,
    .llr_table = lac256_llr_table,
    .secret_key_bytes = LAC256_SECRETKEYBYTES,
    .public_key_bytes = LAC256_PUBLICKEYBYTES,
    .message_bytes = LAC256_BYTES,
    .session_bytes = LAC256_SESSIONBYTES,
    .ciphertext_bytes = LAC256_CIPHERTEXTBYTES,
    .seed_bytes = LAC256_SEEDBYTES,
    .ntt_q = LAC256_NTTQ,
    .ntt_inv_q = LAC256_NTT_INV_Q,
    .ntt_inv_n = LAC256_NTT_INV_N,
    .ntt_k = LAC256_NTT_K,
    .bitlen_q = LAC256_BITLEN_Q,
    .b_q = LAC256_B_Q,
    .inverse_n = LAC256_INVERSE_N,
    .inverse_n_beta = LAC256_INVERSE_N_BETA,
    .ntt_omega_count = LAC256_NTT_OMEGA_COUNT,
    .ntt_omega = LAC256_NTT_OMEGA,
#ifdef NTT_HARDWARE_IMPL
    .ntt_hw_config = NTTMR_COENUM_1024 | NTTMR_LOOP_8 | NTTMR_SEL_32BIT | NTTMR_PARAM_D_13
#endif
};
