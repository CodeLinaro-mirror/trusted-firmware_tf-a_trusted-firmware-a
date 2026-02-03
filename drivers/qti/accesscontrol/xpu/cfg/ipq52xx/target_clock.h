/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef _TARGET_CLOCK_H
#define _TARGET_CLOCK_H

#include <stdint.h>

/**
 * @brief Target-specific XPU clock configuration
 *
 * Note: SDCC clocks are not enabled by previous bootloaders in
 * non-eMMC flash types, so they must be enabled here to prevent
 * AC static config failures for SDCC.
 *
 * Statically defined structure with clocks to be turned on
 */
static const uint32_t ac_xpu_clocks[] = {
	HWIO_ADDR(GCC_SDCC1_AHB_CBCR),
	HWIO_ADDR(GCC_SDCC1_ICE_CORE_CBCR),
	HWIO_ADDR(GCC_QPIC_CBCR),
	HWIO_ADDR(GCC_QPIC_AHB_CBCR),
};

/* Number of clocks */
#define AC_XPU_CLOCKS_COUNT (sizeof(ac_xpu_clocks) / sizeof(uint32_t))

#endif /* _TARGET_CLOCK_H */
