/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef HAL_XPU4_TARGET_H
#define HAL_XPU4_TARGET_H

#include <stdint.h>

#include "access_control_target.h"
#include "hal_xpu4.h"

/*
 * Protected range configuration for milestone-specific XPU setup
 */
typedef struct {
	uint64_t start;
	uint64_t end;
	HAL_xpu2_XPU2Type xpu_id;
} ACXpuProtectedRangeMS;

#endif /* HAL_XPU4_TARGET_H */
