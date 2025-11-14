#
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: ISC
#

# xPU (Access Control Library) driver makefile

XPU_DIR := drivers/qti/accesscontrol/xpu

# Base include directories
XPU_INCLUDES := -I${XPU_DIR}/inc \
               -I${XPU_DIR}/cfg/$(CHIPSET) \
               -Iinclude/drivers/qti/accesscontrol/xpu

# Base xPU sources (always included when xPU is enabled)
XPU_SOURCES := ${XPU_DIR}/src/ac_xpu_core.c \
              ${XPU_DIR}/src/ac_xpu_utils.c \
              ${XPU_DIR}/src/ac_xpu_isr.c \
              ${XPU_DIR}/src/ac_xpu.c \
              ${XPU_DIR}/src/ac_config_interface.c

# Chipset-specific sources
XPU_CHIPSET_SOURCES := ${XPU_DIR}/cfg/$(CHIPSET)/ac_xpu_static_config.c \
                      ${XPU_DIR}/cfg/$(CHIPSET)/ac_target_data.c \
                      ${XPU_DIR}/cfg/$(CHIPSET)/ac_target.c

# Version-specific files and defines
# Default to v4 if XPU_VERSION is not specified
ifeq ($(XPU_VERSION),)
    XPU_VERSION := v4
    $(info XPU: XPU_VERSION not specified, defaulting to v4)
endif

ifeq ($(XPU_VERSION),v4)
    XPU_SOURCES += ${XPU_DIR}/src/v4/hal_xpu4.c \
                  ${XPU_DIR}/src/v4/hal_xpu4_util.c
    XPU_INCLUDES += -I${XPU_DIR}/inc/v4/inc
    $(eval $(call add_define,XPU_VERSION_4))
endif

# Combine all sources
XPU_ALL_SOURCES := $(XPU_SOURCES) $(XPU_CHIPSET_SOURCES)

# Add xPU sources to BL31
BL31_SOURCES += ${XPU_ALL_SOURCES}

# Add xPU includes
PLAT_INCLUDES += ${XPU_INCLUDES}

# Base xPU support define
$(eval $(call add_define,ENABLE_XPU))

# Check for XPU_DISABLE flag
ifeq ($(XPU_DISABLE),1)
    $(eval $(call add_define,XPU_DISABLE))
    $(info XPU: XPU_DISABLE=1 - XPU functionality will be disabled at runtime)
endif

# Suppress enum conversion warnings for access control driver
TF_CFLAGS += -Wno-enum-conversion
