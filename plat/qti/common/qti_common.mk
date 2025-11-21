# Common QTI makefile fragment for conditional compilation

# Conditionally compile DIAG logging sources based on DIAG_LOG flag
# Platform-specific platform.mk should set DIAG_LOG := 1 (or 0) before including this file
ifeq ($(DIAG_LOG),1)
QTI_BL31_SOURCES += $(QTI_PLAT_PATH)/common/src/aarch64/qti_diag_console.S \
                    $(QTI_PLAT_PATH)/common/src/qti_diag.c
endif
