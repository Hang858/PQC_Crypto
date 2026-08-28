/*
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 * -- Adaptable modifications made for Smartchip Processors. --
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
#ifndef __RMSIS_CORE_H__
#define __RMSIS_CORE_H__

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#include "rmsis_version.h"

/**
 * \ingroup RMSIS_Core_VersionControl
 * @{
 */
/* The following enum __SMARTCHIP_RV_REV/__SMARTCHIP_RVX_REV definition in this file
 * is only used for doxygen documentation generation,
 * The <device>.h is the real file to define it by vendor
 */
#if defined(__ONLY_FOR_DOXYGEN_DOCUMENT_GENERATION__)
/**
 * \brief Smartchip RV class core revision number
 * \details
 * Reversion number format: [15:8] revision number, [7:0] patch number
 * \attention
 * This define is exclusive with \ref __SMARTCHIP_RVX_REV
 */
#define __SMARTCHIP_RV_REV                   (0x0104)
/**
 * \brief Smartchip RVX class core revision number
 * \details
 * Reversion number format: [15:8] revision number, [7:0] patch number
 * \attention
 * This define is exclusive with \ref __SMARTCHIP_RV_REV
 */
#define __SMARTCHIP_RVX_REV                  (0x0100)
#endif /* __ONLY_FOR_DOXYGEN_DOCUMENT_GENERATION__ */
/** @} */ /* End of Group RMSIS_Core_VersionControl */

#include "rmsis_compiler.h"     /* RMSIS compiler specific defines */

/* === Include Smartchip Core Related Headers === */
/* Include core base feature header file */
#include "core_feature_base.h"



/* Include core fpu feature header file */
#include "core_feature_fpu.h"
/* Include core dsp feature header file */
#include "core_feature_dsp.h"
/* Include core vector feature header file */
#include "core_feature_vector.h"
/* Include core bitmanip feature header file */
#include "core_feature_bitmanip.h"
/* Include core pmp feature header file */
#include "core_feature_pmp.h"
/* Include core cache feature header file */
#include "core_feature_cache.h"

/* Include compatiable functions header file */
#include "core_compatiable.h"

#ifndef __RMSIS_GENERIC
/* Include core clic feature header file */
#include <core_feature_clic.h>
/* Include core systimer feature header file */
#include "core_feature_timer.h"
#endif


#ifdef __cplusplus
}
#endif
#endif /* __RMSIS_CORE_H__ */
