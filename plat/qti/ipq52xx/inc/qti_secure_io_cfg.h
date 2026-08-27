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

#ifndef QTI_SECURE_IO_CFG_H
#define QTI_SECURE_IO_CFG_H

#include <stdint.h>

/*
 * List of peripheral/IO memory areas that are protected from
 * non-secure world but not required to be secure.
 */

#define TCSR_BOOT_MISC_DETECT	0x195C100
#define TCSR_BOOT_INFO		0x195C158
#define EUD_EUD_EN2		0x7A000
#define TCSR_FUSE_SEC_HW_KEY_0	0x193D424
#define TCSR_FUSE_SEC_HW_KEY_1	0x193D428
#define TCSR_FUSE_SEC_HW_KEY_2	0x193D42C
#define TCSR_FUSE_SEC_HW_KEY_3	0x193D430
#define TCSR_FUSE_SEC_HW_KEY_4	0x193D434
#define TCSR_FUSE_SEC_HW_KEY_5	0x193D438
#define TCSR_FUSE_SEC_HW_KEY_6	0x193D43C
#define TCSR_FUSE_SEC_HW_KEY_7	0x193D440

static const uintptr_t qti_secure_io_allowed_regs[] = {
	TCSR_BOOT_MISC_DETECT,
	TCSR_BOOT_INFO,
	EUD_EUD_EN2,
	TCSR_FUSE_SEC_HW_KEY_0,
	TCSR_FUSE_SEC_HW_KEY_1,
	TCSR_FUSE_SEC_HW_KEY_2,
	TCSR_FUSE_SEC_HW_KEY_3,
	TCSR_FUSE_SEC_HW_KEY_4,
	TCSR_FUSE_SEC_HW_KEY_5,
	TCSR_FUSE_SEC_HW_KEY_6,
	TCSR_FUSE_SEC_HW_KEY_7
};

#endif /* QTI_SECURE_IO_CFG_H */

