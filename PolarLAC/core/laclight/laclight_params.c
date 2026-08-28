#undef LAC_LIGHT
#undef LAC128
#undef LAC256
#define LAC_LIGHT
#include "polarlac_param.h"
#include "laclight_params.h"

#define info_nodes laclight_info_nodes
#define lambda_offset laclight_lambda_offset
#define llr_layer_vec laclight_llr_layer_vec
#define bit_layer_vec laclight_bit_layer_vec
#define polar laclight_polar
#include "polarlight.h"
#include "laclight_llr.h"
#undef info_nodes
#undef lambda_offset
#undef llr_layer_vec
#undef bit_layer_vec
#undef polar

const polarlac_params_t laclight_params = {
    .level = LACLIGHT_LEVEL,
    .name = LACLIGHT_NAME,
    .dim_n = LACLIGHT_DIM_N,
    .dim_n_2 = LACLIGHT_DIM_N_2,
    .dim_n_4 = LACLIGHT_DIM_N_4,
    .dim_n_8 = LACLIGHT_DIM_N_8,
    .seed_len = LACLIGHT_SEED_LEN,
    .pk_len = LACLIGHT_PK_LEN,
    .sk_part_len = LACLIGHT_SK_PART_LEN,
    .sk_len = LACLIGHT_SK_LEN,
    .message_len = LACLIGHT_MESSAGE_LEN,
    .cipher_len = LACLIGHT_CIPHER_LEN,
    .c2_vec_num = LACLIGHT_C2_VEC_NUM,
    .num_one = LACLIGHT_NUM_ONE,
    .hash_type = LACLIGHT_HASH_TYPE,
    .sample_len = LACLIGHT_SAMPLE_LEN,
    .data_len = LACLIGHT_DATA_LEN,
    .code_len = LACLIGHT_CODE_LEN,
    .polar_n = LACLIGHT_POLAR_N,
    .polar_log_n = LACLIGHT_POLAR_LOG_N,
    .polar_k = LACLIGHT_POLAR_K,
    .polar_ecc_bytes = LACLIGHT_POLAR_ECC_BYTES,
    .polar_info_nodes = laclight_info_nodes,
    .polar_lambda_offset = laclight_lambda_offset,
    .polar_llr_layer_vec = laclight_llr_layer_vec,
    .polar_bit_layer_vec = laclight_bit_layer_vec,
    .llr_table = laclight_llr_table,
    .secret_key_bytes = LACLIGHT_SECRETKEYBYTES,
    .public_key_bytes = LACLIGHT_PUBLICKEYBYTES,
    .message_bytes = LACLIGHT_BYTES,
    .session_bytes = LACLIGHT_SESSIONBYTES,
    .ciphertext_bytes = LACLIGHT_CIPHERTEXTBYTES,
    .seed_bytes = LACLIGHT_SEEDBYTES,
    .ntt_q = LACLIGHT_NTTQ,
    .ntt_inv_q = LACLIGHT_NTT_INV_Q,
    .ntt_inv_n = LACLIGHT_NTT_INV_N,
    .ntt_k = LACLIGHT_NTT_K,
    .bitlen_q = LACLIGHT_BITLEN_Q,
    .b_q = LACLIGHT_B_Q,
    .inverse_n = LACLIGHT_INVERSE_N,
    .inverse_n_beta = LACLIGHT_INVERSE_N_BETA,
    .ntt_omega_count = LACLIGHT_NTT_OMEGA_COUNT,
    .ntt_omega = LACLIGHT_NTT_OMEGA,
};
