#ifndef PQC_HQC_LEVEL_NAMESPACE_H
#define PQC_HQC_LEVEL_NAMESPACE_H

#define HQC_NS_EXPAND(name) HQC_LEVEL_NAMESPACE(name)
#define HQC_NS(name) HQC_NS_EXPAND(name)

#define crypto_kem_keypair HQC_NS(crypto_kem_keypair)
#define crypto_kem_enc HQC_NS(crypto_kem_enc)
#define crypto_kem_dec HQC_NS(crypto_kem_dec)

#define hqc_pke_keygen HQC_NS(hqc_pke_keygen)
#define hqc_pke_encrypt HQC_NS(hqc_pke_encrypt)
#define hqc_pke_decrypt HQC_NS(hqc_pke_decrypt)

#define hqc_dk_pke_from_string HQC_NS(hqc_dk_pke_from_string)
#define hqc_ek_pke_from_string HQC_NS(hqc_ek_pke_from_string)
#define hqc_c_kem_to_string HQC_NS(hqc_c_kem_to_string)
#define hqc_c_kem_from_string HQC_NS(hqc_c_kem_from_string)

#define prng_init HQC_NS(prng_init)
#define prng_get_bytes HQC_NS(prng_get_bytes)
#define xof_init HQC_NS(xof_init)
#define xof_get_bytes HQC_NS(xof_get_bytes)
#define hash_g HQC_NS(hash_g)
#define hash_h HQC_NS(hash_h)
#define hash_i HQC_NS(hash_i)
#define hash_j HQC_NS(hash_j)

#define gf_generate HQC_NS(gf_generate)
#define gf_mul HQC_NS(gf_mul)
#define gf_square HQC_NS(gf_square)
#define gf_inverse HQC_NS(gf_inverse)

#define vect_generate_random_support1 HQC_NS(vect_generate_random_support1)
#define vect_generate_random_support2 HQC_NS(vect_generate_random_support2)
#define vect_write_support_to_vector HQC_NS(vect_write_support_to_vector)
#define vect_sample_fixed_weight1 HQC_NS(vect_sample_fixed_weight1)
#define vect_sample_fixed_weight2 HQC_NS(vect_sample_fixed_weight2)
#define vect_set_random HQC_NS(vect_set_random)
#define vect_add HQC_NS(vect_add)
#define vect_compare HQC_NS(vect_compare)
#define vect_truncate HQC_NS(vect_truncate)
#define vect_print HQC_NS(vect_print)
#define vect_mul HQC_NS(vect_mul)

#define fft HQC_NS(fft)
#define fft_retrieve_error_poly HQC_NS(fft_retrieve_error_poly)

#define reed_muller_encode HQC_NS(reed_muller_encode)
#define reed_muller_decode HQC_NS(reed_muller_decode)
#define reed_solomon_encode HQC_NS(reed_solomon_encode)
#define reed_solomon_decode HQC_NS(reed_solomon_decode)
#define compute_generator_poly HQC_NS(compute_generator_poly)

#define code_encode HQC_NS(code_encode)
#define code_decode HQC_NS(code_decode)

#endif
