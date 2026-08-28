// Copyright (c) 2025 Ziyao Liu
// Affiliation: Key Laboratory of Cyberspace Security Defense, Institute of Information Engineering, CAS
//              School of Cyber Security, University of Chinese Academy of Sciences

// This file provides the detailed implementation of polar code encoding and decoding

# include <stdio.h>
#include <stdint.h>
# include "polarlac_param.h"
# include "memops.h"
# include "malloc.h"

#define sign_macro(x) ((x > 0) - (x < 0))
#define absl_macro(x) (((x > 0) - (x < 0)) * x)
#define mini_macro(x, y) ((x < y) ? x : y)
#define f_macro(L1, L2) sign_macro(L1) * sign_macro(L2) * mini_macro(absl_macro(L1), absl_macro(L2))
#define g_macro(u, L1, L2) (((1 - 2*u) * L1) + L2)

/**
 * polar encode
 * Algorithm idea inspired by:
 * https://github.com/sravan-ankireddy/polar_codes (Repository does not specify a license)
 */
void encode_polar(uint8_t *u)
{ 
    int32_t stage_size = 1; // current butterfly size
    const int32_t polar_n = POLARLAC_PARAM_POLAR_LOG_N;

    for (int32_t level = 0; level < polar_n; level++)
    {
        int32_t group_size = stage_size * 2;
        int32_t num_groups = (1 << polar_n) / group_size;

        for (int32_t block = 0; block < num_groups; block++)
        {
            uint8_t *segment = u + block * group_size;

            /* Apply butterfly transform */
            for (int32_t offset = 0; offset < stage_size; offset++)
            {
                uint8_t left  = segment[offset];
                uint8_t right = segment[offset + stage_size];

                segment[offset] = left ^ right;
            }
        }

        stage_size <<= 1; // move to next level
    }
}

/**
 * polar decode
 * Algorithm idea inspired by:
 * https://github.com/YuYongRun/PolarCodeDecodersInMatlab (Repository does not specify a license)
 */
void decode_polar(uint8_t *m_cap, const int64_t *llr)
{
    const int32_t polar_N = POLARLAC_PARAM_POLAR_N;
    const int32_t polar_n = POLARLAC_PARAM_POLAR_LOG_N;
    const uint8_t *info_nodes = POLARLAC_PARAM_POLAR_INFO_NODES;
    const int32_t *lambda_offset = POLARLAC_PARAM_POLAR_LAMBDA_OFFSET;
    const int32_t *llr_layer_vec = POLARLAC_PARAM_POLAR_LLR_LAYER_VEC;
    const int32_t *bit_layer_vec = POLARLAC_PARAM_POLAR_BIT_LAYER_VEC;
    uint8_t (*inter_bit)[2] = (uint8_t (*)[2])my_malloc((2 * polar_N - 1) * sizeof(*inter_bit));  // internal bit vector
    int64_t *inter_llr = my_malloc((polar_N - 1) * sizeof(*inter_llr));  // internal llr vector, inter_llr[0] used for decision
    int32_t msg_index = 0;
    if (inter_bit == NULL || inter_llr == NULL) {
        my_free(inter_bit);
        my_free(inter_llr);
        abort();
    }

    for(int32_t i = 0; i < polar_N; i++) // decode each u_i
    {
        if(i == 0)
        {
            int32_t index1 = lambda_offset[polar_n - 1];
            int32_t beta = 0;
            int32_t end_beta = index1 - 1;
            
            for(; beta <= end_beta; beta++)
            {
                inter_llr[beta + index1 - 1] = f_macro(llr[beta], llr[beta + index1]);
            }

            for(int32_t layer = polar_n - 2; layer >= -1; layer--)
            {
                int32_t index1 = lambda_offset[layer];
                int32_t index2 = lambda_offset[layer + 1];
                int32_t beta = index1 - 1;
                int32_t end_beta = index2 - 2;
                
                for(; beta <= end_beta; beta++)
                {
                    inter_llr[beta] = f_macro(inter_llr[beta + index1], inter_llr[beta + index2]);
                }
            }
        }
        else if(i == polar_N/2)
        {
            int32_t index1 = lambda_offset[polar_n - 1];
            int32_t beta = 0;
            int32_t end_beta = index1 - 1;
            
            for(; beta <= end_beta; beta++)
            {
                inter_llr[beta + index1 - 1] = g_macro(inter_bit[beta + index1 - 1][0], llr[beta], llr[beta + index1]);
            }

            for(int32_t layer = polar_n - 2; layer >= -1; layer--)
            {
                int32_t index1 = lambda_offset[layer];
                int32_t index2 = lambda_offset[layer + 1];
                int32_t beta = index1 - 1;
                int32_t end_beta = index2 - 2;
                
                for(; beta <= end_beta; beta++)
                {
                    inter_llr[beta] = f_macro(inter_llr[beta + index1], inter_llr[beta + index2]);
                }
            }
        }
        else
        {
            int32_t llr_layer = llr_layer_vec[i];
            int32_t index1 = lambda_offset[llr_layer];
            int32_t index2 = lambda_offset[llr_layer + 1];
            int32_t beta = index1 - 1;
            int32_t end_beta = index2 - 2;
            
            for(; beta <= end_beta; beta++)
            {
                inter_llr[beta] = g_macro(inter_bit[beta][0], inter_llr[beta + index1], inter_llr[beta + index2]);
            }

            for(int32_t layer = llr_layer - 1; layer >= -1; layer--)
            {
                int32_t index1 = lambda_offset[layer];
                int32_t index2 = lambda_offset[layer + 1];
                int32_t beta = index1 - 1;
                int32_t end_beta = index2 - 2;
                
                for(; beta <= end_beta; beta++)
                {
                    inter_llr[beta] = f_macro(inter_llr[beta + index1], inter_llr[beta + index2]);
                }  
            }
        }

        int32_t i_mod_2 = i & 1;
        if(info_nodes[i] == 0)
        {
            inter_bit[0][i_mod_2] = 0;
        }
        else
        {
            int32_t u_i = (inter_llr[0] < 0); // decision
            inter_bit[0][i_mod_2] = u_i;
            m_cap[msg_index] = u_i;
            msg_index++;
        }
        if(i_mod_2 == 1) // bit recursion
        {
            int32_t bit_layer = bit_layer_vec[i];
            int32_t index1;
            int32_t index2;
            for(int32_t layer = 0; layer <= bit_layer - 1; layer++)
            {
                index1 = lambda_offset[layer];
                index2 = lambda_offset[layer + 1];
                for(int32_t beta = index1 - 1; beta <= index2 - 2; beta++)
                {
                    inter_bit[beta + index1][1] = inter_bit[beta][0] ^ inter_bit[beta][1];
                    inter_bit[beta + index2][1] = inter_bit[beta][1];
                }
            }

            index1 = lambda_offset[bit_layer];
            index2 = lambda_offset[bit_layer + 1];
            for (int32_t beta = index1 - 1; beta <= index2 - 2; beta++)
            {
                inter_bit[beta + index1][0] = inter_bit[beta][0] ^ inter_bit[beta][1];
                inter_bit[beta + index2][0] = inter_bit[beta][1];
            }
        }
    }
    my_free(inter_bit);
    my_free(inter_llr);
}
