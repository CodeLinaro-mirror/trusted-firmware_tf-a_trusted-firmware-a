/*
 * Copyright (c) 2015-2018, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2018,2020, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */
#include <assert.h>
#include <errno.h>
#include <stdint.h>

#include <common/debug.h>
#include <arch_helpers.h>
#include <bl31/interrupt_mgmt.h>
#include <drivers/arm/gic_common.h>
#include <lib/el3_runtime/context_mgmt.h>

#include <platform.h>
#include <qti_interrupt_svc.h>
#include <qtiseclib_defs_plat.h>
#include <qtiseclib_interface.h>

#define QTI_INTR_INVALID_INT_NUM		0xFFFFFFFFU

/*
 * Top-level EL3 interrupt handler.
 */
static uint64_t qti_el3_interrupt_handler(uint32_t id, uint32_t flags,
					  void *handle, void *cookie)
{
	uint32_t irq = QTI_INTR_INVALID_INT_NUM;

	/*
	 * Previously, the system asserted that interrupts should not occur when
	 * at EL3 / Secure. This has been updated to handle both secure and
	 * non-secure interrupt sources by checking the interrupt's origin.
	 *
	 * If the interrupt source indicates a Non-Secure origin, the current
	 * context (`handle`) must match the `NON_SECURE` context.
	 * Otherwise (if the interrupt source is Secure), the current context
	 * must match the `SECURE` context. This ensures that the system
	 * handles the interrupt within the appropriate security state and
	 * prevents unauthorized access or context mixing.
	 */
	if (get_interrupt_src_ss(flags) == NON_SECURE) {
		assert(handle == cm_get_context(NON_SECURE));
	} else {
		assert(handle == cm_get_context(SECURE));
	}

	irq = plat_ic_acknowledge_interrupt();

	qtiseclib_invoke_isr(irq, handle);

	/* End of Interrupt. */
	if (irq < 1022U) {
		plat_ic_end_of_interrupt(irq);
	}

	return (uint64_t) handle;
}

/*
 * Handler for interrupts delegated to EL3 via SMC from SEL1/OP-TEE .
 *
 * In GICv2 systems with a Secure payload (OP-TEE), all secure interrupts are
 * initially routed to SEL1/OP-TEE because GICv2 supports only a single group
 * for secure interrupts (Group 0). However, certain interrupts need to be
 * handled at EL3 rather than SEL1. To support this, OP-TEE makes an SMC call
 * to delegate these specific interrupts back to EL3 for processing.
 *
 * This function validates the interrupt ID and invokes the appropriate EL3
 * interrupt service routine. Uses the existing qtiseclib interrupt management
 * framework to register and invoke ISRs. Return Failure on invalid interrupt.
 */
int qti_handle_sel1_routed_interrupt(uint32_t intr_num, void *handle)
{
	if (handle != cm_get_context(SECURE)) {
		WARN("Interrupt from Non-secure caller not permitted.\n");
		return -EPERM;
	}

	/*
	 * Verify the interrupt is active. SEL1 should have acknowledged
	 * the interrupt, transitioning it from pending to active state,
	 * before delegating to EL3.
	 */
	if (!plat_ic_get_interrupt_active(intr_num)) {
		WARN("Interrupt %u not acknowledged by SEL1\n", intr_num);
		return -EINVAL;
	}

	switch (intr_num) {
	case QTISECLIB_INT_ID_SEC_WDOG_BARK:
	case QTISECLIB_INT_ID_NON_SEC_WDOG_BITE:
		/* Valid interrupt - invoke the ISR */
		qtiseclib_invoke_isr(intr_num, handle);
		return 0;
	default:
		/* Invalid interrupt number */
		WARN("SEL1 routed Interrupt ID invalid: %u\n", intr_num);
		return -EINVAL;
	}
}

int qti_interrupt_svc_init(bool have_sel1)
{
	int ret;
	uint64_t flags = 0U;

	/*
	 * Route EL3 interrupts to EL3 when in Non-secure.
	 * Note: EL3 won't have interrupts enabled.
	 * When we have a Secure EL1 interrupt handler, allow it
	 * to handle Secure interrupts.
	 */
	set_interrupt_rm_flag(flags, NON_SECURE);
	if (!have_sel1)
		set_interrupt_rm_flag(flags, SECURE);

	/* Register handler for EL3 interrupts */
	ret = register_interrupt_type_handler(INTR_TYPE_EL3,
					      qti_el3_interrupt_handler, flags);
	assert(ret == 0);

	return ret;
}
