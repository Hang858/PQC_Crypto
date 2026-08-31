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
#ifndef __CORE_FEATURE_CLIC__
#define __CORE_FEATURE_CLIC__
/*!
 * @file     core_feature_clic.h
 * @brief    CLIC feature API header file for Smartchip RV/RVX Core
 */
/*
 * CLIC Feature Configuration Macro:
 * 1. __CLIC_PRESENT:  Define whether Core Local Interrupt Controller (CLIC) Unit is present or not
 *   * 0: Not present
 *   * 1: Present
 * 2. __CLIC_BASEADDR:  Base address of the CLIC unit.
 * 3. CLIC_GetInfoCtlbits():  Define the number of hardware bits are actually implemented in the clicintctl registers.
 *   Valid number is 1 - 8.
 * 4. __CLIC_INTNUM  : Define the external interrupt number of CLIC Unit
 *
 */
#ifdef __cplusplus
 extern "C" {
#endif

#if defined(__CLIC_PRESENT) && (__CLIC_PRESENT == 1)
/**
 * \defgroup RMSIS_Core_CLIC_Registers     Register Define and Type Definitions Of CLIC
 * \ingroup RMSIS_Core_Registers
 * \brief   Type definitions and defines for clic registers.
 *
 * @{
 */

/**
 * \brief  Union type to access CLICFG configure register.
 */
typedef union
{
    struct {
        uint8_t _reserved0:1;                   /*!< bit:     0   Overflow condition code flag */
        uint8_t nlbits:4;                       /*!< bit:     29  Carry condition code flag */
        uint8_t _reserved1:2;                   /*!< bit:     30  Zero condition code flag */
        uint8_t _reserved2:1;                   /*!< bit:     31  Negative condition code flag */
    } b;                                        /*!< Structure used for bit  access */
    uint8_t w;                                  /*!< Type      used for byte access */
} CLICCFG_Type;

/**
 * \brief  Union type to access CLICINFO information register.
 */
typedef union {
    struct {
        uint32_t numint:13;                     /*!< bit:  0..12   number of maximum interrupt inputs supported */
        uint32_t version:8;                     /*!< bit:  13..20  20:17 for architecture version,16:13 for implementation version */
        uint32_t intctlbits:4;                  /*!< bit:  21..24  specifies how many hardware bits are actually implemented in the clicintctl registers */
        uint32_t _reserved0:7;                  /*!< bit:  25..31  Reserved */
    } b;                                        /*!< Structure used for bit  access */
    uint32_t w;                                 /*!< Type      used for word access */
} CLICINFO_Type;

/**
 * \brief Access to the structure of a vector interrupt controller.
 */
typedef struct {
    __IOM uint8_t  INTIP;                       /*!< Offset: 0x000 (R/W)  Interrupt set pending register */
    __IOM uint8_t  INTIE;                       /*!< Offset: 0x001 (R/W)  Interrupt set enable register */
    __IOM uint8_t  INTATTR;                     /*!< Offset: 0x002 (R/W)  Interrupt set attributes register */
    __IOM uint8_t  INTCTRL;                     /*!< Offset: 0x003 (R/W)  Interrupt configure register */
} CLIC_CTRL_Type;

typedef struct {
    __IOM uint8_t   CFG;                        /*!< Offset: 0x000 (R/W)  CLIC configuration register */
    uint8_t RESERVED0[3];
    __IM uint32_t  INFO;                        /*!< Offset: 0x004 (R/ )  CLIC information register */
    uint8_t RESERVED1[3];
    __IOM uint8_t  ILT;                         /*Interrupt Level Threshold*/
    uint32_t RESERVED2[0x3FD];
    CLIC_CTRL_Type CTRL[4096];                  /*!< Offset: 0x1000 (R/W) CLIC register structure for INTIP, INTIE, INTATTR, INTCTL */
} CLIC_Type;

#define CLIC_CLICCFG_NLBIT_Pos                 1U                                       /*!< CLIC CLICCFG: NLBIT Position */
#define CLIC_CLICCFG_NLBIT_Msk                 (0xFUL << CLIC_CLICCFG_NLBIT_Pos)        /*!< CLIC CLICCFG: NLBIT Mask */

#define CLIC_CLICINFO_CTLBIT_Pos                21U                                     /*!< CLIC INTINFO: __CLIC_GetInfoCtlbits() Position */
#define CLIC_CLICINFO_CTLBIT_Msk                (0xFUL << CLIC_CLICINFO_CTLBIT_Pos)     /*!< CLIC INTINFO: __CLIC_GetInfoCtlbits() Mask */

#define CLIC_CLICINFO_VER_Pos                  13U                                      /*!< CLIC CLICINFO: VERSION Position */
#define CLIC_CLICINFO_VER_Msk                  (0xFFUL << CLIC_CLICCFG_NLBIT_Pos)       /*!< CLIC CLICINFO: VERSION Mask */

#define CLIC_CLICINFO_NUM_Pos                  0U                                       /*!< CLIC CLICINFO: NUM Position */
#define CLIC_CLICINFO_NUM_Msk                  (0xFFFUL << CLIC_CLICINFO_NUM_Pos)       /*!< CLIC CLICINFO: NUM Mask */

#define CLIC_INTIP_IP_Pos                      0U                                       /*!< CLIC INTIP: IP Position */
#define CLIC_INTIP_IP_Msk                      (0x1UL << CLIC_INTIP_IP_Pos)             /*!< CLIC INTIP: IP Mask */

#define CLIC_INTIE_IE_Pos                      0U                                       /*!< CLIC INTIE: IE Position */
#define CLIC_INTIE_IE_Msk                      (0x1UL << CLIC_INTIE_IE_Pos)             /*!< CLIC INTIE: IE Mask */

#define CLIC_INTATTR_TRIG_Pos                  1U                                       /*!< CLIC INTATTR: TRIG Position */
#define CLIC_INTATTR_TRIG_Msk                  (0x3UL << CLIC_INTATTR_TRIG_Pos)         /*!< CLIC INTATTR: TRIG Mask */

#define CLIC_INTATTR_SHV_Pos                   0U                                       /*!< CLIC INTATTR: SHV Position */
#define CLIC_INTATTR_SHV_Msk                   (0x1UL << CLIC_INTATTR_SHV_Pos)          /*!< CLIC INTATTR: SHV Mask */

#define CLIC_MAX_NLBITS                       8U                                       /*!< Max nlbit of the CLICINTCTLBITS */
#define CLIC_MODE_MTVEC_Msk                   3U                                       /*!< CLIC Mode mask for MTVT CSR Register */

/**\brief CLIC Trigger Enum for different Trigger Type */
typedef enum CLIC_TRIGGER {
    CLIC_LEVEL_TRIGGER = 0x0,          /*!< Level Triggerred, trig[0] = 0 */
    CLIC_POSTIVE_EDGE_TRIGGER = 0x1,   /*!< Postive/Rising Edge Triggered, trig[1] = 1, trig[0] = 0 */
    CLIC_NEGTIVE_EDGE_TRIGGER = 0x3,   /*!< Negtive/Falling Edge Triggered, trig[1] = 1, trig[0] = 0 */
    CLIC_MAX_TRIGGER = 0x3             /*!< MAX Supported Trigger Mode */
} CLIC_TRIGGER_Type;

#ifndef __CLIC_BASEADDR
/* Base address of CLIC(__CLIC_BASEADDR) should be defined in <Device.h> */
#error "__CLIC_BASEADDR is not defined, please check!"
#endif

#ifndef __CLIC_INTCTLBITS
/* Define __CLIC_INTCTLBITS to get via CLIC->INFO if not defined */
#define __CLIC_INTCTLBITS                  (__CLIC_GetInfoCtlbits())
#endif

/* CLIC Memory mapping of Device */
#define CLIC_BASE                          __CLIC_BASEADDR                            /*!< CLIC Base Address */
#define CLIC                               ((CLIC_Type *) CLIC_BASE)                  /*!< CLIC configuration struct */

/** @} */ /* end of group RMSIS_Core_CLIC_Registers */

/* ##########################   CLIC functions  #################################### */
/**
 * \defgroup   RMSIS_Core_IntExc        Interrupts and Exceptions
 * \brief Functions that manage interrupts and exceptions via the CLIC.
 *
 * @{
 */

/**
 * \brief  Definition of IRQn numbers
 * \details
 * The core interrupt enumeration names for IRQn values are defined in the file <b><Device>.h</b>.
 * - Interrupt ID(IRQn) from 0 to 18 are reserved for core internal interrupts.
 * - Interrupt ID(IRQn) start from 19 represent device-specific external interrupts.
 * - The first device-specific interrupt has the IRQn value 19.
 *
 * The table below describes the core interrupt names and their availability in various Smartchip Cores.
 */
/* The following enum IRQn definition in this file
 * is only used for doxygen documentation generation,
 * The <Device>.h is the real file to define it by vendor
 */
#if defined(__ONLY_FOR_DOXYGEN_DOCUMENT_GENERATION__)
typedef enum IRQn {
    /* ========= Smartchip RV/RVX Core Specific Interrupt Numbers  =========== */
    /* Core Internal Interrupt IRQn definitions */
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

    /* ========= Device Specific Interrupt Numbers  =================== */
    /* ToDo: add here your device specific external interrupt numbers.
     * 19~max(NUM_INTERRUPT, 1023) is reserved number for user.
     * Maxmum interrupt supported could get from clicinfo.NUM_INTERRUPT.
     * According the interrupt handlers defined in startup_Device.S
     * eg.: Interrupt for Timer#1       clic_tim1_handler   ->   TIM1_IRQn */
    FirstDeviceSpecificInterrupt_IRQn    = 19,    /*!< First Device Specific Interrupt */
    SOC_INT_MAX,                                  /*!< Number of total interrupts */
} IRQn_Type;
#endif /* __ONLY_FOR_DOXYGEN_DOCUMENT_GENERATION__ */

#ifdef RMSIS_CLIC_VIRTUAL
    #ifndef RMSIS_CLIC_VIRTUAL_HEADER_FILE
        #define RMSIS_CLIC_VIRTUAL_HEADER_FILE "rmsis_clic_virtual.h"
    #endif
    #include RMSIS_CLIC_VIRTUAL_HEADER_FILE
#else
    #define CLIC_SetCfgNlbits            __CLIC_SetCfgNlbits
    #define CLIC_GetCfgNlbits            __CLIC_GetCfgNlbits
    #define CLIC_GetInfoVer              __CLIC_GetInfoVer
    #define CLIC_GetInfoCtlbits          __CLIC_GetInfoCtlbits
    #define CLIC_GetInfoNum              __CLIC_GetInfoNum
	#define CLIC_SetIlt                  __CLIC_SetIlt
	#define CLIC_GetIlt                  __CLIC_GetIlt
    #define CLIC_EnableIRQ               __CLIC_EnableIRQ
    #define CLIC_GetEnableIRQ            __CLIC_GetEnableIRQ
    #define CLIC_DisableIRQ              __CLIC_DisableIRQ
    #define CLIC_SetPendingIRQ           __CLIC_SetPendingIRQ
    #define CLIC_GetPendingIRQ           __CLIC_GetPendingIRQ
    #define CLIC_ClearPendingIRQ         __CLIC_ClearPendingIRQ
    #define CLIC_SetTrigIRQ              __CLIC_SetTrigIRQ
    #define CLIC_GetTrigIRQ              __CLIC_GetTrigIRQ
    #define CLIC_SetShvIRQ               __CLIC_SetShvIRQ
    #define CLIC_GetShvIRQ               __CLIC_GetShvIRQ
    #define CLIC_SetCtrlIRQ              __CLIC_SetCtrlIRQ
    #define CLIC_GetCtrlIRQ              __CLIC_GetCtrlIRQ
    #define CLIC_SetLevelIRQ             __CLIC_SetLevelIRQ
    #define CLIC_GetLevelIRQ             __CLIC_GetLevelIRQ
    #define CLIC_SetPriorityIRQ          __CLIC_SetPriorityIRQ
    #define CLIC_GetPriorityIRQ          __CLIC_GetPriorityIRQ

#endif /* RMSIS_CLIC_VIRTUAL */

#ifdef RMSIS_VECTAB_VIRTUAL
    #ifndef RMSIS_VECTAB_VIRTUAL_HEADER_FILE
        #define RMSIS_VECTAB_VIRTUAL_HEADER_FILE "rmsis_vectab_virtual.h"
    #endif
    #include RMSIS_VECTAB_VIRTUAL_HEADER_FILE
#else
    #define CLIC_SetVector              __CLIC_SetVector
    #define CLIC_GetVector              __CLIC_GetVector
#endif  /* (RMSIS_VECTAB_VIRTUAL) */

/**
 * \brief  Set nlbits value
 * \details
 * This function set the nlbits value of CLICCFG register.
 * \param [in]    nlbits    nlbits value
 * \remarks
 * - nlbits is used to set the width of level in the CLICINTCTL[i].
 * \sa
 * - \ref CLIC_GetCfgNlbits
 */
__STATIC_FORCEINLINE void __CLIC_SetCfgNlbits(uint32_t nlbits)
{
    CLIC->CFG &= ~CLIC_CLICCFG_NLBIT_Msk;
    CLIC->CFG |= (uint8_t)((nlbits <<CLIC_CLICCFG_NLBIT_Pos) & CLIC_CLICCFG_NLBIT_Msk);
}

/**
 * \brief  Get nlbits value
 * \details
 * This function get the nlbits value of CLICCFG register.
 * \return   nlbits value of CLICCFG register
 * \remarks
 * - nlbits is used to set the width of level in the CLICINTCTL[i].
 * \sa
 * - \ref CLIC_SetCfgNlbits
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetCfgNlbits(void)
{
    return ((uint32_t)((CLIC->CFG & CLIC_CLICCFG_NLBIT_Msk) >> CLIC_CLICCFG_NLBIT_Pos));
}

/**
 * \brief  Get the CLIC version number
 * \details
 * This function gets the hardware version information from CLICINFO register.
 * \return   hardware version number in CLICINFO register.
 * \remarks
 * - This function gets harware version information from CLICINFO register.
 * - Bit 20:17 for architecture version, bit 16:13 for implementation version.
 * \sa
 * - \ref CLIC_GetInfoNum
*/
__STATIC_FORCEINLINE uint32_t __CLIC_GetInfoVer(void)
{
    return ((uint32_t)((CLIC->INFO & CLIC_CLICINFO_VER_Msk) >> CLIC_CLICINFO_VER_Pos));
}

/**
 * \brief  Get CLICINTCTLBITS
 * \details
 * This function gets CLICINTCTLBITS from CLICINFO register.
 * \return  CLICINTCTLBITS from CLICINFO register.
 * \remarks
 * - In the CLICINTCTL[i] registers, with 2 <= CLICINTCTLBITS <= 8.
 * - The implemented bits are kept left-justified in the most-significant bits of each 8-bit
 *   CLICINTCTL[I] register, with the lower unimplemented bits treated as hardwired to 1.
 * \sa
 * - \ref CLIC_GetInfoNum
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetInfoCtlbits(void)
{
    return ((uint32_t)((CLIC->INFO & CLIC_CLICINFO_CTLBIT_Msk) >> CLIC_CLICINFO_CTLBIT_Pos));
}

/**
 * \brief  Get number of maximum interrupt inputs supported
 * \details
 * This function gets number of maximum interrupt inputs supported from CLICINFO register.
 * \return  number of maximum interrupt inputs supported from CLICINFO register.
 * \remarks
 * - This function gets number of maximum interrupt inputs supported from CLICINFO register.
 * - The num_interrupt field specifies the actual number of maximum interrupt inputs supported in this implementation.
 * \sa
 * - \ref CLIC_GetInfoCtlbits
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetInfoNum(void)
{
    return ((uint32_t)((CLIC->INFO & CLIC_CLICINFO_NUM_Msk) >> CLIC_CLICINFO_NUM_Pos));
}

/**
 * \brief  Set Machine Mode Interrupt Level Threshold
 * \details
 * This function sets machine mode interrupt level threshold.
 * \param [in]  ilt       Interrupt Level Threshold.
 * \sa
 * - \ref CLIC_GetIlt
 */
__STATIC_FORCEINLINE void __CLIC_SetIlt(uint8_t ilt)
{
    CLIC->ILT = ilt;
}

/**
 * \brief  Get Machine Mode Interrupt Level Threshold
 * \details
 * This function gets machine mode interrupt level threshold.
 * \return       Interrupt Level Threshold.
 * \sa
 * - \ref CLIC_SetIlt
 */
__STATIC_FORCEINLINE uint8_t __CLIC_GetIlt(void)
{
    return (CLIC->ILT);
}

/**
 * \brief  Enable a specific interrupt
 * \details
 * This function enables the specific interrupt \em IRQn.
 * \param [in]  IRQn  Interrupt number
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_DisableIRQ
 */
__STATIC_FORCEINLINE void __CLIC_EnableIRQ(IRQn_Type IRQn)
{
    CLIC->CTRL[IRQn].INTIE |= CLIC_INTIE_IE_Msk;
}

/**
 * \brief  Get a specific interrupt enable status
 * \details
 * This function returns the interrupt enable status for the specific interrupt \em IRQn.
 * \param [in]  IRQn  Interrupt number
 * \returns
 * - 0  Interrupt is not enabled
 * - 1  Interrupt is pending
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_EnableIRQ
 * - \ref CLIC_DisableIRQ
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetEnableIRQ(IRQn_Type IRQn)
{
    return((uint32_t) (CLIC->CTRL[IRQn].INTIE) & CLIC_INTIE_IE_Msk);
}

/**
 * \brief  Disable a specific interrupt
 * \details
 * This function disables the specific interrupt \em IRQn.
 * \param [in]  IRQn  Number of the external interrupt to disable
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_EnableIRQ
 */
__STATIC_FORCEINLINE void __CLIC_DisableIRQ(IRQn_Type IRQn)
{
    CLIC->CTRL[IRQn].INTIE &= ~CLIC_INTIE_IE_Msk;
}

/**
 * \brief  Get the pending specific interrupt
 * \details
 * This function returns the pending status of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \returns
 * - 0  Interrupt is not pending
 * - 1  Interrupt is pending
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetPendingIRQ
 * - \ref CLIC_ClearPendingIRQ
 */
__STATIC_FORCEINLINE int32_t __CLIC_GetPendingIRQ(IRQn_Type IRQn)
{
    return((uint32_t)(CLIC->CTRL[IRQn].INTIP) & CLIC_INTIP_IP_Msk);
}

/**
 * \brief  Set a specific interrupt to pending
 * \details
 * This function sets the pending bit for the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_GetPendingIRQ
 * - \ref CLIC_ClearPendingIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetPendingIRQ(IRQn_Type IRQn)
{
    CLIC->CTRL[IRQn].INTIP |= CLIC_INTIP_IP_Msk;
}

/**
 * \brief  Clear a specific interrupt from pending
 * \details
 * This function removes the pending state of the specific interrupt \em IRQn.
 * \em IRQn cannot be a negative number.
 * \param [in]      IRQn  Interrupt number
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetPendingIRQ
 * - \ref CLIC_GetPendingIRQ
 */
__STATIC_FORCEINLINE void __CLIC_ClearPendingIRQ(IRQn_Type IRQn)
{
    CLIC->CTRL[IRQn].INTIP &= ~ CLIC_INTIP_IP_Msk;
}

/**
 * \brief  Set trigger mode and polarity for a specific interrupt
 * \details
 * This function set trigger mode and polarity of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      trig
 *                   - 00  level trigger, \ref CLIC_LEVEL_TRIGGER
 *                   - 01  positive edge trigger, \ref CLIC_POSTIVE_EDGE_TRIGGER
 *                   - 02  level trigger, \ref CLIC_LEVEL_TRIGGER
 *                   - 03  negative edge trigger, \ref CLIC_NEGTIVE_EDGE_TRIGGER
 * \remarks
 * - IRQn must not be negative.
 *
 * \sa
 * - \ref CLIC_GetTrigIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetTrigIRQ(IRQn_Type IRQn, uint32_t trig)
{
    CLIC->CTRL[IRQn].INTATTR &= ~CLIC_INTATTR_TRIG_Msk;
    CLIC->CTRL[IRQn].INTATTR |= (uint8_t)(trig<<CLIC_INTATTR_TRIG_Pos);
}

/**
 * \brief  Get trigger mode and polarity for a specific interrupt
 * \details
 * This function get trigger mode and polarity of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return
 *                 - 00  level trigger, \ref CLIC_LEVEL_TRIGGER
 *                 - 01  positive edge trigger, \ref CLIC_POSTIVE_EDGE_TRIGGER
 *                 - 02  level trigger, \ref CLIC_LEVEL_TRIGGER
 *                 - 03  negative edge trigger, \ref CLIC_NEGTIVE_EDGE_TRIGGER
 * \remarks
 *     - IRQn must not be negative.
 * \sa
 *     - \ref CLIC_SetTrigIRQ
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetTrigIRQ(IRQn_Type IRQn)
{
    return ((int32_t)(((CLIC->CTRL[IRQn].INTATTR) & CLIC_INTATTR_TRIG_Msk)>>CLIC_INTATTR_TRIG_Pos));
}

/**
 * \brief  Set interrupt working mode for a specific interrupt
 * \details
 * This function set selective hardware vector working mode of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_GetShvIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetShvIRQ(IRQn_Type IRQn)
{
    CLIC->CTRL[IRQn].INTATTR &= ~CLIC_INTATTR_SHV_Msk;
    CLIC->CTRL[IRQn].INTATTR |= (uint8_t)(0x01<<CLIC_INTATTR_SHV_Pos);
}

/**
 * \brief  Get interrupt working mode for a specific interrupt
 * \details
 * This function get selective hardware vector working mode of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return       shv
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetShvIRQ
 */
__STATIC_FORCEINLINE uint32_t __CLIC_GetShvIRQ(IRQn_Type IRQn)
{
    return ((int32_t)(((CLIC->CTRL[IRQn].INTATTR) & CLIC_INTATTR_SHV_Msk)>>CLIC_INTATTR_SHV_Pos));
}

/**
 * \brief  Modify CLIC Interrupt Input Control Register for a specific interrupt
 * \details
 * This function modify CLIC Interrupt Input Control(CLICINTCTL[i]) register of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      intctrl  Set value for CLICINTCTL[i] register
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_GetCtrlIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetCtrlIRQ(IRQn_Type IRQn, uint8_t intctrl)
{
    CLIC->CTRL[IRQn].INTCTRL = intctrl;
}

/**
 * \brief  Get CLIC Interrupt Input Control Register value for a specific interrupt
 * \details
 * This function modify CLIC Interrupt Input Control register of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return       value of CLIC Interrupt Input Control register
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetCtrlIRQ
 */
__STATIC_FORCEINLINE uint8_t __CLIC_GetCtrlIRQ(IRQn_Type IRQn)
{
    return (CLIC->CTRL[IRQn].INTCTRL);
}

/**
 * \brief  Set CLIC Interrupt level of a specific interrupt
 * \details
 * This function set interrupt level of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      lvl_abs   Interrupt level
 * \remarks
 * - IRQn must not be negative.
 * - If lvl_abs to be set is larger than the max level allowed, it will be force to be max level.
 * - When you set level value you need use clciinfo.nlbits to get the width of level.
 *   Then we could know the maximum of level. CLICINTCTLBITS is how many total bits are
 *   present in the CLICINTCTL register.
 * \sa
 * - \ref CLIC_GetLevelIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetLevelIRQ(IRQn_Type IRQn, uint8_t lvl_abs)
{
    uint8_t nlbits = __CLIC_GetCfgNlbits();
    uint8_t intctlbits = (uint8_t)__CLIC_INTCTLBITS;

    if (nlbits == 0) {
        return;
    }

    if (nlbits > intctlbits) {
        nlbits = intctlbits;
    }
    uint8_t maxlvl = ((1 << nlbits) - 1);
    if (lvl_abs > maxlvl) {
        lvl_abs = maxlvl;
    }
    uint8_t lvl = lvl_abs << (CLIC_MAX_NLBITS - nlbits);
    uint8_t cur_ctrl = __CLIC_GetCtrlIRQ(IRQn);
    cur_ctrl = cur_ctrl << nlbits;
    cur_ctrl = cur_ctrl >> nlbits;
    __CLIC_SetCtrlIRQ(IRQn, (cur_ctrl | lvl));
}

/**
 * \brief  Get CLIC Interrupt level of a specific interrupt
 * \details
 * This function get interrupt level of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return         Interrupt level
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetLevelIRQ
 */
__STATIC_FORCEINLINE uint8_t __CLIC_GetLevelIRQ(IRQn_Type IRQn)
{
    uint8_t nlbits = __CLIC_GetCfgNlbits();
    uint8_t intctlbits = (uint8_t)__CLIC_INTCTLBITS;

    if (nlbits == 0) {
        return 0;
    }

    if (nlbits > intctlbits) {
        nlbits = intctlbits;
    }
    uint8_t intctrl = __CLIC_GetCtrlIRQ(IRQn);
    uint8_t lvl_abs = intctrl >> (CLIC_MAX_NLBITS - nlbits);
    return lvl_abs;
}

/**
 * \brief  Get CLIC Interrupt priority of a specific interrupt
 * \details
 * This function get interrupt priority of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      pri   Interrupt priority
 * \remarks
 * - IRQn must not be negative.
 * - If pri to be set is larger than the max priority allowed, it will be force to be max priority.
 * - Priority width is CLICINTCTLBITS minus clciinfo.nlbits if clciinfo.nlbits
 *   is less than CLICINTCTLBITS. Otherwise priority width is 0.
 * \sa
 * - \ref CLIC_GetPriorityIRQ
 */
__STATIC_FORCEINLINE void __CLIC_SetPriorityIRQ(IRQn_Type IRQn, uint8_t pri)
{
    uint8_t nlbits = __CLIC_GetCfgNlbits();
    uint8_t intctlbits = (uint8_t)__CLIC_INTCTLBITS;
    if (nlbits < intctlbits) {
        uint8_t maxpri = ((1 << (intctlbits - nlbits)) - 1);
        if (pri > maxpri) {
            pri = maxpri;
        }
        pri = pri << (CLIC_MAX_NLBITS - intctlbits);
        uint8_t mask = ((uint8_t)(-1)) >> intctlbits;
        pri = pri | mask;
        uint8_t cur_ctrl = __CLIC_GetCtrlIRQ(IRQn);
        cur_ctrl = cur_ctrl >> (CLIC_MAX_NLBITS - nlbits);
        cur_ctrl = cur_ctrl << (CLIC_MAX_NLBITS - nlbits);
        __CLIC_SetCtrlIRQ(IRQn, (cur_ctrl | pri));
    }
}

/**
 * \brief  Get CLIC Interrupt priority of a specific interrupt
 * \details
 * This function get interrupt priority of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return   Interrupt priority
 * \remarks
 * - IRQn must not be negative.
 * \sa
 * - \ref CLIC_SetPriorityIRQ
 */
__STATIC_FORCEINLINE uint8_t __CLIC_GetPriorityIRQ(IRQn_Type IRQn)
{
    uint8_t nlbits = __CLIC_GetCfgNlbits();
    uint8_t intctlbits = (uint8_t)__CLIC_INTCTLBITS;
    if (nlbits < intctlbits) {
        uint8_t cur_ctrl = __CLIC_GetCtrlIRQ(IRQn);
        uint8_t pri = cur_ctrl << nlbits;
        pri = pri >> nlbits;
        pri = pri >> (CLIC_MAX_NLBITS - intctlbits);
        return pri;
    } else {
        return 0;
    }
}

/**
 * \brief  Set Interrupt Vector of a specific interrupt
 * \details
 * This function set interrupt handler address of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \param [in]      vector   Interrupt handler address
 * \remarks
 * - IRQn must not be negative.
 * - You can set the \ref CSR_CSR_MTVT to set interrupt vector table entry address.
 * - If your vector table is placed in readonly section, the vector for IRQn will not be modified.
 *   For this case, you need to use the correct irq handler name defined in your vector table as
 *   your irq handler function name.
 * - This function will only work correctly when the vector table is placed in an read-write enabled section.
 * \sa
 * - \ref CLIC_GetVector
 */
__STATIC_FORCEINLINE void __CLIC_SetVector(IRQn_Type IRQn, rv_csr_t vector)
{
    volatile unsigned long vec_base;
    vec_base = ((unsigned long)__RV_CSR_READ(CSR_MTVT));
    vec_base += ((unsigned long)IRQn) * sizeof(unsigned long);
    (* (unsigned long *) vec_base) = vector;
#if (defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1))
#if (defined(__CCM_PRESENT) && (__CCM_PRESENT == 1))
    MFlushDCacheLine((unsigned long)vec_base);
#endif
#endif
#if (defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1))
#if (defined(__CCM_PRESENT) && (__CCM_PRESENT == 1))
    MInvalICacheLine((unsigned long)vec_base);
#else
    __FENCE_I();
#endif
#endif
}

/**
 * \brief  Get Interrupt Vector of a specific interrupt
 * \details
 * This function get interrupt handler address of the specific interrupt \em IRQn.
 * \param [in]      IRQn  Interrupt number
 * \return        Interrupt handler address
 * \remarks
 * - IRQn must not be negative.
 * - You can read \ref CSR_CSR_MTVT to get interrupt vector table entry address.
 * \sa
 *     - \ref CLIC_SetVector
 */
__STATIC_FORCEINLINE rv_csr_t __CLIC_GetVector(IRQn_Type IRQn)
{
#if __RISCV_XLEN == 32
    return (*(uint32_t *)(__RV_CSR_READ(CSR_MTVT)+IRQn*4));
#elif __RISCV_XLEN == 64
    return (*(uint64_t *)(__RV_CSR_READ(CSR_MTVT)+IRQn*8));
#else // TODO Need cover for XLEN=128 case in future
    return (*(uint64_t *)(__RV_CSR_READ(CSR_MTVT)+IRQn*8));
#endif
}

/**
 * \brief  Set Exception entry address
 * \details
 * This function set exception handler address to 'CSR_MTVEC'.
 * \param [in]      addr  Exception handler address
 * \remarks
 * - This function use to set exception handler address to 'CSR_MTVEC'. Address is 4 bytes align.
 * \sa
 * - \ref __get_exc_entry
 */
__STATIC_FORCEINLINE void __set_exc_entry(rv_csr_t addr)
{
    addr &= (rv_csr_t)(~0x3F);
    addr |= CLIC_MODE_MTVEC_Msk;
    __RV_CSR_WRITE(CSR_MTVEC, addr);
}

/**
 * \brief  Get Exception entry address
 * \details
 * This function get exception handler address from 'CSR_MTVEC'.
 * \return       Exception handler address
 * \remarks
 * - This function use to get exception handler address from 'CSR_MTVEC'. Address is 4 bytes align
 * \sa
 * - \ref __set_exc_entry
 */
__STATIC_FORCEINLINE rv_csr_t __get_exc_entry(void)
{
    unsigned long addr = __RV_CSR_READ(CSR_MTVEC);
    return (addr & ~CLIC_MODE_MTVEC_Msk);
}

/**
 * \brief  Get NMI interrupt entry from 'CSR_MTVEC'
 * \details
 * This function get NMI interrupt address from 'CSR_MTVEC'.
 * \return      NMI interrupt handler address
 * \remarks
 * - This function use to get NMI interrupt handler address from 'CSR_MTVEC'. 
 */
__STATIC_FORCEINLINE rv_csr_t __get_nmi_entry(void)
{
    return __RV_CSR_READ(CSR_MTVEC);
}

/**
 * \brief   Save necessary CSRs into variables for vector interrupt nesting
 * \details
 * This macro is used to declare variables which are used for saving
 * CSRs(MCAUSE, MEPC, MSUB), and it will read these CSR content into
 * these variables, it need to be used in a vector-interrupt if nesting
 * is required.
 * \remarks
 * - Interrupt will be enabled after this macro is called
 * - It need to be used together with \ref RESTORE_IRQ_CSR_CONTEXT
 * - Don't use variable names __mcause, __mpec,  in your ISR code
 * - If you want to enable interrupt nesting feature for vector interrupt,
 * you can do it like this:
 * \code
 * // __INTERRUPT attribute will generates function entry and exit sequences suitable
 * // for use in an interrupt handler when this attribute is present
 * __INTERRUPT void clic_mtip_handler(void)
 * {
 *     // Must call this to save CSRs
 *     SAVE_IRQ_CSR_CONTEXT();
 *     // !!!Interrupt is enabled here!!!
 *     // !!!Higher priority interrupt could nest it!!!
 *
 *     // put you own interrupt handling code here
 *
 *     // Must call this to restore CSRs
 *     RESTORE_IRQ_CSR_CONTEXT();
 * }
 * \endcode
 */
#define SAVE_IRQ_CSR_CONTEXT()                                              \
		rv_csr_t __mstatus = __RV_CSR_READ(CSR_MSTATUS);                    \
        rv_csr_t __mcause = __RV_CSR_READ(CSR_MCAUSE);                      \
        rv_csr_t __mepc = __RV_CSR_READ(CSR_MEPC);                          \
        __enable_irq();

/**
 * \brief   Restore necessary CSRs from variables for vector interrupt nesting
 * \details
 * This macro is used restore CSRs(MCAUSE, MEPC, MSUB) from pre-defined variables
 * in \ref SAVE_IRQ_CSR_CONTEXT macro.
 * \remarks
 * - Interrupt will be disabled after this macro is called
 * - It need to be used together with \ref SAVE_IRQ_CSR_CONTEXT
 */
#define RESTORE_IRQ_CSR_CONTEXT()                                           \
        __disable_irq();                                                    \
        __RV_CSR_WRITE(CSR_MEPC, __mepc);                                   \
        __RV_CSR_WRITE(CSR_MCAUSE, __mcause);								\
        __RV_CSR_WRITE(CSR_MSTATUS, __mstatus);

/** @} */ /* End of Doxygen Group RMSIS_Core_IntExc */

#endif /* defined(__CLIC_PRESENT) && (__CLIC_PRESENT == 1) */

#ifdef __cplusplus
}
#endif
#endif /** __CORE_FEATURE_CLIC__ */
