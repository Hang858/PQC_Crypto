#if !defined(SHA256_H_)
#define SHA256_H_

#if defined( EXT_SHA256_H )
#include EXT_SHA256_H
#else

#include <stdint.h>

/* SHA256 context. */
typedef struct {
  uint32_t h[8];                     /* state; this is in the CPU native format */
  uint32_t Nl, Nh;                   /* number of bits processed so far */
  uint32_t num;                      /* number of bytes within the below */
                                     /* buffer */
  unsigned char data[64];            /* input buffer.  This is in byte vector format */
} SHA256_CTX;

void SHA256_Init(SHA256_CTX *);  /* context */

void SHA256_Update(SHA256_CTX *, /* context */
                  const void *, /* input block */ 
                  uint32_t);/* length of input block */

void SHA256_Final(unsigned char *,
                 SHA256_CTX *);

#endif /* EXT_SHA256_H */

#if !defined( SHA256_LEN )
#define SHA256_LEN 32    /* The length of a SHA256 hash output */
#endif

#endif /* ifdef(SHA256_H_) */
