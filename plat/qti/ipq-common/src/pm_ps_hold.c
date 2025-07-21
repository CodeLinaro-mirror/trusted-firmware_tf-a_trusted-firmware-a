/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */
#include <assert.h>

#include <arch_helpers.h>
#include <qtiseclib_interface.h>

/*
 * This driver implements IPQ96xx PSCI reset support
 */

__dead2 void qti_system_off(void)
{
	qtiseclib_psci_system_off();
	while(1); /* Should not return */
}

__dead2 void qti_system_reset(void)
{
	qtiseclib_psci_system_reset();
	while(1); /* Should not return */
}
