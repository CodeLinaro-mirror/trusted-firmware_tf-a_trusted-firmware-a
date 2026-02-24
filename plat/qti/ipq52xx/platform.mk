#
# Copyright (c) 2017-2018, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Make for ipq52xx QTI platform.

QTI_PLAT_PATH		:=	plat/qti
CHIPSET			:=	${PLAT}

# Turn On Separate code & data.
SEPARATE_CODE_AND_RODATA	:=	1
USE_COHERENT_MEM		:=	0
WARMBOOT_ENABLE_DCACHE_EARLY	:=	1

# Disable the PSCI platform compatibility layer
ENABLE_PLAT_COMPAT		:=	0

# Enable PSCI v1.0 extended state ID format
PSCI_EXTENDED_STATE_ID	:=	1
ARM_RECOM_STATE_ID_ENC	:=	1
PSCI_OS_INIT_MODE	:=	1

COLD_BOOT_SINGLE_CPU		:=	1
PROGRAMMABLE_RESET_ADDRESS	:=	1

RESET_TO_BL31			:=	0

# GIC driver version used by the platform
QTI_USE_GIC_DRIVER		:=	2
$(eval $(call add_define,QTI_USE_GIC_DRIVER))
#Route Secure interrupts to SEL1
GICV2_G0_FOR_EL3		:=	0

QTI_SDI_BUILD			:=	1
$(eval $(call assert_boolean,QTI_SDI_BUILD))
$(eval $(call add_define,QTI_SDI_BUILD))

# Disable memory assign service for platforms without coreboot
DISABLE_QTI_MEM_ASSIGN := 1
$(eval $(call assert_boolean,DISABLE_QTI_MEM_ASSIGN))
$(eval $(call add_define,DISABLE_QTI_MEM_ASSIGN))

# Enable stack protector with 'strong' level protection
ENABLE_STACK_PROTECTOR	:=	strong

# Enable platform-specific linker script for IMEM region
PLAT_EXTRA_LD_SCRIPT := 1
$(eval $(call add_define,PLAT_EXTRA_LD_SCRIPT))

QTI_EXTERNAL_INCLUDES	:=	-I${QTI_PLAT_PATH}/${CHIPSET}/inc			\
				-I${QTI_PLAT_PATH}/common/inc				\
				-I${QTI_PLAT_PATH}/common/inc/$(ARCH)			\
				-I${QTI_PLAT_PATH}/qtiseclib/inc			\
				-I${QTI_PLAT_PATH}/qtiseclib/inc/${CHIPSET}		\

QTI_BL31_SOURCES	:=	$(QTI_PLAT_PATH)/common/src/$(ARCH)/qti_helpers.S	\
				$(QTI_PLAT_PATH)/common/src/$(ARCH)/qti_uart_console.S	\
				$(QTI_PLAT_PATH)/common/src/$(ARCH)/a53.S		\
				$(QTI_PLAT_PATH)/common/src/qti_stack_protector.c	\
				$(QTI_PLAT_PATH)/common/src/qti_rng.c			\
				$(QTI_PLAT_PATH)/common/src/qti_common.c		\
				$(QTI_PLAT_PATH)/common/src/qti_bl31_setup.c		\
				$(QTI_PLAT_PATH)/common/src/qti_gic_v2.c		\
				$(QTI_PLAT_PATH)/common/src/qti_interrupt_svc.c		\
				$(QTI_PLAT_PATH)/common/src/qti_topology.c		\
				$(QTI_PLAT_PATH)/common/src/qti_syscall.c		\
				$(QTI_PLAT_PATH)/common/src/qti_pm.c			\
				$(QTI_PLAT_PATH)/ipq-common/src/pm_ps_hold.c		\
				$(QTI_PLAT_PATH)/qtiseclib/src/qtiseclib_cb_interface.c	\
				$(QTI_PLAT_PATH)/common/src/qti_syscall.c		\

# Enable DIAG LOG Console
DIAG_LOG			:=	1
$(eval $(call add_define,DIAG_LOG))

# Include common QTI makefile for conditional compilation
include $(QTI_PLAT_PATH)/common/qti_common.mk

PLAT_INCLUDES		:=	-Idrivers/arm/gic/common/				\
				-Idrivers/arm/gic/v2/					\
				-Iinclude/plat/common/					\

PLAT_INCLUDES		+=	${QTI_EXTERNAL_INCLUDES}

include lib/xlat_tables_v2/xlat_tables.mk
PLAT_BL_COMMON_SOURCES	+=	${XLAT_TABLES_LIB_SRCS}				\
				plat/common/aarch64/crash_console_helpers.S	\
				common/desc_image_load.c			\
				lib/bl_aux_params/bl_aux_params.c


#PSCI Sources.
PSCI_SOURCES		:=	plat/common/plat_psci_common.c			\

#Timer sources
TIMER_SOURCES		:=	drivers/delay_timer/generic_delay_timer.c	\
				drivers/delay_timer/delay_timer.c

# Include GICv2 driver files
include drivers/arm/gic/v2/gicv2.mk
#GIC sources.
GIC_SOURCES		:=	plat/common/plat_gicv2.c			\
				${GICV2_SOURCES}				\

# xPU Configuration
XPU_VERSION := v4

# Base xPU support define
ENABLE_XPU	:=	0
$(eval $(call add_define,ENABLE_XPU))

# Include xPU driver
ifeq (${ENABLE_XPU},1)
include drivers/qti/accesscontrol/xpu/xpu.mk
endif

# Prohibit using deprecated interfaces. We rely on this for this platform.
ERROR_DEPRECATED	:=	1

BL31_SOURCES		+=	${QTI_BL31_SOURCES}				\
				${PSCI_SOURCES}					\
				${GIC_SOURCES}					\
				${TIMER_SOURCES}				\

LIB_QTI_PATH	:=	${QTI_PLAT_PATH}/qtiseclib/lib/${CHIPSET}


# Enable NOC error decode for IPQ52xx
ENABLE_NOC_DECODE := 1
# Define feature flag to enable NOC error decode (optional, enabled by default for IPQ5424)
$(eval $(call add_define,ENABLE_NOC_DECODE))

# Enable NOC for IPQ5424
ENABLE_ICB := 1
# Define feature flag to enable ICB error handler
$(eval $(call add_define,ENABLE_ICB))

ifeq (${ENABLE_ICB},1)
include drivers/qti/icb/common/icb.mk
endif

# Override this on the command line to point to the qtiseclib library which
# will be available in coreboot.org
QTISECLIB_PATH ?=

ifeq ($(QTISECLIB_PATH),)
# if No lib then use stub implementation for qtiseclib interface
$(warning QTISECLIB_PATH is not provided while building, using stub implementation. \
		Please refer docs/plat/qti.rst for more details \
		THIS FIRMWARE WILL NOT BOOT!)
BL31_SOURCES	+=	plat/qti/qtiseclib/src/qtiseclib_interface_stub.c
else
# use library provided by QTISECLIB_PATH
LDFLAGS += -L $(dir $(QTISECLIB_PATH))
LDLIBS += -l$(patsubst lib%.a,%,$(notdir $(QTISECLIB_PATH)))
endif
