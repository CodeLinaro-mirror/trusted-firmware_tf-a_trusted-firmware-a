/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * XPU ISR functionality for TF-A environment
 */

#include <stdbool.h>
#include <stdint.h>

#include <common/debug.h>
#include <lib/spinlock.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <qtiseclib_defs_plat.h>
#include <qti_xpu_err.h>

#include "access_control_target.h"
#include "access_control_xpu.h"
#include "ac_xpu.h"
#include "ac_xpu_isr.h"
#include "ac_xpu_target_info.h"
#include "hal_hwio.h"
#include "target_hwio.h"
#ifdef XPU_VERSION_4
#include "hal_xpu4.h"
#include "hal_xpu4_util.h"
#endif

/* Interrupt definitions */
#define AC_INT_XPU_SEC			QTISECLIB_INT_ID_XPU_VIOLATION
#define AC_INT_XPU_SEC_DESC		"XPU Violation Interrupt"

static bool s_ac_silent_logging_enabled;

/* Spinlock for ISR logging exclusivity */
extern spinlock_t isr_log_sync_lock;

/* External variables from ac_target_config.c */
extern ac_register_tuple g_ac_xpu_intr_status_reg[XPU_ERR_INT_REG_NUM];
extern ac_register_tuple g_ac_xpu_intr_en_reg[XPU_ERR_INT_REG_NUM];
extern const ac_xpu_err_mapping_t
	g_ac_xpu_err_pos_to_xpu_map[XPU_ERR_INT_REG_NUM][XPU_ERR_NUM_PER_REG];

/*
 * Initialize XPU and get XPU info structure
 *
 * @param xpu_id	XPU identifier
 * @param xpu_info	Pointer to store XPU info structure
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
static AC_ERROR ac_init_xpu_and_get_info(HAL_xpu2_XPU2Type xpu_id,
					  ac_xpu4_priv_info **xpu_info)
{
	ac_xpu4_priv_info *info = NULL;
	AC_ERROR rc = AC_FAILURE;

	if ((xpu_info == NULL) || (xpu_id >= HAL_XPU2_COUNT) || (xpu_id < 0))
		return AC_ERR_VMID_FAIL;

	info = ac_xpu_get_xpu_info(xpu_id);
	if (!info) {
		ERROR("AC_XPU No XPU info for xpu_id=%u\n", xpu_id);
		return AC_ERR_NO_XPU_INFO;
	}

	if (!info->rev) {
		rc = ac_xpu_init_xpu(info);
		if (rc != AC_SUCCESS)
			return rc;
	}

	*xpu_info = info;
	return AC_SUCCESS;
}

/*
 * Append silent log for XPU violation
 *
 * @param xpu_id	XPU identifier
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_append_silent_log(HAL_xpu2_XPU2Type xpu_id)
{
	ac_xpu4_priv_info *xpu_info;
	AC_ERROR rc;

	rc = ac_init_xpu_and_get_info(xpu_id, &xpu_info);
	if (rc != AC_SUCCESS) {
		VERBOSE("No XPU info: rc=%d, xpu_id=%u\n", rc, xpu_id);
		return AC_ERR_NO_XPU_INFO;
	}

	ac_xpu_append_silent_log(xpu_info);

	return AC_SUCCESS;
}

/*
 * Log XPU violation details
 *
 * @param xpu_id	XPU identifier
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_log_violation(HAL_xpu2_XPU2Type xpu_id)
{
	ac_xpu4_priv_info *xpu_info;
	AC_ERROR rc;

	rc = ac_init_xpu_and_get_info(xpu_id, &xpu_info);
	if (rc != AC_SUCCESS) {
		ERROR("AC_XPU No XPU info: rc=%d, xpu_id=%u\n", rc, xpu_id);
		return AC_ERR_NO_XPU_INFO;
	}

	ac_xpu_log_violation(xpu_info);

	return AC_SUCCESS;
}

/*
 * Clear XPU violation status
 *
 * @param xpu_id	XPU identifier
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
AC_ERROR ac_clear_violation(HAL_xpu2_XPU2Type xpu_id)
{
	ac_xpu4_priv_info *xpu_info;
	AC_ERROR rc;

	rc = ac_init_xpu_and_get_info(xpu_id, &xpu_info);
	if (rc != AC_SUCCESS) {
		ERROR("AC_XPU No XPU info: rc=%d, xpu_id=%u\n", rc, xpu_id);
		return AC_ERR_NO_XPU_INFO;
	}

	ac_xpu_clear_violation(xpu_info);

	VERBOSE("XPU violation cleared: xpu_id=%u, xpu_info addr=0x%x\n",
		xpu_id, xpu_info->addr);
	return AC_SUCCESS;
}

/*
 * IPC fault notification - raises interrupt to HLOS
 */
void ipc_fault(void)
{
	/* Raise interrupt to HLOS (High Level Operating System) */
	VERBOSE("AC_XPU IPC fault - raising interrupt %d to HLOS\n",
	     NOTIFY_KERNEL_IRQ);
	plat_ic_set_interrupt_pending(NOTIFY_KERNEL_IRQ);
}

/*
 * Enable XPU APSVIOE
 *
 * @return AC_SUCCESS on success, AC_FAILURE otherwise
 */
int ac_enable_xpu_apsvioe(void)
{
	uint32_t hw_value = 0;

	HWIO_TCSR_ACVIOCR_OUT(HWIO_TCSR_ACVIOCR_APSVIOE_BMSK);
	hw_value = HWIO_TCSR_ACVIOCR_IN;

	if (hw_value != HWIO_TCSR_ACVIOCR_APSVIOE_BMSK) {
		ERROR("[ACLIB ERROR] Write mismatch - addr: 0x%x, exp: 0x%x, act: 0x%x\n",
		      HWIO_TCSR_ACVIOCR_ADDR,
		      HWIO_TCSR_ACVIOCR_APSVIOE_BMSK, hw_value);
		return AC_FAILURE;
	}

	return AC_SUCCESS;
}

