/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "noc_error.h"
#include "noc_error_platform.h"
#include <stdlib.h>

/*
 * qti_noc_error_init_target() - Initialize target specific NOC error handling
 * @noc_info: Pointer to NOC info structure
 * @noc_cnt: Number of NOC instances
 * @noc_info_oem: Pointer to OEM NOC info structure
 */
void qti_noc_error_init_target(nocerr_info_type *noc_info,
				uint32_t noc_cnt,
				nocerr_info_type_oem *noc_info_oem)
{
	(void)noc_info;
	(void)noc_cnt;
	(void)noc_info_oem;
}

/*
 * qti_noc_error_handle_target() - Handle target specific NOC error processing
 * @noc_info: Pointer to NOC info structure
 * @noc_info_oem: Pointer to OEM NOC info structure
 * @delay_crash: Pointer to flag indicating whether to delay crash
 *
 * Return: true if error was handled successfully
 */
bool qti_noc_error_handle_target(nocerr_info_type *noc_info,
				  nocerr_info_type_oem *noc_info_oem,
				  bool *delay_crash)
{
	(void)noc_info;
	(void)noc_info_oem;

	if (delay_crash != NULL)
		*delay_crash = false;

	return true;
}
