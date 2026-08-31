/****************************************************************************
 * @file uart_test_interface.h
 * @brief UART测试接口头文件
 * @details 包含10种后量子算法的UART帧协议接口定义、宏定义、算法参数表和函数声明
 *
 * 算法ID映射表（严格遵循最新文档）：
 * 0x00 : ML-KEM       KEM算法
 * 0x01 : FrodoKEM     KEM算法
 * 0x02 : PolarLAC     KEM算法
 * 0x03 : DAWN         KEM算法
 * 0x04 : SCloud+      KEM算法
 * 0x05 : HQC          KEM算法
 * 0x06 : ML-DSA       签名算法
 * 0x07 : SPHINCS+(SLH-DSA) 签名算法
 * 0x08 : SQI-Sign     签名算法
 * 0x09 : LMS/HSS      签名/调试接口
 ***************************************************************************/

#ifndef UART_TEST_INTERFACE_H
#define UART_TEST_INTERFACE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * 算法启用宏（10种，仅允许一个宏 = 1，其余= 0）
 * 编译期会进行强校验，确保仅启用一个算法
 ***************************************************************************/
#define ENABLE_ALG_MLKEM        0
#define ENABLE_ALG_FRODOKEM     1
#define ENABLE_ALG_POLARLAC     0
#define ENABLE_ALG_DAWN         0
#define ENABLE_ALG_SCLOUDPLUS   0
#define ENABLE_ALG_HQC          0
#define ENABLE_ALG_MLDSA        0
#define ENABLE_ALG_SPHINCS      0
#define ENABLE_ALG_SQISIGN      0
#define ENABLE_ALG_LMSHSS       0

/****************************************************************************
 * 编译期强校验：禁止同时开启多个算法
 ***************************************************************************/
#if (ENABLE_ALG_MLKEM      + ENABLE_ALG_FRODOKEM + ENABLE_ALG_POLARLAC + \
     ENABLE_ALG_DAWN       + ENABLE_ALG_SCLOUDPLUS + ENABLE_ALG_HQC + \
     ENABLE_ALG_MLDSA      + ENABLE_ALG_SPHINCS + ENABLE_ALG_SQISIGN + ENABLE_ALG_LMSHSS) != 1
#error "配置错误：仅能启用一个算法宏"
#endif

/****************************************************************************
 * 算法ID定义
 ***************************************************************************/
#define ALG_ID_MLKEM        0x00
#define ALG_ID_FRODOKEM     0x01
#define ALG_ID_POLARLAC     0x02
#define ALG_ID_DAWN         0x03
#define ALG_ID_SCLOUDPLUS   0x04
#define ALG_ID_HQC          0x05
#define ALG_ID_MLDSA        0x06
#define ALG_ID_SPHINCS      0x07
#define ALG_ID_SQISIGN      0x08
#define ALG_ID_LMSHSS       0x09

/****************************************************************************
 * 全局协议常量（遵循串口帧规范）
 ***************************************************************************/
#define FRAME_HEAD          0xA5        // 帧头固定值
#define ERR_SUCCESS         0           // 执行成功
#define ERR_FAIL            (-1)        // 执行失败

#define UART_RX_BUF_LEN     65536       // 串口接收缓冲区大小
#define UART_TX_BUF_LEN     65536       // 串口发送缓冲区大小（需容纳最大响应数据）

#define FRAME_MIN_LEN       6           // 最小帧长度(含XOR校验)
#define FRAME_HEAD_LEN      5           // 帧头长度(不含数据和校验)

/****************************************************************************
 * 命令码定义
 ***************************************************************************/
#define CMD_INIT            0x00        // 初始化命令
#define CMD_KEYGEN          0x01        // 密钥对生成命令
#define CMD_ENCAP_SIGN      0x02        // 封装/签名生成命令
#define CMD_DECAP_VERIFY    0x03        // 解封装/签名验证命令

/****************************************************************************
 * 安全等级枚举定义（由各算法头文件定义）
 ***************************************************************************/

