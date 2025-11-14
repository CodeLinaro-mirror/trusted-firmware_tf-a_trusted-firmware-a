/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * HAL XPU4 implementation
 */

#include <stdbool.h>
#include <stdint.h>

#include <qti_xpu_err.h>

#include "access_control_target.h"
#include "access_control_xpu.h"
#include "ac_xpu.h"
#include "ac_xpu_target_info.h"
#include "hal_hwio.h"
#include "hal_xpu4.h"
#include "hal_xpu4_util.h"
#include "target_hwio.h"

#define APNSPE_DISABLE			0
#define APNSPE_ENABLE			1

/*
 * For XPRESSCFG enabled XPUs there will be a set of registers which represent
 * a bitmap, where each bit is mapped to the XPU RG of the same index. This
 * macro will calculate the number of 32 bit registers that will be present
 * based on the number of RGs present in the XPU hardware
 */
#define NRG_TO_IGNORED_REG_COUNT(nrg)	((nrg+31)/32)

/* Bit to write in GRP{0/1}_RG_PRG Register to trigger XPRESSCFG profile loading */
#define XPRESSCFG_TRIGGER_BIT		0x1

/*
 * Index needs to correspond to the QAD's value.
 * i.e. APPS is bit 0, at index 0, TME_ROM is QAD 1 and is at index 1, etc
 */
static const uint32_t g_valid_qads[] = {
	(APP_SEC_QAD_BIT | APP_NSEC_QAD_BIT),	/* Bit 0 */
	TME_ROM_QAD_BIT,			/* Bit 1 */
	TME_FW_QAD_BIT,				/* Bit 2 */
	DEBUG_QAD_BIT,				/* Bit 3 */
	AOP_QAD_BIT,				/* Bit 4 */
	MODEM_QAD_BIT,				/* Bit 5 */
	PRIME_QAD_BIT,				/* Bit 6 */
	RESERVED_7_QAD_BIT,			/* Bit 7 */
	RESERVED_8_QAD_BIT,			/* Bit 8 */
	RESERVED_9_QAD_BIT,			/* Bit 9 */
	RESERVED_10_QAD_BIT,			/* Bit 10 */
	RESERVED_11_QAD_BIT,			/* Bit 11 */
};
static const uint32_t g_num_valid_qads = ARRAY_SIZE(g_valid_qads);

/*
 * Read APPS0 RG PRG register
 *
 * @param xpu_info	XPU info structure
 *
 * @return Register value
 */
static inline bool xpu4_read_apps0_rg_prg(ac_xpu4_priv_info *xpu_info)
{
	return XPU4_INF(xpu_info->addr, APPS_GRP0_RG_PRG, APPS0_RG_PRG) == 1;
}

/*
 * Write APPS0 RG PRG register
 *
 * @param xpu_info	XPU info structure
 * @param value		Value to write
 */
static inline void xpu4_write_apps0_rg_prg(ac_xpu4_priv_info *xpu_info,
					    uint32_t value)
{
	XPU4_OUTF(xpu_info->addr, APPS_GRP0_RG_PRG, APPS0_RG_PRG, value);
}

/*
 * Read unmapped region permissions
 *
 * @param xpu_info	XPU info structure
 *
 * @return Unmapped region permissions
 */
static inline ac_xpu4_qad_vector xpu4_read_unmapped_region_perms(ac_xpu4_priv_info *xpu_info)
{
	return XPU4_IN(xpu_info->addr, UMRPERMREG);
}

/*
 * Write unmapped region permissions
 *
 * @param xpu_info	XPU info structure
 * @param umr_perm	Unmapped region permissions
 */
static inline void xpu4_write_unmapped_region_perms(ac_xpu4_priv_info *xpu_info,
						     ac_xpu4_qad_vector umr_perm)
{
	XPU4_OUT(xpu_info->addr, UMRPERMREG, umr_perm);
}

/*
 * Read configuration owner
 *
 * @param xpu_info	XPU info structure
 *
 * @return Configuration owner vector
 */
static inline ac_xpu4_qad_vector xpu4_read_cfg_owner(ac_xpu4_priv_info *xpu_info)
{
	return XPU4_IN(xpu_info->addr, CFGOWNER);
}

/*
 * Write configuration owner
 *
 * @param xpu_info		XPU info structure
 * @param cfg_owner_vector	Configuration owner vector
 */
static inline void xpu4_write_cfg_owner(ac_xpu4_priv_info *xpu_info,
					 ac_xpu4_qad_vector cfg_owner_vector)
{
	XPU4_OUT(xpu_info->addr, CFGOWNER, cfg_owner_vector);
}

/*
 * Write HW policy ignored register
 *
 * @param xpu_info	XPU info structure
 * @param value		Value to write
 * @param index		Register index
 */
static inline void xpu4_write_hw_policy_ignored(ac_xpu4_priv_info *xpu_info,
						 uint32_t value, uint16_t index)
{
	XPU4_OUTI(xpu_info->addr, HW_POLICY_IGNOREDr, index, value);
}

/*
 * Read HW policy ignored register
 *
 * @param xpu_info	XPU info structure
 * @param index		Register index
 *
 * @return Register value
 */
static inline uint32_t xpu4_read_hw_policy_ignored(ac_xpu4_priv_info *xpu_info,
						    uint16_t index)
{
	return XPU4_INI(xpu_info->addr, HW_POLICY_IGNOREDr, index);
}

/*
 * Read the APNSPE bit
 *
 * @param xpu_info	XPU info structure
 *
 * @return APNSPE bit value
 */
static inline bool xpu4_read_apnspe(ac_xpu4_priv_info *xpu_info)
{
	return (XPU4_INF(xpu_info->addr, GCR, APNSPE) == APNSPE_ENABLE);
}

/*
 * Read the xPU R/W permission from HW
 *
 * @param xpu_info		XPU info structure
 * @param rg_num		Region number
 * @param read_perm_vector	Pointer to store read permission vector
 * @param write_perm_vector	Pointer to store write permission vector
 */
static inline void xpu4_read_rg_perm(ac_xpu4_priv_info *xpu_info,
				      uint32_t rg_num,
				      ac_xpu4_qad_vector *read_perm_vector,
				      ac_xpu4_qad_vector *write_perm_vector)
{
	*read_perm_vector = XPU4_INI(xpu_info->addr, RGRDRn, rg_num);
	*write_perm_vector = XPU4_INI(xpu_info->addr, RGWRRn, rg_num);
}

/*
 * Read the write protection from HW
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Region number
 *
 * @return true if write protected, false otherwise
 */
static inline bool xpu4_is_rg_write_protected(ac_xpu4_priv_info *xpu_info,
					       uint32_t rg_num)
{
	return (XPU4_INFI(xpu_info->addr, RGCR0n, rg_num, RGWOWP) == ENABLE);
}

/*
 * Read the lock configuration from xPU HW
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Region number
 *
 * @return Lock configuration vector
 */
static inline ac_xpu4_qad_vector xpu4_read_rg_lock(ac_xpu4_priv_info *xpu_info,
						    uint32_t rg_num)
{
	return XPU4_INI(xpu_info->addr, QADRGLn, rg_num);
}

/*
 * Read the boot cfg status register from xPU HW
 *
 * @param xpu_info	XPU info structure
 *
 * @return Boot configuration status
 */
static inline uint32_t xpu4_read_boot_cfg_status(ac_xpu4_priv_info *xpu_info)
{
	return XPU4_IN(xpu_info->addr, BOOT_CFG_STS);
}

/*
 * Check the exec env lock is enabled in HW
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Region number
 * @param lock_vec	Lock vector
 *
 * @return true if lock enabled, false otherwise
 */
static inline bool xpu4_rg_lock_enabled_for_env(ac_xpu4_priv_info *xpu_info,
						 uint32_t rg_num,
						 ac_xpu4_qad_vector lock_vec)
{
	uint32_t hw_lock_value = xpu4_read_rg_lock(xpu_info, rg_num);

	return ((hw_lock_value & lock_vec) == lock_vec);
}

/*
 * Enable APNSPE in xPU_GCR space enforcing APP protection
 * After this xPU RGs/Unmapped space are only accessible by APP-S/APP-NS
 * APP-S access to APP-NS is controlled by TCSR APNS configuration
 *
 * @param xpu_info	XPU info structure
 */
static inline void xpu4_enable_apnspe(ac_xpu4_priv_info *xpu_info)
{
	XPU4_OUTF(xpu_info->addr, GCR, APNSPE, APNSPE_ENABLE);
}

/*
 * Disable APNSPE in xPU_GCR space
 * After this xPU RGs/Unmapped space are accessible by ALL QADs
 *
 * @param xpu_info	XPU info structure
 */
static inline void xpu4_disable_apnspe(ac_xpu4_priv_info *xpu_info)
{
	XPU4_OUTF(xpu_info->addr, GCR, APNSPE, APNSPE_DISABLE);
}

/*
 * Enable/Disable the xPU RG/partition
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Region number
 * @param enable	Enable flag
 */
static inline void xpu4_enable_rg(ac_xpu4_priv_info *xpu_info,
				   uint32_t rg_num,
				   bool enable)
{
	uint32_t value = enable ? 1 : 0;

	XPU4_OUTFI(xpu_info->addr, RGCR1n, rg_num, RGE, value);
}

/*
 * Set XPRESSCFG for earlier execution environment
 *
 * @param xpu_info	XPU info structure
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static inline AC_ERROR xpu4_xc_set_for_earlier_ee(ac_xpu4_priv_info *xpu_info)
{
	uint32_t mask, value = xpu4_read_boot_cfg_status(xpu_info);

	mask = XPU4_FMSK(BOOT_CFG_STS, TME_GRP0_WO);
	if ((value & mask) != mask) {
		XPU4_OUTF(xpu_info->addr, TME_GRP0_RG_PRG, TME0_RG_PRG,
			  XPRESSCFG_TRIGGER_BIT);
		value = xpu4_read_boot_cfg_status(xpu_info);
		if ((value & mask) != mask)
			return AC_ERR_XPU_XC_STATUS_NOT_SET;
	}

	mask = XPU4_FMSK(BOOT_CFG_STS, TME_GRP1_WO);
	if ((value & mask) != mask) {
		XPU4_OUTF(xpu_info->addr, TME_GRP1_RG_PRG, TME1_RG_PRG,
			  XPRESSCFG_TRIGGER_BIT);
		value = xpu4_read_boot_cfg_status(xpu_info);
		if ((value & mask) != mask)
			return AC_ERR_XPU_XC_STATUS_NOT_SET;
	}
	return AC_SUCCESS;
}

/*
 * Read the MPU partition start/end address
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Region number
 * @param start		Pointer to store start address
 * @param end		Pointer to store end address
 */
