/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef HAL_HWIO_H
#define HAL_HWIO_H

/* Function definitions */
#define HWIO_ADDR(hwiosym)				HWIO_##hwiosym##_ADDR
#define HWIO_IN(hwiosym)				__msmhwio_in(hwiosym)
#define HWIO_INF(io, field) \
	(HWIO_INM(io, HWIO_FMSK(io, field)) >> HWIO_SHFT(io, field))
#define HWIO_INM(hwiosym, mask)				__msmhwio_inm(hwiosym, mask)
#define HWIO_OUT(hwiosym, val)				__msmhwio_out(hwiosym, val)
#define HWIO_OUTF(io, field, val) \
	HWIO_OUT(io, (HWIO_IN(io) & ~HWIO_FMSK(io, field)) | ((val) << HWIO_SHFT(io, field)))
#define HWIO_FMSK(io, field)				 HWIO_##io##_##field##_BMSK
#define HWIO_SHFT(io, field)				 HWIO_##io##_##field##_SHFT

/* Helper macros */
#define __msmhwio_in(hwiosym)				in_dword(HWIO_##hwiosym##_ADDR)
#define __msmhwio_inm(hwiosym, mask) \
	in_dword_masked(HWIO_##hwiosym##_ADDR, mask)
#define __msmhwio_out(hwiosym, val)			out_dword(HWIO_##hwiosym##_ADDR, val)

/* Memory access functions */
static inline uint32_t in_dword(uintptr_t addr)
{
	return *(volatile uint32_t *)addr;
}

static inline uint32_t in_dword_masked(uintptr_t addr, uint32_t mask)
{
	return (*(volatile uint32_t *)addr) & mask;
}

static inline void out_dword(uintptr_t addr, uint32_t val)
{
	*(volatile uint32_t *)addr = val;
}

static inline void out_dword_masked_ns(uintptr_t addr, uint32_t mask,
					uint32_t val, uint32_t current_reg_content)
{
	*(volatile uint32_t *)addr = ((current_reg_content & (~mask)) | (val & mask));
}

#endif /* HAL_HWIO_H */
