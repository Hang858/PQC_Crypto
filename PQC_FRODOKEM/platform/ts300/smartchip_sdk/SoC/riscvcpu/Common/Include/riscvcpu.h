/******************************************************************************
 * @file     riscvcpu.h
 * @brief    RMSIS Core Peripheral Access Layer Header File for
 *           Smartchip RISCVCPU evaluation SoC which support Smartchip RV/RVX class cores
 * @version  V1.00
 * @date     22. Nov 2019
 ******************************************************************************/
/*
 * Copyright (c) 2020 Smartchip Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RISCVCPU_H__
#define __RISCVCPU_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Smartchip
  * @{
  */


/** @addtogroup riscvcpu
  * @{
  */


/** @addtogroup Configuration_of_RMSIS
  * @{
  */



/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn
{
/* =======================================  Smartchip Core Specific Interrupt Numbers  ======================================== */

    Reserved0_IRQn            =   0,              /*!<  Internal reserved */
    Reserved1_IRQn            =   1,              /*!<  Internal reserved */
    Reserved2_IRQn            =   2,              /*!<  Internal reserved */
    SysTimerSW_IRQn           =   3,              /*!<  System Timer SW interrupt */
    Reserved3_IRQn            =   4,              /*!<  Internal reserved */
    Reserved4_IRQn            =   5,              /*!<  Internal reserved */
    Reserved5_IRQn            =   6,              /*!<  Internal reserved */
    SysTimer_IRQn             =   7,              /*!<  System Timer Interrupt */
    Reserved6_IRQn            =   8,              /*!<  Internal reserved */
    Reserved7_IRQn            =   9,              /*!<  Internal reserved */
    Reserved8_IRQn            =  10,              /*!<  Internal reserved */
    Reserved9_IRQn            =  11,              /*!<  Internal reserved */
    Reserved10_IRQn           =  12,              /*!<  Internal reserved */
    Reserved11_IRQn           =  13,              /*!<  Internal reserved */
    Reserved12_IRQn           =  14,              /*!<  Internal reserved */
    Reserved13_IRQn           =  15,              /*!<  Internal reserved */
    Reserved14_IRQn           =  16,              /*!<  Internal reserved */
    Reserved15_IRQn           =  17,              /*!<  Internal reserved */
    Reserved16_IRQn           =  18,              /*!<  Internal reserved */

/* ===========================================  riscvcpu Specific Interrupt Numbers  ========================================= */
/* ToDo: add here your device specific external interrupt numbers. 19~1023 is reserved number for user. Maxmum interrupt supported
         could get from clicinfo.NUM_INTERRUPT. According the interrupt handlers defined in startup_Device.s
         eg.: Interrupt for Timer#1       clic_tim1_handler   ->   TIM1_IRQn */
    SOC_INT19_IRQn           = 19,                /*!< Device Interrupt */
    SOC_INT20_IRQn           = 20,                /*!< Device Interrupt */
    SOC_INT21_IRQn           = 21,                /*!< Device Interrupt */
    SOC_INT22_IRQn           = 22,                /*!< Device Interrupt */
    SOC_INT23_IRQn           = 23,                /*!< Device Interrupt */
    SOC_INT24_IRQn           = 24,                /*!< Device Interrupt */
    SOC_INT25_IRQn           = 25,                /*!< Device Interrupt */
    SOC_INT26_IRQn           = 26,                /*!< Device Interrupt */
    SOC_INT27_IRQn           = 27,                /*!< Device Interrupt */
    SOC_INT28_IRQn           = 28,                /*!< Device Interrupt */
    SOC_INT29_IRQn           = 29,                /*!< Device Interrupt */
    SOC_INT30_IRQn           = 30,                /*!< Device Interrupt */
    SOC_INT31_IRQn           = 31,                /*!< Device Interrupt */
    SOC_INT32_IRQn           = 32,                /*!< Device Interrupt */
    SOC_INT33_IRQn           = 33,                /*!< Device Interrupt */
    SOC_INT34_IRQn           = 34,                /*!< Device Interrupt */
    SOC_INT35_IRQn           = 35,                /*!< Device Interrupt */
    SOC_INT36_IRQn           = 36,                /*!< Device Interrupt */
    SOC_INT37_IRQn           = 37,                /*!< Device Interrupt */
    SOC_INT38_IRQn           = 38,                /*!< Device Interrupt */
    SOC_INT39_IRQn           = 39,                /*!< Device Interrupt */
    SOC_INT40_IRQn           = 40,                /*!< Device Interrupt */
    SOC_INT41_IRQn           = 41,                /*!< Device Interrupt */
    SOC_INT42_IRQn           = 42,                /*!< Device Interrupt */
    SOC_INT43_IRQn           = 43,                /*!< Device Interrupt */
    SOC_INT44_IRQn           = 44,                /*!< Device Interrupt */
    SOC_INT45_IRQn           = 45,                /*!< Device Interrupt */
    SOC_INT46_IRQn           = 46,                /*!< Device Interrupt */
    SOC_INT47_IRQn           = 47,                /*!< Device Interrupt */
    SOC_INT48_IRQn           = 48,                /*!< Device Interrupt */
    SOC_INT49_IRQn           = 49,                /*!< Device Interrupt */
    SOC_INT50_IRQn           = 50,                /*!< Device Interrupt */
    SOC_INT51_IRQn           = 51,                /*!< Device Interrupt */
    SOC_INT52_IRQn           = 52,                /*!< Device Interrupt */
    SOC_INT53_IRQn           = 53,                /*!< Device Interrupt */
    SOC_INT54_IRQn           = 54,                /*!< Device Interrupt */
    SOC_INT55_IRQn           = 55,                /*!< Device Interrupt */
    SOC_INT56_IRQn           = 56,                /*!< Device Interrupt */
    SOC_INT57_IRQn           = 57,                /*!< Device Interrupt */
    SOC_INT58_IRQn           = 58,                /*!< Device Interrupt */
    SOC_INT59_IRQn           = 59,                /*!< Device Interrupt */
    SOC_INT60_IRQn           = 60,                /*!< Device Interrupt */
    SOC_INT61_IRQn           = 61,                /*!< Device Interrupt */
    SOC_INT62_IRQn           = 62,                /*!< Device Interrupt */
    SOC_INT63_IRQn           = 63,                /*!< Device Interrupt */
    SOC_INT64_IRQn           = 64,                /*!< Device Interrupt */
    SOC_INT65_IRQn           = 65,                /*!< Device Interrupt */
    SOC_INT66_IRQn           = 66,                /*!< Device Interrupt */
    SOC_INT67_IRQn           = 67,                /*!< Device Interrupt */
    SOC_INT68_IRQn           = 68,                /*!< Device Interrupt */
    SOC_INT69_IRQn           = 69,                /*!< Device Interrupt */
    SOC_INT70_IRQn           = 70,                /*!< Device Interrupt */
    CORE1_SDBG_AUTHIRQ_IRQn           = 71,                /*!< Device Interrupt */
    CORE1_SEC_ERR_IRQ_IRQn           = 72,                /*!< Device Interrupt */
    SOC_INT_MAX,
} IRQn_Type;

