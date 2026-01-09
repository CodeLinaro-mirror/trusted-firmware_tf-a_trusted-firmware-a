/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef __QTI_SECURE_IO_CFG_H__
#define __QTI_SECURE_IO_CFG_H__

#include <stdint.h>

/*
 * List of peripheral/IO memory areas that are protected from
 * non-secure world but not required to be secure.
 */

#define TCSR_BOOT_MISC_DETECT	0x195C100
#define TCSR_BOOT_INFO		0x195C158

static const uintptr_t qti_secure_io_allowed_regs[] = {
	TCSR_BOOT_MISC_DETECT,
	TCSR_BOOT_INFO
};

#endif /* __QTI_SECURE_IO_CFG_H__ */

