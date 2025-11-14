/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef HAL_XPU4_UTIL_H
#define HAL_XPU4_UTIL_H

#include "hal_xpu4_hwio.h"
#include "hal_xpu4_hwio_generic.h"

typedef enum {
	REGION_EQUAL,
	REGION_NO_OVERLAP,
	REGION_ADJACENT,
	REGION_OVERLAP_INNER,
	REGION_OVERLAP_INNER_ADJACENT,
	REGION_OVERLAP_OUTER,
	REGION_OVERLAP_OUTER_ADJACENT,
	REGION_OVERLAP,
} region_overlap_t;

static inline uint32_t xpu4_decode_idr_xpress_cfg_en(ac_xpu4_priv_info *xpuInfo)
{
	return XPU4_INFC(xpuInfo->idr[0], IDR0, XPRESSCFGEN);
}

static inline uint32_t xpu4_decode_xpu_type(ac_xpu4_priv_info *xpuInfo)
{
	return XPU4_INFC(xpuInfo->idr[0], IDR0, XPU_TYPE);
}

static inline uint32_t xpu4_get_msb_client_addr_width(ac_xpu4_priv_info *xpuInfo)
{
	return XPU4_INFC(xpuInfo->idr[1], IDR1, CLIENT_ADDR_WIDTH);
}

static inline uint32_t xpu4_decode_idr_addr_msb(ac_xpu4_priv_info *xpuInfo)
{
	return XPU4_INFC(xpuInfo->idr[1], IDR1, ADDR_MSB);
}

static inline uint32_t xpu4_decode_idr_addr_lsb(ac_xpu4_priv_info *xpuInfo)
{
	return XPU4_INFC(xpuInfo->idr[1], IDR1, ADDR_LSB);
}

static inline uint32_t xpu4_read_idr2_qad(ac_xpu4_priv_info *xpuInfo)
{
	return XPU4_INFC(xpuInfo->idr[2], IDR2, NQAD);
}

static inline uint32_t xpu4_get_rg_count(ac_xpu4_priv_info *xpuInfo)
{
	return XPU4_INFC(xpuInfo->idr[0], IDR0, NRG);
}

static inline uint32_t xpu4_get_minor_version(ac_xpu4_priv_info *xpuInfo)
{
	return XPU4_INFC(xpuInfo->rev, REV, MINOR);
}

/*
 * Read the xPU RG enable status from HW
 * Internal API caller makes sure ptr is not NULL
 */
static inline bool xpu4_is_rg_enabled(ac_xpu4_priv_info *xpuInfo, uint32_t rg_num)
{
	return (XPU4_INFI(xpuInfo->addr, RGCR1n, rg_num, RGE) == ENABLE);
}

/*
 * Get the xPU information structure from the xPU ID
 *
 * @param xpu_id	xPU ID for which xPU information structure has to be fetched
 *
 * @return Valid xPU information structure on success (or) NULL in failure
 */
ac_xpu4_priv_info *ac_xpu_get_xpu_info(HAL_xpu2_XPU2Type xpu_id);

/*
 * Determine if the given xPU is RA type
 *
 * @param xpu_id	xPU ID for which xPU RA information has to be fetched
 *
 * @return TRUE if the xPU is an RA xPU else FALSE
 */
bool ac_is_ra_xpu(HAL_xpu2_XPU2Type xpu_id);

/*
 * Get the QAD vector for current execution environment
 * Compile time flag determines the current execution environment
 *
 * @return QAD vector
 */
ac_xpu4_qad_vector ac_xpu4_get_env_qad_vector(void);

AC_ERROR region_overlap(uint64_t start1, uint64_t end1,
			uint64_t start2, uint64_t end2,
			region_overlap_t *overlap);

/*
 * Validate the QAD configuration xPU static program possibility from earlier QAD RoT
 * Compile time flag determines the current execution environment as well earlier RoT
 *
 * @param readPermVector	Read/Write QAD vector
 * @param writePermVector	Write QAD vector
 *
 * @return True if configuration possibility by earlier QAD (or) false if not
 */
bool ac_xpu_rg_configured_by_earlier_rot(ac_xpu4_qad_vector readPermVector,
					  ac_xpu4_qad_vector writePermVector);

/*
 * Convert the AC secure domain structure format details in
 * xPU HW specific Read/Write/Lock QAD vector format
 *
 * @param perms			Array of structure holding AC secure domain ID,
 *				R/W permission and lock details
 * @param count			Number of elements in the array
 * @param readPermVector	Output where read permission QAD to be filled
 * @param writePermVector	Output where Write permission QAD to be filled
 * @param lockVector		Output where Lock QAD to be filled
 *
 * NOTE: Struct elements in "perms" arg must be validated before calling this function
 */
void ac_xpu_encode_qad_vector3(ac_sec_domain_perm *perms,
			       uint32_t count,
				ac_xpu4_qad_vector *readPermVector,
				ac_xpu4_qad_vector *writePermVector,
				ac_xpu4_qad_vector *lockVector);

/*
 * Convert the AC secure domain ID to HW QAD vector
 *
 * @param domainIds	Array holding AC secure domain IDs
 * @param count		Number of elements in the array
 *
 * @return QAD vector format
 */
ac_xpu4_qad_vector ac_xpu_encode_qad_vector(ac_sec_domain_id *domainIds,
					     uint32_t count);

/*
 * Fetch the R/W QAD programmed during protect/lock from
 * data cached in SW
 *
 * @param xpuInfo		xPU information structure holding ID, dynamic RG details
 * @param rg_num		RG number for which R/W QAD has to be fetched
 * @param readPermVector	Output where read permission QAD to be filled
 * @param writePermVector	Output where write permission QAD to be filled
 */
void ac_xpu_get_shadow_perm(ac_xpu4_priv_info *xpuInfo,
			     uint32_t rg_num,
			     ac_xpu4_qad_vector *readPermVector,
			     ac_xpu4_qad_vector *writePermVector);

/*
 * Store the R/W QAD programmed during protect/lock into
 * SW cached data structure
 *
 * @param xpuInfo		xPU information structure holding ID, dynamic RG details
 * @param rg_num		RG number for which R/W QAD has to be fetched
 * @param readPermVector	Read permission QAD to be populated
 * @param writePermVector	Write permission QAD to be populated
 */
void ac_xpu_cache_shadow_perm(ac_xpu4_priv_info *xpuInfo,
			       uint32_t rg_num,
			       ac_xpu4_qad_vector readPermVector,
			       ac_xpu4_qad_vector writePermVector);

/*
 * Convert the SOC address into address to be programmed in xPU HW
 *
 * @param xpuInfo	xPU information structure holding ID, dynamic RG details
 * @param addr		Input addr which has to be formatted
 *
 * @return Formatted output address
 */
uint64_t ac_xpu_addr_soc2_peripheral_xpu(ac_xpu4_priv_info *xpuInfo, uint64_t addr);

/*
 * Read CLERE register value from XPU
 *
 * @param xpu_info	XPU info structure
 *
 * @return CLERE register value
 */
ac_xpu4_qad_vector ac_xpu_read_clere(ac_xpu4_priv_info *xpu_info);

/*
 * Write CLERE register value to XPU
 *
 * @param xpu_info	XPU info structure
 * @param value		Value to write to CLERE register
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_write_clere(ac_xpu4_priv_info *xpu_info, ac_xpu4_qad_vector value);

#endif /* HAL_XPU4_UTIL_H */
