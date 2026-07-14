#include <stdint.h>
#include <string.h>
//#include <stdlib.h>

#include "sha3.h"
#include "sha3_interface.h"

/**
 * @note   Read/write register ONLY ONCE for all settings, max performance
 * @param  is_little_endian  1: little-endian mode, 0: big-endian mode
 * @param  use_dma           1: DMA transfer mode, 0: CPU mode
 * @param  use_soft_padding 1: software padding mode, 0: hardware padding mode
 * @return none
 */
void sha3_set_hw_config(uint8_t is_little_endian, uint8_t use_dma, uint8_t use_soft_padding)
{
    // Read configuration register once (minimize bus access)
    uint32_t reg_val = sha3_reg_read(SCCFG_OFFSET);

    // Bitwise clear all configurable fields
    reg_val &= ~(SCCFG_BYTE_LITEND | SCCFG_DMA_EN | SCCFG_SOFT_PADDING);

    // Bitwise set new configuration flags
    if (is_little_endian)
        reg_val |= SCCFG_BYTE_LITEND;
    if (use_dma)
        reg_val |= SCCFG_DMA_EN;
    if (use_soft_padding)
        reg_val |= SCCFG_SOFT_PADDING;

    // Write back configuration once (high efficiency)
    sha3_reg_write(SCCFG_OFFSET, reg_val);
}

uint32_t sha3_get_algorithm_params(uint8_t alg, sha3_alg_params_t *params)
{
    if (params == NULL) {
        return SHA3_ERR_NULL_PTR; // ������Ч
    }

    switch (alg) {
        case OP_ALG_SHA3_224:
            params->block_size  = SHA3_BLOCK_SIZE_224;
            params->padding     = 0x06;
            params->output_size = SHA3_HASH_SIZE_224;
            params->scmr_val    = SCMR_SHA3_224;
            break;
        case OP_ALG_SHA3_256:
            params->block_size  = SHA3_BLOCK_SIZE_256;
            params->padding     = 0x06;
            params->output_size = SHA3_HASH_SIZE_256;
            params->scmr_val    = SCMR_SHA3_256;
            break;
        case OP_ALG_SHA3_384:
            params->block_size  = SHA3_BLOCK_SIZE_384;
            params->padding     = 0x06;
            params->output_size = SHA3_HASH_SIZE_384;
            params->scmr_val    = SCMR_SHA3_384;
            break;
        case OP_ALG_SHA3_512:
            params->block_size  = SHA3_BLOCK_SIZE_512;
            params->padding     = 0x06;
            params->output_size = SHA3_HASH_SIZE_512;
            params->scmr_val    = SCMR_SHA3_512;
            break;
        case OP_ALG_SHAKE128:
            params->block_size  = SHA3_BLOCK_SIZE_SHAKE128;
            params->padding     = 0x1F;
            params->output_size = SHA3_HASH_SIZE_SHAKE128;
            params->scmr_val    = SCMR_SHAKE128;
            break;
        case OP_ALG_SHAKE256:
            params->block_size  = SHA3_BLOCK_SIZE_SHAKE256;
            params->padding     = 0x1F;
            params->output_size = SHA3_HASH_SIZE_SHAKE256;
            params->scmr_val    = SCMR_SHAKE256;
            break;
        default:
            return SHA3_ERR_ARG; // ��֧�ֵ��㷨
    }

    return SHA3_OK;
}

void hw_sha3_absorb_block(void *input, uint32_t r)
{
    uint32_t      i;
    uint8_t  *in_ptr  = (uint8_t *)input;
    
    sha3_reg_write(SCLEN_OFFSET, r);
    sha3_reg_write(SCCR_OFFSET, SCCR_AE);

     for (i = 0; i < r/4; i++)
     {
	     uint32_t reg_data = (*(const uint32_t *)(in_ptr + i*4));
         sha3_reg_write(SCDR_OFFSET, reg_data);
     }
//    memcpy((volatile uint32_t *)((uintptr_t)SHA3CHAIN_BASE_ADDR + SCDR_OFFSET),(const uint32_t*)in_ptr,r/4);


}

