/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <stdbool.h>
#include <stdint.h>

#include <lib/mmio.h>
#include <qti_plat.h>
#include <platform.h>

/**
 * Configure LLCC scheme ID for non-secure world context switch
 * Maps CPU cores to LLCC scheme IDs:
 * - Cores 0-3: Scheme ID 0
 * - Core 4: Scheme ID 1
 * @param [in] void
 * @return void.
 */
#ifdef ENABLE_LLCC_CFG
void qti_configure_clusterthreadsid_nsworld(void)
{
	uint32_t cpu_num;
	bool is_last_core;

	cpu_num = ((plat_my_core_pos()) % QTISECLIB_PLAT_CORE_COUNT);
	is_last_core = (cpu_num == (QTISECLIB_PLAT_CORE_COUNT - 1));

	return qti_configure_clusterthreadsid_for_nsworld(is_last_core);
}
#endif

/**
 * Platform-specific post cold init function for ipq96xx
 * Configures MIBU infrastructure registers required for LLCC operation
 * @param [in] void
 * @return void.
 */
void qti_post_cold_init(void)
{
#ifdef ENABLE_LLCC_CFG
	/* Configure MIBU infrastructure registers for LLCC */
	mmio_write_32(APSS_SHARED_MIBU_INFRA_SCID_ADDR, MIBU_INFRA_SCID_VALUE);
	mmio_write_32(APSS_SHARED_MIBU_INFRA_QOS_ADDR, MIBU_INFRA_QOS_VALUE);
#endif
}