static void xpu4_fetch_mpu_partition_addr(ac_xpu4_priv_info *xpu_info,
					   uint32_t rg_num,
					   uint64_t *start,
					   uint64_t *end)
{
	uint32_t hw_start_low_value = 0, hw_end_low_value = 0;
	uint64_t hw_start_high_value = 0, hw_end_high_value = 0;

	/* Actual width is one more than ClientAddrWidth value */
	if (xpu4_get_msb_client_addr_width(xpu_info) > 31) {
		hw_start_high_value = XPU4_INI(xpu_info->addr, RGCSAR1n, rg_num);
		hw_start_high_value = hw_start_high_value << 32U;
		hw_end_high_value = XPU4_INI(xpu_info->addr, RGCEAR1n, rg_num);
		hw_end_high_value = hw_end_high_value << 32U;
	}

	hw_start_low_value = XPU4_INI(xpu_info->addr, RGCSAR0n, rg_num);
	hw_end_low_value = XPU4_INI(xpu_info->addr, RGCEAR0n, rg_num);

	*start = hw_start_high_value | hw_start_low_value;
	*end = hw_end_high_value | hw_end_low_value;
}

/*
 * Read the HW configuration and validate against the configuration
 * passed for programming
 *
 * @param xpu_info		XPU info structure
 * @param rg_num		Region number
 * @param start			Start address
 * @param end			End address
 * @param wowp			Write-once write-protect flag
 * @param read_perm_vector	Read permission vector
 * @param write_perm_vector	Write permission vector
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_read_and_check_rg_cfg(ac_xpu4_priv_info *xpu_info,
					    uint32_t rg_num,
					    uint64_t start,
					    uint64_t end,
					    bool *wowp,
					    ac_xpu4_qad_vector read_perm_vector,
					    ac_xpu4_qad_vector write_perm_vector)
{
	ac_xpu4_qad_vector hw_read_value;
	ac_xpu4_qad_vector hw_write_value;

	if (wowp && (xpu4_is_rg_write_protected(xpu_info, rg_num) != *wowp)) {
		VERBOSE("AC_XPU ErrCode: %d, DEBUG: xpu soc_addr: 0x%x, RG num: %d\n",
		      AC_ERR_XPU_RG_WOWP_MISMATCH, xpu_info->soc_addr, rg_num);
		return AC_ERR_XPU_RG_WOWP_MISMATCH;
	}

	if (!xpu4_is_rg_enabled(xpu_info, rg_num)) {
		VERBOSE("AC_XPU ErrCode: %d, DEBUG: xpu soc_addr: 0x%x, RG num: %d\n",
		      AC_ERR_XPU_RG_NOT_ENABLED, xpu_info->soc_addr, rg_num);
		return AC_ERR_XPU_RG_NOT_ENABLED;
	}

	xpu4_read_rg_perm(xpu_info, rg_num, &hw_read_value, &hw_write_value);
	if ((hw_read_value != read_perm_vector) ||
	    (hw_write_value != write_perm_vector)) {
		VERBOSE("AC_XPU ErrCode: %d, DEBUG: xpu soc_addr: 0x%x, RG num: %d",
		      AC_ERR_XPU_RG_PERM_MISMATCH, xpu_info->soc_addr, rg_num);
		VERBOSE(" read: 0x%x, hw_r: 0x%x, write: 0x%x, hw_w: 0x%x\n",
		      read_perm_vector, hw_read_value,
		      write_perm_vector, hw_write_value);
		return AC_ERR_XPU_RG_PERM_MISMATCH;
	}

	if (xpu4_decode_xpu_type(xpu_info) == AC_MPU) {
		uint64_t hw_start_value;
		uint64_t hw_end_value;

		xpu4_fetch_mpu_partition_addr(xpu_info, rg_num, &hw_start_value,
					      &hw_end_value);
		if (hw_start_value != start || hw_end_value != end) {
			VERBOSE("AC_XPU ErrCode: %d, DEBUG: xpu soc_addr: 0x%x, RG num: %d",
			      AC_ERR_XPU_RG_ADDR_MISMATCH, xpu_info->soc_addr, rg_num);
			VERBOSE(" start_h:0x%x, start_l:0x%x, hw_s_h:0x%x, hw_s_l:0x%x",
				(uint32_t)(start >> 32), (uint32_t)start,
				(uint32_t)(hw_start_value >> 32), (uint32_t)hw_start_value);
			VERBOSE(" end_hi: 0x%x, end_lo: 0x%x, hw_end_hi:0x%x, hw_end_lo: 0x%x\n",
				(uint32_t)(end >> 32), (uint32_t)end,
				(uint32_t)(hw_end_value >> 32), (uint32_t)hw_end_value);
			return AC_ERR_XPU_RG_ADDR_MISMATCH;
		}
	}

	return AC_SUCCESS;
}

/*
 * Find free RG in xPU HW
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Pointer to store region number
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_find_free_rg(ac_xpu4_priv_info *xpu_info, uint32_t *rg_num)
{
	uint32_t rg_start;
	uint32_t rg_end;
	uint32_t i;

	if (!xpu_info->dyn_rgs) {
		ERROR("AC_XPU ErrCode: %d, No dynamic RG info for XPU at 0x%x\n",
		      AC_ERR_NO_DYNAMIC_RG_INFO, xpu_info->soc_addr);
		return AC_ERR_NO_DYNAMIC_RG_INFO;
	}

	rg_start = xpu_info->dyn_rgs->rg_start;
	rg_end = xpu_info->dyn_rgs->rg_start + xpu_info->dyn_rgs->rg_count;

	for (i = rg_start; i < rg_end; i++) {
		if (xpu4_is_rg_enabled(xpu_info, i))
			continue;

		*rg_num = i;
		return AC_SUCCESS;
	}

	ERROR("AC_XPU ErrCode: %d, No free RG for XPU at 0x%x\n",
	      AC_ERR_XPU_NO_FREE_RG, xpu_info->soc_addr);
	return AC_ERR_XPU_NO_FREE_RG;
}

/*
 * Locate enabled RG based on address range and permissions
 *
 * @param xpu_info		XPU info structure
 * @param start			Start address
 * @param end			End address
 * @param read_perm_vector	Read permission vector
 * @param write_perm_vector	Write permission vector
 * @param rg_num_exact		First RG number whose address range matches exactly
 * @param rg_num_super		First RG number whose end address matches exactly
 * @param rg_num_front		First RG number whose end address matches start
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_locate_enabled_rg(ac_xpu4_priv_info *xpu_info,
					uint64_t start,
					uint64_t end,
					ac_xpu4_qad_vector *read_perm_vector,
					ac_xpu4_qad_vector *write_perm_vector,
					uint32_t *rg_num_exact,
					uint32_t *rg_num_super,
					uint32_t *rg_num_front)
{
	bool seek_exact = false;
	bool seek_super = false;
	bool seek_front = false;
	uint32_t rg_start;
	uint32_t rg_end;
	uint32_t i;

	if (!rg_num_exact && !rg_num_super && !rg_num_front) {
		/* Not looking for any... just return success */
		return AC_SUCCESS;
	}

	if (!xpu_info->dyn_rgs) {
		ERROR("AC_XPU ErrCode: %d, No dynamic RG info for XPU at 0x%x\n",
		      AC_ERR_NO_DYNAMIC_RG_INFO, xpu_info->soc_addr);
		return AC_ERR_NO_DYNAMIC_RG_INFO;
	}

	rg_start = xpu_info->dyn_rgs->rg_start;
	rg_end = xpu_info->dyn_rgs->rg_start + xpu_info->dyn_rgs->rg_count;

	if (rg_num_exact) {
		*rg_num_exact = UINT32_MAX;
		seek_exact = true;
	}

	if (rg_num_super) {
		*rg_num_super = UINT32_MAX;
		seek_super = true;
	}

	if (rg_num_front) {
		*rg_num_front = UINT32_MAX;
		seek_front = true;
	}

	for (i = rg_start; i < rg_end; i++) {
		bool *seek_ptr;
		uint32_t *rg_num_ptr;
		uint64_t hw_start_value;
		uint64_t hw_end_value;

		if (!seek_exact && !seek_super && !seek_front)
			break;
		if (!xpu4_is_rg_enabled(xpu_info, i))
			continue;

		xpu4_fetch_mpu_partition_addr(xpu_info, i, &hw_start_value,
					      &hw_end_value);

		seek_ptr = NULL;
		rg_num_ptr = NULL;

		do {
			if (seek_exact && (hw_start_value == start) &&
			    (hw_end_value == end)) {
				seek_ptr = &seek_exact;
				rg_num_ptr = rg_num_exact;
				break;
			}

			if (seek_super && (hw_start_value < start) &&
			    (hw_end_value == end)) {
				seek_ptr = &seek_super;
				rg_num_ptr = rg_num_super;
				break;
			}

			if (seek_front && (hw_end_value == start)) {
				seek_ptr = &seek_front;
				rg_num_ptr = rg_num_front;
				break;
			}
		} while (0);

		if (!seek_ptr || !rg_num_ptr)
			continue;

		if (read_perm_vector && write_perm_vector) {
			ac_xpu4_qad_vector hw_read_value;
			ac_xpu4_qad_vector hw_write_value;
			ac_xpu4_qad_vector shadow_read_value;
			ac_xpu4_qad_vector shadow_write_value;

			xpu4_read_rg_perm(xpu_info, i, &hw_read_value, &hw_write_value);
			ac_xpu_get_shadow_perm(xpu_info, i, &shadow_read_value,
					       &shadow_write_value);

			if (((hw_read_value | shadow_read_value) != *read_perm_vector) ||
			    ((hw_write_value | shadow_write_value) != *write_perm_vector))
				continue;
		}

		*rg_num_ptr = i;
		*seek_ptr = false;
	}

	return AC_SUCCESS;
}

