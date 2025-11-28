#ifndef __NOC_ERROR_DECODE_H__
#define __NOC_ERROR_DECODE_H__
/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "noc_error.h"
#include "qtiseclib_cb_interface.h"
#include "string.h"
#include <inttypes.h>

typedef struct NOC_decode_data {
	bool offset_flag;
	uint8_t target_index;
	uint8_t initiator_index;
	uint8_t initiator_type;
	uint8_t bid;
	uint8_t pid;
	uint8_t mid;
	uint8_t lpid;
	uint8_t srcid;
	uint8_t tgtid;
	uint8_t errcode;
	uint8_t log_info_vld;
} noc_decode_data_type;

void qti_noc_error_decode(nocerr_info_type *nocerr_info);

void noc_decode_target_initiator(uint32_t errlog1_low, char *noc_type);
void noc_decode_violation_addr(uint32_t errlog2_high, uint32_t errlog2_low,
			       char *noc_type);
void noc_decode_id(uint32_t errlog1_high, char *noc_type);
void noc_decode_errcode(uint32_t errlog0_low, char *noc_type);

#endif /* __NOC_ERROR_DECODE_H__ */