/* =========================================================================================================================== */
/* ================                                  Exception Code Definition                                ================ */
/* =========================================================================================================================== */

typedef enum EXCn {
/* =======================================  Smartchip RV/RVX Specific Exception Code  ======================================== */
    InsUnalign_EXCn          =   0,              /*!<  Instruction address misaligned */
    InsAccFault_EXCn         =   1,              /*!<  Instruction access fault */
    IlleIns_EXCn             =   2,              /*!<  Illegal instruction */
    Break_EXCn               =   3,              /*!<  Beakpoint */
    LdAddrUnalign_EXCn       =   4,              /*!<  Load address misaligned */
    LdFault_EXCn             =   5,              /*!<  Load access fault */
    StAddrUnalign_EXCn       =   6,              /*!<  Store or AMO address misaligned */
    StAccessFault_EXCn       =   7,              /*!<  Store or AMO access fault */
    UmodeEcall_EXCn          =   8,              /*!<  Environment call from User mode */
    MmodeEcall_EXCn          =  11,              /*!<  Environment call from Machine mode */
    NMI_EXCn                 = 0xfff,            /*!<  NMI interrupt */
} EXCn_Type;

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ToDo: set the defines according your Device */
/* ToDo: define the correct core revision */
#if __riscv_xlen == 32

