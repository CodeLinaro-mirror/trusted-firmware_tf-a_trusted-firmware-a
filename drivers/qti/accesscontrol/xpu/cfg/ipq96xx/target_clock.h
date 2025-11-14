/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef TARGET_CLOCK_H
#define TARGET_CLOCK_H

#include <stdint.h>


/*
 * Target-specific XPU clock configuration
 *
 * IPQ96xx requires enabling the following clocks for XPU operation:
 * 1. SDCC1_AHB_CBCR - SDCC1 AHB clock
 * 2. SDCC1_ICE_CORE_CBCR - SDCC1 ICE core clock
 * 3. QPIC_CBCR - QPIC clock (must be enabled before QPIC_AHB)
 * 4. QPIC_AHB_CBCR - QPIC AHB clock
 *
 * Note: SDCC clocks are not enabled by previous bootloaders in
 * non-eMMC flash types, so they must be enabled here to prevent
 * AC static config failures for SDCC.
 */
static const uint32_t ac_xpu_clocks[] = {
	HWIO_ADDR(GCC_SDCC1_AHB_CBCR),
	HWIO_ADDR(GCC_SDCC1_ICE_CORE_CBCR),
	HWIO_ADDR(GCC_QPIC_CBCR),
	HWIO_ADDR(GCC_QPIC_AHB_CBCR),
};

/* Number of clocks */
#define AC_XPU_CLOCKS_COUNT	(sizeof(ac_xpu_clocks) / sizeof(uint32_t))

#endif /* TARGET_CLOCK_H */
