/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef QTI_RNG_IO_H
#define QTI_RNG_IO_H

/*
 * HERMOSA (IPQ5200) QRNG Hardware Register Addresses
 * These registers are mapped to EE5 currently aligning
 * with PBL
 */
#define SEC_PRNG_STATUS			0x4C5004
#define SEC_PRNG_STATUS_DATA_AVAIL_BMSK	0x1
#define SEC_PRNG_DATA_OUT		0x4C5000

#endif /* QTI_RNG_IO_H */