uint32_t sha3_init(uint8_t alg, void *s, uint32_t s_len)
{
    sha3_alg_params_t params;
	sha3_ctx_t* pCtx = NULL;
    if (sha3_get_algorithm_params(alg, &params) != SHA3_OK)  return SHA3_ERR_ARG;
	if (s_len == 0) return SHA3_ERR_INVALID_LEN;
    if (s == NULL) return SHA3_ERR_NULL_PTR;

	pCtx = (sha3_ctx_t*)s;
	pCtx->pos = 0;
	pCtx->IS_FINALIZED = 0;

    sha3_reg_write(SCSR_OFFSET, 0);
    while(0!= sha3_reg_read(SCSR_OFFSET));
    sha3_reg_write(SCCFG_OFFSET, SCCFG_SHA3_CLR);
    while(0!= sha3_reg_read(SCCFG_OFFSET));

    sha3_reg_write(SCMR_OFFSET, SCMR_NO_CHAIN | params.scmr_val);
    sha3_reg_write(SCIER_OFFSET, 0U);
    sha3_set_hw_config(LITTLE_ENDIAN, CPU_EN, SOFT_PADDING);
    return SHA3_OK;
}

uint32_t sha3_absorb(uint8_t alg, void *s, uint32_t s_len, const uint8_t *input, uint32_t input_len)
{
    const uint8_t *in = (const uint8_t *)input;
    uint32_t remain = input_len;
    sha3_alg_params_t params;
	sha3_ctx_t* pCtx = (sha3_ctx_t*)s;
	
    if (sha3_get_algorithm_params(alg, &params) != SHA3_OK)  return SHA3_ERR_ARG;
    if (input == NULL) return SHA3_ERR_NULL_PTR;
	if (input_len == 0||s_len == 0) return SHA3_ERR_INVALID_LEN;
	
    uint8_t r = params.block_size;


    while (remain > 0)
    {
    	uint8_t can_copy = r - pCtx->pos;
        if (can_copy > remain)
            can_copy = remain;


        memcpy(&pCtx->buf[pCtx->pos], in, can_copy);
        in += can_copy;
        pCtx->pos += can_copy;
        remain -= can_copy;


        if (pCtx->pos == r)
        {
            hw_sha3_absorb_block(pCtx->buf, r);
            //attention: not last block,clr chain down ,diff to HW padding
//            while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_LOOP_DONE));
            while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_DONE));
            sha3_reg_write(SCSR_OFFSET, 0);
            pCtx->pos = 0;
        }
    }

    return SHA3_OK;
}

uint32_t sha3_squeeze(uint8_t alg, void *s, uint32_t s_len, uint8_t *output, uint32_t output_len)
{
    sha3_alg_params_t params;
    if (sha3_get_algorithm_params(alg, &params) != SHA3_OK)  return SHA3_ERR_ARG;
    if (output == NULL) return SHA3_ERR_NULL_PTR;
	if (output_len == 0||s_len == 0) return SHA3_ERR_INVALID_LEN;
	

    uint32_t out_idx = 0;
    uint8_t *out_buf = (uint8_t *)output;
    uint8_t block_size = params.block_size;
	sha3_ctx_t* pCtx = (sha3_ctx_t*)s;
	uint8_t reg_blocks = 0;

	if(0x5a != pCtx->IS_FINALIZED)
	{
		   if (pCtx->pos < block_size)
		    {

		        pCtx->buf[pCtx->pos++] = params.padding;

		        memset(&pCtx->buf[pCtx->pos], 0, block_size - pCtx->pos);

		        pCtx->buf[block_size - 1] |= 0x80;

		        hw_sha3_absorb_block(pCtx->buf, block_size);
		        //last block,clr sr done
		        while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_DONE));
		        pCtx->pos = 0;
		        pCtx->IS_FINALIZED = 0x5a;
		    }

			reg_blocks = params.output_size / 4U;

		    while (out_idx < output_len) {

		        for (uint8_t i = 0; i < reg_blocks && out_idx < output_len; i++) {
		            uint32_t reg_data = sha3_reg_read(SCDR_OFFSET);
					*(uint32_t *)(out_buf + out_idx) = reg_data;
					out_idx += 4;
		        }

		        if (out_idx < output_len) {
		        	sha3_reg_write(SCSR_OFFSET, 0);
		            sha3_reg_write(SCCR_OFFSET, SCCR_AE_SHAKE);
		            while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_DONE));
		        }
		    }
	}
	else
	{
		reg_blocks = params.output_size / 4U;

		while (out_idx < output_len) {
			sha3_reg_write(SCSR_OFFSET, 0);
			sha3_reg_write(SCCR_OFFSET, SCCR_AE_SHAKE);
			while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_DONE));

			for (uint8_t i = 0; i < reg_blocks && out_idx < output_len; i++) {
				uint32_t reg_data = sha3_reg_read(SCDR_OFFSET);
				*(uint32_t *)(out_buf + out_idx) = reg_data;
				out_idx += 4;
			}
		}
	}