#ifndef __SMARTCHIP_CORE_REV
#define __SMARTCHIP_RV_REV            0x0104    /*!< Core Revision r1p4 */
#else
#define __SMARTCHIP_RV_REV            __SMARTCHIP_CORE_REV
#endif

#elif __riscv_xlen == 64

#ifndef __SMARTCHIP_CORE_REV
#define __SMARTCHIP_RVX_REV           0x0100    /*!< Core Revision r1p0 */
#else
#define __SMARTCHIP_RVX_REV           __SMARTCHIP_CORE_REV
#endif

#endif /* __riscv_xlen == 64 */

/* ToDo: define the correct core features for the riscvcpu */
#define __CLIC_PRESENT           1                     /*!< Set to 1 if CLIC is present */
#define __CLIC_BASEADDR          0x0C000000UL          /*!< Set to CLIC baseaddr of your device */

//#define __CLIC_INTCTLBITS        3                     /*!< Set to 1 - 8, the number of hardware bits are actually implemented in the clicintctl registers. */
#define __CLIC_INTNUM            51                    /*!< Set to 1 - 1024, total interrupt number of CLIC Unit */
#define __SYSTIMER_PRESENT        1                     /*!< Set to 1 if System Timer is present */
#define __SYSTIMER_BASEADDR       0x02000000UL          /*!< Set to SysTimer baseaddr of your device */

/*!< Set to 0, 1, or 2, 0 not present, 1 single floating point unit present, 2 double floating point unit present */
#if !defined(__riscv_flen)
#define __FPU_PRESENT             0
#elif __riscv_flen == 32
#define __FPU_PRESENT             1
#else
#define __FPU_PRESENT             2
#endif

/* __riscv_bitmanip/__riscv_dsp/__riscv_vector is introduced
 * in smartchip gcc 10.2 when b/p/v extension compiler option is selected.
 * For example:
 * -march=rv32imacb -mabi=ilp32 : __riscv_bitmanip macro will be defined
 * -march=rv32imacp -mabi=ilp32 : __riscv_dsp macro will be defined
 * -march=rv64imacv -mabi=lp64 : __riscv_vector macro will be defined
 */
#if defined(__riscv_bitmanip)
#define __BITMANIP_PRESENT        1                     /*!< Set to 1 if Bitmainpulation extension is present */
#else
#define __BITMANIP_PRESENT        0                     /*!< Set to 1 if Bitmainpulation extension is present */
#endif
#if defined(__riscv_dsp)
#define __DSP_PRESENT             1                     /*!< Set to 1 if Partial SIMD(DSP) extension is present */
#else
#define __DSP_PRESENT             0                     /*!< Set to 1 if Partial SIMD(DSP) extension is present */
#endif
#if defined(__riscv_vector)
#define __VECTOR_PRESENT          1                     /*!< Set to 1 if Vector extension is present */
#else
#define __VECTOR_PRESENT          0                     /*!< Set to 1 if Vector extension is present */
#endif

#define __PMP_PRESENT             1                     /*!< Set to 1 if PMP is present */
#define __PMP_ENTRY_NUM           16                    /*!< Set to 8 or 16, the number of PMP entries */



