// Original copyright: LAC v3

// Modified by:
// Copyright (c) 2025 Ziyao Liu
// Affiliation: Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
//              School of Cyber Security, University of Chinese Academy of Sciences

// Modification: added functions `polar_ecc_enc` and `polar_ecc_dec` to implement polar code encoding and decoding

#include "polar.h"
#include "ecc.h"
#include <string.h>
#include <stdlib.h>

//error corretion encode(polar)
int32_t polar_ecc_enc(uint8_t *d) // d:source sequence 
{
	/* polar */
	encode_polar(d);
	
	return 0;
}

//error corrction decode(polar)
int32_t polar_ecc_dec(uint8_t *d, const int64_t *c) // d:estimated message c:LLR of the received signal
{
	/*polar*/
	decode_polar(d, c);
	
	return 0;
}