//    sha3_reg_write(SCSR_OFFSET, 0);
//    sha3_reg_write(SCCFG_OFFSET, SCCFG_SHA3_CLR);

    return SHA3_OK;
}

uint32_t sha3_hash(uint8_t alg, uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t output_len)
{
    sha3_alg_params_t params;
    if (input == NULL || output == NULL ) return SHA3_ERR_NULL_PTR;
	if (input_len == 0 || output_len == 0) return SHA3_ERR_INVALID_LEN;
    if (sha3_get_algorithm_params(alg, &params) != SHA3_OK) return SHA3_ERR_ARG;

    uint8_t  *in_ptr  = (uint8_t *)input;
    uint8_t  *out_ptr = (uint8_t *)output;
    uint32_t  remain_len = input_len;
    uint32_t  out_idx = 0;
    uint32_t  reg_data;
    uint32_t  i;
    uint32_t  flag = 0;

    sha3_reg_write(SCSR_OFFSET, 0);
    while(0!= sha3_reg_read(SCSR_OFFSET));
    sha3_reg_write(SCCFG_OFFSET, SCCFG_SHA3_CLR);
    while(0!= sha3_reg_read(SCCFG_OFFSET));

    /* -------------------------- Hardware initialization ------------------------- */
	sha3_reg_write(SCMR_OFFSET, SCMR_NO_CHAIN | params.scmr_val);
	
    sha3_reg_write(SCIER_OFFSET, 0U);

    sha3_set_hw_config(LITTLE_ENDIAN, CPU_EN, HW_PADDING);

	/* ------------------------------ Data feed loop ------------------------------ */
    while (remain_len > 0)
    {
        uint8_t curr_len = (remain_len > params.block_size) ? params.block_size : remain_len;
        sha3_reg_write(SCLEN_OFFSET, curr_len);
        sha3_reg_write(SCCR_OFFSET, SCCR_AE);


        uint8_t words = curr_len / 4U;
        uint8_t rem = curr_len % 4U;
        uint8_t i;
        uint32_t Lastword = 0;

        /* Write full 4-byte word blocks to SCDR register (little-endian direct cast) */
        for (i = 0; i < words; i++)
        {
            /* Directly cast byte pointer to uint32_t to read full little-endian 32-bit word */
            uint32_t reg_val = (*(const uint32_t *)(in_ptr + i * 4));
            /* Write complete 32-bit word to input data register */
            sha3_reg_write(SCDR_OFFSET, reg_val);
        }

        /* Process residual bytes less than 4 bytes, pad high bits with zero */
        if (rem > 0U)
        {

            uint32_t base_idx = words * 4U;

            /* Fill valid low bytes, upper unused bits keep zero padding */
            for (i = 0; i < rem; i++)
            {
            	Lastword |= (uint32_t)in_ptr[base_idx + i] << (8U * i);

            }

            /* Write zero-padded partial word to SCDR register */
            sha3_reg_write(SCDR_OFFSET, Lastword);
        }


        /* Wait for block completion */
        if (remain_len >= params.block_size)
        {
            while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_LOOP_DONE));
            sha3_reg_write(SCSR_OFFSET, 0);
            
            if(remain_len == params.block_size)
                flag = 0x5a;
        }
        
        in_ptr     += curr_len;
        remain_len -= curr_len;
    }

    if(flag)
    {
        /* Trigger final padding if last block was exactly full */
        sha3_reg_write(SCLEN_OFFSET, 0);
        sha3_reg_write(SCCR_OFFSET, SCCR_AE);
    }


    while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_DONE));
    while (out_idx < output_len)
    {
	    uint8_t   output_regcnt = params.output_size / 4U;
        for (i = 0; i < output_regcnt && out_idx < output_len; i++)
        {

//			*(uint32_t *)(out_ptr + out_idx) = __builtin_bswap32(sha3_reg_read(SCDR_OFFSET));
			*(uint32_t *)(out_ptr + out_idx) =  sha3_reg_read(SCDR_OFFSET);
			out_idx += 4U;
		}

        if (out_idx < output_len)
        {
        	sha3_reg_write(SCSR_OFFSET, 0);
            sha3_reg_write(SCCR_OFFSET, SCCR_AE_SHAKE);
            while (!(sha3_reg_read(SCSR_OFFSET) & SCSR_DONE));
        }
    }

    sha3_reg_write(SCSR_OFFSET, 0);
    sha3_reg_write(SCCFG_OFFSET, SCCFG_SHA3_CLR);

    return SHA3_OK;
}

