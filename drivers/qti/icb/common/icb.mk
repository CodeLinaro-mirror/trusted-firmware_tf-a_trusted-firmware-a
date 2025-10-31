# ICB (Interconnect Bus) Driver Makefile

# ICB Driver Sources - Only files needed for qti_icb_error_init
ICB_SOURCES :=  drivers/qti/icb/common/noc_error.c		\
		drivers/qti/icb/${CHIPSET}/noc_error_target.c	\
		drivers/qti/icb/${CHIPSET}/noc_error_data.c	\
		drivers/qti/icb/${CHIPSET}/noc_error_oem_data.c

# Conditionally add NOC decode source if enabled
ifeq (${ENABLE_NOC_DECODE},1)
ICB_SOURCES += drivers/qti/icb/${CHIPSET}/noc_error_decode.c
endif

# ICB Driver Includes
ICB_INCLUDES := -Idrivers/qti/icb/common	\
		-Idrivers/qti/icb/${CHIPSET}	\
		-Iinclude/drivers/qti/icb

# Add ICB includes to PLAT_INCLUDES
PLAT_INCLUDES += ${ICB_INCLUDES}

BL31_SOURCES += ${ICB_SOURCES}