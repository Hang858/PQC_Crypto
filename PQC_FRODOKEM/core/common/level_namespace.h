#ifndef PQC_FRODOKEM_LEVEL_NAMESPACE_H
#define PQC_FRODOKEM_LEVEL_NAMESPACE_H

#define FRODOKEM_NS_JOIN2(a, b) a##b
#define FRODOKEM_NS_JOIN(a, b) FRODOKEM_NS_JOIN2(a, b)

#if defined(FRODOKEM_LEVEL_640)
#define FRODOKEM_NAMESPACE(sym) FRODOKEM_NS_JOIN(frodokem640_, sym)
#elif defined(FRODOKEM_LEVEL_976)
#define FRODOKEM_NAMESPACE(sym) FRODOKEM_NS_JOIN(frodokem976_, sym)
#elif defined(FRODOKEM_LEVEL_1344)
#define FRODOKEM_NAMESPACE(sym) FRODOKEM_NS_JOIN(frodokem1344_, sym)
#else
#error "FRODOKEM level not selected"
#endif

#define frodo_pack FRODOKEM_NAMESPACE(frodo_pack)
#define frodo_unpack FRODOKEM_NAMESPACE(frodo_unpack)
#define frodo_sample_n FRODOKEM_NAMESPACE(frodo_sample_n)
#define ct_verify FRODOKEM_NAMESPACE(ct_verify)
#define ct_select FRODOKEM_NAMESPACE(ct_select)
#define clear_bytes FRODOKEM_NAMESPACE(clear_bytes)
#define CDF_TABLE FRODOKEM_NAMESPACE(CDF_TABLE)
#define CDF_TABLE_LEN FRODOKEM_NAMESPACE(CDF_TABLE_LEN)
#define frodo_mul_add_as_plus_e FRODOKEM_NAMESPACE(frodo_mul_add_as_plus_e)
#define frodo_mul_add_sa_plus_e FRODOKEM_NAMESPACE(frodo_mul_add_sa_plus_e)
#define frodo_mul_add_sb_plus_e FRODOKEM_NAMESPACE(frodo_mul_add_sb_plus_e)
#define frodo_mul_bs FRODOKEM_NAMESPACE(frodo_mul_bs)
#define frodo_add FRODOKEM_NAMESPACE(frodo_add)
#define frodo_sub FRODOKEM_NAMESPACE(frodo_sub)
#define frodo_key_encode FRODOKEM_NAMESPACE(frodo_key_encode)
#define frodo_key_decode FRODOKEM_NAMESPACE(frodo_key_decode)
#define crypto_kem_keypair_enc FRODOKEM_NAMESPACE(crypto_kem_keypair_enc)
#define crypto_kem_dec FRODOKEM_NAMESPACE(crypto_kem_dec)

#endif
