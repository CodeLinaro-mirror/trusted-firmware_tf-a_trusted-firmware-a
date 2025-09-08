#
# Copyright (c) 2017-2018, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Make for ipq5200 QTI platform.

QTI_PLAT_PATH		:=	plat/qti
CHIPSET			:=	${PLAT}
SOC			:=	$(patsubst ipq%,%,${PLAT})

# Turn On Separate code & data.
SEPARATE_CODE_AND_RODATA	:=	1
USE_COHERENT_MEM		:=	0
WARMBOOT_ENABLE_DCACHE_EARLY	:=	1

# Enable DIAG LOG Console
DIAG_LOG			:=	1
$(eval $(call add_define,DIAG_LOG))

# Disable the PSCI platform compatibility layer
ENABLE_PLAT_COMPAT		:=	0

# Enable PSCI v1.0 extended state ID format
PSCI_EXTENDED_STATE_ID	:=	1
ARM_RECOM_STATE_ID_ENC	:=	1
PSCI_OS_INIT_MODE	:=	1

COLD_BOOT_SINGLE_CPU		:=	1
PROGRAMMABLE_RESET_ADDRESS	:=	1

RESET_TO_BL31			:=	0

GICV2_G0_FOR_EL3		:=	1

QTI_SDI_BUILD			:=	1
$(eval $(call assert_boolean,QTI_SDI_BUILD))
$(eval $(call add_define,QTI_SDI_BUILD))

# Disable memory assign service for platforms without coreboot
DISABLE_QTI_MEM_ASSIGN := 1
$(eval $(call assert_boolean,DISABLE_QTI_MEM_ASSIGN))
$(eval $(call add_define,DISABLE_QTI_MEM_ASSIGN))

# Enable stack protector.
ENABLE_STACK_PROTECTOR	:=	0

# Enable platform-specific linker script for IMEM region
PLAT_EXTRA_LD_SCRIPT := 1
$(eval $(call add_define,PLAT_EXTRA_LD_SCRIPT))

QTI_5200_PLATFORM	:=	1
$(eval $(call add_define,QTI_5200_PLATFORM))

QTI_EXTERNAL_INCLUDES	:=	-I${QTI_PLAT_PATH}/${CHIPSET}/inc			\
				-I${QTI_PLAT_PATH}/common/inc				\
				-I${QTI_PLAT_PATH}/common/inc/$(ARCH)			\
				-I${QTI_PLAT_PATH}/qtiseclib/inc			\
				-I${QTI_PLAT_PATH}/qtiseclib/inc/${CHIPSET}		\

QTI_BL31_SOURCES	:=	$(QTI_PLAT_PATH)/common/src/$(ARCH)/qti_helpers.S	\
				$(QTI_PLAT_PATH)/common/src/$(ARCH)/a53.S		\
				$(QTI_PLAT_PATH)/common/src/aarch64/qti_diag_console.S	\
				$(QTI_PLAT_PATH)/common/src/qti_stack_protector.c	\
				$(QTI_PLAT_PATH)/common/src/qti_common.c		\
				$(QTI_PLAT_PATH)/common/src/qti_bl31_setup.c		\
				$(QTI_PLAT_PATH)/common/src/qti_gic_v2.c		\
				$(QTI_PLAT_PATH)/common/src/qti_interrupt_svc.c		\
				$(QTI_PLAT_PATH)/common/src/qti_topology.c		\
				$(QTI_PLAT_PATH)/common/src/qti_pm.c			\
				$(QTI_PLAT_PATH)/ipq-common/src/pm_ps_hold.c		\
				$(QTI_PLAT_PATH)/qtiseclib/src/qtiseclib_cb_interface.c	\
				$(QTI_PLAT_PATH)/common/src/qti_diag.c			\

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
				${GICV2_SOURCES}					\

# Prohibit using deprecated interfaces. We rely on this for this platform.
ERROR_DEPRECATED	:=	1

BL31_SOURCES		+=	${QTI_BL31_SOURCES}				\
				${PSCI_SOURCES}					\
				${GIC_SOURCES}					\
				${TIMER_SOURCES}				\

LIB_QTI_PATH	:=	${QTI_PLAT_PATH}/qtiseclib/lib/${CHIPSET}

# By default libqtisec_dbg.a used by debug variant. When this library doesn't exist,
# debug variant will use release version (libqtisec.a) of the library.
QTISECLIB = qtisec
ifneq (${DEBUG}, 0)
ifneq ("$(wildcard $(LIB_QTI_PATH)/libqtisec_dbg.a)","")
QTISECLIB = qtisec_dbg
else
$(warning Release version of qtisec library used in Debug build!!..)
endif
endif

LDFLAGS += -z max-page-size=4096 -L ${LIB_QTI_PATH}
LDLIBS += -l$(QTISECLIB)
