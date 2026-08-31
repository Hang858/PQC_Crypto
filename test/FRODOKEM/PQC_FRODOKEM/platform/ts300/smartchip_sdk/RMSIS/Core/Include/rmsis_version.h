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
#ifndef __RMSIS_VERSION_H
#define __RMSIS_VERSION_H

/**
 * \defgroup RMSIS_Core_VersionControl    Version Control
 * \ingroup  RMSIS_Core
 * \brief    Version \#define symbols for RMSIS release specific C/C++ source code
 * \details
 *
 * We followed the [semantic versioning 2.0.0](https://semver.org/) to control RMSIS version.
 * The version format is **MAJOR.MINOR.PATCH**, increment the:
 * 1. MAJOR version when you make incompatible API changes,
 * 2. MINOR version when you add functionality in a backwards compatible manner, and
 * 3. PATCH version when you make backwards compatible bug fixes.
 *
 * The header file `rmsis_version.h` is included by each core header so that these definitions are available.
 *
 * **Example Usage for RMSIS Version Check**:
 * \code
 *   #if defined(__RMSIS_VERSION) && (__RMSIS_VERSION >= 0x00010105)
 *      #warning "Yes, we have RMSIS 1.1.5 or later"
 *   #else
 *      #error "We need RMSIS 1.1.5 or later!"
 *   #endif
 * \endcode
 *
 * @{
 */

/*!
 * \file     rmsis_version.h
 * \brief    RMSIS Version definitions
 **/

/**
 * \brief   Represent the RMSIS major version
 * \details
 * The RMSIS major version can be used to
 * differentiate between RMSIS major releases.
 * */
#define __RMSIS_VERSION_MAJOR            (1U)

/**
 * \brief   Represent the RMSIS minor version
 * \details
 * The RMSIS minor version can be used to
 * query a RMSIS release update including new features.
 *
 **/
#define __RMSIS_VERSION_MINOR            (0U)

/**
 * \brief   Represent the RMSIS patch version
 * \details
 * The RMSIS patch version can be used to
 * show bug fixes in this package.
 **/
#define __RMSIS_VERSION_PATCH            (1U)
/**
 * \brief   Represent the RMSIS Version
 * \details
 * RMSIS Version format: **MAJOR.MINOR.PATCH**
 * * MAJOR: \ref __RMSIS_VERSION_MAJOR, stored in `bits [31:16]` of \ref __RMSIS_VERSION
 * * MINOR: \ref __RMSIS_VERSION_MINOR, stored in `bits [15:8]` of \ref __RMSIS_VERSION
 * * PATCH: \ref __RMSIS_VERSION_PATCH, stored in `bits [7:0]` of \ref __RMSIS_VERSION
 **/
#define __RMSIS_VERSION                  ((__RMSIS_VERSION_MAJOR << 16U) | (__RMSIS_VERSION_MINOR << 8) | __RMSIS_VERSION_PATCH)

/** @} */ /* End of Doxygen Group RMSIS_Core_VersionControl */
#endif
