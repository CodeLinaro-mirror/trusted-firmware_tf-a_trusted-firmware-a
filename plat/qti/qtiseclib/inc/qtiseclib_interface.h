/*
 * Copyright (c) 2018-2021, The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef QTISECLIB_INTERFACE_H
#define QTISECLIB_INTERFACE_H

#include <stdbool.h>
#include <stdint.h>

#include <qtiseclib_defs.h>

typedef struct memprot_ipa_info_s {
	uint64_t mem_addr;
	uint64_t mem_size;
} memprot_info_t;

typedef struct memprot_dst_vm_perm_info_s {
	uint32_t dst_vm;
	uint32_t dst_vm_perm;
	uint64_t ctx;
	uint32_t ctx_size;
} memprot_dst_vm_perm_info_t;

typedef  void *(*int_svc_isr_api_t) (uint32_t intnum, void *handle, void *ctx);

typedef enum{
	DBG_ERR_FATAL_NONE,
	DBG_ERR_FATAL_PANIC,
	DBG_ERR_FATAL_NON_SECURE_WDT,
	DBG_ERR_FATAL_NOC_ERROR,
	DBG_ERR_FATAL_AC_ERROR,
	DBG_ERR_FATAL_ASSERT,
	DBG_ERR_FATAL_MAX
} dbg_err_fatal_t;

/*
 * QTISECLIB Published API's.
 */

/*
 * Assembly API's
 */

/*
 * CPUSS common reset handler for all CPU wake up (both cold & warm boot).
 * Executes on all core. This API assume serialization across CPU
 * already taken care before invoking.
 *
 * Clobbers: x0 - x17, x30
 */
void qtiseclib_cpuss_reset_asm(uint32_t bl31_cold_boot_state);

/*
 * Execute CPU (Kryo4 gold) specific reset handler / system initialization.
 * This takes care of executing required CPU errata's.
 *
 * Clobbers: x0 - x16
 */
void qtiseclib_kryo4_gold_reset_asm(void);

/*
 * Execute CPU (Kryo46 gold) specific reset handler / system initialization.
 * This takes care of executing required CPU errata's.
 *
 * Clobbers: x0 - x16
 */
void qtiseclib_kryo6_gold_reset_asm(void);

/*
 * Execute CPU (Kryo4 silver) specific reset handler / system initialization.
 * This takes care of executing required CPU errata's.
 *
 * Clobbers: x0 - x16
 */
void qtiseclib_kryo4_silver_reset_asm(void);

/*
 * Execute CPU (Kryo6 silver) specific reset handler / system initialization.
 * This takes care of executing required CPU errata's.
 *
 * Clobbers: x0 - x16
 */
void qtiseclib_kryo6_silver_reset_asm(void);

/*
 * C Api's
 */
void qtiseclib_bl31_platform_setup(void);
void qtiseclib_invoke_isr(uint32_t irq, void *handle);
void qtiseclib_panic(void);

int qtiseclib_mem_assign(const memprot_info_t *mem_info,
			 uint32_t mem_info_list_cnt,
			 const uint32_t *source_vm_list,
			 uint32_t src_vm_list_cnt,
			 const memprot_dst_vm_perm_info_t *dest_vm_list,
			 uint32_t dst_vm_list_cnt);

int qtiseclib_psci_init(uintptr_t warmboot_entry);
int qtiseclib_psci_node_power_on(u_register_t mpidr);
void qtiseclib_psci_node_on_finish(const uint8_t *states);
void qtiseclib_psci_cpu_standby(uint8_t pwr_state);
void qtiseclib_psci_node_power_off(const uint8_t *states);
void qtiseclib_psci_node_suspend(const uint8_t *states);
void qtiseclib_psci_system_reset(void);
void qtiseclib_psci_system_off(void);
void qtiseclib_psci_node_suspend_finish(const uint8_t *states);
void qtiseclib_disable_cluster_coherency(uint8_t state);
int qtiseclib_set_cpu_ctx_buf(uintptr_t addr, uint32_t size);

int int_svc_register_isr(uint32_t intnum, const char *intdesc,
			size_t intdesc_size, int_svc_isr_api_t isr,
			void *ctx);

void dbg_err_fatal(dbg_err_fatal_t err);


#endif /* QTISECLIB_INTERFACE_H */
