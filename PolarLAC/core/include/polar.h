// Copyright (c) 2025 Ziyao Liu
// Affiliation: Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
//              School of Cyber Security, University of Chinese Academy of Sciences

// Header file for polar code encoding and decoding

#include <stdint.h>

//polar parameter struct
struct polar_control {
	uint32_t    N; // code length
	uint32_t    n; // log2(N)
	uint32_t    K; // message length
	uint32_t    ecc_bytes; // N/8
};
//polar encode
void encode_polar(uint8_t *u);
//polar decode
void decode_polar(uint8_t *m_cap, const int64_t *llr);
