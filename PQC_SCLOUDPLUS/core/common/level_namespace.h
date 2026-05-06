#ifndef PQC_SCLOUDPLUS_LEVEL_NAMESPACE_H
#define PQC_SCLOUDPLUS_LEVEL_NAMESPACE_H

#define SCLOUDPLUS_NS_JOIN2(a, b) a##b
#define SCLOUDPLUS_NS_JOIN(a, b) SCLOUDPLUS_NS_JOIN2(a, b)

#if defined(SCLOUDPLUS_LEVEL_128)
#define SCLOUDPLUS_NAMESPACE(sym) SCLOUDPLUS_NS_JOIN(scloudplus128_, sym)
#elif defined(SCLOUDPLUS_LEVEL_192)
#define SCLOUDPLUS_NAMESPACE(sym) SCLOUDPLUS_NS_JOIN(scloudplus192_, sym)
#elif defined(SCLOUDPLUS_LEVEL_256)
#define SCLOUDPLUS_NAMESPACE(sym) SCLOUDPLUS_NS_JOIN(scloudplus256_, sym)
#else
#define SCLOUDPLUS_NAMESPACE(sym) sym
#endif

#define scloud_kemkeygen SCLOUDPLUS_NAMESPACE(crypto_kem_keypair)
#define scloud_kemencaps SCLOUDPLUS_NAMESPACE(crypto_kem_enc)
#define scloud_kemdecaps SCLOUDPLUS_NAMESPACE(crypto_kem_dec)

#define scloudplus_pkekeygen SCLOUDPLUS_NAMESPACE(pkekeygen)
#define scloudplus_pkeenc SCLOUDPLUS_NAMESPACE(pkeenc)
#define scloudplus_pkedec SCLOUDPLUS_NAMESPACE(pkedec)

#define scloudplus_msgencode SCLOUDPLUS_NAMESPACE(msgencode)
#define scloudplus_msgdecode SCLOUDPLUS_NAMESPACE(msgdecode)
#define scloudplus_packsk SCLOUDPLUS_NAMESPACE(packsk)
#define scloudplus_unpacksk SCLOUDPLUS_NAMESPACE(unpacksk)
#define scloudplus_packpk SCLOUDPLUS_NAMESPACE(packpk)
#define scloudplus_unpackpk SCLOUDPLUS_NAMESPACE(unpackpk)
#define scloudplus_compressc1 SCLOUDPLUS_NAMESPACE(compressc1)
#define scloudplus_decompressc1 SCLOUDPLUS_NAMESPACE(decompressc1)
#define scloudplus_compressc2 SCLOUDPLUS_NAMESPACE(compressc2)
#define scloudplus_decompressc2 SCLOUDPLUS_NAMESPACE(decompressc2)
#define scloudplus_packc1 SCLOUDPLUS_NAMESPACE(packc1)
#define scloudplus_unpackc1 SCLOUDPLUS_NAMESPACE(unpackc1)
#define scloudplus_packc2 SCLOUDPLUS_NAMESPACE(packc2)
#define scloudplus_unpackc2 SCLOUDPLUS_NAMESPACE(unpackc2)

#define scloudplus_add SCLOUDPLUS_NAMESPACE(add)
#define scloudplus_sub SCLOUDPLUS_NAMESPACE(sub)
#define scloudplus_mul_cs SCLOUDPLUS_NAMESPACE(mul_cs)
#define scloudplus_mul_add_sb_e SCLOUDPLUS_NAMESPACE(mul_add_sb_e)

#define scloudplus_mul_add_as_e SCLOUDPLUS_NAMESPACE(mul_add_as_e)
#define scloudplus_mul_add_sa_e SCLOUDPLUS_NAMESPACE(mul_add_sa_e)
#define scloudplus_sampleeta1 SCLOUDPLUS_NAMESPACE(sampleeta1)
#define scloudplus_sampleeta2 SCLOUDPLUS_NAMESPACE(sampleeta2)
#define scloudplus_samplepsi SCLOUDPLUS_NAMESPACE(samplepsi)
#define scloudplus_samplephi SCLOUDPLUS_NAMESPACE(samplephi)
#define readu8ton SCLOUDPLUS_NAMESPACE(readu8ton)
#define readu8tom SCLOUDPLUS_NAMESPACE(readu8tom)

#define complex_add SCLOUDPLUS_NAMESPACE(complex_add)
#define complex_sub SCLOUDPLUS_NAMESPACE(complex_sub)
#define complex_mul SCLOUDPLUS_NAMESPACE(complex_mul)
#define my_round SCLOUDPLUS_NAMESPACE(my_round)
#define euclidean_distance SCLOUDPLUS_NAMESPACE(euclidean_distance)
#define bddbwn SCLOUDPLUS_NAMESPACE(bddbwn)
#define readu16tou32 SCLOUDPLUS_NAMESPACE(readu16tou32)

#endif
