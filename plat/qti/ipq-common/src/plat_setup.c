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
typedef enum {
	CLUSTPART_CONTROL_L3_DEFAULT = 0,
	CLUSTPART_CONTROL_L3_25_PERCENT = 1,
	CLUSTPART_CONTROL_L3_50_PERCENT = 2,
	CLUSTPART_CONTROL_L3_75_PERCENT = 3,
} cluster_part_control_func_id;

/*
 * Configure CLUSTERPARTCR_EL1 register for LLCC cluster partitioning
 * @func_id: L3 cache partition size (0=default, 1=25%, 2=50%, 3=75%)
 * Return: 0 on success, -1 on invalid function ID
 */
int qti_configure_cluster_partition_control(uint32_t func_id)
{
	uint64_t value = 0;

	switch (func_id) {
	case CLUSTPART_CONTROL_L3_DEFAULT:
		value = 0x0;
		break;
	case CLUSTPART_CONTROL_L3_25_PERCENT:
		value = 0x2D;
		break;
	case CLUSTPART_CONTROL_L3_50_PERCENT:
		value = 0x69;
		break;
	case CLUSTPART_CONTROL_L3_75_PERCENT:
		value = 0xE1;
		break;
	default:
		return -1;
	}

	/* Write to CLUSTERPARTCR_EL1 (S3_0_C15_C4_3) */
	__asm__ volatile(
		"msr S3_0_C15_C4_3, %0\n"
		"dsb sy\n"
		"isb\n"
		:
		: "r"(value)
		: "memory"
	);

	return 0;
}

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
