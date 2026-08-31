#include <stdint.h>
#include <string.h>
#include "uart_test_interface.h"
#include "uart.h"
/****************************************************************************
 * 全局缓冲区定义
 ***************************************************************************/
static uint8_t uart_rx_buf[UART_RX_BUF_LEN];  // UART接收缓冲区
static uint8_t uart_tx_buf[UART_TX_BUF_LEN];  // UART发送缓冲区
static uint16_t rx_idx = 0;                   // 接收缓冲区索引

/****************************************************************************
 * 通用工具函数（非静态，供外部调用）
 ***************************************************************************/
/**
 * @brief 计算缓冲区数据的异或校验值
 * @param pbuf 数据缓冲区指针
 * @param len 数据长度
 * @return 异或校验结果
 */
uint8_t calc_xor(uint8_t *pbuf, uint16_t len)
{
    uint8_t xor_sum = 0;
    for (uint16_t i = 0; i < len; i++)
    {
        xor_sum ^= pbuf[i];
    }
    return xor_sum;
}

/**
 * @brief 小端2字节转uint16
 * @param p 2字节数据指针
 * @return 转换后的uint16值
 */
uint16_t le16_to_u16(uint8_t *p)
{
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

/**
 * @brief uint16转小端2字节
 * @param dat 要转换的uint16值
 * @param p 输出2字节数据指针
 */
void u16_to_le16(uint16_t dat, uint8_t *p)
{
    p[0] = (dat & 0x00FF);
    p[1] = ((dat >> 8) & 0x00FF);
}

uint16_t get_u16_from_buf(uint8_t *p)
{
    return (uint16_t)p[1] | ((uint16_t)p[0] << 8);
}

uint16_t store_u16_to_buf(uint16_t dat, uint8_t *p)
{
    p[1] = (dat & 0x00FF);
    p[0] = ((dat >> 8) & 0x00FF);
}

/****************************************************************************
 * 底层UART接口（用户需实现硬件相关逻辑）
 ***************************************************************************/
//extern void UART_SendData(uint8_t *data, uint16_t len);
//extern uint8_t UART_GetOneByte(uint8_t *ch);

/****************************************************************************
 * ====================== 10种算法业务逻辑实现（全编译条件）====================
 ***************************************************************************/

/************************ ALG 0x00 : ML-KEM ************************/
#if ENABLE_ALG_MLKEM
#include "../../include/mlkem.h"
#define ALG_ID 0x00

/* 公钥/私钥/密文/共享密钥长度表（对应不同安全等级）*/
const uint16_t mlkem_pk_len[]   = {800, 1184, 1568};
const uint16_t mlkem_sk_len[]   = {1632, 2400, 3168};
const uint16_t mlkem_ct_len[]   = {800, 1184, 1568};
const uint16_t mlkem_ss_len[]   = {32, 32, 32};

/**
 * @brief ML-KEM算法业务处理（CMD:0x00~0x03 共4个接口）
 * CMD00: 初始化
 * CMD01: 生成密钥对
 * CMD02: 封装
 * CMD03: 解封装
 * @param cmd 命令号
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int mlkem_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint8_t level;
    uint16_t offset = 0;
    uint8_t *p_pk, *p_sk, *p_ct;
    *out_len = 0;

    switch (cmd)
    {
        case 0x00:
            // 00: 算法初始化（无参数）
            return MLKEM_init() ? ERR_FAIL : ERR_SUCCESS;

        case 0x01:
            // 01: 生成密钥对 [1B等级][PK+SK]
            if (in_len != 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if (MLKEM_crypto_kem_keypair(level, dat_out, dat_out + mlkem_pk_len[level]))
            {
                return ERR_FAIL;
            }
            *out_len = mlkem_pk_len[level] + mlkem_sk_len[level];
            break;

        case 0x02:
            // 02: 封装 [1B等级][PK][CT+SS]
            if (in_len < 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if ((offset + mlkem_pk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_pk = dat_in + offset;
            if (MLKEM_crypto_kem_enc(level, dat_out, dat_out + mlkem_ct_len[level], p_pk))
            {
                return ERR_FAIL;
            }
            *out_len = mlkem_ct_len[level] + mlkem_ss_len[level];
            break;

        case 0x03:
            // 03: 解封装 [1B等级][CT][SK][SS]
            if (in_len < 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if ((offset + mlkem_ct_len[level] + mlkem_sk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_ct = dat_in + offset;
            p_sk = dat_in + offset + mlkem_ct_len[level];
            if (MLKEM_crypto_kem_dec(level, dat_out, p_ct, p_sk))
            {
                return ERR_FAIL;
            }
            *out_len = mlkem_ss_len[level];
            break;

        default:
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

#elif ENABLE_ALG_FRODOKEM
#include "../../include/api.h"
#define ALG_ID 0x01

/* 公钥/私钥/密文/共享密钥长度表（对应不同安全等级）*/
const uint16_t frodokem_pk_len[] = {9616, 15632, 21520};
const uint16_t frodokem_sk_len[] = {19888, 31296, 43088};
const uint16_t frodokem_ct_len[] = {9752, 15792, 21696};
const uint16_t frodokem_ss_len[] = {16, 24, 32};

/**
 * @brief FrodoKEM算法业务处理
 * CMD00: 生成密钥对
 * CMD01: 加密封装（生成CT + SS）
 * CMD02: 解密解封装（生成SS）
 * @param cmd 命令字
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int frodokem_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint8_t level;
    uint16_t offset = 0;
    uint8_t *p_pk, *p_sk, *p_ct;
    *out_len = 0;

    switch (cmd)
    {
        case 0x00:
            // 00: 生成密钥对 [1B等级][PK+SK]
            if (in_len != 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if (FRODOKEM_gen_keypair(level, dat_out, dat_out + frodokem_pk_len[level]))
            {
                return ERR_FAIL;
            }
            *out_len = frodokem_pk_len[level] + frodokem_sk_len[level];
            break;

        case 0x01:
            // 01: 封装 [1B等级][PK][CT+SS]
            if (in_len < 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if ((offset + frodokem_pk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_pk = dat_in + offset;
            if (FRODOKEM_enc(level, dat_out, dat_out + frodokem_ct_len[level], p_pk))
            {
                return ERR_FAIL;
            }
            *out_len = frodokem_ct_len[level] + frodokem_ss_len[level];
            break;

        case 0x02:
            // 02: 解封装 [1B等级][CT][SK][SS]
            if (in_len < 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if ((offset + frodokem_ct_len[level] + frodokem_sk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_ct = dat_in + offset;
            p_sk = dat_in + offset + frodokem_ct_len[level];
            if (FRODOKEM_dec(level, dat_out, p_ct, p_sk))
            {
                return ERR_FAIL;
            }
            *out_len = frodokem_ss_len[level];
            break;

        default:
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

/************************ ALG 0x02 : PolarLAC ************************/
#elif ENABLE_ALG_POLARLAC
#include "../../include/polarlac.h"
#define ALG_ID 0x02

/* 公钥/私钥/密文/共享密钥长度表（对应不同安全等级）*/
const uint16_t polarlac_pk_len[] = {352, 704, 1056};
const uint16_t polarlac_sk_len[] = {720, 1440, 2144};
const uint16_t polarlac_ct_len[] = {424, 848, 1280};
const uint16_t polarlac_ss_len[] = {32, 32, 32};

/**
 * @brief PolarLAC算法业务处理（CMD:0x00~0x03 共4个接口）
 * CMD00: KEM生成密钥对
 * CMD01: KEM加密封装
 * CMD02: KEM解密解封装
 * @param cmd 命令字
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int polarlac_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint8_t level;
    uint16_t offset = 0;
    uint8_t *p_pk, *p_sk, *p_ct;
    *out_len = 0;

    switch (cmd)
    {
        case 0x00:
            // 00: 生成密钥对 [1B等级][PK+SK]
            if (in_len != 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if (polarlac_crypto_kem_keypair(level, dat_out, dat_out + polarlac_pk_len[level]))
            {
                return ERR_FAIL;
            }
            *out_len = polarlac_pk_len[level] + polarlac_sk_len[level];
            break;

        case 0x01:
            // 01: 封装 [1B等级][PK][CT+SS]
            if (in_len < 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if ((offset + polarlac_pk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_pk = dat_in + offset;
            if (polarlac_crypto_kem_enc(level, dat_out, dat_out + polarlac_ct_len[level], p_pk))
            {
                return ERR_FAIL;
            }
            *out_len = polarlac_ct_len[level] + polarlac_ss_len[level];
            break;

        case 0x02:
            // 02: 解封装 [1B等级][CT][SK][SS]
            if (in_len < 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if ((offset + polarlac_ct_len[level] + polarlac_sk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_ct = dat_in + offset;
            p_sk = dat_in + offset + polarlac_ct_len[level];
            if (polarlac_crypto_kem_dec(level, dat_out, p_ct, p_sk))
            {
                return ERR_FAIL;
            }
            *out_len = polarlac_ss_len[level];
            break;

        default:
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

/************************ ALG 0x03 : DAWN ************************/
#elif ENABLE_ALG_DAWN
#include "../../include/dawn.h"
#define ALG_ID 0x03

/**
 * @brief DAWN算法业务处理
 * CMD00: 初始化
 * CMD01: 生成密钥对
 * CMD02: 加密封装
 * CMD03: 解密解封装
 * @param cmd 命令字
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int dawn_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint16_t dim_n;
    *out_len = 0;
    
    switch (cmd)
    {
        case 0x00: 
            // DAWN初始化（无输入参数）
            if (in_len != 0)
            {
                return ERR_FAIL;
            }
            return DAWN_init() ? ERR_FAIL : ERR_SUCCESS;

        case 0x01:  
            // 生成密钥对 [2B维度n]
            if (in_len != 2)
            {
                return ERR_FAIL;
            }
            dim_n = get_u16_from_buf(dat_in);
            if (DAWN_KeyGen(dim_n, dat_out, dat_out + 128, dat_out + 256, dat_out + 384, dat_out + 512))
            {
                return ERR_FAIL;
            }
            *out_len = 544;  // 128(pk) + 128(k) + 128(f) + 128(f2) + 32(H_pk)
            break;

        case 0x02:  
            // 加密封装 [2B维度n][PK][随机数]
            dim_n = get_u16_from_buf(dat_in);
            if (DAWN_Enc(dim_n, dat_out, dat_out + 128, dat_in + 2))  // 修正原代码bug：dim → dim_n
            {
                return ERR_FAIL;
            }
            *out_len = 160;  // 128(c) + 32(K)
            break;

        case 0x03:  
            // 解密解封装 [2B维度n][CT][PK][k][f][f2][H_pk]
            dim_n = get_u16_from_buf(dat_in);
            if (DAWN_Dec(dim_n, dat_in + 2, dat_out, dat_in + 130, dat_in + 258, dat_in + 386, dat_in + 514, dat_in + 642))
            {
                return ERR_FAIL;
            }
            *out_len = 32;  // 32(K)
            break;

        default: 
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

/************************ ALG 0x04 : SCloud+ ************************/
#elif ENABLE_ALG_SCLOUDPLUS
#include "../../include/scloudplus_api.h"
#define ALG_ID 0x04

/* 公钥/私钥/密文/共享密钥长度表（对应不同安全等级）*/
const uint16_t scloudplus_pk_len[] = {7216, 11152, 18760};
const uint16_t scloudplus_sk_len[] = {8480, 13008, 21904};
const uint16_t scloudplus_ct_len[] = {5456, 10832, 16916};
const uint16_t scloudplus_ss_len[] = {16, 24, 32};

/**
 * @brief SCloud+算法业务处理（CMD:0x00~0x03 共4个接口）
 * CMD00: 初始化
 * CMD01: 生成密钥对
 * CMD02: 封装
 * CMD03: 解封装
 * @param cmd 命令号
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int scloudplus_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint8_t level;
    uint16_t offset = 0;
    uint8_t *p_pk, *p_sk, *p_ct;
    *out_len = 0;

    switch (cmd)
    {
        case 0x00:
            // 00: 生成密钥对 [1B等级][PK+SK]
            if (in_len != 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if (crypto_kem_keypair(level, dat_out, dat_out + scloudplus_pk_len[level]))
            {
                return ERR_FAIL;
            }
            *out_len = scloudplus_pk_len[level] + scloudplus_sk_len[level];
            break;

        case 0x01:
            // 01: 封装 [1B等级][PK][CT+SS]
            if (in_len < 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if ((offset + scloudplus_pk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_pk = dat_in + offset;
            if (crypto_kem_enc(level, dat_out, dat_out + scloudplus_ct_len[level], p_pk))
            {
                return ERR_FAIL;
            }
            *out_len = scloudplus_ct_len[level] + scloudplus_ss_len[level];
            break;

        case 0x02:
            // 02: 解封装 [1B等级][CT][SK][SS]
            if (in_len < 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if ((offset + scloudplus_ct_len[level] + scloudplus_sk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_ct = dat_in + offset;
            p_sk = dat_in + offset + scloudplus_ct_len[level];
            if (crypto_kem_dec(level, dat_out, p_ct, p_sk))
            {
                return ERR_FAIL;
            }
            *out_len = scloudplus_ss_len[level];
            break;

        default:
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

/************************ ALG 0x05 : HQC ************************/
#elif ENABLE_ALG_HQC
#include "../../include/HQC_API.h"
#define ALG_ID 0x05

/* 公钥/私钥/密文/共享密钥长度（固定值）*/
#define HQC_PK_LEN 2241
#define HQC_SK_LEN 2321
#define HQC_CT_LEN 4433
#define HQC_SS_LEN 32

/**
 * @brief HQC算法业务处理（CMD:0x00~0x03 共4个接口）
 * CMD00: 生成密钥对
 * CMD01: 加密封装
 * CMD02: 解密解封装
 * @param cmd 命令字
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int hqc_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint16_t offset = 0;
    uint8_t *p_pk, *p_sk, *p_ct;
    *out_len = 0;

    switch (cmd)
    {
        case 0x00:
            // 00: 生成密钥对 [PK+SK]
            if (in_len != 0)
            {
                return ERR_FAIL;
            }
            if (crypto_kem_keypair(dat_out, dat_out + HQC_PK_LEN))
            {
                return ERR_FAIL;
            }
            *out_len = HQC_PK_LEN + HQC_SK_LEN;
            break;

        case 0x01:
            // 01: 封装 [PK][CT+SS]
            if (in_len != HQC_PK_LEN)
            {
                return ERR_FAIL;
            }
            p_pk = dat_in + offset;
            if (crypto_kem_enc(dat_out, dat_out + HQC_CT_LEN, p_pk))
            {
                return ERR_FAIL;
            }
            *out_len = HQC_CT_LEN + HQC_SS_LEN;
            break;

        case 0x02:
            // 02: 解封装 [CT][SK][SS]
            if (in_len != HQC_CT_LEN + HQC_SK_LEN)
            {
                return ERR_FAIL;
            }
            p_ct = dat_in + offset;
            p_sk = dat_in + offset + HQC_CT_LEN;
            if (crypto_kem_dec(dat_out, p_ct, p_sk))
            {
                return ERR_FAIL;
            }
            *out_len = HQC_SS_LEN;
            break;

        default:
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

/************************ ALG 0x06 : ML-DSA (签名) ************************/
#elif ENABLE_ALG_MLDSA
#include "../../include/mldsa.h"
#define ALG_ID 0x06

/* 公钥/私钥/签名长度表（对应不同安全等级）*/
const uint16_t mldsa_pk_len[]  = {1312, 1952, 2592};
const uint16_t mldsa_sk_len[]  = {2560, 4032, 4896};
const uint16_t mldsa_sig_len[] = {2420, 3309, 4627};

/**
 * @brief ML-DSA算法业务处理（CMD:0x00~0x0A 共10个接口）
 * CMD00: 初始化
 * CMD01: 生成密钥对
 * CMD02: 带ctx签名生成
 * CMD03: 带ctx签名验证
 * @param cmd 命令号
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int mldsa_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, size_t *out_len)
{
    uint8_t level;
    uint16_t offset = 0, msglen, ctxlen, siglen;
    uint8_t *p_msg, *p_sk, *p_pk, *p_sig, *p_ctx;
    *out_len = 0;

    switch (cmd)
    {
        case 0x00:
            // 00: 算法初始化（无参数）
            return MLDSA_init() ? ERR_FAIL : ERR_SUCCESS;

        case 0x01:
            // 01: 生成密钥对 [1B等级][PK+SK]
            if (in_len != 1)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            if (MLDSA_crypto_sign_keypair(level, dat_out, dat_out + mldsa_pk_len[level]))
            {
                return ERR_FAIL;
            }
            *out_len = mldsa_pk_len[level] + mldsa_sk_len[level];
            break;

        case 0x02:
            // 02: 带ctx签名生成 [1B等级][1Bctxlen][ctx][2Bmsglen][msg][SK]
            if (in_len < 1 + 1 + 2)
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            ctxlen = dat_in[offset++];//get_u16_from_buf(dat_in + offset);
            //offset += 2;
            p_ctx = dat_in + offset;
            offset += ctxlen;
            msglen = get_u16_from_buf(dat_in + offset);
            offset += 2;
            p_msg = dat_in + offset;
            offset += msglen;
            if ((offset + mldsa_sk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }
            p_sk = dat_in + offset;
            if (MLDSA_crypto_sign_signature_ctx(level, dat_out, out_len, p_msg, msglen, p_ctx, ctxlen, p_sk))
            {
                return ERR_FAIL;
            }
            break;

        case 0x03:
            // 03: 带ctx签名验证 [1B等级][1Bctxlen][ctx][2Bmsglen][msg][sig][PK]
            if (in_len < 1 + 1 + 2 )
            {
                return ERR_FAIL;
            }
            level = dat_in[offset++];
            if (level > 2)
            {
                return ERR_FAIL;
            }
            ctxlen = dat_in[offset++];//get_u16_from_buf(dat_in + offset);
			//offset += 2;
			p_ctx = dat_in + offset;
			offset += ctxlen;

			msglen = get_u16_from_buf(dat_in + offset);
			offset += 2;
			p_msg = dat_in + offset;
			offset += msglen;

			if ((offset + mldsa_sig_len[level] + mldsa_pk_len[level]) != in_len)
			{
				return ERR_FAIL;
			}
            siglen = mldsa_sig_len[level];//get_u16_from_buf(dat_in + offset);
            p_sig = dat_in + offset;
            offset += siglen;


/*            if ((offset + mldsa_pk_len[level]) != in_len)
            {
                return ERR_FAIL;
            }*/
            p_pk = dat_in + offset;
            if (MLDSA_crypto_sign_verify_ctx(level, p_sig, siglen, p_msg, msglen, p_ctx, ctxlen, p_pk))
            {
                return ERR_FAIL;
            }
            break;

        default:
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

/************************ ALG 0x07 : SPHINCS+(SLH-DSA) ************************/
#elif ENABLE_ALG_SPHINCS
#include "../../include/sphincs.h"
#define ALG_ID 0x07

/* 公钥/私钥/签名长度表（对应不同安全等级）*/
const uint16_t sphincs_pk_len[]  = {32, 48, 64};
const uint16_t sphincs_sk_len[]  = {64, 96, 128};
const uint16_t sphincs_sig_len[] = {17088, 35664, 49856};

/**
 * @brief 获取安全等级对应的数组索引
 * @param lv 安全等级值
 * @return 索引（0/1/2），无效返回0xFF
 */
static uint8_t get_idx(uint8_t lv)
{
    if (lv == 1)
    {
        return 0;
    }
    if (lv == 3)
    {
        return 1;
    }
    if (lv == 5)
    {
        return 2;
    }
    return 0xFF;
}

/**
 * @brief SPHINCS+算法业务处理（CMD:0x00~0x03 共4个接口）
 * CMD00: 生成密钥对
 * CMD01: 基于种子生成密钥对
 * CMD02: 签名生成
 * CMD03: 签名验证
 * @param cmd 命令字
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int sphincs_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint8_t lv, idx;
    uint16_t offset = 0, seedlen, msglen, siglen;
    uint8_t *seed, *msg, *sk, *pk, *sig;
    *out_len = 0;

    switch (cmd)
    {
        case 0x00:
            // 生成密钥对 [1B等级]
            if (in_len != 1)
            {
                return ERR_FAIL;
            }
            lv = dat_in[offset++];
            idx = get_idx(lv);
            if (idx == 0xFF)
            {
                return ERR_FAIL;
            }
            if (sphincs_keypair(dat_out, dat_out + sphincs_pk_len[idx], lv))
            {
                return ERR_FAIL;
            }
            *out_len = sphincs_pk_len[idx] + sphincs_sk_len[idx];
            break;

        case 0x01:  
            // 基于种子生成密钥对 [2B种子长度][种子][1B等级]
            if (in_len < 3)
            {
                return ERR_FAIL;
            }
            seedlen = get_u16_from_buf(dat_in + offset);
            offset += 2;
            seed = dat_in + offset;
            offset += seedlen;  // 修正原代码bug：offset += seed → seedlen
            if (offset + 1 != in_len)
            {
                return ERR_FAIL;
            }
            lv = dat_in[offset];
            idx = get_idx(lv);
            if (idx == 0xFF)
            {
                return ERR_FAIL;
            }
            if (sphincs_seed_keypair(dat_out, dat_out + sphincs_pk_len[idx], seed, lv))
            {
                return ERR_FAIL;
            }
            *out_len = sphincs_pk_len[idx] + sphincs_sk_len[idx];
            break;

        case 0x02:  
            // 签名生成 [2B消息长度][消息][SK][1B等级]
            if (in_len < 2 + sphincs_sk_len[0] + 1)
            {
                return ERR_FAIL;
            }
            msglen = get_u16_from_buf(dat_in + offset);
            offset += 2;
            msg = dat_in + offset;
            offset += msglen;
            if (offset + sphincs_sk_len[0] + 1 != in_len)
            {
                return ERR_FAIL;
            }
            sk = dat_in + offset;
            offset += sphincs_sk_len[0];
            lv = dat_in[offset];
            idx = get_idx(lv);
            if (idx == 0xFF)
            {
                return ERR_FAIL;
            }
            siglen = sphincs_sig_len[idx];
            if (sphincs_sign_signature(dat_out, &siglen, msg, msglen, sk, lv))
            {
                return ERR_FAIL;
            }
            *out_len = siglen;
            break;

        case 0x03:  
            // 签名验证 [1B等级][2B签名长度][签名][2B消息长度][消息][PK]
            if (in_len < 1 + 4 + sphincs_pk_len[0])
            {
                return ERR_FAIL;
            }
            lv = dat_in[offset++];
            idx = get_idx(lv);
            if (idx == 0xFF)
            {
                return ERR_FAIL;
            }
            siglen = get_u16_from_buf(dat_in + offset);
            offset += 2;
            sig = dat_in + offset;
            offset += siglen;
            msglen = get_u16_from_buf(dat_in + offset);
            offset += 2;
            msg = dat_in + offset;
            offset += msglen;
            if (offset + sphincs_pk_len[idx] != in_len)
            {
                return ERR_FAIL;
            }
            pk = dat_in + offset;
            if (sphincs_verify_signature(sig, siglen, msg, msglen, pk, lv))
            {
                return ERR_FAIL;
            }
            break;

        default: 
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

/************************ ALG 0x08 : SQI-Sign ************************/
#elif ENABLE_ALG_SQISIGN
#include "../../include/sqisignapi.h"
#define ALG_ID 0x08

/* 公钥/私钥/签名长度（固定值）*/
#define SQISIGN_PK_LEN 65
#define SQISIGN_SK_LEN 353
#define SQISIGN_SIG_REDUNDANCY 148

/**
 * @brief SQI-Sign算法业务处理（CMD:0x00~0x03 共4个接口）
 * CMD00: 初始化
 * CMD01: 生成密钥对
 * CMD02: 签名生成
 * CMD03: 签名验证
 * @param cmd 命令号
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static int sqisign_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint16_t offset = 0, msglen, siglen;
    uint8_t *p_msg, *p_sk, *p_pk, *p_sig;
    *out_len = 0;

    switch (cmd)
    {
        case 0x00:
            // 00: 生成密钥对 [PK+SK]
            if (in_len != 0)
            {
                return ERR_FAIL;
            }
            if (crypto_sign_keypair(dat_out, dat_out + SQISIGN_PK_LEN))
            {
                return ERR_FAIL;
            }
            *out_len = SQISIGN_PK_LEN + SQISIGN_SK_LEN;
            break;

        case 0x01:
            // 01: 签名生成 [2Bmsglen][msg][SK][sig]
            if (in_len < 2)
            {
                return ERR_FAIL;
            }
            msglen = get_u16_from_buf(dat_in + offset);
            offset += 2;
            p_msg = dat_in + offset;
            offset += msglen;
            if ((offset + SQISIGN_SK_LEN) != in_len)
            {
                return ERR_FAIL;
            }
            p_sk = dat_in + offset;
            if (crypto_sign(dat_out, (unsigned long long *)out_len, p_msg, msglen, p_sk))
            {
                return ERR_FAIL;
            }
            break;

        case 0x02:
            // 02: 签名验证 [2Bsiglen][sig][2Bmsglen][msg][PK]
            if (in_len < 2 + 2)
            {
                return ERR_FAIL;
            }
            siglen = get_u16_from_buf(dat_in + offset);
            offset += 2;
            p_sig = dat_in + offset;
            offset += siglen;
            msglen = get_u16_from_buf(dat_in + offset);
            offset += 2;
            p_msg = dat_in + offset;
            offset += msglen;
            if ((offset + SQISIGN_PK_LEN) != in_len)
            {
                return ERR_FAIL;
            }
            p_pk = dat_in + offset;
            {
                uint8_t *sm = dat_out;
                unsigned long long smlen = siglen + msglen;
                unsigned long long out_mlen;
                memcpy(sm, p_sig, siglen);
                memcpy(sm + siglen, p_msg, msglen);
                if (crypto_sign_open(dat_out, &out_mlen, sm, smlen, p_pk))
                {
                    return ERR_FAIL;
                }
            }
            break;

        default:
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}
/************************ ALG 0x09 : LMS/HSS ************************/
#elif ENABLE_ALG_LMSHSS
#include "../../include/hss.h"
#include "../../include/hss_verify.h"
#define ALG_ID 0x09

/**
 * @brief LMS/HSS算法业务处理
 * CMD00: 生成密钥对
 * CMD01: 签名生成
 * CMD02: 签名验证
 * @param cmd 命令字
 * @param dat_in 输入数据缓冲区
 * @param in_len 输入数据长度
 * @param dat_out 输出数据缓冲区
 * @param out_len 输出数据长度（输出参数）
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
static bool lmshss_update_private_key(unsigned char *private_key, size_t len_private_key, void *context) {
    (void)context;
    if (len_private_key <= 64) {
        memcpy(context, private_key, len_private_key);
    }
    return true;
}

static int lmshss_proc(uint8_t cmd, uint8_t *dat_in, uint16_t in_len, uint8_t *dat_out, uint16_t *out_len)
{
    uint16_t off = 0, msglen, siglen;
    uint8_t levels = 1;
    param_set_t lm_type = 0, lm_ots_type = 0;
    struct hss_extra_info info;
    *out_len = 0;
    memset(&info, 0, sizeof(info));
    
    switch (cmd)
    {
        case 0x00:
            // 生成密钥对 [层级+配置参数]
            if (in_len >= 1) {
                levels = dat_in[0];
            }
            *out_len = 96;  // PK(32) + SK(64)
            if (!hss_generate_private_key(NULL, levels, &lm_type, &lm_ots_type, 
                    lmshss_update_private_key, dat_out + 32, 
                    dat_out, 32, NULL, 0, &info))
            {
                return ERR_FAIL;
            }
            break;

        case 0x01:
            // 签名生成 [1B层级][2B消息长度][消息][SK]
            off = 0;
            if (in_len >= 1) {
                levels = dat_in[off];
            }
            off++;
            if (off + 2 > in_len) {
                return ERR_FAIL;
            }
            msglen = get_u16_from_buf(dat_in + off);
            off += 2;
            if (off + msglen > in_len) {
                return ERR_FAIL;
            }
            siglen = 1024;
            if (!hss_generate_signature(NULL, lmshss_update_private_key, dat_out + siglen,
                    dat_in + off, msglen, dat_out, siglen, &info))
            {
                return ERR_FAIL;
            }
            *out_len = siglen;
            break;

        case 0x02:
            // 签名验证 [1B层级][2B消息长度][消息][2B签名长度][签名][PK]
            off = 0;
            if (in_len >= 1) {
                levels = dat_in[off];
            }
            off++;
            if (off + 2 > in_len) {
                return ERR_FAIL;
            }
            msglen = get_u16_from_buf(dat_in + off);
            off += 2;
            if (off + msglen + 2 > in_len) {
                return ERR_FAIL;
            }
            siglen = get_u16_from_buf(dat_in + off + msglen);
            off += msglen + 2;
            if (off + siglen > in_len) {
                return ERR_FAIL;
            }
            if (!hss_validate_signature(dat_in + off + siglen, dat_in + 1, msglen,
                    dat_in + off, siglen, &info))
            {
                return ERR_FAIL;
            }
            break;

        default:
            return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

#endif  // 算法选择条件编译结束

/****************************************************************************
 * 帧解析与响应处理
 * 协议格式：
 * 请求帧: [0xA5][ALG][CMD][LEN(小端2B)][DATA][XOR]
 * 响应帧: [0xA5][ALG][CMD][LEN(小端2B)][状态码+数据][XOR]
 ***************************************************************************/

#define DEBUG_UART	0
/**
 * @brief 帧解析主函数
 * 帧格式：[HEAD(1B)][ALG_ID(1B)][CMD(1B)][LEN(2B)][DATA(NB)][XOR(1B)]
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
int UART_FrameParse(void)
{
    uint8_t head = uart_rx_buf[0];
    uint8_t alg_id = uart_rx_buf[1];
    uint8_t cmd = uart_rx_buf[2];
    uint16_t dat_len = get_u16_from_buf(&uart_rx_buf[3]);
    uint8_t *dat_in = uart_rx_buf + 5;
    uint8_t xor_recv = uart_rx_buf[5 + dat_len];
    
    // 重置接收索引，准备接收下一帧
    rx_idx = 0;
    
    // 帧头校验
    if (head != FRAME_HEAD)
    {
    	UART_SendData("FRAME_HEAD ERR!",15);
        return ERR_FAIL;
    }
    
    // ALG_ID校验
    if (alg_id != ALG_ID)
    {
    	UART_SendData("ALG_ID ERR!",11);
        return ERR_FAIL;
    }
    
    // XOR校验
    uint8_t xor_calc = calc_xor(uart_rx_buf, 5 + dat_len);
    if (xor_calc != xor_recv)
    {
    	UART_SendData("CHECK_SUM ERR!",14);
        return ERR_FAIL;
    }
    
    // 业务处理
    size_t out_len = 0;
    uint8_t *dat_out = uart_tx_buf + 6;  // 业务数据从uart_tx_buf[6]开始（跳过头部5B和状态码1B）
    
#if ENABLE_ALG_MLKEM
    int ret = mlkem_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_FRODOKEM
    int ret = frodokem_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_POLARLAC
    int ret = polarlac_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_DAWN
    int ret = dawn_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_SCLOUDPLUS
    int ret = scloudplus_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_HQC
    int ret = hqc_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_MLDSA
    int ret = mldsa_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_SPHINCS
    int ret = sphincs_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_SQISIGN
    int ret = sqisign_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#elif ENABLE_ALG_LMSHSS
    int ret = lmshss_proc(cmd, dat_in, dat_len, dat_out, &out_len);
#endif
    
    // 组装响应帧
    uart_tx_buf[0] = FRAME_HEAD;
    uart_tx_buf[1] = ALG_ID;
    uart_tx_buf[2] = cmd;
    
    // 响应数据格式: [状态码(1B)][业务数据]
    uart_tx_buf[5] = (ret == ERR_SUCCESS) ? ERR_SUCCESS : ERR_FAIL;  // 状态码
    
    // 业务数据已在uart_tx_buf[6..]存储，只需计算总长度
    if (out_len > 0 && ret == ERR_SUCCESS) {
        out_len += 1;  // 加上状态码长度
    } else {
        out_len = 1;   // 只有状态码
    }
    
    store_u16_to_buf(out_len, &uart_tx_buf[3]);
    uart_tx_buf[5 + out_len] = calc_xor(uart_tx_buf, 5 + out_len);
#if DEBUG_UART != 1
    // 发送响应
    UART_SendData(uart_tx_buf, 5 + out_len + 1);
#endif

    return (ret == ERR_SUCCESS) ? ERR_SUCCESS : ERR_FAIL;
}


int UART_GetOneByte(void)
{

	//UART_RcvData(&(uart_rx_buf[rx_idx]), 1);
	//rx_idx++;
	return 1;
}

/**
 * @brief UART数据接收处理
 * 循环读取串口数据，当帧完整时调用帧解析
 */
#if DEBUG_UART
static const char* cmds[] = {
		"A50100000100A5",
//		"A50601000101A2",
//		"A506020FE7010C746573745F636F6E74657874001754657374206D65737361676520666F72204D4C2D445341DC9520AA87096DFA55E42EAFD2DAED402A46947591471814A5DEC86B86E68AD534CD0FB12CA848A7F05CCF3589251A58297A0E1206F0DBE7CB8C5D884543F3118FFAA5D9CAA1572CEE4B1495212BF6E93FA1952CDE2834946233FF0F24AC06589F85C91CC1596BCC701D6382749754C5C51B876349A71027616AEF70AB873D6B82268312117666812383153788563517647487706550686267223277768163387381246687670013866013378030700286000412133822010312585171674438020310455625367068131021006752535761724808481631553222611653050072757206467380565220628331418376477611674723447263858404352618271434111158652271868406102052676556475883586871548750124318318220175623774258161477752838810528371558453151680223472552108584055418134646375866253076186625760478272207568171475065377805711557314656706231611617227602501837580047361871124042584285611577075478277353640570766458725402143718676036483431721270112033855673615347242802847171674150077226765538185036335177141784378678230375376855377842470161310072122165561874212022208330075121344065750156481362765812335324187715738755362085644880383211421222812026342080261327148130526363043232741550726048834630025155031516161310568226615112543305116767848532406746480517342386542336326515744463343786632230444613575283242343244028548676054333124677615873616865047262202333252641082604713831181274530603035618551800818172280501266073084421017401878028225565811746054832437018804753446188583146332058151344734140675811805277317300362487683211350215777605777386620340720433033568784435865315417267070543573004712133740322314623436088036085440608051440335844168568513526653001131016646052828158852065152521546554476201188154218602013642141045076482873002416447234114137321607807005703463687467581788220443327222871503626878517228586710344484378626044526811634046180561473530831106177477772557480311244283122647417033133727404743352561676653181316611822776488801661833502317724043157056351471218374126807501043801785458844836435803865078662388316447010145871157124576523886715342487423372235035667740227654203570476161250551373635038660404025287650305561110048642638758840176465761877240221044037070665812614806630335132055718323103261335307843551573443680163216643233538787804462264025532570682588382415306847474755332578453811517776454013346161052706360365683536664778552155014683434507830627124114256101036574472376551723503435646077317047146347125040066818060556010588058547460221838535382184506108727374581774212431485625664663731816813133341184664376877085785466152261222273551518130824561714515156127227213236580305105160173216083842306720370515052253187485273650330658661113514646145312724753537545614582523101875035280213477651510682000850307827067302302261016230832601837681624270228032013644401637585778721860143557162334054546014226221866523147173872816872088658824153724586575168544438153577547474630126058786754430145168211538556112162606476154468535318335852584155301288312256524404130072766087464823572157287284578462843303235220805786528511705713700304678624472574422384340705663546761438043101411802083033654431266826144662403220577311201336247347245450834660788218652285736834032147038548530210678250382C56C8B58DD86712732344743E5A8646421B35CD0687351B5D5BBF5BD1BF9C458B52B658CCD48A573165C95AB3C4FE1B0A3D80B1AC7B8609BB4818E455872DBBA23F6FB20BEDCE3F593D508DDF87DF375B1D49DF83DD3D97972FFA5E5506860822C643FEA28E0D523F13439D0704F5EEE80B31774382D01F339D5050E41C42C91F472BBE43C7711D6F50F1611B7C6D286BB767134E7D89FE7C8E6A94D424F6E2AF6BCAC5AE17C8ADB209C55A2F8C28301D2E7F1542B518225FBB84003571D1733F7138947600F55714B56B3B0E4D1BBB7E4F063013134EA903EDF0FA834EB148FB00F2DA123F66EE677EE9313F01A76392742761D64987789A27442BF1D6832104941BFB3D9CB00378A10CB8C3D83918E1EE6DACA080BF715FBF30053D2029907BD09B28F02CE9DFC976938D3E3BD03CB100679D6516CAFF0F2E07C349437D6A069EF6BA23861255CF3CE7620F5AE5D5C8D1C6372983C4F04AC0E6B963DF1A5BA6414E062250F99F1F5092BE2EDD1BFDED849518361E120BB6CF2C884058A25253C914E3A0686075E45533F89CDD4CF23FF403531FAED9313360BE525E671697CD91EC6F6115AE1C4E22839007740E8C3D20435F5BA3890ECE330FC8DFC09FFD0B9A803B0A2375A46DF9866158D24A5FEC2BC686C0D10714D062440174E4C8D7CD86F5A27B469C82BAD9E5A49B73A09D4C0C6BEF6F1486ECDE80603EF887643443CC92B671E3E2B3B037F17FE88CAA082A34D9344EF730D704B6A9269941B9C5E3912B83C7E947C150D9BA3A4615F0A1CC1233C62B1CDE442B66E006D8EDC4C312D81070A6DD77ADD6C765EA3282E07C8BB8ED9CB6DF75C94A1F0D03C9FB65D570A86181F1A8D37B55CAB4DEA2A531BB7852CF68DEAD6FA486CC87CA340E8D4B5E1E3A98ACBB8121DBE508C7EAD68BC246EDC108F19F237F5F602A8528D2274271E6806B9A283525CF503407F4A92FFB2A95B5BF37048FEF229E44E2568A635AD49BE6985BA3CF82CF112B83C93D752342FFA4F588E435903E7DA29361D828CE91CFF8E750288989D4306DFAC5DAE45822CB306E895EAFCABCA722ECFAA9050660D5BA1F1D5170485C3E12B56FA9D152C5D8E1D06C1F255A3D109426426FB4CB4CDA5D5D43460FC939C27F2C9718341879A77F58266CACC490EFAAD23A67658FFE10E529671E202D13EC77F40B2F63626439D22672773C6FEE42E9B17E4577AFC195F84720B3680731CED92DCD89498894A72418CD4B6DEB1905DE90755556647FD3C6DA13229EF620F9AA689C9098E2C046B334A3569BDB4C45ACE6B60AD610DD302A66ED2D877A163907211051065EDDD4E33FC80F4D4C938CDE36685E00CDC8629BCAB94BFF407C5E0391F7CEAB656A952FC3857AA37E78D107ACB001A90ED3F7101CDDEB38E36879FA93C2FC46B42FCE163F49C910EFA2CB935D3EF0C72B4015F39BD8E5533D082B065E0B109A52E009183D49C75860B28985C304C1A7D53CF8907763A09F0D739FA4071A102870A3693C77FCE8E43DF27E9FE021C11A5C283D1C5BF30A2BD8F8CC624AAE0D760F62948C35DD27D5A50B7BA62BCC2C15AA7CD5F03049EE08B62D73306A8A15B11A57502973CDDE9B71E190295F0016319D3B04FC15FB45BEE26E2286D15020A8F3CE982A17505535DECB9081E384964BFBE2945673C59ADD8906C88D49FB0A0159C2C5A48FE840B3B3C4432EFA5E3E3F97989B2BE2F8FD911E571DEA3D2F227F92102B42D25966E1DF38E21DAC18ED9CA841763E6B60E5C8139433FC8FE7FA9BBB9AE309E87D472539FA944AFD03E97ABB0F29777F138E3650A862AFB31982F48566521D26F01E77024ABB26939FCFDFECEDCD7974470D312C0A2AC2E928E69F863C4B8A60E44CE50DB413D7A099B0546503C1BF91FBB7B2FB4F98C74616839C92527CFEF9DCD3D67E3DBABC1FC4053CF18013BE253305EB026627A3C98CF4889B0490088D712B9D911AB0952FB1DA704DBF24029E4FFBE74E02316BE031C414869C90651F940B68DD972AFC1A0A8B1FDA83755AF77D128E39846A5F0A56E94748145E01DBDC217ED7D207E74C0742BC5BC9315C9D2C1CE320B8A486BCC0AD4D05D11CE6829D9B023F6A7764F393D666453A95A7B521941F0371D5F4417EF71F26B526BF504F4D0E9676AAFFB5EB7F265D3D9C4D6E5AE02383EDCC0C362124A8A9192A4504C7F39F8A529B291CB73699013FC1B96BC35EEEFF33CEE1D87FB83771DBC1E3CA44C5BB17689106850DC776C67204FCF23B16545BFF4A267D2ED0E079683E0DBE167E60CB08A6C1059B19AB3E4B6F6D90CD96C617109247A84BF1DE21ECBCCB13497592EFD5BAF3CB0736D812B1D85B43D3456148138A845376F4DFEC7AF7BE18E090CE60C57D54C2E0662CFAFE183A93850F603BFCC244A86F5929DA5E3765D20CA59B587693619856290FD2585E627056E87C1E2FEC9C21BD51D20A5E271C891B630D3A92B335C25CE38670D3F4872A1A14DB51DDBB4270BA1C8AA4BF09345646C4A61827BEC5CAD5F00FDDF805F6F2078826B28E272B7F0A9EA34C59C910E66A7C3713376E589B5FB27B62C35F51ED7486D2FD6C714BB40C233C8498990694F1E533E9FE8A182ED01AF3798289AC0E3BFAF95F45C94B60674420FF3690B9E174D33898A6E92D251242F1E2FC9AAF88B64C6BD5C4F0216F825C7E4CC013FCB14E025D544CA851BE6622FA334594B6A2E2243038357AB2981F75A6FE2C9BE467BA5F9F227AACDA08D4BC803DF094D21923BFE1111A92DFB5883BEA52B47B12385442B65B4BEF0B8DAB8D105C452A53D9FF3AEEFC5A5F9B28301C70599B0B18DCE137C6042EBCC4D40D971D7EEA0BB200381EC7294D2583650AB7A6B38AAAF91109CA6069077BFE2E670C45AE02CEDFA548E8199875FCD227C9F1239B9C778865A365277F600AFF6567F2FE0B8578F72DE187E9F449D3E31D0AD9A38E2A3A3EC60415E760CE827395E9B43FFAB2D63B704490E8267316E5067C20966CD29CFAC14E3B1AB6EF69FF0E4482A9575F65513E57BF7D126873A1056F57BFA510104652E1FB992100CAFA375C576ADD953E4F44AA22571AE24AA2C66A2DBDDE2C6ADD75263C49FB03ED9BAEE3E4B3DF217CF3739AF73545AA4F167D21D46DB9631ADF51714D9BFBCD0A51B00BF44F68359E7A55D8EF4B65F716BCBD2B9AFCDCB34895AC408C81088DF9C748D75AFCE090BF5C45E4590F49477E6C8F0A641A30264761CE88C659B079412C2DEF4E9125286963A0C29534DFCC43F42972146FB87BCB73DC2AF349FB2904D1E2D99EA31FE86B30F848EB250C891858CBC0E2B61B8A62CD2AF601CA61463ADBFF595411D044E912BA861D7C65F131724FB0AA029CC56602460BC1276A5AD0953AC764FF44F07027FBCFEF43516DA40E8C4E2D6A4097F0D60B38477076A834691A27AD0B6D6C82945CEADAFDBF98DFC74AD7173E9F3977491C6208960C5B5FFAF7851C5F5C701B60C305495259D7918515E19471FB4BABE63CAAB5D206CA57EF9EC56B7A49",
		"A5060314B4010C746573745F636F6E74657874001754657374206D65737361676520666F72204D4C2D445341D07D600E4B90692756C771CF0D511FFB8E05AA636D8E56264BC54E3C9D4B42701D8AE80794B25824A25309282D6543475B21C1068BFB167130C07F7305E4ABB6E096FFA3BBCEAC626BDC693F92FBB884FE7A57EEBD5D49CA462DDE25513ABDA1460A5A7D3812BAAD4ED4F55B700972AE644002610DCD0A203CEE5D8A555DB3B8CF025F440F8427B15A28F22824EDABA6D92A67A11855D78ED44BB7C51F8967B87FF5089D7DEE9BD4ED3CF2CFCBCA82B06B2B4A30D9565C56F7266B6663159A5166B32F494E9773347521742105602D1E01C8FDFB950BED0C20078752DFE906996C4F8C2F707239AC83CB488FFFE767F3F6867FB5C526DE31E613E562B7D1E0B1E6C0EDEA37609B0704CF1323F22F0AD2D1BA146A02445CBEBEE6CB19B91F3D8F968473123F05ECAF5A767E64ED4A88B9E43FDA82EBC1C18F565BB1756E512396AEF065456ACECB3565F3A52F81650F3EC54EEAACF907D57E4462AEB71AEE98014AF92192C446B54DBC6BF2FA52793226F5CCE2909BB7A28E3033F9405D5974B03FA450CF33226D4F7E23A334CEB792EFED084272F9FE53C361F83784DDEC8289D6A9E00E317ABF1B264D68D1A6457F5509A6F37AF0BD61471D1A88C6FAD87F449137B2279205FFA8D06A21EC9A8025FCA31B732BF17FEB2476A928192AC3A096DEC7DFFFD1EA038CBC7CBB8339744EAF6B36574B75D38204C7060D92068B3052F557046F1DED0363CDCC22A0E0C09F51B55271F810A4DFBECA3F3EF64AAD408D0AC2166EFAB49D974BDDEA3407D37E3E6FAF4EB34AEEE0133D88057AC90CE5625A44DB2E7E7C41D0A52D4E6AD98230AACDE4DF10A3DEB657B095CEE66CA8C800B83A9445DC51F44858373C0F86E64519D6C629E13DE7EF562E5CD0972FC2A9D625CD097EC42ADBCD0E79F97CEF0060F43A780F0C1EF3C65AA29B92C0AFD336EF84239567E0307F0EFD4E464A1AE09FF6BCB7A082A72B5A038BB53E3AABA477C40C6165269F03BC1F2C7508282434F42B9BCB067F07FFCC857373447BD98D6BC9AC87AD4BEBC40EE567DEE9106F7E54588F47BBC053A90CFF8716BF9D4FAE5526128BB87CEA83B8E7A7264FCF0661E7D825B3EE966CEE5EA3A4C697DEB474E359892D1F3CAC7A0F117DFE3796B3F09395853654EEAF56770C5CB8D650FE575C0443C3CDF1153C1F672D8F16B3FA60C9E52C1E63E6506072B813B199CBD1398F158C4EEFA384D246ABEF8BA829D8C333D7BFDEB23E315648A8AFD09A4879E46A6477D609E6C846F94A318AAD0E54A798471C2606E14779FED3F66043784E9E42982793C4BCCB3B8AE3623E361079CB3AF8CB10F6E8A23F548E81FD603788C70B553D03569CA6E486CB2DD9ED7F2F680B5CCD8F62EB3BA4DACCAD1274CD0CED8B4A3E419678A7552803827C80D5395770CB9D56DE2044DBBE9F9C6D2B592FD2FC9909063A4CEFBF6AA836FE682ECCAA2527654FA3B925745B4536E1496273F0F9FFB7670F9177C6813CCD19771767434DA6D36FA1551333A5E718EE4D7FCF50C5F10DDED779BD034E9DCA11E3CE43D2F9776FBCA75CE03093D8F9FA326FAC14BF3E00A63F6FD2A2D156D6051F86F804E3D65270C5BD3E73FFD0B37A8F36C36B0F5A0F27EE33E5BEAB84A3E21395330B4D5D76AD19EE83F655D71368210DB55108725385C6AC1FEC4CE59BB5F6C74F0E56A2E6D1BFF62E841FECEE9FACEC12D755517F25046B03E5E10554DA067F29FE96482167ED777D34C1D2F30BFFC49C49C3EAA7EB3B41E64A55837F83608A816D8E24800F3461A4DBADFF1E46AEC99FF4A4B96F5016509A2CB469356762CD7F81DD845EDCAF37BDD58626526D207D2F400ECDCB84C205429E330A1F045E732E3FB95A94EA89819B3E814354F512561BADCFE8C6948D8AC9CED49408D05072C3C56679926EC2A9C13D8AB56EB33F9AF05139D8FA279492BE162C5EF3D78008B574A8FA83AE3496AE081C40301D73F3883B75971372195E783CAE5F910F339416DAD88523145B80BD2B9C4FAE5A9DEF95D12257C0BB3F5270E1063B4ECDDBAFE639A3DE40A8CF249C657BAFD7E28B160DBC351D187BCD6CF300940B42F8871CA0399D53A4A193CE643AE9DD70E7B51ACE21FA7BF0B2E3E170DA2F42F011F966A55ED411AFCDF4BEA908796C783E5A7EBF528E9C5A1E470FEC84C2B2DA5DA80054F938D8D17BD8AB267235C668B24C28264B8D4DA78ACDEDC8ABB4DCFADA71607FCB2F3BD3D42DB35B86FD8B633E817AA33D5221BB73EED2F3FCAE61AA9AD734782DCBFD5D48E48B87E18C1D03C4BD5A8E3A0ABE1E1FC1574C74CFCF46AAF7CE8C0D4996AB835D30B42110B1FDD5CD83C865B5712E0AAD07208B351FF0A6B432F140CF05EF32462996A8A6C681B9D59A226DE007673F1743C0792368CB6F6829B994CF7C1E6019CC4713FE5BBA86D0FE04E8CA950BA430453BEC0863E10BFC6BA13B185DE159456C864C844F68AF1EB14FFE478644DDA67175441BE98120DF12D3B64DB0CC4CD754EE4B414BAD0CADDAE6A9A927C9583F7D96B309DA74208216A35711A4F00BFDCC6F3C558C5FC750032B477546044650475DF430F5D9C3AD7203BD51232E64FA2849BDBE60933E17FD7C2EA2BBC58CD228CB1D0E4D02602A317A3A1EABFA9A98F5B119971447DE0FD8278C1D7C1D2CEAF2AC28EB947F5B5FA2958714378BFC18CE2B27BEE3BD7F221A8FBC12496B3B7E8C7D7A83283DDA64B6F27BF67D977DCB4D515125F4B67925FC052D5416876C61BC7451CE3808AA7912DD47FBC48225F423426C9796E7DDD9B7230C2640128B85CAC188876E8B38C93F32F7111BF834D41F839C51B1F282C7933C428813945D4F413A75C39FC005D49D6D65B39F726AF7E3A82A4B678D5A8C75ABC6DAC6B2FF0CE1B0AFDA90CF370694F091D2593BCAF82BD9A813BBFE59B1F54D008C3383326C171B768E93174B0C4B705917BB43C3A4F430BF4F0C712E0FACCCD48779E0937F0F462D8C59CD61BD8D97245346A507D23CA61E7946CB699110E3A15ACCFF6728A6966EEDE0C871AFDAFF8AF0A518404A37186009E79A4E91C4789A18CA4A51307551634608844AA9818064E7EDAA877EDC914B1EFE5512269EAD5E2739E40EF4BC90AA83D2DA27E2C7E4C41D7CF5504E9C68B807C151F4C641C75A53D325644684630298E777F049C9C604A6062C625B29A5674D3EACCF5B4F1FDA864E21C765D077E15D90F29303DC808C9D9F437FA89069865CEB74407710C821F40574989D7CFD18DC87A542BE072947C900493B6C61B6DA48130D21AFA5B2AB6BCC6CBD951C2DC828C5965D483E079929C3CEFC0ADE8F274DFE36057C51CF5C93995018AC93033B1AB5578F4AEF72E22EE82F5848D8E6E40BB6F90ED835B3D633D25840933DFC50F52A2140A85CD2D2ADDAA48D0818CD052BC478D269E7CA26FAFD3D3AF9B410FA18BAFFDE4F3BE1BA9D60CFB715AE6165DE64748084C455490158E9DEE123A184E5A60B02727887CC8D543F9806347FEA477A0B99CB67356A307300A47DBE70D686E9C452A1A9F98297D9306000FAF48F9B9107ACAADBC00FB50A86DD574A25B19288BB265C78E4BB642E2BC20F3C9064472703ADA38B825C38456DE5CA60B433539FC8F04A10981B56CE7809C5B1929C255118234AC36DD9F4ABE0C6AC18CF1AE58C7E20451E5CAB7973DB70ECBB64702130AD72D61E35FA8D40F037B60CC99F1CB0ECFF46C06283E9E6E902DA41F4157AA72D4D772F2BD2E9C5045261EA8E6C6B111F32247BA6FF844FEEAB7786888402CC641418A0A5953BC572F82710341B63565A36D88EB5E3601754E84875ACC86F3633EEFD7366C99FF14A3C7D603AD87D828D534E4F5E043BB3C662F502B53DA01C4847904B403D52FCD3CC97E5D532F89F8E0FEC2B1F419188893BB12D2DF6DA0453BDF2E8298F398DA89BD23C14F30C972AA03F5F6F58CC0B8C9794E7AD5E4F414F68191C2B1D02CCDF4850117BFC435BC2E234EFA495A10E5F399189A3DAC450F38542128338F2EB83DDFE5FC758EB2A2EF426784585089368C480E7BDAA3299F10B1E193203C8378DE52C45C9E4EB1F17DD84F35960177F515CD61B7C8825CBB41041F2F2BC3211BCCECAEB07B348DC33B51D0DD1655429B183A6F863A1CF753C8E3E4DAB5D049885B40EACFC25EE3D5A333DDC0F63C029EF7EA58D29B9438B89402982622EC17E7FF3588E6B995753BB902418254D2474452A5021CB6E19FE5942D8FBED67C2AACFAB877C68F2B2718403C3E6E17C0C34415ED11B3311E63156FD37551311D8880BE46AD34114BAE854055BD5E3CE73D3A2908783A972B3C2E00E37D253F9990C1585ACBB6099F15017A6463D64C602AD6380C0C832B776E8717BBA855FB6A5B8013FE8965AA6DA65ECA320FBD552A3740E5F050A50DEE005CB0BA7CCA59762FB7405F7751683A5D5B0A1E9BB1723B3D1F1275A77C8CFF4EE67A14191FD87DB351A541005E80FE530D181993129655ADCDEA236C3AA6B3EE79A3403AFBB6A7CA875E20946B575E06B461F6EC8711C3D03558E6EFF4BA7A277E02C2769FA2D7FD84285D35CBC5CFC1F8263D3D6864934E30C6AB0C4CB3D6A6E797A7EA1AF2F4A8593A8C6CEE7E894DA2A5F6285C2D8DEEBFA091A353F6E8598A5B1D4D6F800000000000000000000050D1618212DDC9520AA87096DFA55E42EAFD2DAED402A46947591471814A5DEC86B86E68AD514A2848DB54ABE4ED5B4232A3AF7F04B52FD87DD76A571D3FA00A84CE3874CC557FA2A13B39F0384141BDB3318F9B0E013C66856D0A253807494C69FC45C48FC347025FD6EC4CC14D7A27DB33787FBCC6B2874272537465CA72FCDAF6DA3BDD9D07477D7B033E4D53FDDEE5D979CB2EFE1438F52C4C38B27AB1CD63E2376B21AD66ECEC54A2CE78B130653014A1431391329BBDED1BC6874572E04D71AE5B7359EA62BBF88106145C40C4A2C2A45DF7E68AC6E114169CB6A38CF60B43886DD3D7E818AF540162B91E25CA5A786CC2F786610642D2C73326E72B62DB64BEF85391F2856024C779423AFF2F323AFBCEACF48FDD7D49FB16AAFB9854BCE0DFC45CECA310AEA4A66FB92F75BAC288AAFE833B6F1016F8E5BE26F4C411FDBEE660C3E5AAC87FACFA3319FF494C07B18667C34828BB1328F7FE722854187CBDFD416BF514E8B3916CA1DE4C3DAF32903D22CB836E99AD27294A08A1CA529E0CA55E0ADBAADA767D1AED0501BBE4F4D6EB5548BD51B19B5A3ADF90FB29B2EFB7A69718B93EB205748751C27BFA2D1106F8AD81F2B3EFC876C1BEE8A8BB659F7A4DB2DEAE51D79D5225BFF0EDB1928283A7EF79032F068E526D63232EE37F4B88BB924D12730C0C1E474E6C21AE433EE0F63F9FE1923F83E1D3A0AE32514DF48978FA7A5898F81B8E579E655254ED7A529D499E1175C2D28A7EC3D69AD50CDAC4A1AF7C230F27F84D3315DE02D8BE0969FBE5D389238CE769A00C19CD4C7B2D230239A6C96AD1E4DDDD9374112B167C9E78D4BACA6A315A5FFF87EA13B2443EF7CA47C7B815C2111916D5454B1AD78F937716F8E03A1C5C040DAF7B08EC63E2D7C32B730A7125E603E3C773DC9F7EC36538D731EAAE5957F921D2E81C9DD8C4AEFC30113F1BD6AB102A67D5315EC0EFCE268451A3A08F9A5A11758FBC5D9FC66E96262475B71647EC70619343133821D72390ECEB97C6516FBEB840ADE0D8E7420993E339550617E544FE39317970566079D4BBDF666E85FC697A123BFD57F253AD959BB4E44D95413F3C847340E5412E30D6012FF7348BC47A872EF708C850586BC7D045AD4636E97C703E5D6E84920CD10465540FEF2AE9480D7D113442B5E9A444BE5CC9C55C3684F219C2B63692130D22EE5F1C5EE969C5C871C25B734B9A02C01D499BF7586FE6E9886370F5E0A5D96DA9B790D71A541B1F99FD5DD752570A2CF64144E896FD1CCD52C3E712915C8D65401E638F5225C9F13C441D6B00F6CFA7ABABB2C6F444B2FE000AE595D8185898E2E441A5CDEFE48DD93C212BB58FD1BA37C84FB2DD388E9EA37419CB666BB53A487BC1F7B63ACE6C83A10F70C208BC7A3DC4C4E09C3FDE7842F3E3EFA8F4D3905058F63E9E000A3F740F095CD575CA2BFD5FA490CDAE8884E24A4BD9627619A34F71947D5CBC9520A96036B5004651C894C107A218AE2D2525267934010C35FF9FF76A0AC90E6EBDDC1CF36CFF337A9FDAEF164BF7CC58DFE2BB09A7D7E1DAD574816D6A993D4F631B4A884E1D60D82C25240E644F27B44CC230C2F1BAF87F02302771DE98769CBD342EF705A2D5F8D86FE2E2FA7E36C4B23037A27740B1052D64C2C2DF57791A84E0B6221E7D46D59EC089F8EC2A512822A69B73B408AD0A2FB8E8D45929B9C2D744F39F1F0C25C940B518336A86D7A6AD11D797AEC5020333D50884B8FA4F24A53FCD02254F862071F306AC42B9862BAEEEDE03B94FD5F8365725FBD6D673B90111D68FA2B59F8074008C597C8A7E39C453902B95A2B52A17E5A88B9A87A1DC32783788D2E75146E77F60E40677BE7C31B34222ECBA5A982649E535949903890209A998A5A53DBE6B39D8E70DA1FC83C6C6876AFA7985DF5115C6E96DDF68533E63108F5A714F02094730E17C5BCD2AECDBE2DBF75F76CE64A7046D3148C79B57B9C3BD29BAD1FB7D964B8A0567F173639515E1AE25785E71B33D48192AA58CC84D95D0D57D2BE77752FBE58D6082F83DE82CF35DD31E7DFA2B503C5C3CAABCE11C808C1E69E7C75FD7C7526F3CC48A034230042743203810353CCF46E603721343F5C0C16B9FB6E5F378C44186460B898533A93F265F4A1CA89BFCC3373860510F1713BCFD222E853B62F91D8C77F7A047C95780FD6DD033BD458B3434F1484F6B111BD2ACAC1EA54D22C655DC8D6F8EDAD517055DE71B78734DB9EAC4355CA022B13F7B9276D5AD380222D9F4406BBAA4AB223FE2498876C476A78E21BF7664C9D0911F442E3511410497165471D6263EE19A1613D94C3D6D75FF88F99DAB3E0A6CDD928DAD093CF23E7749BE8E734884CD9ADD53C0775B2263657DC7312E4C311C84A4A29DC94451EBB1F93E2E710EC7C1C80572C1B88E1933259C79BB75D514F4B56D5532FF51AC8DEE94464F2DC9C67CF30E0ADA4E6E7C5B8F7141D0221E384C2220062D3C818C49F4475CC57B7E7436312C6F994110A872399FD82801C9B64C7D6960DA5B00BFE759E7ADF071E491D8928ACA08738ABBBD6AF1EA97BB3F24A365FB16B11827DB85960BC11BA7099DAA87BA09CF51B2330D024371739F72B12B9A6ABC32ACA0E2043C7F965EF5481AD0F81C9063957B05C6B92F74859BFD9DB59D1A4A892F929BE38C3DE30BFBAA9EC7044A910413624D7A749271E164EA75C0DC9C6F00D6EF375D48BB8029FD939543E9FE87FB91434C7B9A31629035D4AAC57402B14524BBA7521"
};
static size_t str_to_cmd(const char *str)
{
    size_t len = 0;

    while (str[0] && str[1]) {
        uint8_t val = 0;

        /* 高 4 位 */
        if      (str[0] >= '0' && str[0] <= '9') val = (str[0] - '0') << 4;
        else if (str[0] >= 'A' && str[0] <= 'F') val = (str[0] - 'A' + 10) << 4;
        else if (str[0] >= 'a' && str[0] <= 'f') val = (str[0] - 'a' + 10) << 4;
        else break;

        /* 低 4 位 */
        if      (str[1] >= '0' && str[1] <= '9') val |= (str[1] - '0');
        else if (str[1] >= 'A' && str[1] <= 'F') val |= (str[1] - 'A' + 10);
        else if (str[1] >= 'a' && str[1] <= 'f') val |= (str[1] - 'a' + 10);
        else break;

        /* 边界保护 */
        if (len >= sizeof(uart_rx_buf))
            break;

        uart_rx_buf[len++] = val;
        str += 2;
    }

    return len;
}

void UART_Process(void)
{
	int32_t cmd_index = 0;
    while (cmd_index != (sizeof(cmds) / 4))
    {
    	rx_idx = str_to_cmd(cmds[cmd_index++]);

        // 最小帧长度：5字节头 + 1字节校验 = 6字节(DATA=0)
        if (rx_idx >= 6)
        {
            uint16_t dat_len = get_u16_from_buf(&uart_rx_buf[3]);
            uint16_t full_len = 5 + dat_len + 1;  // 总长度(含XOR)
            
            // 帧长度满足时解析
            if (rx_idx == full_len)
            {
                UART_FrameParse();
            }
        }
    }
}
#else

#define UART_BAUD_RATE		38400

void UART_Process(void)
{
    uint8_t ch;
    UART_Init(UART_BAUD_RATE);
    UART_SendData("Uart is ok!",11);
	//UART_Init_Syno(9600);
    //while (UART_GetOneByte())
    ch = UART_RcvChar();
    while (1)
    {
        uart_rx_buf[rx_idx++] = ch;

        // 最小帧长度：5字节头 + 1字节校验 = 6字节(DATA=0)
        if (rx_idx >= 6)
        {
            uint32_t dat_len = get_u16_from_buf(&uart_rx_buf[3]);
            uint32_t full_len = 5 + dat_len + 1;  // 总长度(含XOR)

            if (full_len > 0xFFFF) {
            	UART_SendData("Data is too long!",17);
            }

            // 帧长度满足时解析
            if (rx_idx == full_len)
            {
                UART_FrameParse();
            }
        }
        ch = UART_RcvChar();
    }
}
#endif