/*
 * Configure error reporting on HW
 * Error reporting QAD has to match the execution environment
 *
 * @param xpu_info		XPU info structure
 * @param err_rep_vector	Error reporting vector
 * @param b_enable		Enable flag
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_configure_client_error_reporting(ac_xpu4_priv_info *xpu_info,
						       ac_xpu4_qad_vector err_rep_vector,
						       bool b_enable)
{
	uint32_t reg_value, readback_value;

	reg_value = XPU4_IN(xpu_info->addr, CLERE);
	if (b_enable) {
		reg_value |= err_rep_vector;
	} else {
		reg_value &= (~err_rep_vector);
	}
	XPU4_OUT(xpu_info->addr, CLERE, reg_value);
	readback_value = XPU4_IN(xpu_info->addr, CLERE);

	/* clearing only 1 APP QAD leaves LSB uncleared */
	if ((false == b_enable) && (err_rep_vector != ac_xpu4_get_env_qad_vector()))
		readback_value = readback_value & ~APP_QAD_RETAIN_BIT;

	if (reg_value != readback_value) {
		return AC_ERR_XPU_RG_WRITE_MISMATCH;
	} else {
		return AC_SUCCESS;
	}
}

/*
 * Program the xPU RG configurations in HW
 * Start/End programmed for MPU
 * Read/Write/WP programmed for all (MPU/RPU/APU)
 *
 * @param xpu_info		XPU info structure
 * @param rg_num		Region number
 * @param start			Start address
 * @param end			End address
 * @param wowp			Write-once write-protect flag
 * @param read_perm_vector	Read permission vector
 * @param write_perm_vector	Write permission vector
 */
static void xpu4_configure_rg_cfg(ac_xpu4_priv_info *xpu_info,
				   uint32_t rg_num,
				   uint64_t start,
				   uint64_t end,
				   bool wowp,
				   ac_xpu4_qad_vector read_perm_vector,
				   ac_xpu4_qad_vector write_perm_vector)
{
	/* Program addresses */
	if (xpu4_decode_xpu_type(xpu_info) == AC_MPU) {
		uint32_t temp;

		temp = (uint32_t)(start & BITMASK32);
		XPU4_OUTI(xpu_info->addr, RGCSAR0n, rg_num, temp);
		temp = (uint32_t)(end & BITMASK32);
		XPU4_OUTI(xpu_info->addr, RGCEAR0n, rg_num, temp);

		/* Actual width is one more than ClientAddrWidth value */
		if (xpu4_get_msb_client_addr_width(xpu_info) > 31) {
			temp = (uint32_t)(start >> 32);
			XPU4_OUTI(xpu_info->addr, RGCSAR1n, rg_num, temp);
			temp = (uint32_t)(end >> 32);
			XPU4_OUTI(xpu_info->addr, RGCEAR1n, rg_num, temp);
		}
	}

	/* Program permissions */
	XPU4_OUTI(xpu_info->addr, RGRDRn, rg_num, read_perm_vector);
	XPU4_OUTI(xpu_info->addr, RGWRRn, rg_num, write_perm_vector);

	/* Program enable */
	xpu4_enable_rg(xpu_info, rg_num, true);

	/* Program write-once write-protect */
	if (wowp)
		XPU4_OUTFI(xpu_info->addr, RGCR0n, rg_num, RGWOWP, 1);
}

/*
 * Program the lock QAD vector into HW
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Region number
 * @param lock_vec	Lock vector
 * @param lock		Lock flag
 */
static void xpu4_lock_rg_for_env(ac_xpu4_priv_info *xpu_info,
				  uint32_t rg_num,
				  ac_xpu4_qad_vector lock_vec,
				  bool lock)
{
	uint32_t new_value = lock ? lock_vec : (~lock_vec & ac_xpu4_get_env_qad_vector());
	uint32_t old_value;

	/* Locking or unlocking both APP-S and APP-NS */
	if ((new_value == 0) || (new_value == ac_xpu4_get_env_qad_vector())) {
		XPU4_OUTI(xpu_info->addr, QADRGLn, rg_num, new_value);
	} else {
		if (lock) {
			old_value = (XPU4_INI(xpu_info->addr, QADRGLn, rg_num) &
				     ac_xpu4_get_env_qad_vector());
			new_value = old_value | new_value;
		}
		XPU4_OUTI(xpu_info->addr, QADRGLn, rg_num, new_value);
	}
}

