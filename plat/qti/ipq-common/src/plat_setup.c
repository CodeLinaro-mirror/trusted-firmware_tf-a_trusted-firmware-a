/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <stdbool.h>
#include <stdint.h>

#include <lib/mmio.h>
#include <qti_plat.h>
#include <platform.h>
#include <drivers/arm/gic_common.h>

/**
 * Configure LLCC scheme ID for non-secure world context switch
 * Maps CPU cores to LLCC scheme IDs:
 * - Regular cores: Scheme ID 0
 * - Last core (reserved): Scheme ID 1
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

	qti_configure_clusterthreadsid_for_nsworld(is_last_core);
}
#endif

/**
 * Platform-specific post cold init function for IPQ platforms
 * Configures CLUSTERBUSQOS and MIBU infrastructure registers
 * Also configures GICD NSACR registers for MSI interrupt handling
 * Called only on core 0 during cold boot
 * Note: ACTLR_EL3/EL2 configured in plat_reset_handler on all cores
 * @param [in] void
 * @return void.
 */
void qti_post_cold_init(void)
{
	int i;

#ifdef ENABLE_LLCC_CFG
	/* Configure CLUSTERBUSQOS_EL1 once on boot core */
	qti_configure_clusterbusqos();

	/* Configure MIBU infrastructure registers */
	mmio_write_32(APSS_SHARED_MIBU_INFRA_SCID_ADDR, MIBU_INFRA_SCID_VALUE);
	mmio_write_32(APSS_SHARED_MIBU_INFRA_QOS_ADDR, MIBU_INFRA_QOS_VALUE);
#endif

	/*
	 * Configure GICD NSACR registers to allow NS world access to MSI interrupts.
	 *
	 * Security Rationale:
	 * - Interrupt IDs in the PCIe MSI range are dedicated to PCIe MSI handling
	 * - These interrupts MUST be accessible from NS world for PCIe device drivers
	 * - MSI interrupts are edge-triggered and do not pose security risk when NS-accessible
	 * - No secure services use these interrupt IDs
	 *
	 * NSACR Configuration:
	 * - Each NSACR register controls 16 interrupts (2 bits per interrupt)
	 * - 0xFFFFFFFF = Full NS access (bits 11) for all 16 interrupts per register
	 * - Allows NS world to: configure priority, enable/disable, set pending, clear active
	 * - Required for Linux PCIe MSI framework to function correctly
	 */
	for (i = 0; i < PCIE_MSI_NSACR_COUNT; i++) {
		gicd_write_nsacr(QTI_GICD_BASE,
				 PCIE_MSI_INT_BASE + (i * 16),
				 GICD_NSACR_NS_ACCESS_FULL);
	}
}
