/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

/*
 * XPU ISR functionality header for TF-A environment
 * This file contains function declarations for both the main ISR functionality
 * and stub implementations previously separated between ACXpuIsr.h and ACXpuIsrStubs.h
 */

#ifndef AC_XPU_ISR_H
#define AC_XPU_ISR_H

#include <stdbool.h>
#include <stdint.h>

#include "hal_xpu4.h"
#include <qti_xpu_err.h>

/*
 * XPU ISR handler for TF-A environment
 *
 * @param intnum	Interrupt number
 * @param handle	Handle passed to ISR
 * @param ctx		Context pointer
 *
 * @return Context pointer
 */
void *ac_xpu_isr(uint32_t intnum, void *handle, void *ctx);

/*
 * Register XPU interrupts for TF-A environment
 *
 * @return AC_SUCCESS on success, error code otherwise
 */
int ac_register_xpu_interrupts(void);

/*
 * Set XPU violation error fatal mode
 *
 * @param enable	True to enable error fatal mode, false to disable
 */
void ac_set_xpu_violation_error_fatal(bool enable);

/*
 * Enable/disable silent logging for XPU violations
 *
 * @param enable	True to enable silent logging, false to disable
 */
void ac_set_silent_logging(bool enable);

/*
 * Append silent log for XPU violation
 *
 * @param xpu_id	XPU identifier
 *
 * @return AC_ERROR - AC_SUCCESS on success
 */
AC_ERROR ac_append_silent_log(HAL_xpu2_XPU2Type xpu_id);

/*
 * Clear XPU violation status
 *
 * @param xpu_id	XPU identifier
 *
 * @return AC_ERROR - AC_SUCCESS on success
 */
AC_ERROR ac_clear_violation(HAL_xpu2_XPU2Type xpu_id);

/*
 * Log XPU violation details
 *
 * @param xpu_id	XPU identifier
 *
 * @return AC_ERROR - AC_SUCCESS on success
 */
AC_ERROR ac_log_violation(HAL_xpu2_XPU2Type xpu_id);

/*
 * Stub implementation for IPC fault notification
 */
void ipc_fault(void);

/*
 * Enable XPU APSVIOE - stubbed for TF-A
 *
 * @return AC_SUCCESS on success, AC_FAILURE on error
 */
int ac_enable_xpu_apsvioe(void);

/* Forward declaration for ISR function type */
typedef void *(*int_svc_isr_api_t)(uint32_t intnum, void *handle, void *ctx);

/*
 * Register ISR with qtiseclib interrupt service
 *
 * @param intnum	Interrupt number
 * @param desc		Description string
 * @param desc_len	Description length
 * @param isr_func	ISR function pointer
 * @param ctx		Context parameter
 *
 * @return 0 on success, error code otherwise
 */
extern int int_svc_register_isr(uint32_t intnum, const char *desc,
				 size_t desc_len, int_svc_isr_api_t isr_func,
				 void *ctx);

#endif /* AC_XPU_ISR_H */