#define __ICACHE_PRESENT          0                     /*!< Set to 1 if I-Cache is present */
#define __DCACHE_PRESENT          0                     /*!< Set to 1 if D-Cache is present */
#define __CCM_PRESENT             1                     /*!< Set to 1 if Cache Control and Mantainence Unit is present */


#ifndef __INC_INTRINSIC_API
#define __INC_INTRINSIC_API       0                     /*!< Set to 1 if intrinsic api header files need to be included */
#endif

#define __Vendor_SysTickConfig    0                     /*!< Set to 1 if different SysTick Config is used */
#define __Vendor_EXCEPTION        0                     /*!< Set to 1 if vendor exception hander is present */

/** @} */ /* End of group Configuration_of_CMSIS */


#include <rmsis_core.h>                         /*!< Smartchip RV/RVX class processor and core peripherals */
/* ToDo: include your system_riscvcpu.h file
         replace 'Device' with your device name */
#include "system_riscvcpu.h"                    /*!< riscvcpu System */


/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

#define RTC_FREQ                    32768
// The TIMER frequency is just the RTC frequency
#define SOC_TIMER_FREQ              RTC_FREQ
/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_peripherals
  * @{
  */

/****************************************************************************
 * Platform definitions
 *****************************************************************************/
// IOF Mappings

#define IOF0_UART0_MASK             _AC(0x00030000, UL)
#define IOF_UART0_RX                (16u)
#define IOF_UART0_TX                (17u)

#define IOF0_UART1_MASK             _AC(0x03000000, UL)
#define IOF_UART1_RX                (24u)
#define IOF_UART1_TX                (25u)

// Interrupt Numbers
#define SOC_CLIC_NUM_INTERRUPTS    32
#define SOC_CLIC_INT_GPIO_BASE     19

// Interrupt Handler Definitions
#define SOC_MTIMER_HANDLER          clic_mtip_handler
#define SOC_SOFTINT_HANDLER         clic_msip_handler

#define GPIO_BIT_ALL_ZERO           (0x0)
#define GPIO_BIT_ALL_ONE            (0xFFFFFFFF)

/**
  * @brief GPIO
  */
typedef struct {  /*!< GPIO Structure */
    __IOM uint32_t INPUT_VAL;
    __IOM uint32_t INPUT_EN;
    __IOM uint32_t OUTPUT_EN;
    __IOM uint32_t OUTPUT_VAL;
    __IOM uint32_t PULLUP_EN;
    __IOM uint32_t DRIVE;
    __IOM uint32_t RISE_IE;
    __IOM uint32_t RISE_IP;
    __IOM uint32_t FALL_IE;
    __IOM uint32_t FALL_IP;
    __IOM uint32_t HIGH_IE;
    __IOM uint32_t HIGH_IP;
    __IOM uint32_t LOW_IE;
    __IOM uint32_t LOW_IP;
    __IOM uint32_t IOF_EN;
    __IOM uint32_t IOF_SEL;
    __IOM uint32_t OUTPUT_XOR;
} GPIO_TypeDef;

/**
  * @brief UART
  */
typedef struct {
    __IOM uint32_t TXFIFO;
    __IOM uint32_t RXFIFO;
    __IOM uint32_t TXCTRL;
    __IOM uint32_t RXCTRL;
    __IOM uint32_t IE;
    __IOM uint32_t IP;
    __IOM uint32_t DIV;
} UART_TypeDef;

typedef struct {
    __IOM uint32_t RII;
    __IOM uint32_t ALU2_TR_WBCK_RES;
    __IOM uint32_t DIV_PCMPS_RS1;
    __IOM uint32_t DIV_PCMPS_RS2;
    __IOM uint32_t DIV_DISTB_RS1;
    __IOM uint32_t DIV_DISTB_RS2;
    __IOM uint32_t MUL_DISTB_RS1;
    __IOM uint32_t MUL_DISTB_RS2;
    __IOM uint32_t ALU1_DISTB_RS1;
    __IOM uint32_t ALU1_DISTB_RS2;
    __IOM uint32_t ALU2_DISTB_RS1;
    __IOM uint32_t ALU2_DISTB_RS2;
    __IOM uint32_t MISC;
}RAND_SEED_VALUE_TypeDef;
typedef struct {
    __IOM uint32_t mtvt;
    __IOM uint32_t mtvec;
    __IOM uint32_t reserved0;
    __IOM uint32_t stvt;
    __IOM uint32_t stvec;
    __IOM uint32_t reserved1;
}VECTOR_REMAP_TypeDef;
typedef struct {
    __IOM uint32_t BBOX_0_Start;
    __IOM uint32_t BBOX_0_End;
    __IOM uint32_t BBOX_1_Start;
    __IOM uint32_t BBOX_1_End;
    __IOM uint32_t BBOX_2_Start;
    __IOM uint32_t BBOX_2_End;
    __IOM uint32_t BBOX_3_Start;
    __IOM uint32_t BBOX_3_End;
    __IOM uint32_t BBOX_4_Start;
    __IOM uint32_t BBOX_4_End;
    __IOM uint32_t BBOX_5_Start;
    __IOM uint32_t BBOX_5_End;
    __IOM uint32_t BBOX_6_Start;
    __IOM uint32_t BBOX_6_End;
    __IOM uint32_t BBOX_7_Start;
    __IOM uint32_t BBOX_7_End;
}BBOX_TypeDef;
typedef struct {
    __IOM uint32_t MPU_Region0_Cfg;
    __IOM uint32_t MPU_Region0_Start;
    __IOM uint32_t MPU_Region0_End;
    __IOM uint32_t MPU_Region1_Cfg;
    __IOM uint32_t MPU_Region1_Start;
    __IOM uint32_t MPU_Region1_End;
    __IOM uint32_t MPU_Region2_Cfg;
    __IOM uint32_t MPU_Region2_Start;
    __IOM uint32_t MPU_Region2_End;
    __IOM uint32_t MPU_Region3_Cfg;
    __IOM uint32_t MPU_Region3_Start;
    __IOM uint32_t MPU_Region3_End;
    __IOM uint32_t MPU_Region4_Cfg;
    __IOM uint32_t MPU_Region4_Start;
    __IOM uint32_t MPU_Region4_End;
    __IOM uint32_t MPU_Region5_Cfg;
    __IOM uint32_t MPU_Region5_Start;
    __IOM uint32_t MPU_Region5_End;
    __IOM uint32_t MPU_Region6_Cfg;
    __IOM uint32_t MPU_Region6_Start;
    __IOM uint32_t MPU_Region6_End;
    __IOM uint32_t MPU_Region7_Cfg;
    __IOM uint32_t MPU_Region7_Start;
    __IOM uint32_t MPU_Region7_End;
}MPU_REGION_TypeDef;
typedef struct {
    __IOM uint32_t Machine_M_Top;
    __IOM uint32_t Machine_M_Buttom;
    __IOM uint32_t Supervisor_M_Top;
    __IOM uint32_t Supervisor_M_Buttom;
    __IOM uint32_t User_M_Top;
    __IOM uint32_t User_M_Buttom;
}STACK_OVERFLOW_CHECK_TypeDef;
typedef struct{
    __IOM uint32_t Sec_Control;
    __IOM uint32_t Sec_Status;
    __IOM uint32_t Lock_Ctrl0;
    __IOM uint32_t Lock_Ctrl1;
    __IOM uint32_t RII_ctrl;
    __IOM uint32_t RRI_Counter;
    __IOM uint32_t RESERVED0[2];
    __IOM uint32_t Request_value0;
    __IOM uint32_t Request_value1;
    __IOM uint32_t RESERVED1[2];
    __IOM uint32_t Threshold_value0;
    __IOM uint32_t Threshold_value1;
    __IOM uint32_t RESERVED2[2];
    RAND_SEED_VALUE_TypeDef rand_seed_value;
    __IOM uint32_t RESERVED3[11];
    __IOM uint32_t Random_Load_Base;
    VECTOR_REMAP_TypeDef Vector_Remap;
    __IOM uint32_t Return_PC;
    __IOM uint32_t RESERVED4[16];
    __IOM uint32_t Auth_Ctrl;
    __IOM uint32_t Auth_data;
    __IOM uint32_t RESERVED5[6];
    BBOX_TypeDef BBOX;
    MPU_REGION_TypeDef MPU_Region;
    __IOM uint32_t RESERVED6[16];
    __IOM uint32_t Security_Status1;
    __IOM uint32_t Security_IRQ;
    __IOM uint32_t Security_IRQ1;
    __IOM uint32_t RESERVED7[9];
    STACK_OVERFLOW_CHECK_TypeDef Security_Stack_Overflow;
}SEC_CTRL_TypeDef;
/*@}*/ /* end of group riscvcpu_fpga_Peripherals */


/* =========================================  End of section using anonymous unions  ========================================= */
#if defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/* Macros for memory access operations */
#define _REG8P(p, i)                        ((volatile uint8_t *) ((uintptr_t)((p) + (i))))
#define _REG16P(p, i)                       ((volatile uint16_t *) ((uintptr_t)((p) + (i))))
#define _REG32P(p, i)                       ((volatile uint32_t *) ((uintptr_t)((p) + (i))))
#define _REG64P(p, i)                       ((volatile uint64_t *) ((uintptr_t)((p) + (i))))
#define _REG8(p, i)                         (*(_REG8P(p, i)))
#define _REG16(p, i)                        (*(_REG16P(p, i)))
#define _REG32(p, i)                        (*(_REG32P(p, i)))
#define _REG64(p, i)                        (*(_REG64P(p, i)))
#define REG8(addr)                          _REG8((addr), 0)
#define REG16(addr)                         _REG16((addr), 0)
#define REG32(addr)                         _REG32((addr), 0)
#define REG64(addr)                         _REG64((addr), 0)

/* Macros for address type convert and access operations */
#define ADDR16(addr)                        ((uint16_t)(uintptr_t)(addr))
#define ADDR32(addr)                        ((uint32_t)(uintptr_t)(addr))
#define ADDR64(addr)                        ((uint64_t)(uintptr_t)(addr))
#define ADDR8P(addr)                        ((uint8_t *)(uintptr_t)(addr))
#define ADDR16P(addr)                       ((uint16_t *)(uintptr_t)(addr))
#define ADDR32P(addr)                       ((uint32_t *)(uintptr_t)(addr))
#define ADDR64P(addr)                       ((uint64_t *)(uintptr_t)(addr))

/* Macros for Bit Operations */
#if __riscv_xlen == 32
#define BITMASK_MAX                         0xFFFFFFFFUL
#define BITOFS_MAX                          31
#else
#define BITMASK_MAX                         0xFFFFFFFFFFFFFFFFULL
#define BITOFS_MAX                          63
#endif

// BIT/BITS only support bit mask for __riscv_xlen
// For RISC-V 32 bit, it support mask 32 bit wide
// For RISC-V 64 bit, it support mask 64 bit wide
#define BIT(ofs)                            (0x1UL << (ofs))
#define BITS(start, end)                    ((BITMASK_MAX) << (start) & (BITMASK_MAX) >> (BITOFS_MAX - (end)))
#define GET_BIT(regval, bitofs)             (((regval) >> (bitofs)) & 0x1)
#define SET_BIT(regval, bitofs)             ((regval) |= BIT(bitofs))
#define CLR_BIT(regval, bitofs)             ((regval) &= (~BIT(bitofs)))
#define FLIP_BIT(regval, bitofs)            ((regval) ^= BIT(bitofs))
#define WRITE_BIT(regval, bitofs, val)      CLR_BIT(regval, bitofs); ((regval) |= ((val) << bitofs) & BIT(bitofs))
#define CHECK_BIT(regval, bitofs)           (!!((regval) & (0x1UL<<(bitofs))))
#define GET_BITS(regval, start, end)        (((regval) & BITS((start), (end))) >> (start))
#define SET_BITS(regval, start, end)        ((regval) |= BITS((start), (end)))
#define CLR_BITS(regval, start, end)        ((regval) &= (~BITS((start), (end))))
#define FLIP_BITS(regval, start, end)       ((regval) ^= BITS((start), (end)))
#define WRITE_BITS(regval, start, end, val) CLR_BITS(regval, start, end); ((regval) |= ((val) << start) & BITS((start), (end)))
#define CHECK_BITS_ALL(regval, start, end)  (!((~(regval)) & BITS((start), (end))))
#define CHECK_BITS_ANY(regval, start, end)  ((regval) & BITS((start), (end)))

#define BITMASK_SET(regval, mask)           ((regval) |= (mask))
#define BITMASK_CLR(regval, mask)           ((regval) &= (~(mask)))
#define BITMASK_FLIP(regval, mask)          ((regval) ^= (mask))
#define BITMASK_CHECK_ALL(regval, mask)     (!((~(regval)) & (mask)))
#define BITMASK_CHECK_ANY(regval, mask)     ((regval) & (mask))

/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/* ToDo: add here your device peripherals base addresses
         following is an example for timer */
/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */
/* Peripheral and SRAM base address */
#define ONCHIP_ROM_BASE         (0x00001000UL)      /*!< (ROM       ) Base Address */
#define ONCHIP_ILM_BASE         (0x80000000UL)      /*!< (ILM       ) Base Address */
#define ONCHIP_DLM_BASE         (0x90000000UL)      /*!< (DLM       ) Base Address */
#define RISCVCPU_PERIPH_BASE       (0x10000000UL)      /*!< (Peripheral) Base Address */

/* Peripheral memory map */
/* Fast-IO Interfaced IP */
#define GPIO_BASE               (RISCVCPU_PERIPH_BASE + 0x12000)          /*!< (GPIO) Base Address */
/* PPI Interfaced IP */
#define UART0_BASE              (RISCVCPU_PERIPH_BASE + 0x13000)          /*!< (UART0) Base Address */
#define UART1_BASE              (RISCVCPU_PERIPH_BASE + 0x23000)          /*!< (UART1) Base Address */

#define SEC_CTRL_BASE           (0x03000000)                             /*!< (SEC) Base Address */
/** @} */ /* End of group Device_Peripheral_peripheralAddr */


/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */


/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer */
/** @addtogroup Device_Peripheral_declaration
  * @{
  */
#define GPIO                    ((GPIO_TypeDef *) GPIO_BASE)
#define UART0                   ((UART_TypeDef *) UART0_BASE)
#define UART1                   ((UART_TypeDef *) UART1_BASE)
#define SEC_CTRL                 ((SEC_CTRL_TypeDef *)SEC_CTRL_BASE )

// Helper functions
//#define _REG8(p, i)             (*(volatile uint8_t *) ((p) + (i)))
//#define _REG32(p, i)            (*(volatile uint32_t *) ((p) + (i)))
//#define _REG32P(p, i)           ((volatile uint32_t *) ((p) + (i)))


#define GPIO_REG(offset)        _REG32(GPIO_BASE, offset)
#define UART0_REG(offset)       _REG32(UART0_BASE, offset)
#define UART1_REG(offset)       _REG32(UART1_BASE, offset)


// Misc

#define NUM_GPIO 32

extern uint32_t get_cpu_freq(void);
extern void delay_1ms(uint32_t count);

/** @} */ /* End of group riscvcpu */

/** @} */ /* End of group Smartchip */

#ifdef __cplusplus
}
#endif

#endif  /* __RISCVCPU_H__ */