/*
 * Validate the lock QAD vector passed for HW layer
 *
 * @param lock_vector	Lock vector
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static inline AC_ERROR xpu4_lock_qad_valid_for_env(ac_xpu4_qad_vector lock_vector)
{
	ac_xpu4_qad_vector ee_lock_vec = ac_xpu4_get_env_qad_vector();

	if (lock_vector & (~ee_lock_vec))
		return AC_QAD_LOCK_NOT_SUPPORTED;

	return AC_SUCCESS;
}

/*
 * Lock for exec environment and validate the xPU RG for details passed
 * Also update the dynamic RG cache permission
 *
 * @param xpu_info		XPU info structure
 * @param rg_num		Region number
 * @param start			Start address
 * @param end			End address
 * @param wowp			Write-once write-protect flag
 * @param read_perm_vector	Read permission vector
 * @param write_perm_vector	Write permission vector
 * @param lock_vector		Lock vector
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_lock_and_validate_for_env(ac_xpu4_priv_info *xpu_info,
						uint32_t rg_num,
						uint64_t start,
						uint64_t end,
						bool wowp,
						ac_xpu4_qad_vector read_perm_vector,
						ac_xpu4_qad_vector write_perm_vector,
						ac_xpu4_qad_vector lock_vector)
{
	bool l_wowp = wowp;
	AC_ERROR rc;

	if (xpu4_lock_qad_valid_for_env(lock_vector) != AC_SUCCESS)
		return AC_QAD_LOCK_NOT_SUPPORTED;

	if (lock_vector) {
		xpu4_lock_rg_for_env(xpu_info, rg_num, lock_vector, true);
		if (!xpu4_rg_lock_enabled_for_env(xpu_info, rg_num, lock_vector)) {
			ERROR("AC_XPU ErrCode: %d, XPU RG write mismatch at 0x%x, RG %u",
			      AC_ERR_XPU_RG_WRITE_MISMATCH, xpu_info->soc_addr, rg_num);
			return AC_ERR_XPU_RG_WRITE_MISMATCH;
		}
	}

	ac_xpu_cache_shadow_perm(xpu_info, rg_num, read_perm_vector, write_perm_vector);

	rc = xpu4_read_and_check_rg_cfg(xpu_info, rg_num, start, end,
					&l_wowp, read_perm_vector, write_perm_vector);
	if (rc != AC_SUCCESS) {
		ERROR("AC_XPU ErrCode: %d, XPU RG write mismatch at 0x%x, RG %u",
		      AC_ERR_XPU_RG_WRITE_MISMATCH, xpu_info->soc_addr, rg_num);
		return AC_ERR_XPU_RG_WRITE_MISMATCH;
	}

	return AC_SUCCESS;
}

/*
 * Unlock the xPU RG for the execution environment
 * Also update the dynamic RG cache permission
 * NOTE: lock_vector must not be 0
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Region number
 * @param lock_vector	Lock vector
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_unlock_for_env(ac_xpu4_priv_info *xpu_info,
				     uint32_t rg_num,
				     ac_xpu4_qad_vector lock_vector)
{
	uint32_t read_perm_vector, write_perm_vector;

	if (xpu4_lock_qad_valid_for_env(lock_vector) != AC_SUCCESS)
		return AC_QAD_LOCK_NOT_SUPPORTED;

	xpu4_lock_rg_for_env(xpu_info, rg_num, lock_vector, false);
	if (xpu4_rg_lock_enabled_for_env(xpu_info, rg_num, lock_vector)) {
		ERROR("AC_XPU ErrCode: %d, XPU RG write mismatch at 0x%x, RG %u",
		      AC_ERR_XPU_RG_WRITE_MISMATCH, xpu_info->soc_addr, rg_num);
		return AC_ERR_XPU_RG_WRITE_MISMATCH;
	}
	ac_xpu_get_shadow_perm(xpu_info, rg_num, &read_perm_vector, &write_perm_vector);
	ac_xpu_cache_shadow_perm(xpu_info, rg_num, read_perm_vector & ~lock_vector,
				 write_perm_vector & ~lock_vector);
	return AC_SUCCESS;
}

/*
 * Verify RG with subset permission
 *
 * @param xpu_info		XPU info structure
 * @param rg_num		Region number
 * @param start			Start address
 * @param end			End address
 * @param wowp			Write-once write-protect flag
 * @param read_perm_vector	Read permission vector
 * @param write_perm_vector	Write permission vector
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_verify_rg_with_subset_permission(ac_xpu4_priv_info *xpu_info,
						       uint32_t rg_num,
						       uint64_t start,
						       uint64_t end,
						       bool *wowp,
						       ac_xpu4_qad_vector read_perm_vector,
						       ac_xpu4_qad_vector write_perm_vector)
{
	ac_xpu4_qad_vector hw_read_value;
	ac_xpu4_qad_vector hw_write_value;
	ac_xpu4_qad_vector env_qad_vec = ac_xpu4_get_env_qad_vector();

	if (wowp && (xpu4_is_rg_write_protected(xpu_info, rg_num) != *wowp)) {
		VERBOSE("AC_XPU ErrCode: %d, DEBUG: xpu soc_addr: 0x%x, RG num: %d",
		      AC_ERR_XPU_RG_WOWP_MISMATCH, xpu_info->soc_addr, rg_num);
		return AC_ERR_XPU_RG_WOWP_MISMATCH;
	}

	if (!xpu4_is_rg_enabled(xpu_info, rg_num)) {
		VERBOSE("AC_XPU ErrCode: %d, DEBUG: xpu soc_addr: 0x%x, RG num: %d",
		      AC_ERR_XPU_RG_NOT_ENABLED, xpu_info->soc_addr, rg_num);
		return AC_ERR_XPU_RG_NOT_ENABLED;
	}

	if (xpu4_decode_xpu_type(xpu_info) == AC_MPU) {
		uint64_t hw_start_value;
		uint64_t hw_end_value;

		xpu4_fetch_mpu_partition_addr(xpu_info, rg_num, &hw_start_value,
					      &hw_end_value);
		if (hw_start_value != start || hw_end_value != end) {
			VERBOSE("AC_XPU ErrCode: %d, DEBUG: xpu soc_addr: 0x%x, RG num: %d",
			      AC_ERR_XPU_RG_ADDR_MISMATCH, xpu_info->soc_addr, rg_num);
			VERBOSE("DEBUG: start_hi: 0x%x,start_lo: 0x%x, hw_s_hi:0x%x, hw_s_lo:0x%x",
				(uint32_t)(start >> 32), (uint32_t)start,
				(uint32_t)(hw_start_value >> 32), (uint32_t)hw_start_value);
			VERBOSE("DEBUG: end_hi: 0x%x,end_lo: 0x%x, hw_end_hi:0x%x, hw_end_lo:0x%x",
				(uint32_t)(end >> 32), (uint32_t)end,
			      (uint32_t)(hw_end_value >> 32), (uint32_t)hw_end_value);
			return AC_ERR_XPU_RG_ADDR_MISMATCH;
		}
	}

	xpu4_read_rg_perm(xpu_info, rg_num, &hw_read_value, &hw_write_value);
	if ((~hw_read_value & read_perm_vector) || (~hw_write_value & write_perm_vector)) {
		VERBOSE("AC_XPU ErrCode: %d, DEBUG: xpu soc_addr: 0x%x, RG num: %d",
		      AC_ERR_XPU_RG_PERM_MISMATCH, xpu_info->soc_addr, rg_num);
		VERBOSE("DEBUG: read_perm: 0x%x, hw_read: 0x%x, write_perm: 0x%x, hw_write: 0x%x",
			read_perm_vector, hw_read_value, write_perm_vector, hw_write_value);
		return AC_ERR_XPU_RG_PERM_MISMATCH;
	}

	if (((hw_read_value & ~env_qad_vec) != (read_perm_vector & ~env_qad_vec)) ||
	    ((hw_write_value & ~env_qad_vec) != (write_perm_vector & ~env_qad_vec)))
		return AC_ERR_ENV_QAD_MISMATCH;

	return AC_SUCCESS;
}

/*
 * It is determined whether it has to be programmed in xPU RG (or)
 * Locking has to be done based Start/End/Permission matches.
 * Lock QAD is validated against QADs on which SW is running.
 * 1. If start/end doesn't match with any RG then new free RG is
 *    programmed/locked with details provided
 * 2. If start/end match with permissions for already programmed RG then
 *    locking is done and config verified
 * 3. If start/end(0x2000-0x3000) overlap with RG-A(0x1000--0x3000) and
 *    there is adjacent RG-B(0x1000--0x2000)
 *    then locking done in RG-A and unlocking done in RG-B
 *
 * @param xpu_info		XPU info structure
 * @param start			Start address
 * @param end			End address
 * @param wowp			Write-once write-protect flag
 * @param read_perm_vector	Read permission vector
 * @param write_perm_vector	Write permission vector
 * @param lock_vector		Lock vector
 * @param allow_merging		Allow merging flag
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_protect_region(ac_xpu4_priv_info *xpu_info,
				     uint64_t start,
				     uint64_t end,
				     bool wowp,
				     ac_xpu4_qad_vector read_perm_vector,
				     ac_xpu4_qad_vector write_perm_vector,
				     ac_xpu4_qad_vector lock_vector,
				     bool allow_merging)
{
	uint32_t new_rg_num;
	uint32_t old_rg_num;
	uint32_t rg_count;
	bool pre_programmed;
	bool merging;
	AC_ERROR rc;

	/* Find RG with exact address range */
	/* Find also a mergeable RG with a contiguous end address */
	rc = xpu4_locate_enabled_rg(xpu_info, start, end,
				     &read_perm_vector, &write_perm_vector,
				     &new_rg_num, NULL, &old_rg_num);
	if (rc != AC_SUCCESS)
		return rc;

	/* Raw IDR value is highest index, add 1 to convert to count */
	rg_count = xpu4_get_rg_count(xpu_info) + 1;
	pre_programmed = new_rg_num < rg_count;
	merging = false;

	if (pre_programmed) {
		/* RG has already been programmed and enabled */
		/* So this request must lock */
		if (!lock_vector) {
			ERROR("AC_XPU ErrCode: %d, No lock requested for region 0x%x%08x-0x%x%08x",
			      AC_XPU_NO_LOCK_REQUESTED, (uint32_t)(start >> 32), (uint32_t)start,
			      (uint32_t)(end >> 32), (uint32_t)end);
			return AC_XPU_NO_LOCK_REQUESTED;
		}
	} else {
		/* Merge the consecutive address ranges */
		if (allow_merging &&
		    old_rg_num < rg_count && !xpu4_is_rg_write_protected(xpu_info, old_rg_num)) {
			uint64_t old_rg_start;
			uint64_t old_rg_end;

			xpu4_fetch_mpu_partition_addr(xpu_info, old_rg_num, &old_rg_start,
						      &old_rg_end);
			if (old_rg_start < old_rg_end && old_rg_end == start) {
				start = old_rg_start;
				merging = true;
			}
		}

		rc = xpu4_find_free_rg(xpu_info, &new_rg_num);
		if (rc != AC_SUCCESS)
			return rc;

		/* Program new RG and Enable it. Based on merging address is updated */
		xpu4_configure_rg_cfg(xpu_info, new_rg_num, start, end,
				       wowp, read_perm_vector, write_perm_vector);
		ac_xpu_cache_shadow_perm(xpu_info, new_rg_num, read_perm_vector,
					 write_perm_vector);
	}

	rc = xpu4_lock_and_validate_for_env(xpu_info, new_rg_num, start, end, wowp,
					     read_perm_vector, write_perm_vector, lock_vector);
	if (rc != AC_SUCCESS)
		return rc;

	if (merging) {
		/* If old RG has lock set, unlock it. Lock not set then disable the RG */
		if (lock_vector &&
		    xpu4_rg_lock_enabled_for_env(xpu_info, old_rg_num, lock_vector)) {
			xpu4_lock_rg_for_env(xpu_info, old_rg_num, lock_vector, false);
			if (xpu4_rg_lock_enabled_for_env(xpu_info, old_rg_num, lock_vector)) {
				ERROR("AC_XPU ErrCode: %d, XPU RG write mismatch at 0x%x, RG %u",
					AC_ERR_XPU_RG_WRITE_MISMATCH, xpu_info->soc_addr,
					old_rg_num);
				rc = AC_ERR_XPU_RG_WRITE_MISMATCH;
				return rc;
			}
		} else if (!xpu4_read_rg_lock(xpu_info, old_rg_num)) {
			xpu4_enable_rg(xpu_info, old_rg_num, false);
			if (xpu4_is_rg_enabled(xpu_info, old_rg_num)) {
				ERROR("AC_XPU ErrCode: %d, XPU RG write mismatch at 0x%x, RG %u",
					AC_ERR_XPU_RG_WRITE_MISMATCH, xpu_info->soc_addr,
					old_rg_num);
				rc = AC_ERR_XPU_RG_WRITE_MISMATCH;
				return rc;
			}
		}

		ac_xpu_cache_shadow_perm(xpu_info, old_rg_num, 0, 0);
	}

	return AC_SUCCESS;
}

/*
 * It is determined whether it has to be unlocked or it has to be disabled or both.
 * Lock QAD is validated against QADs on which SW is running.
 * If RG start/end matches exactly (or) falls completely within the address range passed
 * then based on lock details passed unlock is done and RG is tried for disablement
 * (might not if some QAD holds the lock). If no QAD lock is passed then RG tried for
 * disablement.
 *
 * @param xpu_info	XPU info structure
 * @param start		Start address
 * @param end		End address
 * @param lock_vector	Lock vector
 * @param blanket	Blanket flag
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_unprotect_region(ac_xpu4_priv_info *xpu_info,
				       uint64_t start,
				       uint64_t end,
				       ac_xpu4_qad_vector lock_vector,
				       bool blanket)
{
	AC_ERROR rc;
	uint32_t rg_start, rg_end, read_perm_vector, write_perm_vector;
	uint32_t i;
	region_overlap_t overlap_ret = REGION_NO_OVERLAP;
	bool is_mpu = (xpu4_decode_xpu_type(xpu_info) == AC_MPU);

	if ((!xpu_info->dyn_rgs) || (!is_mpu)) {
		ERROR("AC_XPU ErrCode: %d, No dynamic RG info for XPU at 0x%x",
		      AC_ERR_NO_DYNAMIC_RG_INFO, xpu_info->soc_addr);
		return AC_ERR_NO_DYNAMIC_RG_INFO;
	}

	rg_start = xpu_info->dyn_rgs->rg_start;
	rg_end = xpu_info->dyn_rgs->rg_start + xpu_info->dyn_rgs->rg_count;

	if (blanket) {
		rg_start = 0;
		/* Raw IDR value is highest index, add 1 to convert to count */
		rg_end = xpu4_get_rg_count(xpu_info) + 1;
	}

	for (i = rg_start; i < rg_end; i++) {
		uint64_t hw_start_value;
		uint64_t hw_end_value;

		if (!xpu4_is_rg_enabled(xpu_info, i))
			continue;

		xpu4_fetch_mpu_partition_addr(xpu_info, i, &hw_start_value, &hw_end_value);

		rc = region_overlap(hw_start_value, hw_end_value, start, end, &overlap_ret);
		if (rc != AC_SUCCESS)
			return AC_XPU_OVERLAP_CHECK_FAIL;
		if (overlap_ret == REGION_NO_OVERLAP)
			continue;

		/*
		 * TODO : Have to consider split and unlock particular range;
		 * currently not supported
		 */
		/*
		 * Unlock is done only for this QAD if other QAD still holds
		 * lock success will be returned
		 */
		if ((blanket == false && (overlap_ret == REGION_EQUAL)) ||
		    (blanket == true && ((overlap_ret == REGION_EQUAL) ||
					 (overlap_ret == REGION_OVERLAP_INNER) ||
					 (overlap_ret == REGION_OVERLAP_INNER_ADJACENT)))) {
			if (lock_vector) {
				rc = xpu4_unlock_for_env(xpu_info, i, lock_vector);
				if (rc != AC_SUCCESS)
					return rc;
				ac_xpu_get_shadow_perm(xpu_info, i, &read_perm_vector,
						       &write_perm_vector);
				if ((read_perm_vector == 0) && (write_perm_vector == 0)) {
					xpu4_enable_rg(xpu_info, i, false);
					if (xpu4_is_rg_enabled(xpu_info, i))
						return AC_XPU_RG_STILL_ENABLED;
				}
			} else {
				xpu4_enable_rg(xpu_info, i, false);
				if (xpu4_is_rg_enabled(xpu_info, i))
					return AC_XPU_RG_STILL_ENABLED;
				ac_xpu_cache_shadow_perm(xpu_info, i, 0, 0);
			}
		}
	}

	return AC_SUCCESS;
}

