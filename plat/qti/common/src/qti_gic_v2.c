/*
 * Copyright (c) 2015-2018, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include <common/bl_common.h>
#include <drivers/arm/gicv2.h>
#include <platform.h>
#include <platform_def.h>
#include <qti_plat.h>
#include <qtiseclib_defs.h>
#include <qtiseclib_defs_plat.h>

/* Array of to be configured by the gic driver */
static const interrupt_prop_t qti_interrupt_props[] = {
	INTR_PROP_DESC(QTISECLIB_INT_ID_CPU_WAKEUP_SGI, GIC_HIGHEST_SEC_PRIORITY,
			GICV2_INTR_GROUP0, GIC_INTR_CFG_LEVEL),
	INTR_PROP_DESC(QTISECLIB_INT_ID_SEC_WDOG_BARK, GIC_HIGHEST_SEC_PRIORITY,
			GICV2_INTR_GROUP0, GIC_INTR_CFG_LEVEL),
	INTR_PROP_DESC(QTISECLIB_INT_ID_NON_SEC_WDOG_BITE, GIC_HIGHEST_SEC_PRIORITY,
			GICV2_INTR_GROUP0, GIC_INTR_CFG_LEVEL),
	INTR_PROP_DESC(QTISECLIB_INT_ID_RESET_SGI, GIC_HIGHEST_SEC_PRIORITY,
			GICV2_INTR_GROUP0, GIC_INTR_CFG_EDGE),
	INTR_PROP_DESC(QTISECLIB_INT_ID_TME_IPC, GIC_HIGHEST_SEC_PRIORITY,
			GICV2_INTR_GROUP0, GIC_INTR_CFG_EDGE)
};

static unsigned int target_mask_array[PLATFORM_CORE_COUNT];

const gicv2_driver_data_t qti_gic_data = {
	.gicd_base = QTI_GICD_BASE,
	.gicc_base = QTI_GICC_BASE,
	.interrupt_props = qti_interrupt_props,
	.interrupt_props_num = ARRAY_SIZE(qti_interrupt_props),
	.target_masks = target_mask_array,
	.target_masks_num = ARRAY_SIZE(target_mask_array),
};

void gic_set_spi_routing(unsigned int id, unsigned int irm, u_register_t target)
{
	return;
}

void plat_qti_gic_driver_init(void)
{
	/*
	 * The GICv2 driver is initialized in EL3 and does not need
	 * to be initialized again in SEL1. This is because the S-EL1
	 * can use GIC system registers to manage interrupts and does
	 * not need GIC interface base addresses to be configured.
	 */
	gicv2_driver_init(&qti_gic_data);
}

/******************************************************************************
 * ARM common helper to initialize the GIC. Only invoked by BL31
 *****************************************************************************/
void plat_qti_gic_init(void)
{

	gicv2_distif_init();
	gicv2_pcpu_distif_init();
	gicv2_set_pe_target_mask(plat_my_core_pos());
	gicv2_cpuif_enable();
}

/******************************************************************************
 * ARM common helper to enable the GIC CPU interface
 *****************************************************************************/
void plat_qti_gic_cpuif_enable(void)
{
	gicv2_cpuif_enable();
	gicv2_set_pe_target_mask(plat_my_core_pos());
}

/******************************************************************************
 * ARM common helper to disable the GIC CPU interface
 *****************************************************************************/
void plat_qti_gic_cpuif_disable(void)
{
	gicv2_cpuif_disable();
}

/******************************************************************************
 * ARM common helper to initialize the per-CPU distributor interface in GICv2
 *****************************************************************************/
void plat_qti_gic_pcpu_init(void)
{
	gicv2_pcpu_distif_init();
	gicv2_set_pe_target_mask(plat_my_core_pos());
}