/*
 * XPU ISR handler for TF-A environment
 *
 * @param intnum	Interrupt number
 * @param handle	Interrupt handle
 * @param ctx		Context pointer
 *
 * @return Context pointer
 */
void *ac_xpu_isr(uint32_t intnum, void *handle, void *ctx)
{
	uint32_t status[XPU_ERR_INT_REG_NUM] = {0};
	bool has_violation = false;
	uint32_t reg_num;
	HAL_xpu2_XPU2Type xpu_id;
	int ret;

	spin_lock(&isr_log_sync_lock);

	if (!s_ac_silent_logging_enabled)
		INFO("AC_XPU xpu: ISR begin\n");

	/* Read interrupt status registers */
	for (reg_num = 0; reg_num < XPU_ERR_INT_REG_NUM; reg_num++) {
		status[reg_num] = in_dword_masked(g_ac_xpu_intr_status_reg[reg_num].addr,
						  g_ac_xpu_intr_status_reg[reg_num].mask);

		if (status[reg_num])
			has_violation = true;
	}

	if (!s_ac_silent_logging_enabled) {
		INFO("AC_XPU XPU INTR 0:1 >> %08x:%08x\n", status[0],
		     status[1]);

		if (!has_violation)
			INFO("AC_XPU No xPU Violation\n");
	}

	/* Process violations */
	for (reg_num = 0; reg_num < XPU_ERR_INT_REG_NUM; reg_num++) {
		uint32_t bit_pos;

		status[reg_num] = in_dword_masked(g_ac_xpu_intr_status_reg[reg_num].addr,
						  g_ac_xpu_intr_status_reg[reg_num].mask);

		if (!status[reg_num])
			continue;

		for (bit_pos = 0; bit_pos < XPU_ERR_NUM_PER_REG; bit_pos++) {
			if (!(status[reg_num] & ((uint32_t)0x1 << bit_pos)))
				continue;

			xpu_id = g_ac_xpu_err_pos_to_xpu_map[reg_num][bit_pos].xpu_id;
			if (xpu_id == HAL_XPU2_COUNT) {
				if (!s_ac_silent_logging_enabled) {
					INFO("AC_XPU xPU Map fail: reg_num:[%u], bit_pos=[%u]\n",
					     reg_num, bit_pos);
				}
				continue;
			}

			/* Print XPU name for debugging */
			if (!s_ac_silent_logging_enabled) {
				INFO("AC_XPU Violation detected on XPU: %s\n",
				     g_ac_xpu_err_pos_to_xpu_map[reg_num][bit_pos].name);
			}

			if (s_ac_silent_logging_enabled) {
				ret = ac_append_silent_log(xpu_id);
				if (ret != AC_SUCCESS)
					break;

				ret = ac_clear_violation(xpu_id);
				if (ret != AC_SUCCESS)
					break;
			} else {
				ret = ac_log_violation(xpu_id);
				if (ret != AC_SUCCESS)
					break;

				ret = ac_clear_violation(xpu_id);
				if (ret != AC_SUCCESS)
					break;
			}
		}
	}

	spin_unlock(&isr_log_sync_lock);

	ipc_fault();

	return ctx;
}

/*
 * Enable XPU TCSR interrupts
 *
 * @return AC_SUCCESS on success, AC_FAILURE otherwise
 */
static int ac_enable_xpu_tcsr_interrupts(void)
{
	uint32_t i = 0;
	uint32_t hw_value = 0;

	for (i = 0; i < XPU_ERR_INT_REG_NUM; i++) {
		out_dword(g_ac_xpu_intr_en_reg[i].addr,
			  g_ac_xpu_intr_en_reg[i].mask);
		hw_value = in_dword(g_ac_xpu_intr_en_reg[i].addr);

		if (hw_value != g_ac_xpu_intr_en_reg[i].mask) {
			ERROR("XPU TCSR interrupt enable failed: addr=0x%x, exp=0x%x, act=0x%x\n",
			      g_ac_xpu_intr_en_reg[i].addr,
			      g_ac_xpu_intr_en_reg[i].mask, hw_value);
			return AC_FAILURE;
		}
	}

	return AC_SUCCESS;
}

/*
 * Register XPU interrupts for TF-A environment
 *
 * @return AC_SUCCESS on success, AC_FAILURE otherwise
 */
int ac_register_xpu_interrupts(void)
{
	int ret;

	ret = ac_enable_xpu_apsvioe();
	if (ret)
		return AC_FAILURE;

	/* Register & enable interrupt handling for XPU's */
	ret = int_svc_register_isr(AC_INT_XPU_SEC, AC_INT_XPU_SEC_DESC,
				   sizeof(AC_INT_XPU_SEC_DESC), ac_xpu_isr,
				   NULL);
	if (ret) {
		ERROR("[ACLIB ERROR] Register interrupt failure: %d\n", ret);
		return AC_FAILURE;
	}

	ret = ac_enable_xpu_tcsr_interrupts();
	if (ret) {
		ERROR("[ACLIB ERROR] Enable interrupt failure: %d\n", ret);
		return AC_FAILURE;
	}

	VERBOSE("AC_XPU XPU interrupts registered successfully for interrupt ID 0x%x\n",
	     AC_INT_XPU_SEC);
	return AC_SUCCESS;
}