/*
 * Parse the xPU configuration data into data structure
 * used by HW HAL layer
 *
 * @param milestone	Initialization milestone
 * @param xpu_cfg	XPU configuration
 * @param index		Index
 * @param is_mpu	MPU flag
 * @param rg_cfg	RG configuration
 */
static void xpu4_get_rg_static_rg_config(ac_milestone milestone,
					  ac_xpu_cfg *xpu_cfg,
					  uint32_t index,
					  bool is_mpu,
					  ac_xpu_rg_static_config_t *rg_cfg)
{
	if (is_mpu) {
		const ac_mpu_rg *rg = &xpu_cfg->rg.mpu[index];

		rg_cfg->start = rg->start;
		rg_cfg->end = rg->end;
		rg_cfg->rg_num = rg->rg_num;
		rg_cfg->rg_profile_flags = rg->profile_flags;
		rg_cfg->read_qads = rg->read_qads;
		rg_cfg->write_qads = rg->write_qads;
	} else {
		const ac_rpu_rg *rg = &xpu_cfg->rg.rpu[index];

		rg_cfg->start = 0;
		rg_cfg->end = 0;
		rg_cfg->rg_num = rg->rg_num;
		rg_cfg->rg_profile_flags = rg->profile_flags;
		rg_cfg->read_qads = rg->read_qads;
		rg_cfg->write_qads = rg->write_qads;
	}
}

/*
 * Read nQAD from IDR2 and create valid QAD mask based on that value
 *
 * @param xpu_info	XPU info structure
 * @param valid_qads	Pointer to store valid QADs
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR xpu4_get_supported_qads(ac_xpu4_priv_info *xpu_info,
					 ac_xpu4_qad_vector *valid_qads)
{
	/* Add APP NS/S by default */
	uint32_t xpu_mask = 0;
	uint32_t num_qad = xpu4_read_idr2_qad(xpu_info);
	uint32_t i;

	if (num_qad < 1 || num_qad > g_num_valid_qads)
		return AC_ERR_NUM_QAD_INVALID;

	/* RA xPU's can support all bits */
	if (ac_is_ra_xpu(xpu_info->xpu_id)) {
		*valid_qads = 0xFFFFFFFF;
		return AC_SUCCESS;
	}

	for (i = 0; i < num_qad; i++)
		xpu_mask |= g_valid_qads[i];

	if (xpu_mask != VALID_QAD_MASK) {
		ERROR("AC_XPU ErrCode: %d, Valid QAD mismatch for 0x%x: got 0x%x, expected 0x%x",
			AC_ERR_VALID_QAD_MISMATCH, xpu_info->soc_addr, xpu_mask, VALID_QAD_MASK);
		return AC_ERR_VALID_QAD_MISMATCH;
	}

	*valid_qads = xpu_mask;
	return AC_SUCCESS;
}

#ifdef xPU_4_2_SUPPORT
/*
 * The caller must ensure XPRESSCFG is supported before calling
 * - Check XPRESSCFG EN bit
 *
 * @param xpu_info	XPU info structure
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR ACSet0RgPrg(ac_xpu4_priv_info *xpu_info)
{
	AC_ERROR rc = AC_FAILURE;
	uint32_t hw_val;

	xpu4_write_apps0_rg_prg(xpu_info, XPRESSCFG_TRIGGER_BIT);
	hw_val = xpu4_read_apps0_rg_prg(xpu_info);
	if (hw_val != XPRESSCFG_TRIGGER_BIT) {
		ERROR("AC_XPU ErrCode: %d, XC0 trigger mismatch for 0x%x: expected 0x%x, got 0x%x",
			AC_ERR_XPU_XC0_TRIGGER_MISMATCH, xpu_info->soc_addr, XPRESSCFG_TRIGGER_BIT,
			hw_val);
		return AC_ERR_XPU_XC0_TRIGGER_MISMATCH;
	}
	rc = AC_SUCCESS;
	return rc;
}

/*
 * Set unmapped region permissions
 *
 * @param xpu_info	XPU info structure
 * @param xpu_cfg	XPU configuration
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_set_unmapped_region_perms(ac_xpu4_priv_info *xpu_info,
					   const ac_xpu_cfg *xpu_cfg)
{
	ac_xpu4_qad_vector hw_umr_perm;
	AC_ERROR rc = AC_FAILURE;
	ac_xpu4_qad_vector umr_perm = xpu_cfg->umr_perm;

	if (xpu_info->rev < XPU4_REV(4, 2, 0) || AC_MPU != xpu4_decode_xpu_type(xpu_info))
		return AC_SUCCESS;

	xpu4_write_unmapped_region_perms(xpu_info, umr_perm);
	hw_umr_perm = xpu4_read_unmapped_region_perms(xpu_info);
	if (umr_perm != hw_umr_perm) {
		ERROR("AC_XPU ErrCode: %d, XPU UMR perm mismatch for 0x%x: expected 0x%x,got 0x%x",
		      AC_ERR_XPU_UMRPERM_MISMATCH, xpu_info->soc_addr, umr_perm, hw_umr_perm);
		return AC_ERR_XPU_UMRPERM_MISMATCH;
	}
	rc = AC_SUCCESS;

	return rc;
}

/*
 * Set configuration owner
 *
 * @param xpu_info	XPU info structure
 * @param xpu_cfg	XPU configuration
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_set_cfg_owner(ac_xpu4_priv_info *xpu_info,
			       const ac_xpu_cfg *xpu_cfg)
{
	ac_xpu4_qad_vector hw_cfg_owner;
	AC_ERROR rc = AC_FAILURE;
	ac_xpu4_qad_vector cfg_owner = xpu_cfg->cfg_owner;

	if (xpu_info->rev < XPU4_REV(4, 2, 0))
		return AC_SUCCESS;

	xpu4_write_cfg_owner(xpu_info, cfg_owner);
	hw_cfg_owner = xpu4_read_cfg_owner(xpu_info);
	if (cfg_owner != hw_cfg_owner) {
		ERROR("AC_XPU ErrCode: %d, XPU CFGOWNER mismatch for 0x%x: expected 0x%x,got 0x%x",
		      AC_ERR_XPU_CFGOWNER_MISMATCH, xpu_info->soc_addr, cfg_owner, hw_cfg_owner);
		return AC_ERR_XPU_CFGOWNER_MISMATCH;
	}

	rc = AC_SUCCESS;
	return rc;
}

/*
 * Update configuration owner
 *
 * @param xpu_info	XPU info structure
 * @param cfg_owner	Configuration owner
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_update_cfg_owner(ac_xpu4_priv_info *xpu_info,
				  const ac_xpu4_qad_vector cfg_owner)
{
	ac_xpu4_qad_vector hw_cfg_owner;
	AC_ERROR rc = AC_FAILURE;

	if (xpu_info->rev < XPU4_REV(4, 2, 0))
		return AC_SUCCESS;

	xpu4_write_cfg_owner(xpu_info, cfg_owner);
	hw_cfg_owner = xpu4_read_cfg_owner(xpu_info);
	if (cfg_owner != hw_cfg_owner) {
		ERROR("AC_XPU ErrCode: %d, XPU CFGOWNER mismatch for 0x%x: expected 0x%x,got 0x%x",
		      AC_ERR_XPU_CFGOWNER_MISMATCH, xpu_info->soc_addr, cfg_owner, hw_cfg_owner);
		return AC_ERR_XPU_CFGOWNER_MISMATCH;
	}

	rc = AC_SUCCESS;
	return rc;
}

/*
 * API to Read CfgOwner with given Xpuinfo
 *
 * @param xpu_info	XPU info structure
 *
 * @return QAD vector
 */
ac_xpu4_qad_vector ac_xpu_read_cfg_owner(ac_xpu4_priv_info *xpu_info)
{
	ac_xpu4_qad_vector hw_cfg_owner = 0;

	hw_cfg_owner = xpu4_read_cfg_owner(xpu_info);
	return hw_cfg_owner;
}

/*
 * Apply XPRESS configuration
 *
 * @param xpu_info	XPU info structure
 * @param xpu_cfg	XPU configuration
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_apply_xpress_config(ac_xpu4_priv_info *xpu_info,
				     const ac_xpu_cfg *xpu_cfg)
{
	AC_ERROR rc = AC_FAILURE;
	uint32_t nrg;
	uint32_t hw_val;
	uint16_t i;

	if (is_ac_disabled())
		return AC_SUCCESS;

	const ac_rg_ignored *rg_ignored = xpu_cfg->rg_ignored;

	if (xpu_info->rev < XPU4_REV(4, 2, 0) || !xpu4_decode_idr_xpress_cfg_en(xpu_info)) {
		/* If this XPU doesn't support XPRESSCFG, nothing to do */
		return AC_SUCCESS;
	}

	if (rg_ignored) {
		nrg = xpu4_get_rg_count(xpu_info) + 1;
		for (i = 0; i < NRG_TO_IGNORED_REG_COUNT(nrg); i++) {
			/* Write out each of the RG ignore values */
			xpu4_write_hw_policy_ignored(xpu_info, rg_ignored[i], i);
			hw_val = xpu4_read_hw_policy_ignored(xpu_info, i);
			if (rg_ignored[i] != hw_val) {
				ERROR("AC_XPU ErrCode: %d, XC for 0x%x: expected 0x%x, got 0x%x",
					AC_ERR_XPU_XC_IGNORED_MISMATCH, xpu_info->soc_addr,
					rg_ignored[i], hw_val);
				return AC_ERR_XPU_XC_IGNORED_MISMATCH;
			}
		}
	}

	/*
	 * Write the APPS_GRP0 (APPS_S) trigger bit
	 * AC SW does not support the XPRESSCFG application to be done for the
	 * first time by apps. The sequence must go TME_FW, xBL_SC, TZ hence triggering
	 * Just incase it was not set previously
	 */
	rc = xpu4_xc_set_for_earlier_ee(xpu_info);
	if (rc != AC_SUCCESS)
		return rc;

	rc = ACSet0RgPrg(xpu_info);
	if (rc != AC_SUCCESS)
		return rc;

	return rc;
}
#endif

