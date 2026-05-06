/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */
#include <assert.h>

#include <arch_helpers.h>
#include <qtiseclib_interface.h>

/*
 * This driver implements IPQ PSCI reset support for all IPQ chipsets
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

int qti_system_reset2(int is_vendor, int reset_type, u_register_t cookie)
{
	qtiseclib_psci_system_reset2(is_vendor, reset_type, cookie);
	return 0;
}
