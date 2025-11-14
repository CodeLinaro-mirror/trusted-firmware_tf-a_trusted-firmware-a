/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * XPU4 HWIO definitions.
 */

#ifndef HAL_XPU4_HWIO_H
#define HAL_XPU4_HWIO_H

#include "hal_hwio.h"

/* Standard IN/OUT/MASK/SHIFT macros */
#define XPU4_IN(base, regsym)				__xpu4hwio_in(base, regsym)
#define XPU4_INI(base, regsym, index)			__xpu4hwio_ini(base, regsym, index)
#define XPU4_INI2(base, regsym, index1, index2) \
	__xpu4hwio_ini2(base, regsym, index1, index2)
#define XPU4_INM(base, regsym, mask)			__xpu4hwio_inm(base, regsym, mask)
#define XPU4_INMI(base, regsym, index, mask)		__xpu4hwio_inmi(base, regsym, index, mask)
#define XPU4_INMI2(base, regsym, index1, index2, mask) \
	__xpu4hwio_inmi2(base, regsym, index1, index2, mask)
#define XPU4_OUT(base, regsym, val)			__xpu4hwio_out(base, regsym, val)
#define XPU4_OUTI(base, regsym, index, val)		__xpu4hwio_outi(base, regsym, index, val)
#define XPU4_OUTI2(base, regsym, index1, index2, val) \
	__xpu4hwio_outi2(base, regsym, index1, index2, val)
#define XPU4_OUTM(base, regsym, mask, val)		__xpu4hwio_outm(base, regsym, mask, val)
#define XPU4_OUTMI(base, regsym, index, mask, val)	\
	__xpu4hwio_outmi(base, regsym, index, mask, val)
#define XPU4_OUTMI2(base, regsym, index1, index2, mask, val) \
	__xpu4hwio_outmi2(base, regsym, index1, index2, mask, val)
#define XPU4_ADDR(base, regsym)				__xpu4hwio_addr(base, regsym)
#define XPU4_ADDRI(base, regsym, index)			__xpu4hwio_addri(base, regsym, index)
#define XPU4_ADDRI2(base, regsym, index1, index2)	\
	__xpu4hwio_addri2(base, regsym, index1, index2)
#define XPU4_RMSK(regsym)				__xpu4hwio_rmsk(regsym)
#define XPU4_RSHFT(regsym)				__xpu4hwio_rshft(regsym)
#define XPU4_SHFT(regsym, fldsym)			__xpu4hwio_shft(regsym, fldsym)
#define XPU4_FMSK(regsym, fldsym)			__xpu4hwio_fmsk(regsym, fldsym)

/* Field manipulation macros */
#define XPU4_INF(base, regsym, field) \
	(XPU4_INM(base, regsym, XPU4_FMSK(regsym, field)) >> XPU4_SHFT(regsym, field))
#define XPU4_INFI(base, regsym, index, field) \
	(XPU4_INMI(base, regsym, index, XPU4_FMSK(regsym, field)) >> XPU4_SHFT(regsym, field))
#define XPU4_OUTF(base, regsym, field, val) \
	XPU4_OUTM(base, regsym, XPU4_FMSK(regsym, field), val << XPU4_SHFT(regsym, field))
#define XPU4_OUTFI(base, regsym, index, field, val) \
	XPU4_OUTMI(base, regsym, index, XPU4_FMSK(regsym, field), val << XPU4_SHFT(regsym, field))

/* Cached field manipulation macro */
#define XPU4_INFC(regval, regsym, field) \
	((regval & XPU4_FMSK(regsym, field)) >> XPU4_SHFT(regsym, field))

/* Direct address manipulation */
#define XPU4_INA(addr)					in_dword(addr)
#define XPU4_INAM(addr, mask)				in_dword_masked(addr, mask)
#define XPU4_OUTA(addr, val)				out_dword(addr, val)
#define XPU4_OUTAM(addr, mask, val) \
	out_dword_masked_ns(addr, mask, val, in_dword(addr))

#define __xpu4hwio_in(base, regsym)			HWIO_XPU4_##regsym##_IN(base)
#define __xpu4hwio_ini(base, regsym, index)		HWIO_XPU4_##regsym##_INI(base, index)
#define __xpu4hwio_ini2(base, regsym, index1, index2) \
	HWIO_XPU4_##regsym##_INI2(base, index1, index2)
#define __xpu4hwio_inm(base, regsym, mask)		HWIO_XPU4_##regsym##_INM(base, mask)
#define __xpu4hwio_inmi(base, regsym, index, mask)	HWIO_XPU4_##regsym##_INMI(base, index, mask)
#define __xpu4hwio_inmi2(base, regsym, index1, index2, mask) \
	HWIO_XPU4_##regsym##_INMI2(base, index1, index2, mask)
#define __xpu4hwio_out(base, regsym, val)		HWIO_XPU4_##regsym##_OUT(base, val)
#define __xpu4hwio_outi(base, regsym, index, val)	HWIO_XPU4_##regsym##_OUTI(base, index, val)
#define __xpu4hwio_outi2(base, regsym, index1, index2, val) \
	HWIO_XPU4_##regsym##_OUTI2(base, index1, index2, val)
#define __xpu4hwio_outm(base, regsym, mask, val)	HWIO_XPU4_##regsym##_OUTM(base, mask, val)
#define __xpu4hwio_outmi(base, regsym, index, mask, val) \
	HWIO_XPU4_##regsym##_OUTMI(base, index, mask, val)
#define __xpu4hwio_outmi2(base, regsym, index1, index2, mask, val) \
	HWIO_XPU4_##regsym##_OUTMI2(base, index1, index2, mask, val)
#define __xpu4hwio_addr(base, regsym)			HWIO_XPU4_##regsym##_ADDR(base)
#define __xpu4hwio_addri(base, regsym, index)		HWIO_XPU4_##regsym##_ADDR(base, index)
#define __xpu4hwio_addri2(base, regsym, index1, index2) \
	HWIO_XPU4_##regsym##_ADDRI2(base, index1, index2)
#define __xpu4hwio_rmsk(regsym)				HWIO_XPU4_##regsym##_RMSK
#define __xpu4hwio_fmsk(regsym, fldsym)			HWIO_XPU4_##regsym##_##fldsym##_BMSK
#define __xpu4hwio_rshft(regsym)			HWIO_XPU4_##regsym##_SHFT
#define __xpu4hwio_shft(regsym, fldsym)			HWIO_XPU4_##regsym##_##fldsym##_SHFT

#endif /* HAL_XPU4_HWIO_H */
