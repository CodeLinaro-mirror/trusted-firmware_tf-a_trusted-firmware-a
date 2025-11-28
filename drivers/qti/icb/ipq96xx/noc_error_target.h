#ifndef __NOC_ERROR_TARGET_H__
#define __NOC_ERROR_TARGET_H__

/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

// FIXME_IPQ96xx
// TODO: Update CHIPINFO when it is available in IPCAT
#define CHIPINFO_FAMILY_IPQ96xx	 157

/* Performs target-specific NOC error handler initialization */
void qti_noc_error_init_target(nocerr_info_type *noc_info_list,
	uint32_t noc_cnt,
	nocerr_info_type_oem *noc_info_oem_list);

/* Performs target-specific error action - returns true if error is fatal */
bool qti_noc_error_handle_target(nocerr_info_type *noc_info_list,
	nocerr_info_type_oem *noc_info_oem_list,
	bool *delay_crash);

#endif /* __NOC_ERROR_TARGET_H__ */