/*
 * Fetch the IDR information from xPU and cache in SW
 *
 * @param xpu_info	XPU info structure
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_init_xpu(ac_xpu4_priv_info *xpu_info)
{
	if (xpu_info != NULL) {
		/* Cache REV and IDR info */
		xpu_info->rev = XPU4_IN(xpu_info->addr, REV);
		xpu_info->idr[0] = XPU4_IN(xpu_info->addr, IDR0);
		xpu_info->idr[1] = XPU4_IN(xpu_info->addr, IDR1);
		xpu_info->idr[2] = XPU4_IN(xpu_info->addr, IDR2);
		return AC_SUCCESS;
	} else {
		return AC_ERR_INPUT_VALIDATION;
	}
}

/*
 * Parse the xPU configuration data into data structure
 * used by HW HAL layer
 *
 * @param milestone	Initialization milestone
 * @param xpu_cfg	XPU configuration
 * @param index		Index
 * @param is_mpu	MPU flag
 * @param rg_cfg	RG configuration
 */
void ac_xpu4_get_rg_static_rg_config(ac_milestone milestone,
				      ac_xpu_cfg *xpu_cfg,
				      uint32_t index,
				      bool is_mpu,
				      ac_xpu_rg_static_config_t *rg_cfg)
{
	xpu4_get_rg_static_rg_config(milestone, xpu_cfg, index, is_mpu, rg_cfg);
}

/*
 * Configure APNSPE
 *
 * @param xpu_info	XPU info structure
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_configure_apnspe(ac_xpu4_priv_info *xpu_info)
{
	if (is_ac_disabled())
		return AC_SUCCESS;

	/* if APNSPE is already enabled skip programming it again in this milestone */
	if (!xpu4_read_apnspe(xpu_info)) {
		xpu4_enable_apnspe(xpu_info);
		if (!xpu4_read_apnspe(xpu_info)) {
			ERROR("AC_XPU ErrCode: %d, Failed to configure APNSPE for XPU at 0x%x",
			      AC_ERR_XPU_CONFIG_APNSPE, xpu_info->soc_addr);
			return AC_ERR_XPU_CONFIG_APNSPE;
		}
	}
	return AC_SUCCESS;
}

/*
 * API to clear the APNSPE bit of the XPU
 *
 * @param xpu_info	XPU info structure
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_clear_apnspe(ac_xpu4_priv_info *xpu_info)
{
	if (is_ac_disabled())
		return AC_SUCCESS;

	/* if APNSPE is already enabled then clear the bit */
	if (xpu4_read_apnspe(xpu_info)) {
		xpu4_disable_apnspe(xpu_info);
		if (xpu4_read_apnspe(xpu_info)) {
			ERROR("AC_XPU ErrCode: %d, Failed to clear APNSPE for XPU at 0x%x",
			      AC_ERR_XPU_CLEAR_APNSPE, xpu_info->soc_addr);
			return AC_ERR_XPU_CLEAR_APNSPE;
		}
	}
	return AC_SUCCESS;
}

/*
 * Enable/Disable error reporting for the QADs passed
 * Validated against QADs on which SW is running
 *
 * @param xpu_info	XPU info structure
 * @param sd_array	Security domain array
 * @param count		Number of domains
 * @param b_enable	Enable flag
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_configure_client_error_reporting(ac_xpu4_priv_info *xpu_info,
						  ac_sec_domain_id *sd_array,
						  uint32_t count,
						  bool b_enable)
{
	ac_xpu4_qad_vector valid_qad_mask = 0;

	if (is_ac_disabled())
		return AC_SUCCESS;
	if ((xpu_info == NULL) || (sd_array == NULL) || (count == 0))
		return AC_ERR_INPUT_VALIDATION;

	ac_xpu4_qad_vector err_rep_ctrl_vec = ac_xpu_encode_qad_vector(sd_array, count);

	/* Fail error reporting other the respective QAD */
	if (xpu4_get_supported_qads(xpu_info, &valid_qad_mask) != AC_SUCCESS ||
	    (valid_qad_mask & err_rep_ctrl_vec) != err_rep_ctrl_vec ||
	    (err_rep_ctrl_vec & (~ac_xpu4_get_env_qad_vector()))) {
		ERROR("AC_XPU ErrCode: %d, Input validation failed for XPU at 0x%x, enable=%u",
		      AC_ERR_INPUT_VALIDATION, xpu_info->soc_addr, (uint32_t)b_enable);
		return AC_ERR_INPUT_VALIDATION;
	}

	if (xpu4_configure_client_error_reporting(xpu_info, err_rep_ctrl_vec,
						   b_enable) != AC_SUCCESS) {
		ERROR("AC_XPU ErrCode: %d, XPU error reporting failed for XPU at 0x%x, enable=%u",
		      AC_ERR_XPU_ERROR_REPORTING, xpu_info->soc_addr, (uint32_t)b_enable);
		return AC_ERR_XPU_ERROR_REPORTING;
	}
	return AC_SUCCESS;
}

/*
 * Process a single RG configuration from the static configuration array
 *
 * @param milestone		Initialization milestone
 * @param xpu_info		XPU info structure
 * @param xpu_cfg		XPU configuration
 * @param index			Index of the RG configuration to process
 * @param is_mpu		MPU flag
 * @param valid_qad_mask	Valid QAD mask
 * @param env_qad_vec		Environment QAD vector
 * @param rg_num		Pointer to current RG number (updated by function)
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR ac_xpu_process_single_rg_config(ac_milestone milestone,
						 ac_xpu4_priv_info *xpu_info,
						 ac_xpu_cfg *xpu_cfg,
						 int32_t index,
						 bool is_mpu,
						 ac_xpu4_qad_vector valid_qad_mask,
						 ac_xpu4_qad_vector env_qad_vec,
						 uint32_t *rg_num)
{
	ac_xpu4_qad_vector lock_qad_vec, unlock_vec;
	ac_xpu4_qad_vector hw_read_value, hw_write_value;
	bool wowp = false;
	ac_xpu_rg_static_config_t rg_cfg;
	AC_ERROR rc = AC_FAILURE;

	xpu4_get_rg_static_rg_config(milestone, xpu_cfg, index, is_mpu, &rg_cfg);

	if (true != ac_is_rg_profile_matched(milestone, rg_cfg.rg_profile_flags))
		return AC_SUCCESS;

	if (rg_cfg.rg_num == *rg_num) {
		/* For the same RG, latest configuration takes precedence */
		/* So skip earlier ones */
		return AC_SUCCESS;
	}
	*rg_num = rg_cfg.rg_num;

	if (is_mpu) {
		rg_cfg.start = ac_xpu_addr_soc2_peripheral_xpu(xpu_info, rg_cfg.start);
		rg_cfg.end = ac_xpu_addr_soc2_peripheral_xpu(xpu_info, rg_cfg.end);
	}

	if ((valid_qad_mask & rg_cfg.read_qads) != rg_cfg.read_qads) {
		ERROR("AC_XPU ErrCode: %d, Read vector invalid: valid=0x%x, requested=0x%x",
		      AC_ERR_READ_VECTOR_INVALID, valid_qad_mask, rg_cfg.read_qads);
		return AC_ERR_READ_VECTOR_INVALID;
	}
	if ((valid_qad_mask & rg_cfg.write_qads) != rg_cfg.write_qads) {
		ERROR("AC_XPU ErrCode: %d, Write vector invalid: valid=0x%x, requested=0x%x",
		      AC_ERR_WRITE_VECTOR_INVALID, valid_qad_mask, rg_cfg.write_qads);
		return AC_ERR_WRITE_VECTOR_INVALID;
	}

	lock_qad_vec = xpu4_read_rg_lock(xpu_info, rg_cfg.rg_num);
	/* Already this static RG is locked by this QAD */
	if (lock_qad_vec & env_qad_vec) {
		/*
		 * Read R/W permissions from HW and make sure lock is done
		 * as per R/W permissions. This to capture a case where
		 * APP-NS only locks for RG which has APP-S R/W permission
		 */
		xpu4_read_rg_perm(xpu_info, *rg_num, &hw_read_value, &hw_write_value);
		if (((hw_read_value | hw_write_value) & env_qad_vec) !=
		    (lock_qad_vec & env_qad_vec))
			return AC_ERR_DOMAIN_LOCK_INVALID;

		/*
		 * If permission matches exactly do nothing. If permission
		 * doesn't match check whether permission is subset without
		 * current EE QAD if so unlock the EE QAD
		 */
		rc = xpu4_read_and_check_rg_cfg(xpu_info, rg_cfg.rg_num,
						 rg_cfg.start, rg_cfg.end,
						 &wowp, rg_cfg.read_qads,
						 rg_cfg.write_qads);
		if (rc != AC_SUCCESS) {
			rc = xpu4_verify_rg_with_subset_permission(xpu_info,
								    rg_cfg.rg_num,
								    rg_cfg.start,
								    rg_cfg.end,
								    &wowp,
								    rg_cfg.read_qads,
								    rg_cfg.write_qads);
			if (rc == AC_SUCCESS) {
				unlock_vec = env_qad_vec &
					(~((rg_cfg.read_qads & env_qad_vec) |
					   (rg_cfg.write_qads & env_qad_vec)));
				unlock_vec = (unlock_vec != 0) ?
					unlock_vec | APP_QAD_RETAIN_BIT : unlock_vec;
				rc = xpu4_unlock_for_env(xpu_info, rg_cfg.rg_num,
							  unlock_vec);
				if (rc == AC_SUCCESS) {
					/*
					 * If only APP-S is holding lock then RG is
					 * disabled, reprogram with appropriate permission.
					 * During reprogramming RG client access is denied
					 * on waipio but in future targets it will be open
					 * to ALL access. In this case RG configuration has
					 * to be moved to TME_FW, so that APPs lock removal
					 * won't disable RG. AC JIRA filed -
					 * https://jira-scrum.qualcomm.com/jira/browse/SECACCESS-2505
					 */
					if (!xpu4_is_rg_enabled(xpu_info,
						rg_cfg.rg_num)) {
						xpu4_configure_rg_cfg(xpu_info,
							rg_cfg.rg_num,
							rg_cfg.start,
							rg_cfg.end,
							wowp,
							rg_cfg.read_qads,
							rg_cfg.write_qads);
					}
					rc = xpu4_read_and_check_rg_cfg(xpu_info,
									rg_cfg.rg_num,
									rg_cfg.start,
									rg_cfg.end,
									&wowp,
									rg_cfg.read_qads,
									rg_cfg.write_qads);
				}
			}
		}
	} else {
		/*
		 * If R/W has TME_FW then RG will be configured by TME_FW,
		 * xBL_SC/TZ has to verify this configuration
		 */
		if (!ac_xpu_rg_configured_by_earlier_rot(rg_cfg.read_qads,
							  rg_cfg.write_qads)) {
			xpu4_configure_rg_cfg(xpu_info, rg_cfg.rg_num,
					       rg_cfg.start, rg_cfg.end,
					       wowp, rg_cfg.read_qads,
					       rg_cfg.write_qads);
		} else {
			/*
			 * ALL_ROT case where TME_FW has R/W but policy is
			 * not available in it
			 */
			rc = xpu4_read_and_check_rg_cfg(xpu_info, rg_cfg.rg_num,
							 rg_cfg.start, rg_cfg.end,
							 &wowp, rg_cfg.read_qads,
							 rg_cfg.write_qads);
			if (rc != AC_SUCCESS) {
				xpu4_configure_rg_cfg(xpu_info, rg_cfg.rg_num,
						       rg_cfg.start, rg_cfg.end,
						       wowp, rg_cfg.read_qads,
						       rg_cfg.write_qads);
			}
		}

		if ((rg_cfg.read_qads & env_qad_vec) ||
		    (rg_cfg.write_qads & env_qad_vec)) {
			lock_qad_vec = (rg_cfg.read_qads & env_qad_vec) |
				       (rg_cfg.write_qads & env_qad_vec);
			rc = xpu4_lock_and_validate_for_env(xpu_info,
							     rg_cfg.rg_num,
							     rg_cfg.start,
							     rg_cfg.end,
							     wowp,
							     rg_cfg.read_qads,
							     rg_cfg.write_qads,
							     lock_qad_vec);
		} else {
			rc = xpu4_read_and_check_rg_cfg(xpu_info,
							rg_cfg.rg_num,
							rg_cfg.start,
							rg_cfg.end,
							&wowp,
							rg_cfg.read_qads,
							rg_cfg.write_qads);
		}
	}
	if (rc != AC_SUCCESS) {
		ERROR("AC_XPU ErrCode: %d, XPU RG write mismatch for XPU at 0x%x, RG %u",
		      AC_ERR_XPU_RG_WRITE_MISMATCH, xpu_info->soc_addr, rg_cfg.rg_num);
		return rc;
	}

	return AC_SUCCESS;
}

