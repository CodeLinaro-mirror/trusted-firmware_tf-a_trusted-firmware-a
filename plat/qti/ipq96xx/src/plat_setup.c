/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <stdbool.h>
#include <stdint.h>

#include <lib/mmio.h>
#include <qti_plat.h>
#include <qtiseclib_interface.h>
#include <platform.h>
/**
 * Helper function to configure Scheme ID for ARM Cores when jumping to NS world/cold init
 * @param [in] void
 * @return void.
 */
#ifdef ENABLE_LLCC_CFG
void qti_configure_clusterthreadsid_nsworld(void)
{
	uint32_t cpu_num;

	cpu_num = ((plat_my_core_pos()) % QTISECLIB_PLAT_CORE_COUNT);

	return mon_configure_clusterthreadsid_for_nsworld(((cpu_num == 4u) ? true : false));
}
#endif

/**
 * Platform-specific post cold init function for ipq96xx
 * Configures MIBU and CLUSTERBUSQOS settings
 * @param [in] void
 * @return void.
 */
void qtiseclib_post_cold_init(void)
{
#ifdef ENABLE_LLCC_CFG
	/* MIBU Configurations required for LLCC SCID Configuration */
	mmio_write_32(APSS_SHARED_MIBU_INFRA_SCID_ADDR, MIBU_INFRA_SCID_VALUE);
	mmio_write_32(APSS_SHARED_MIBU_INFRA_QOS_ADDR, MIBU_INFRA_QOS_VALUE);

	/* Configure CLUSTERBUSQOS_EL1 during cold/warm boot of the ARM cores */
	mon_configure_clusterbusqos();

	/*
	 * Configure ACTLR_EL3.SMEN to 1, so that S-EL1 have write access to
	 * several cluster control registers including CLUSTERPARTCR_EL1, which
	 * is needed for L3 partitioning
	 */
	mon_configure_actlr_el3();

	/* Configure ACTLR_EL2.SMEN to 1 for EL2 access control */
	mon_configure_actlr_el2();

#endif
}
