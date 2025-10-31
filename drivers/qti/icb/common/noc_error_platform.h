#ifndef __NOC_ERROR_PLATFORM_H__
#define __NOC_ERROR_PLATFORM_H__
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "noc_error.h"

/*
 * qti_noc_error_platform_get_propdata() - Get platform property data
 *
 * Return: Pointer to internal property data structure
 */
nocerr_propdata_type *qti_noc_error_platform_get_propdata(void);

/*
 * qti_noc_error_platform_get_propdata_oem() - Get OEM property data
 *
 * Return: Pointer to OEM property data structure
 */
nocerr_propdata_type_oem *qti_noc_error_platform_get_propdata_oem(void);

#endif /* __NOC_ERROR_PLATFORM_H__ */