/*
 * AC xPU static configuration applied at different Milestones
 * Program or Lock RGs mentioned in xPU static configuration array
 *
 * @param milestone	Initialization milestone
 * @param xpu_info	XPU info structure
 * @param xpu_cfg	XPU configuration
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_apply_static_config(ac_milestone milestone,
				     ac_xpu4_priv_info *xpu_info,
				     ac_xpu_cfg *xpu_cfg)
{
	ac_xpu4_qad_vector valid_qad_mask;
	ac_xpu4_qad_vector env_qad_vec = ac_xpu4_get_env_qad_vector();
	bool is_mpu = false;
	uint32_t rg_num = 0xFFFFUL;
	int32_t i, temp;
	AC_ERROR rc = AC_FAILURE;

	if (is_ac_disabled())
		return AC_SUCCESS;
	if ((xpu_info == NULL) || (xpu_cfg == NULL))
		return AC_ERR_INPUT_VALIDATION;

	rc = xpu4_get_supported_qads(xpu_info, &valid_qad_mask);
	if (rc != AC_SUCCESS)
		return rc;

	is_mpu = (xpu4_decode_xpu_type(xpu_info) == AC_MPU);

	temp = xpu_cfg->nrg - 1;
	if ((temp > xpu_cfg->nrg) || (xpu_cfg->nrg > rg_num))
		return AC_ERR_INPUT_VALIDATION;

	for (i = temp; i >= 0; i--) {
		rc = ac_xpu_process_single_rg_config(milestone, xpu_info, xpu_cfg, i,
						     is_mpu, valid_qad_mask,
						     env_qad_vec, &rg_num);
		if (rc != AC_SUCCESS)
			return rc;
	}

	return AC_SUCCESS;
}

/*
 * API to protect/lock the memory region with permission passed
 *
 * @param xpu_info		XPU info structure
 * @param start			Start address
 * @param size			Region size
 * @param wowp			Write-once write-protect flag
 * @param sd_perm_array		Security domain permission array
 * @param count			Number of permissions
 * @param merge			Merge flag
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_protect_region(ac_xpu4_priv_info *xpu_info,
				uint64_t start,
				uint64_t size,
				bool wowp,
				ac_sec_domain_perm *sd_perm_array,
				uint32_t count,
				bool merge)
{
	uint64_t s;
	uint64_t e;
	ac_xpu4_qad_vector read_vec;
	ac_xpu4_qad_vector write_vec;
	ac_xpu4_qad_vector lock_vec;
	ac_xpu4_qad_vector valid_qad_mask;
	AC_ERROR rc = AC_FAILURE;

	if (is_ac_disabled())
		return AC_SUCCESS;

	if ((xpu_info == NULL) || (sd_perm_array == NULL) || (start > UINT64_MAX - size) ||
	    (count == 0) || (count > AC_SD_END_MARKER))
		return AC_ERR_INPUT_VALIDATION;

	s = ac_xpu_addr_soc2_peripheral_xpu(xpu_info, start);
	e = ac_xpu_addr_soc2_peripheral_xpu(xpu_info, start + size);
	ac_xpu_encode_qad_vector3(sd_perm_array, count, &read_vec, &write_vec, &lock_vec);

	rc = xpu4_get_supported_qads(xpu_info, &valid_qad_mask);
	if (rc != AC_SUCCESS)
		return rc;

	if ((valid_qad_mask & read_vec) != read_vec) {
		ERROR("AC_XPU ErrCode: %d, Read vector invalid: valid=0x%x, requested=0x%x",
		      AC_ERR_READ_VECTOR_INVALID, valid_qad_mask, read_vec);
		return AC_ERR_READ_VECTOR_INVALID;
	}
	if ((valid_qad_mask & write_vec) != write_vec) {
		ERROR("AC_XPU ErrCode: %d, Write vector invalid: valid=0x%x, requested=0x%x",
		      AC_ERR_WRITE_VECTOR_INVALID, valid_qad_mask, write_vec);
		return AC_ERR_WRITE_VECTOR_INVALID;
	}
	if ((valid_qad_mask & lock_vec) != lock_vec) {
		ERROR("AC_XPU ErrCode: %d, Lock vector invalid: valid=0x%x, requested=0x%x",
		      AC_ERR_LOCK_VECTOR_INVALID, valid_qad_mask, lock_vec);
		return AC_ERR_LOCK_VECTOR_INVALID;
	}

	/* Fail locking other the respective QAD */
	if (lock_vec & (~ac_xpu4_get_env_qad_vector())) {
		/* With APNSPE=1, protecting means locking */
		ERROR("AC_XPU ErrCode: %d, Not locking for XPU at 0x%x, region 0x%x%08x-0x%x%08x",
		      AC_ERR_NOT_LOCKING, xpu_info->soc_addr, (uint32_t)(s >> 32), (uint32_t)s,
		      (uint32_t)(e >> 32), (uint32_t)e);
		return AC_ERR_NOT_LOCKING;
	}

	rc = xpu4_protect_region(xpu_info, s, e, wowp, read_vec, write_vec, lock_vec, merge);
	return rc;
}

/*
 * API to unprotect/unlock the memory region for the execution environment
 *
 * @param xpu_info	XPU info structure
 * @param start		Start address
 * @param size		Region size
 * @param sd_array	Security domain array
 * @param count		Number of domains
 * @param blanket	Blanket flag
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_unprotect_region(ac_xpu4_priv_info *xpu_info,
				  uint64_t start,
				  uint64_t size,
				  ac_sec_domain_id *sd_array,
				  uint32_t count,
				  bool blanket)
{
	uint64_t s;
	uint64_t e;
	ac_xpu4_qad_vector lock_vec = 0;
	ac_xpu4_qad_vector valid_qad_mask = 0;
	AC_ERROR rc = AC_FAILURE;

	if (is_ac_disabled())
		return AC_SUCCESS;

	if ((xpu_info == NULL) || (sd_array == NULL) || (start > UINT64_MAX - size) ||
	    (count == 0) || (count > AC_SD_END_MARKER))
		return AC_ERR_INPUT_VALIDATION;

	s = ac_xpu_addr_soc2_peripheral_xpu(xpu_info, start);
	e = ac_xpu_addr_soc2_peripheral_xpu(xpu_info, start + size);
	lock_vec = ac_xpu_encode_qad_vector(sd_array, count);

	rc = xpu4_get_supported_qads(xpu_info, &valid_qad_mask);
	if (rc != AC_SUCCESS)
		return rc;

	if ((valid_qad_mask & lock_vec) != lock_vec) {
		ERROR("AC_XPU ErrCode: %d, Lock vector invalid: valid=0x%x, requested=0x%x",
		      AC_ERR_LOCK_VECTOR_INVALID, valid_qad_mask, lock_vec);
		return AC_ERR_LOCK_VECTOR_INVALID;
	}

	/* Fail unlocking other the respective QAD */
	if (lock_vec & (~ac_xpu4_get_env_qad_vector())) {
		ERROR("AC_XPU ErrCode: %d, Not locking for XPU at 0x%x, region 0x%x%08x-0x%x%08x",
		      AC_ERR_NOT_LOCKING, xpu_info->soc_addr, (uint32_t)(s >> 32), (uint32_t)s,
		      (uint32_t)(e >> 32), (uint32_t)e);
		return AC_ERR_NOT_LOCKING;
	}

	rc = xpu4_unprotect_region(xpu_info, s, e, lock_vec, blanket);

	return rc;
}

