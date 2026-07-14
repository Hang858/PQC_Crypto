#include <stdint.h>
#include <string.h>
#include "sha3.h"
#include "sha3_interface.h"


#define SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL1    64
#define SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL3    80
#define SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL5    96

#define SHAKE256_TLEN_HWPAD_SCLEN_LEVEL1    608
#define SHAKE256_TLEN_HWPAD_SCLEN_LEVEL3    1280
#define SHAKE256_TLEN_HWPAD_SCLEN_LEVEL5    2208

#define SHAKE256_CHAIN_SOFTPAD_SCLEN    136



static inline uint8_t get_read_word_count(uint8_t level)
{
    switch (level) {
        case 1:  return 4;  
        case 3:  return 6;  
        case 5:  return 8;  
        default: return 4;
    }
}

static inline void sha3_set_security_level(uint8_t level)
{
    uint32_t reg_val = sha3_reg_read(SCCFG_OFFSET);

    reg_val &= ~SEC_LVL_MASK;

    switch (level)
    {
        case 3:
            reg_val |= SEC_LVL_3;
            break;
        case 5:
            reg_val |= SEC_LVL_5;
            break;
        default:
            reg_val |= SEC_LVL_1;
            break;
    }

    sha3_reg_write(SCCFG_OFFSET, reg_val);
}

static inline void sha3_set_seclevel_sclen_cpu(uint8_t level, uint8_t is_tlen)
{
    
    sha3_set_security_level(level);
    
    if (is_tlen) {
        uint32_t sclen = (level == 1) ? SHAKE256_TLEN_HWPAD_SCLEN_LEVEL1 : (level == 3) ? SHAKE256_TLEN_HWPAD_SCLEN_LEVEL3 : SHAKE256_TLEN_HWPAD_SCLEN_LEVEL5;
        sha3_reg_write(SCLEN_OFFSET, sclen);
    } else {
        uint32_t sclen = (level == 1) ? SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL1 : (level == 3) ? SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL3 : SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL5;
        sha3_reg_write(SCLEN_OFFSET, sclen);
    }
}

static inline void sha3_common_io(
    uint8_t  level,
    uint8_t* input,  uint32_t input_len,
    uint8_t* output)
{
    uint32_t i;
    const uint8_t* in_buf = input;
    uint32_t* out_word_buf = (uint32_t *)output;

    for (i = 0; i < input_len/4; i++)
    {
	     uint32_t reg_data = (*(const uint32_t *)(in_buf + i*4));
        sha3_reg_write(SCDR_OFFSET, reg_data);
    }

    while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_DONE));

    uint8_t read_cnt = get_read_word_count(level);

    uint32_t reg_data;
    for (i = 0U; i < read_cnt; i++)
    {
        reg_data = sha3_reg_read(SCDR_OFFSET);
        out_word_buf[i] = reg_data;
    }

    sha3_reg_write(SCSR_OFFSET, 0);
    sha3_reg_write(SCCFG_OFFSET, SCCFG_SHA3_CLR);

}

uint32_t shake256_hash_chain(uint8_t level, void* input, uint32_t input_len, uint8_t* output, uint32_t output_len, uint8_t link_count)
{
    if (input == NULL || output == NULL ) return SHA3_ERR_NULL_PTR;
	if (input_len == 0 || output_len == 0) return SHA3_ERR_INVALID_LEN;
    if ((link_count == 0)||(link_count>15)) return SHA3_ERR_LINK_COUNT;
    if (input_len != SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL1 && 
    input_len != SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL3 && 
    input_len != SHAKE256_CHAIN_HWPAD_SCLEN_LEVEL5)
    {
        return SHA3_ERR_INVALID_LEN;
    }
   
    sha3_reg_write(SCSR_OFFSET, 0);
    while(0!= sha3_reg_read(SCSR_OFFSET));
    sha3_reg_write(SCCFG_OFFSET, SCCFG_SHA3_CLR);
    while(0!= sha3_reg_read(SCCFG_OFFSET));

    sha3_reg_write(SCMR_OFFSET, SCMR_CHAIN_SEL | SCMR_SHAKE256);
    sha3_reg_write(SCITERCNT_OFFSET, link_count);
    sha3_reg_write(SCIER_OFFSET, 0U);
	

    sha3_set_hw_config(LITTLE_ENDIAN, CPU_EN, HW_PADDING);
	
    sha3_set_seclevel_sclen_cpu(level, 0);        
    sha3_reg_write(SCCR_OFFSET, SCCR_AE);
    sha3_common_io(level, input, input_len, output); 
	return SHA3_OK;
}

uint32_t shake256_prf(uint8_t level, void* input, uint32_t input_len, uint8_t* output, uint32_t output_len)
{
    
	return(sha3_hash(OP_ALG_SHAKE256, input, input_len, output, output_len));
}

uint32_t shake256_Tlen(uint8_t level, void* input, uint32_t input_len, uint8_t* output, uint32_t output_len)
{
	return(sha3_hash(OP_ALG_SHAKE256, input, input_len, output, output_len));
}