/****************************************************************************
 * 算法参数长度表（extern声明，定义在uart_test_interface.c中）
 * 测试脚本/上位机可据此构造和解析数据包
 ***************************************************************************/

/* ML-KEM参数长度 */
extern const uint16_t mlkem_pk_len[3];
extern const uint16_t mlkem_sk_len[3];
extern const uint16_t mlkem_ct_len[3];
extern const uint16_t mlkem_ss_len[3];

/* FrodoKEM参数长度 */
extern const uint16_t frodokem_pk_len[3];
extern const uint16_t frodokem_sk_len[3];
extern const uint16_t frodokem_ct_len[3];
extern const uint16_t frodokem_ss_len[3];

/* PolarLAC参数长度 */
extern const uint16_t polarlac_pk_len[3];
extern const uint16_t polarlac_sk_len[3];
extern const uint16_t polarlac_ct_len[3];
extern const uint16_t polarlac_ss_len[3];

/* DAWN参数长度 */
extern const uint16_t dawn_pk_len[3];
extern const uint16_t dawn_sk_len[3];
extern const uint16_t dawn_ct_len[3];
extern const uint16_t dawn_ss_len[3];

/* SCloud+参数长度 */
extern const uint16_t scloudplus_pk_len[3];
extern const uint16_t scloudplus_sk_len[3];
extern const uint16_t scloudplus_ct_len[3];
extern const uint16_t scloudplus_ss_len[3];

/* HQC参数长度（固定值）*/
#define HQC_PK_LEN 2241
#define HQC_SK_LEN 2321
#define HQC_CT_LEN 4433
#define HQC_SS_LEN 32

/* ML-DSA参数长度 */
extern const uint16_t mldsa_pk_len[3];
extern const uint16_t mldsa_sk_len[3];
extern const uint16_t mldsa_sig_len[3];

/* SPHINCS+参数长度 */
extern const uint16_t sphincs_pk_len[3];
extern const uint16_t sphincs_sk_len[3];
extern const uint16_t sphincs_sig_len[3];

/* SQI-Sign参数长度 */
/* SQI-Sign参数长度（固定值）*/
#define SQISIGN_PK_LEN 65
#define SQISIGN_SK_LEN 353
#define SQISIGN_SIG_REDUNDANCY 148

/****************************************************************************
 * 通用工具函数声明
 ***************************************************************************/

/**
 * @brief 计算缓冲区数据的异或校验值
 * @param pbuf 数据缓冲区指针
 * @param len 数据长度
 * @return 异或校验结果
 */
uint8_t calc_xor(uint8_t *pbuf, uint16_t len);

/**
 * @brief 小端2字节转uint16
 * @param p 2字节数据指针
 * @return 转换后的uint16值
 */
uint16_t le16_to_u16(uint8_t *p);

/**
 * @brief uint16转小端2字节
 * @param dat 要转换的uint16值
 * @param p 输出2字节数据指针
 */
void u16_to_le16(uint16_t dat, uint8_t *p);

/****************************************************************************
 * 底层UART接口函数声明（用户需实现硬件相关逻辑）
 ***************************************************************************/

/**
 * @brief UART发送数据
 * @param data 数据缓冲区指针
 * @param len 数据长度
 */
//extern void UART_SendData(uint8_t *data, uint16_t len);

/**
 * @brief UART读取一个字节
 * @param ch 输出字节指针
 * @return 1表示成功读取，0表示无数据
 */
//extern uint8_t UART_GetOneByte(uint8_t *ch);

/****************************************************************************
 * 帧协议处理函数声明
 ***************************************************************************/

/**
 * @brief 帧解析主函数
 * 帧格式：[HEAD(1B)][ALG_ID(1B)][CMD(1B)][LEN(2B)][DATA(NB)][XOR(1B)]
 * @return 执行结果：ERR_SUCCESS/ERR_FAIL
 */
int UART_FrameParse(void);

/**
 * @brief UART数据接收处理
 * 循环读取串口数据，当帧完整时调用帧解析
 */
void UART_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* UART_TEST_INTERFACE_H */