/*
 * API to unprotect the RG for the execution environment
 *
 * @param xpu_info	XPU info structure
 * @param rg_num	Region number
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_xpu_unprotect_by_rg(ac_xpu4_priv_info *xpu_info, uint32_t rg_num)
{
	AC_ERROR rc = AC_FAILURE;
	ac_xpu4_qad_vector lock_vector = ac_xpu4_get_env_qad_vector();

	/*
	 * TODO: Need to check if we need to re-program the RG with other QAD
	 * in case RG is disabled
	 */
	rc = xpu4_unlock_for_env(xpu_info, rg_num, lock_vector);

	if (rc != AC_SUCCESS)
		return rc;

	return rc;
}

/*
 * Read XPU error syndrome registers
 *
 * @param xpu_info	XPU info structure
 * @param syndrome	Pointer to store syndrome
 */
void ac_xpu_read_syndrome(ac_xpu4_priv_info *xpu_info, void *syndrome)
{
	ac_xpu_error_syndrome *syn = (ac_xpu_error_syndrome *)syndrome;

	syn->esr = XPU4_IN(xpu_info->addr, ESR);
	syn->synar0 = XPU4_IN(xpu_info->addr, SYNAR0);
	syn->synar1 = XPU4_IN(xpu_info->addr, SYNAR1);
	syn->synr0 = XPU4_IN(xpu_info->addr, SYNR0);
	syn->synr1 = XPU4_IN(xpu_info->addr, SYNR1);
	syn->synr2 = XPU4_IN(xpu_info->addr, SYNR2);
}

/*
 * Log XPU violation details with comprehensive syndrome information
 *
 * @param xpu_info	XPU info structure
 */
void ac_xpu_log_violation(ac_xpu4_priv_info *xpu_info)
{
	ac_xpu_error_syndrome syndrome;

	ac_xpu_read_syndrome(xpu_info, &syndrome);

	ERROR("XPU=0x%x ESR=0x%x SYNAR0=0x%x SYNAR1=0x%x SYNR0=0x%x SYNR1=0x%x SYNR2=0x%x\n",
	      xpu_info->soc_addr,
	      syndrome.esr,
	      syndrome.synar0,
	      syndrome.synar1,
	      syndrome.synr0,
	      syndrome.synr1,
	      syndrome.synr2);

	ERROR("ESR: 0x%08x, CLMULTI: %x, CFGMULTI: %x, CLERR: %x, CFGERR: %x\n",
	      syndrome.esr,
	      (syndrome.esr & HWIO_XPU4_ESR_CLMULTI_BMSK) >> HWIO_XPU4_ESR_CLMULTI_SHFT,
	      (syndrome.esr & HWIO_XPU4_ESR_CFGMULTI_BMSK) >> HWIO_XPU4_ESR_CFGMULTI_SHFT,
	      (syndrome.esr & HWIO_XPU4_ESR_CLERR_BMSK) >> HWIO_XPU4_ESR_CLERR_SHFT,
	      (syndrome.esr & HWIO_XPU4_ESR_CFGERR_BMSK) >> HWIO_XPU4_ESR_CFGERR_SHFT);

	ERROR("SYNAR0_SYNADDR: 0x%08x\n",
	      (syndrome.synar0 & HWIO_XPU4_SYNAR0_SYNADDR_BMSK) >>
	      HWIO_XPU4_SYNAR0_SYNADDR_SHFT);

	ERROR("SYNAR1_SYNADDR: 0x%08x\n",
	      (syndrome.synar1 & HWIO_XPU4_SYNAR1_SYNADDR_BMSK) >>
	      HWIO_XPU4_SYNAR1_SYNADDR_SHFT);

	ERROR("SYNR0: 0x%08x, PH: %x, AC_CFG: %x, AC_CL: %x,BURSTLEN: %x, SSIZE: %x\n",
	      syndrome.synr0,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_PH_BMSK) >> HWIO_XPU4_SYNR0_PH_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_AC_CFG_BMSK) >> HWIO_XPU4_SYNR0_AC_CFG_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_AC_CL_BMSK) >> HWIO_XPU4_SYNR0_AC_CL_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_BURSTLEN_BMSK) >>
	      HWIO_XPU4_SYNR0_BURSTLEN_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_SSIZE_BMSK) >> HWIO_XPU4_SYNR0_SSIZE_SHFT);

	ERROR("LEN: %x, QAD: %x, REQ_OPC: %x, PRIV: %x, INST: %x, XPROTNS: %x\n",
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_LEN_BMSK) >> HWIO_XPU4_SYNR0_LEN_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_QAD_BMSK) >> HWIO_XPU4_SYNR0_QAD_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_REQ_OPC_BMSK) >>
	      HWIO_XPU4_SYNR0_REQ_OPC_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_PRIV_BMSK) >> HWIO_XPU4_SYNR0_PRIV_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_INST_BMSK) >> HWIO_XPU4_SYNR0_INST_SHFT,
	      (syndrome.synr0 & HWIO_XPU4_SYNR0_XPROTNS_BMSK) >>
	      HWIO_XPU4_SYNR0_XPROTNS_SHFT);

	ERROR("SYNR1: 0x%08x, TID: %x, TRTYPE: %x, BID: %x, PID: %x, MID: %x\n",
	      syndrome.synr1,
	      (syndrome.synr1 & HWIO_XPU4_SYNR1_TID_BMSK) >> HWIO_XPU4_SYNR1_TID_SHFT,
	      (syndrome.synr1 & HWIO_XPU4_SYNR1_TRTYPE_BMSK) >> HWIO_XPU4_SYNR1_TRTYPE_SHFT,
	      (syndrome.synr1 & HWIO_XPU4_SYNR1_BID_BMSK) >> HWIO_XPU4_SYNR1_BID_SHFT,
	      (syndrome.synr1 & HWIO_XPU4_SYNR1_PID_BMSK) >> HWIO_XPU4_SYNR1_PID_SHFT,
	      (syndrome.synr1 & HWIO_XPU4_SYNR1_MID_BMSK) >> HWIO_XPU4_SYNR1_MID_SHFT);

	ERROR("SYNR2: 0x%08x, APSVIOE: %x, SLE: %x, CFG_OWNER: %x, APNSEE:%x,CESDE:%x\n",
	      syndrome.synr2,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_APSVIOE_BMSK) >>
	      HWIO_XPU4_SYNR2_APSVIOE_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_SLE_BMSK) >> HWIO_XPU4_SYNR2_SLE_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_CFG_OWNER_BMSK) >>
	      HWIO_XPU4_SYNR2_CFG_OWNER_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_APNSEE_BMSK) >> HWIO_XPU4_SYNR2_APNSEE_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_CESDE_BMSK) >> HWIO_XPU4_SYNR2_CESDE_SHFT);

	ERROR("OPTRW_EN: %x, ATOPC: %x, REDIRBITS: %x, INNERCACHEABLE: %x, MEMTYPE: %x\n",
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_OPTRW_EN_BMSK) >>
	      HWIO_XPU4_SYNR2_OPTRW_EN_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_ATOPC_BMSK) >> HWIO_XPU4_SYNR2_ATOPC_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_REDIRBITS_BMSK) >>
	      HWIO_XPU4_SYNR2_REDIRBITS_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_INNERCACHEABLE_BMSK) >>
	      HWIO_XPU4_SYNR2_INNERCACHEABLE_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_MEMTYPE_BMSK) >>
	      HWIO_XPU4_SYNR2_MEMTYPE_SHFT);

	ERROR("TRANSIENT: %x, NOALLOCATE:%x, WRITETHROUGH: %x, CACHEALLOCATION: %x\n",
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_TRANSIENT_BMSK) >>
	      HWIO_XPU4_SYNR2_TRANSIENT_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_NOALLOCATE_BMSK) >>
	      HWIO_XPU4_SYNR2_NOALLOCATE_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_WRITETHROUGH_BMSK) >>
	      HWIO_XPU4_SYNR2_WRITETHROUGH_SHFT,
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_CACHEALLOCATION_BMSK) >>
	      HWIO_XPU4_SYNR2_CACHEALLOCATION_SHFT);

	ERROR("DIRTYINFO: %x\n",
	      (syndrome.synr2 & HWIO_XPU4_SYNR2_DIRTYINFO_BMSK) >>
	      HWIO_XPU4_SYNR2_DIRTYINFO_SHFT);
}

/*
 * Clear XPU violation status registers
 *
 * @param xpu_info	XPU info structure
 */
void ac_xpu_clear_violation(ac_xpu4_priv_info *xpu_info)
{
	XPU4_OUT(xpu_info->addr, ESR, 0);
}

/*
 * Append silent log for XPU violation
 *
 * @param xpu_info	XPU info structure
 */
void ac_xpu_append_silent_log(ac_xpu4_priv_info *xpu_info)
{
	/* For TF-A, we'll use the logging system instead of a silent buffer */
	ac_xpu_error_syndrome syndrome;

	ac_xpu_read_syndrome(xpu_info, &syndrome);

	/* Log at VERBOSE level for silent logging */
	ERROR("AC_XPU: DEBUG: Silent XPU violation: XPU=0x%x ESR=0x%x SYNAR0=0x%x SYNAR1=0x%x",
	      xpu_info->soc_addr, syndrome.esr, syndrome.synar0, syndrome.synar1);
}

/*
 * Returns whether silent logging is enabled currently in target or not
 *
 * @return true if silent logging enabled, false otherwise
 */
bool ac_is_silent_logging_enabled(void)
{
	if (is_ac_disabled())
		return false;
	return HWIO_INF(TCSR_ACXPU4_DCR, SLE) == TME_SLE_ENABLED;
}

/*
 * Update Config Owner to Secure
 *
 * This function sets the CFGOWNS bit in the TCSR_ACXPU4_GCR register
 * to configure the XPU config owner as secure. Uses HWIO macros for
 * proper register access.
 */
void ac_xpu_v4_set_config_owner_secure(void)
{
	if (is_ac_disabled())
		return;
	/* Set CFGOWNS bit (bit 0) in TCSR_ACXPU4_GCR register to 1 */
	HWIO_OUTF(TCSR_ACXPU4_GCR, CFGOWNS, 1);
}
