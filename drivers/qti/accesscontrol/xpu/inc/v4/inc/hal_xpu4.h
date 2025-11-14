/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef HAL_XPU4_H
#define HAL_XPU4_H

#include <qti_xpu_err.h>

#include "ac_xpu.h"
#include "ac_xpu_target_info.h"

#define APP_QAD_RETAIN_BIT		0x1

#define ENABLE				1
#define XPU_DISABLED			0
#define XPU_ENABLED			1
#define BITMASK32			0xFFFFFFFFUL
#define BITMASK16			0xFFFFUL
#define RG_PERM_R			1
#define RG_PERM_W			2
#define DEVPRGRWBITSHFT			16
#define DEVPRGRWBITMASK			0xFFFF0000UL

#define INC_NO_OVERFLOW(c)		do { if ((c) + 1 > (c)) (c)++; } while (0)

#define XPU4_REV(major, minor, step)	((major << HWIO_XPU4_REV_MAJOR_SHFT) | \
					 (minor << HWIO_XPU4_REV_MINOR_SHFT) | \
					 (step  << HWIO_XPU4_REV_STEP_SHFT))

typedef uint32_t ac_xpu4_qad_vector;
typedef uint32_t ac_rg_ignored;

/*
 * XPU Error Syndrome Structure
 * Contains all XPU violation syndrome register values for comprehensive error reporting
 */
typedef struct {
	uint32_t esr;		/* Error Status Register */
	uint32_t synar0;	/* Syndrome Address Register 0 */
	uint32_t synar1;	/* Syndrome Address Register 1 */
	uint32_t synr0;		/* Syndrome Register 0 */
	uint32_t synr1;		/* Syndrome Register 1 */
	uint32_t synr2;		/* Syndrome Register 2 */
} ac_xpu_error_syndrome;

/*
 * Resource group configuration for MPUs
 */
typedef struct {
	uint64_t start;		/* Start of the partition. */
	uint64_t end;		/* End of the partition, not included in the range */
	uint32_t rg_num;	/* Resource group number */
} ac_mpu_rg_dev_prog;

/*
 * Resource group configuration for APUs and RPUs
 */
typedef struct {
	uint32_t rg_num;	/* Resource group number */
} ac_rpu_rg_dev_prog;

/*
 * Resource group configuration for MPUs
 */
typedef struct {
	uint64_t start;		/* Start of the partition. */
	uint64_t end;		/* End of the partition, not included in the range */
	/*
	 * Configuration flags to determine the applicability of this configuration
	 * based on device state and target platform.
	 */
	uint32_t profile_flags;

	uint32_t read_qads;	/* Bitmap of QAD's able to read this partition. */
	uint32_t write_qads;	/* Bitmap of QAD's able to write this partition. */
	uint16_t rg_num;	/* Resource group number */
} ac_mpu_rg;

/*
 * Resource group configuration for APUs and RPUs
 */
typedef struct {
	/*
	 * Configuration flags to determine the applicability of this configuration
	 * based on device state and target platform.
	 */
	uint32_t profile_flags;
	uint32_t read_qads;	/* Bitmap of QAD's able to read this partition. */
	uint32_t write_qads;	/* Bitmap of QAD's able to write this partition. */
	uint16_t rg_num;	/* Resource group number */
} ac_rpu_rg;

/*
 * XPU global configuration details
 */
typedef struct {
	/* Physical base address of the XPU. */
	uint32_t base_addr;
	/*
	 * Configuration flags to determine the applicability of this configuration
	 * based on device state and target platform.
	 */
	uint32_t profile_flags;

	/* XPU id. @see \c HAL_xpu_XPUType. */
	uint32_t xpu_id;

	/* Decides whether to enable or disable the xPU */
	uint16_t status;

	/* Number of configured resource groups. */
	uint16_t nrg;

	union {
		const void *raw;
		const ac_rpu_rg *rpu;
		const ac_mpu_rg *mpu;
	} rg;
#ifdef xPU_4_2_SUPPORT
	/*
	 * QAD which has permissions to the XPU's config registers
	 *   - If left as 0, APNSPE controls permissions to config registers
	 *   - Otherwise, the QAD set will have exclusive permissions to config space
	 */
	uint32_t cfg_owner;

	/* Unmapped regions permissions (MPU only) */
	uint32_t umr_perm;
	/* Array of values to write out to ignore XPRESSCFG for individual RGs */
	const ac_rg_ignored *rg_ignored;
#endif
} ac_xpu_cfg;

/*
 * Dynamic xPU configuration details
 */
typedef struct {
	uint32_t rg_start;
	uint32_t rg_count;
	uint32_t *shadow_read_perm;
	uint32_t *shadow_write_perm;
} ac_xpu_dynamic_rgs;

/*
 * xPU details maintaining HW IDR/Rev values
 * Provides support for SOC to CPU view address translation as well
 */
typedef struct {
	const ac_xpu_dynamic_rgs *dyn_rgs;
	uint32_t soc_addr;	/* absolute SOC address */
	uint32_t addr;		/* address from CPU view */
	uint32_t xpu_id;
	uint32_t idr[3];
	uint32_t rev;
} ac_xpu4_priv_info;

/* XPU static config record details */
typedef struct {
	uint64_t start;
	uint64_t end;
	uint32_t rg_num;
	uint32_t rg_profile_flags;
	uint32_t read_qads;
	uint32_t write_qads;
} ac_xpu_rg_static_config_t;

/* XPU4 IDs/ types - To keep backward compatible same name is used */
typedef enum {
	HAL_XPU2_APCS_MPU,				/* 0 */
	HAL_XPU2_CRYPTO0_BAM,
	HAL_XPU2_DCC,
	HAL_XPU2_IPA_0_GSI_TOP,
	HAL_XPU2_IPA,
	HAL_XPU2_MCCC_APU,
	HAL_XPU2_MMSS,
	HAL_XPU2_MMSS_CC,
	HAL_XPU2_BAM_BLSP1_DMA,
	HAL_XPU2_BAM_BLSP2_DMA,
	HAL_XPU2_SKL,					/* 10 */
	HAL_XPU2_SP_SCSR,
	HAL_XPU2_SSC_BLSP_BAM,
	HAL_XPU2_TLMM_XPU_EAST,
	HAL_XPU2_TLMM_XPU_NORTH,
	HAL_XPU2_TLMM_XPU_WEST,
	HAL_XPU2_UFS_ICE,
	HAL_XPU2_MNOC,
	HAL_XPU2_GCC_RPU,
	HAL_XPU2_TCSR_MUTEX,
	HAL_XPU2_TCSR_REGS,				/* 20 */
	HAL_XPU2_BIMC_APU,
	HAL_XPU2_MCCC_APU2,
	HAL_XPU2_MPM2,
	HAL_XPU2_IMEM_APU,
	HAL_XPU2_RAMBLUR_PIMEM_APU,
	HAL_XPU2_SEC_CTRL_APU,
	HAL_XPU2_SPDM_APU,
	HAL_XPU2_ANOC1_MPU,
	HAL_XPU2_ANOC2_MPU,
	HAL_XPU2_BOOT_ROM,				/* 30 */
	HAL_XPU2_CNOC_A1NOC_MPU,
	HAL_XPU2_CNOC_A2NOC_MPU,
	HAL_XPU2_CNOC_QDSS_MPU,
	HAL_XPU2_CNOC_SNOC_MPU,
	HAL_XPU2_CNOC_SNOC_MS_MPU,
	HAL_XPU2_LPASS_Q6SS_MPU,
	HAL_XPU2_MSS_MPU,
	HAL_XPU2_MSS_Q6_MPU,
	HAL_XPU2_IMEM_MPU,
	HAL_XPU2_PMIC_ARB,				/* 40 */
	HAL_XPU2_QM_MPU_CFG,
	HAL_XPU2_RAMBLUR_PIMEM_MPU,
	HAL_XPU2_RPM_MSTR_MPU,
	HAL_XPU2_CFG_SSC,
	HAL_XPU2_SSC_Q6_MPU,
	HAL_XPU2_BIMC_MPU0,				/* BIMC DDR0 */
	HAL_XPU2_BIMC_MPU1,				/* BIMC DDR1 */
	HAL_XPU2_TLMM,					/* This is not supported on 8998 */
	HAL_XPU2_MSS_NAV_MPU,
	HAL_XPU2_LPASS_CFG_MPU,				/* 50 */
	HAL_XPU2_TURING_Q6_MPU,
	HAL_XPU2_LPASS_SENSOR_BLSP_BAM,
	HAL_XPU2_TLMM_XPU_CENTER,
	HAL_XPU2_TLMM_XPU_SOUTH,
	HAL_XPU2_SDC1_SDCC_ICE,
	HAL_XPU2_TITAN_SS_APU,
	HAL_XPU2_CNOC_AOSS_MPU,
	HAL_XPU2_LLCC_BROADCAST_MPU,
	HAL_XPU2_BIMC_BROADCAST_MPU = HAL_XPU2_LLCC_BROADCAST_MPU,
	HAL_XPU2_SSC_SDC_MPU,
	HAL_DC_NOC_BROADCAST_MPU,			/* 60 */
	HAL_DC_NOC_NON_BROADCAST_MPU,
	HAL_AOSS_MPU,
	HAL_XPU2_BIMC_MPU2,
	HAL_XPU2_BIMC_MPU3,
	HAL_MEMNOC_MS_MPU,
	HAL_SNOC_IPA_MS_MPU,
	HAL_XPU2_PKA_APU,
	HAL_XPU2_QPIC_APU,
	HAL_XPU2_SPMI_FETHCER,
	HAL_XPU2_Q6_TCM_MPU,				/* 70 */
	HAL_XPU2_QPIC_MPU,
	HAL_XPU2_LPASS_SSC_Q6_MPU,
	HAL_XPU2_LPASS_SSC_SDC_MPU,
	HAL_XPU2_LPASS_SSC_BLSP_BAM,
	HAL_XPU2_GEMNOC_MS_MPU,
	HAL_XPU2_BAM_BLSP3_DMA,
	HAL_XPU2_DC_NOC_SHRM_MPU,
	HAL_XPU2_BOOT_IMEM_SS_MPU,
	HAL_XPU2_UFS_G4_ICE,
	HAL_XPU2_DRE_MPU,				/* 80 */
	HAL_XPU2_BIMC_MPU4,
	HAL_XPU2_BIMC_MPU5,
	HAL_XPU2_BIMC_MPU6,
	HAL_XPU2_BIMC_MPU7,
	HAL_XPU2_WCSS_MPU,
	HAL_XPU2_QSPI_APU,
	HAL_XPU2_LPASS_RXTX_MPU,
	HAL_XPU2_LPASS_WSA_MPU,
	HAL_XPU2_LPASS_VA_MPU,
	HAL_XPU2_IPC_MPU,				/* 90 */
	HAL_XPU2_AOP_MS_MPU,
	HAL_XPU2_MDSS_XPU,
	HAL_XPU2_MDSP_MPU,
	HAL_XPU2_MAPSS_APU,
	HAL_XPU2_SNOC_MS_MPU,
	HAL_XPU2_GEMNOC_MDSP_MS_MPU,
	HAL_XPU2_GEMNOC_MCDMA_MS_MPU,
	HAL_XPU2_GIC_MS_MPU,
	HAL_XPU2_CRYPTO0_CRYPTO_KM_APU,
	HAL_XPU2_HWKM_CFG_KM_APU,			/* 100 */
	HAL_XPU2_MSS_CRYPTO_KM_APU,
	HAL_XPU2_UFS_CARD_ICE_KM_APU,
	HAL_XPU2_UFS_MEM_ICE_KM_APU,
	HAL_XPU2_CAMSS,
	HAL_XPU2_LPASS_XPU_AUD,
	HAL_XPU2_LPASS_XPU_AUD_SB,
	HAL_XPU2_SDC1_SDCC_ICE_KM_APU,
	HAL_XPU2_MDSS0,
	HAL_XPU2_MDSS1,
	HAL_XPU2_CNOC_SMSS_MPU,				/* 110 */
	HAL_XPU2_SAFETY_MGR_SMSS_M_MPU,
	HAL_XPU2_SAFETY_MGR_SMSS_S_MPU,
	HAL_XPU2_SAFETY_MGR_SMSS_QUP,
	HAL_XPU2_AON_CAMERA_MPU,
	HAL_XPU2_AOSS_AOPSS_MPU,
	HAL_XPU2_AOSS_ICBM_MPU,
	HAL_XPU2_AOSS_IPCC_MPU,
	HAL_XPU2_AOSS_PERPH_MPU,
	HAL_XPU2_AOSS_RPMH_MPU,
	HAL_XPU2_CNOC_DAP_MPU,				/* 120 */
	HAL_XPU2_CNOC_DCC_MPU,
	HAL_XPU2_GEMNOC_PCIE_MPU,
	HAL_XPU2_LPASS_DEBUG_IP_MPU,
	HAL_XPU2_LPASS_WSA2_DMA_MPU,
	HAL_XPU2_PRNG_APU,
	HAL_XPU2_SSC_MCC_MPU,
	HAL_XPU2_SSC_QUP_MPU,
	HAL_XPU2_TME_CNOC_MPU,
	HAL_XPU2_TME_QFPROM_APU,
	HAL_XPU2_LLCC_AND_BROADCAST_MPU,		/* 130 */
	HAL_XPU2_LLCC_OR_BROADCAST_MPU,
	HAL_XPU2_CRYPTO0_BAM_RA,
	HAL_XPU2_IPA_RA,
	HAL_XPU2_GIC_MPU_RA,
	HAL_XPU2_WPSS_MS_MPU,
	HAL_XPU2_LPASS_ENPU_MPU,
	HAL_XPU2_LPASS_VA_SEQ_MPU,
	HAL_XPU2_ENPU_CAMERA_MPU,
	HAL_XPU2_ENPU_AUDIO_MPU,
	HAL_XPU2_LLCC_SCID_MPU0,			/* 140 */
	HAL_XPU2_LLCC_SCID_MPU1,
	HAL_XPU2_LLCC_SCID_MPU2,
	HAL_XPU2_LLCC_SCID_MPU3,
	HAL_XPU2_LLCC_OR_SCID_MPU,
	HAL_XPU2_LLCC_AND_SCID_MPU,
	HAL_XPU2_LPASS_DSP_AHBM_MPU,
	HAL_XPU2_LPASS_AOC_MPU,
	HAL_XPU2_LPASS_SDC_MPU,
	HAL_XPU2_LPASS_CORE_SSC_MPU,
	HAL_XPU2_LPASS_LPIAON_MPU,			/* 150 */
	HAL_XPU2_TITAN_RT1_MPU,
	HAL_XPU2_TITAN_RT2_MPU,
	HAL_XPU2_CNOC_SNOC3_MPU,
	HAL_XPU2_TSCSS_MPU,
	HAL_XPU2_MODEM_MS_MPU,
	HAL_XPU2_XM_DAP_MS_MPU,
	HAL_XPU2_ECPRI_APU,
	HAL_XPU2_ECPRI_GSI_0_APU = HAL_XPU2_ECPRI_APU,
	HAL_XPU2_ECPRI_CLK_RPU,
	HAL_XPU2_NOC_IPA_RA,
	HAL_XPU2_PCIE_RSCC_RA_MPU,			/* 160 */
	HAL_XPU2_MVMSS_CNOC_MPU,
	HAL_XPU2_BAM_BLSP4_DMA,
	HAL_XPU2_AP_UBWCP_MPU,
	HAL_XPU2_LPASS_ENPU_SCH_MPU,
	HAL_XPU2_LLCC_SCID_MPU4,
	HAL_XPU2_LLCC_SCID_MPU5,
	HAL_XPU2_LLCC_SCID_MPU6,
	HAL_XPU2_LLCC_SCID_MPU7,
	HAL_XPU2_APSS_QSS_NSINOC_DEBUG,
	HAL_XPU2_APSS_QSS_NSINOC_MPU,			/* 170 */
	HAL_XPU2_APSS_XS_GIC,
	HAL_XPU2_APSS_QHS_CPUCP,
	HAL_XPU2_APSS_QHS_NSINW,
	HAL_XPU2_APSS_QHS_PERIPH,
	HAL_XPU2_APSS_QNS_NSINOC,
	HAL_XPU2_APSS_QHS_SRVC_NTW,
	HAL_XPU2_APSS_PS_APSS,
	HAL_XPU2_APSS_XM_CE0_RA,
	HAL_XPU2_APSS_XM_CE1_RA,
	HAL_XPU2_APSS_XM_CE2_RA,			/* 180 */
	HAL_XPU2_APSS_HM_APSS_XM_CPUCP_RA,
	HAL_XPU2_ECPRI_GSI_1_APU,
	HAL_XPU2_ECPRI_GSI_2_APU,
	HAL_XPU2_MM_RSCC_MPU,
	HAL_XPU2_PRBDMA_MPU,
	HAL_XPU2_RSC_CRM_MPU,
	HAL_XPU2_CMSR_MPU_REGS_XPU,
	HAL_XPU2_SOCCP_DEBUG_MS_MPU,
	HAL_XPU2_SOCCP_CNOC_SLV_MPU,
	HAL_XPU2_GCC_NE_RPU,				/* 190 */
	HAL_XPU2_GCC_NW_RPU,
	HAL_XPU2_GCC_SE_RPU,
	HAL_XPU2_CNOC_D2D_CONTROL_MPU,
	HAL_XPU2_BIMC_AND_GLB_MPU,
	HAL_XPU2_LLCC_SCID_OR_GLB_MPU,
	HAL_XPU2_PM_APMB_DEBUG_MPU,
	HAL_XPU2_CPUSS_NOC_MPU,
	HAL_XPU2_CNOC_HIGH_BW_MPU,
	HAL_XPU2_QHM_CNOC_AOSS_MPU,
	HAL_XPU2_CRYPTO1_BAM,				/* 200 */
	HAL_XPU2_CRYPTO1_BAM_RA,
	HAL_XPU2_CRYPTO1_CRYPTO_KM_APU,
	HAL_XPU2_CRYPTO2_BAM,
	HAL_XPU2_CRYPTO2_BAM_RA,
	HAL_XPU2_CRYPTO2_CRYPTO_KM_APU,
	HAL_XPU2_CNOC_SAIL_MPU,
	HAL_XPU2_ETH_MS_MPU,
	HAL_XPU2_R52_MS_MPU,
	HAL_XPU2_DMA_APU,
	HAL_XPU2_DPRX_0_APU,				/* 210 */
	HAL_XPU2_DPRX_1_APU,
	HAL_XPU2_HPASS_AUDIO_DP0_MPU,
	HAL_XPU2_HPASS_AUDIO_DP1_MPU,
	HAL_XPU2_HPASS_AUDIO_QXM_DDR_MPU,
	HAL_XPU2_HPASS_AUDIO_QXM_TCM_MPU,
	HAL_XPU2_LLCC_SCID_AND_GLB_MPU,
	HAL_XPU2_BAM_BLSP0_DMA,
	HAL_XPU2_BIMC_OR_GLB_MPU,
	HAL_XPU2_PCIE_DMA_0_MPU,
	HAL_XPU2_PCIE_DMA_1_MPU,			/* 220 */
	HAL_XPU2_PCIE_DMA_2_MPU,
	HAL_XPU2_PM_DEBUG_MPU,
	HAL_XPU2_HPASS_DSP0_MPU,
	HAL_XPU2_HPASS_DSP1_MPU,
	HAL_XPU2_HPASS_DSP2_MPU,
	HAL_XPU2_HPASS_DSP0_0_MPU,
	HAL_XPU2_HPASS_DSP0_1_MPU,
	HAL_XPU2_HPASS_DSP1_0_MPU,
	HAL_XPU2_HPASS_DSP1_1_MPU,
	HAL_XPU2_HPASS_DSP2_0_MPU,			/* 230 */
	HAL_XPU2_HPASS_DSP2_1_MPU,
	HAL_XPU2_QHS_BCAST_GLB_AND_MPU,
	HAL_XPU2_QHS_BCAST_GLB_OR_MPU,
	HAL_XPU2_APSS_QHS_CLOCK_MPU,
	HAL_XPU2_QHS_NON_BCAST_GLB_AND_MPU,
	HAL_XPU2_QHS_NON_BCAST_GLB_OR_MPU,
	HAL_XPU2_MDSS_RA,
	HAL_XPU2_MDSS0_RA,
	HAL_XPU2_MDSS1_RA,
	HAL_XPU2_HPASS_CRM_XPU,				/* 240 */
	HAL_XPU2_APSS_NOC_MPU,
	HAL_XPU2_HPASS_COFIG_NOC_MPU,
	HAL_XPU2_SOCCP_CNOC_MST_MPU,
	HAL_XPU2_LPASS_0_LPMLA_MPU,
	HAL_XPU2_LPASS_1_LPMLA_MPU,
	HAL_XPU2_DCP_RVCP_SLV_MPU,
	HAL_XPU2_QNM_CNOC_MPU,
	HAL_XPU2_QPACE_MPU,
	HAL_XPU2_LPASS_CORE_SSC_1_MPU,
	HAL_XPU2_LPASS_SSC_BLSP_1_BAM,			/* 250 */
	HAL_XPU2_QUPV3_1_MPU,
	HAL_XPU2_QUPV3_2_MPU,
	HAL_XPU2_QUPV3_3_MPU,
	HAL_XPU2_QUPV3_4_MPU,
	HAL_XPU2_LLCC_CH02_AND_MPU,
	HAL_XPU2_LLCC_CH02_AND_SCID_MPU,
	HAL_XPU2_LLCC_CH02_OR_MPU,
	HAL_XPU2_LLCC_CH02_OR_SCID_MPU,
	HAL_XPU2_LLCC_CH13_AND_MPU,
	HAL_XPU2_LLCC_CH13_AND_SCID_MPU,		/* 260 */
	HAL_XPU2_LLCC_CH13_OR_MPU,
	HAL_XPU2_LLCC_CH13_OR_SCID_MPU,
	HAL_XPU2_MDSS_XPU_RA_MPU,
	HAL_XPU2_PM_DEBUG_ER_MPU,
	HAL_XPU2_QNM_CNOC_ER_MPU,
	HAL_XPU2_SCC_MPU,
	HAL_XPU2_LPASS_SSC_BLSP1_BAM,
	HAL_XPU2_APSS_QHS_NCCCP_MPU,
	HAL_XPU2_APSS_QNM_APSS_MPU,
	HAL_XPU2_APSS_QNM_CPUSS_MPU,			/* 270 */
	HAL_XPU2_APSS_QSS_CPUSS_MPU,
	HAL_XPU2_CNOC_HSCNOC_MPU,
	HAL_XPU2_CNOC_HSCNOC_PCIE_EAST_MPU,
	HAL_XPU2_CNOC_HSCNOC_PCIE_WEST_MPU,
	HAL_XPU2_CNOC_QHM_AOSS_MPU,
	HAL_XPU2_LPASS_RXTX_SDCA_MPU,
	HAL_XPU2_LPASS_WSA_SDCA_MPU,
	HAL_XPU2_OOBMS_CRYPTO_BAM_XPU_ER_APU,
	HAL_XPU2_OOBMS_CRYPTO_BAM_XPU_RA_APU,
	HAL_XPU2_OOBMS_CRYPTO_KM_APU,			/* 280 */
	HAL_XPU2_OOBMS_OOBMSS_ANOC_SS_MPU,
	HAL_XPU2_OOBMS_OOBMSS_CNOC_MS_MPU,
	HAL_XPU2_OOBMS_OOBMSS_INTERNAL_SS_MPU,
	HAL_XPU2_PCIE_3B_ICE_KM_APU,
	HAL_XPU2_PCIE_3B_ICE_APU,
	HAL_XPU2_PCIE_3B_APU,
	HAL_XPU2_PCIE_5_ICE_KM_APU,
	HAL_XPU2_PCIE_5_ICE_APU,
	HAL_XPU2_PCIE_5_APU,
	HAL_XPU2_WPSS_MPU_CFG_XPU4,			/* 290 */
	HAL_XPU2_WPSS_MPU,
	HAL_XPU2_TITAN_RT3_MPU,
	HAL_XPU2_LPASS_SSC_BLSP_BAM_MPU,
	HAL_XPU2_DC_NOC_SHRM_1_MPU,
	HAL_XPU2_DC_NOC_SHRM_2_MPU,
	HAL_XPU2_DC_NOC_SHRM_3_MPU,
	HAL_XPU2_DDR_SLICE_QHS_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE_QHS_NON_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE_QHM_SHRM_MPU,
	HAL_XPU2_DDR_SLICE_CH0_LLCC_SCID_MPU,		/* 300 */
	HAL_XPU2_DDR_SLICE_CH1_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE_CH2_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE_CH3_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE_CH0_LLCC_MPU,
	HAL_XPU2_DDR_SLICE_CH1_LLCC_MPU,
	HAL_XPU2_DDR_SLICE_CH2_LLCC_MPU,
	HAL_XPU2_DDR_SLICE_CH3_LLCC_MPU,
	HAL_XPU2_DDR_SLICE0_QHS_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE1_QHS_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE2_QHS_BROADCAST_MPU,		/* 310 */
	HAL_XPU2_DDR_SLICE3_QHS_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE0_QHS_NON_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE1_QHS_NON_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE2_QHS_NON_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE3_QHS_NON_BROADCAST_MPU,
	HAL_XPU2_DDR_SLICE0_CH0_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE1_CH0_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE2_CH0_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE3_CH0_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE0_CH0_BIMC_MPU,		/* 320 */
	HAL_XPU2_DDR_SLICE1_CH0_BIMC_MPU,
	HAL_XPU2_DDR_SLICE2_CH0_BIMC_MPU,
	HAL_XPU2_DDR_SLICE3_CH0_BIMC_MPU,
	HAL_XPU2_DDR_SLICE0_CH1_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE1_CH1_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE2_CH1_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE3_CH1_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE0_CH1_BIMC_MPU,
	HAL_XPU2_DDR_SLICE1_CH1_BIMC_MPU,
	HAL_XPU2_DDR_SLICE2_CH1_BIMC_MPU,		/* 330 */
	HAL_XPU2_DDR_SLICE3_CH1_BIMC_MPU,
	HAL_XPU2_DDR_SLICE0_CH2_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE1_CH2_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE2_CH2_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE3_CH2_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE0_CH2_BIMC_MPU,
	HAL_XPU2_DDR_SLICE1_CH2_BIMC_MPU,
	HAL_XPU2_DDR_SLICE2_CH2_BIMC_MPU,
	HAL_XPU2_DDR_SLICE3_CH2_BIMC_MPU,
	HAL_XPU2_DDR_SLICE0_CH3_LLCC_SCID_MPU,		/* 340 */
	HAL_XPU2_DDR_SLICE1_CH3_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE2_CH3_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE3_CH3_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE0_CH3_BIMC_MPU,
	HAL_XPU2_DDR_SLICE1_CH3_BIMC_MPU,
	HAL_XPU2_DDR_SLICE2_CH3_BIMC_MPU,
	HAL_XPU2_DDR_SLICE3_CH3_BIMC_MPU,
	HAL_XPU2_DDR_SLICE0_LLCC_AND_BCAST_LLCC_MPU,
	HAL_XPU2_DDR_SLICE0_LLCC_AND_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE0_LLCC_OR_BCAST_LLCC_MPU,	/* 350 */
	HAL_XPU2_DDR_SLICE0_LLCC_OR_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE1_LLCC_AND_BCAST_LLCC_MPU,
	HAL_XPU2_DDR_SLICE1_LLCC_AND_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE1_LLCC_OR_BCAST_LLCC_MPU,
	HAL_XPU2_DDR_SLICE1_LLCC_OR_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE2_LLCC_AND_BCAST_LLCC_MPU,
	HAL_XPU2_DDR_SLICE2_LLCC_AND_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE2_LLCC_OR_BCAST_LLCC_MPU,
	HAL_XPU2_DDR_SLICE2_LLCC_OR_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE3_LLCC_AND_BCAST_LLCC_MPU,	/* 360 */
	HAL_XPU2_DDR_SLICE3_LLCC_AND_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_DDR_SLICE3_LLCC_OR_BCAST_LLCC_MPU,
	HAL_XPU2_DDR_SLICE3_LLCC_OR_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_QHM_HW_DSC_MPU_ER,
	HAL_XPU2_GIC_NOC_EPS_MPU_RA,
	HAL_XPU2_AWMP_MPU_ER,
	HAL_XPU2_XM_AWM_MPU_ER,
	HAL_XPU2_LPASS_CRYPTO_BAM_APU_ER,
	HAL_XPU2_LPASS_CRYPTO_BAM_APU_RA,
	HAL_XPU2_CRYPTO_KM_APU_ER,			/* 370 */
	HAL_XPU2_LPASS_QHS_GPU_LCL_MPU_ER,
	HAL_XPU2_LPASS_QHS_GPU_M0_MPU_ER,
	HAL_XPU2_LPASS_QHS_GPU_M1_MPU_ER,
	HAL_XPU2_LPASS_QHS_OSPI_DMA_MPU_ER,
	HAL_XPU2_SWMP_MPU_ER,
	HAL_XPU2_LPASS_WDSS_ER_MPU,
	HAL_XPU2_SLICE_0_MACH9_0_LLCC_MPU,
	HAL_XPU2_SLICE_0_MACH9_0_LLCC_SCID,
	HAL_XPU2_SLICE_0_MACH9_1_LLCC_MPU,
	HAL_XPU2_SLICE_0_MACH9_1_LLCC_SCID,		/* 380 */
	HAL_XPU2_SLICE_0_MACH9_2_LLCC_MPU,
	HAL_XPU2_SLICE_0_MACH9_2_LLCC_SCID,
	HAL_XPU2_SLICE_0_MACH9_3_LLCC_MPU,
	HAL_XPU2_SLICE_0_MACH9_3_LLCC_SCID,
	HAL_XPU2_SLICE_1_MACH9_0_LLCC_MPU,
	HAL_XPU2_SLICE_1_MACH9_0_LLCC_SCID,
	HAL_XPU2_SLICE_1_MACH9_1_LLCC_MPU,
	HAL_XPU2_SLICE_1_MACH9_1_LLCC_SCID,
	HAL_XPU2_SLICE_1_MACH9_2_LLCC_MPU,
	HAL_XPU2_SLICE_1_MACH9_2_LLCC_SCID,		/* 390 */
	HAL_XPU2_SLICE_1_MACH9_3_LLCC_MPU,
	HAL_XPU2_SLICE_1_MACH9_3_LLCC_SCID,
	HAL_XPU2_SLICE_2_MACH9_0_LLCC_MPU,
	HAL_XPU2_SLICE_2_MACH9_0_LLCC_SCID,
	HAL_XPU2_SLICE_2_MACH9_1_LLCC_MPU,
	HAL_XPU2_SLICE_2_MACH9_1_LLCC_SCID,
	HAL_XPU2_SLICE_2_MACH9_2_LLCC_MPU,
	HAL_XPU2_SLICE_2_MACH9_2_LLCC_SCID,
	HAL_XPU2_SLICE_2_MACH9_3_LLCC_MPU,
	HAL_XPU2_SLICE_2_MACH9_3_LLCC_SCID,		/* 400 */
	HAL_XPU2_SLICE_3_MACH9_0_LLCC_MPU,
	HAL_XPU2_SLICE_3_MACH9_0_LLCC_SCID,
	HAL_XPU2_SLICE_3_MACH9_1_LLCC_MPU,
	HAL_XPU2_SLICE_3_MACH9_1_LLCC_SCID,
	HAL_XPU2_SLICE_3_MACH9_2_LLCC_MPU,
	HAL_XPU2_SLICE_3_MACH9_2_LLCC_SCID,
	HAL_XPU2_SLICE_3_MACH9_3_LLCC_MPU,
	HAL_XPU2_SLICE_3_MACH9_3_LLCC_SCID,
	HAL_XPU2_SLICE_0_QHS_BROADCAST_MPU,
	HAL_XPU2_SLICE_0_QHS_NON_BROADCAST_MPU,		/* 410 */
	HAL_XPU2_SLICE_0_LLCC_AND_BCAST_LLCC_MPU,
	HAL_XPU2_SLICE_0_LLCC_AND_BCAST_LLCC_SCID,
	HAL_XPU2_SLICE_0_LLCC_OR_BCAST_LLCC_MPU,
	HAL_XPU2_SLICE_0_LLCC_OR_BCAST_LLCC_SCID,
	HAL_XPU2_SLICE_1_QHS_BROADCAST_MPU,
	HAL_XPU2_SLICE_1_QHS_NON_BROADCAST_MPU,
	HAL_XPU2_SLICE_1_LLCC_AND_BCAST_LLCC_MPU,
	HAL_XPU2_SLICE_1_LLCC_AND_BCAST_LLCC_SCID,
	HAL_XPU2_SLICE_1_LLCC_OR_BCAST_LLCC_MPU,
	HAL_XPU2_SLICE_1_LLCC_OR_BCAST_LLCC_SCID,	/* 420 */
	HAL_XPU2_SLICE_2_QHS_BROADCAST_MPU,
	HAL_XPU2_SLICE_2_QHS_NON_BROADCAST_MPU,
	HAL_XPU2_SLICE_2_LLCC_AND_BCAST_LLCC_MPU,
	HAL_XPU2_SLICE_2_LLCC_AND_BCAST_LLCC_SCID,
	HAL_XPU2_SLICE_2_LLCC_OR_BCAST_LLCC_MPU,
	HAL_XPU2_SLICE_2_LLCC_OR_BCAST_LLCC_SCID,
	HAL_XPU2_SLICE_3_QHS_BROADCAST_MPU,
	HAL_XPU2_SLICE_3_QHS_NON_BROADCAST_MPU,
	HAL_XPU2_SLICE_3_LLCC_AND_BCAST_LLCC_MPU,
	HAL_XPU2_SLICE_3_LLCC_AND_BCAST_LLCC_SCID,	/* 430 */
	HAL_XPU2_SLICE_3_LLCC_OR_BCAST_LLCC_MPU,
	HAL_XPU2_SLICE_3_LLCC_OR_BCAST_LLCC_SCID,
	HAL_XPU2_CNOC_QECP_MPU,
	HAL_XPU2_LPASS_AG_NOC_MPU,
	HAL_XPU2_LPASS_AUD_QH_MPU,
	HAL_XPU2_LPASS_AUD_QX_MPU,
	HAL_XPU2_LPASS_DEBUG_APBM_MPU,
	HAL_XPU2_LPASS_PRBDMA_MPU,
	HAL_XPU2_LPASS_RSC_CRM_MPU,
	HAL_XPU2_MACHX_0_LLCC_MPU,			/* 440 */
	HAL_XPU2_MACHX_0_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_1_LLCC_MPU,
	HAL_XPU2_MACHX_1_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_2_LLCC_MPU,
	HAL_XPU2_MACHX_2_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_3_LLCC_MPU,
	HAL_XPU2_MACHX_3_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_AND_BCAST_LLCC_MPU,
	HAL_XPU2_MACHX_AND_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_CH02_AND_BCAST_LLCC_MPU,		/* 450 */
	HAL_XPU2_MACHX_CH02_AND_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_CH02_OR_BCAST_LLCC_MPU,
	HAL_XPU2_MACHX_CH02_OR_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_CH13_AND_BCAST_LLCC_MPU,
	HAL_XPU2_MACHX_CH13_AND_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_CH13_OR_BCAST_LLCC_MPU,
	HAL_XPU2_MACHX_CH13_OR_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_MACHX_OR_BCAST_LLCC_MPU,
	HAL_XPU2_MACHX_OR_BCAST_LLCC_SCID_MPU,
	HAL_XPU2_QECP_RVCPLP_SLV_MPU,			/* 460 */
	HAL_XPU2_SSC_SFE_MPU,
	HAL_XPU2_TITAN_RA_MPU,

	HAL_XPU2_MPM2_MPM_APU,
	HAL_XPU2_QDSS_CFG_DCC_DCC,
	HAL_XPU2_QUPV3_5_GSI_TOP,

	HAL_XPU2_CNOC_CMN_PLL_CFG_MPU,
	HAL_XPU2_CNOC_LPASS_CFG_MPU,
	HAL_XPU2_CNOC_NOC_CFG_MPU,
	HAL_XPU2_CNOC_NSSNOC_MPU,
	HAL_XPU2_CNOC_PCIE_CFG_MPU,
	HAL_XPU2_CNOC_PROCESSOR_CFG_MPU,
	HAL_XPU2_CNOC_QDSS_CFG_MPU,
	HAL_XPU2_CNOC_SDCC_CFG_MPU,
	HAL_XPU2_CNOC_TCU_CFG_MPU,
	HAL_XPU2_CNOC_USBCTL_CFG_MPU,
	HAL_XPU2_CNOC_USBPHY_USB2_CORE_PHY_CFG_MPU,
	HAL_XPU2_GEMNOC_CFG_MPU,
	HAL_XPU2_MACHX_LLCC_MPU,
	HAL_XPU2_MACHX_LLCC_SCID_MPU,
	HAL_XPU2_QMIP_XPU_CFG,
	HAL_XPU2_XS_PCIE_1LANE_0_MPU,
	HAL_XPU2_XS_PCIE_1LANE_1_MPU,
	HAL_XPU2_XS_PCIE_2LANE_0_MPU,
	HAL_XPU2_XS_PCIE_2LANE_1_MPU,
	HAL_XPU2_XS_PCIE_2LANE_2_MPU,

	HAL_XPU2_COUNT,
	HAL_XPU2_SIZE = 0x7FFFFFFF /* force to 32-bit enum */
} HAL_xpu2_XPU2Type;

typedef enum {
	AC_MPU = 0,
	AC_APU = 1,
	AC_RPU = 2,
} ACXpuVariant;

/*
 * Fetch the IDR information from xPU and cache in SW
 *
 * @param xpu_info	structure holding xPU ID/address for which data has to be
 *			cached in SW (into same struture)
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_init_xpu(ac_xpu4_priv_info *xpu_info);

/*
 * API to enable APNSPE
 *
 * @param xpu_info	xPU info structure for which configuration has to be done
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_configure_apnspe(ac_xpu4_priv_info *xpu_info);

/*
 * Access control initialization.
 *
 * @pre xPU Clocks has to be inited
 *
 * @param milestone	Milestone call in which API is invoked
 * @param xpu_info	xPU info structure for which static configuration has to be done
 * @param xpu_cfg	Static configuration which has to be applied for this xPU
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_apply_static_config(ac_milestone milestone,
				     ac_xpu4_priv_info *xpu_info,
				     ac_xpu_cfg *xpu_cfg);

/*
 * API to protect/lock region in xPU
 *
 * @details xPU protect region based on start/size, xPU is picked for programming
 *          It is determined whether it has to be programmed in xPU RG (or)
 *          Locking has to be done based Start/End/Permission matches
 *          1. If start/end doesn't match with any RG then new free RG is
 *             programmed/locked with details provided
 *          2. If start/end match with permissions for already programmed RG
 *             then locking is done and config verified
 *          3. If start/end(0x2000-0x3000) overlap with RG-A(0x1000--0x3000)
 *             and there is adjacent RG-B(0x1000--0x2000)
 *             then locking done in RG-A and unlocking done in RG-B
 *
 * @pre Clocks for xPU protecting the client address space has to be enabled
 *
 * @param xpu_info		xPU info structure for which configuration has to be done
 * @param start_addr		start address which has to be protected
 *				Has to be aligned with MPU width size
 * @param size			Size of region which has to be protected (alignment same as above)
 * @param wowp			Write protection to be enabled for the configuration
 * @param sd_perm_array		Base address of Array structure holding input Secure Domain ID/VMID,
 *				R/W Permission, Lock details
 * @param count			Number of elements passed in ac_sec_domain_perm field
 * @param merge			Merge flag decides whether address has to be merged if
 *				consecutive with other address range with same permissions
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_protect_region(ac_xpu4_priv_info *xpu_info,
				uint64_t start,
				uint64_t size,
				bool wowp,
				ac_sec_domain_perm *sd_perm_array,
				uint32_t count,
				bool merge);

/*
 * API to unlock/unprotect the region in xPU
 *
 * @details xPU picked up based on start/size
 *          It is determined whether it has to be unlocked or it has to be disabled or both
 *          If RG start/end matches exactly (or) falls completely within the address range passed
 *          then based on lock details passed unlock is done and RG is tried for disablement
 *          (might not if some QAD holds the lock). If no QAD lock is passed then RG tried for
 *          disablement
 *
 * @param xpu_info		xPU info structure for which configuration has to be done
 * @param start_addr		start address which has to be unprotected
 *				Has to be aligned with MPU width size
 * @param size			Size of region which has to be unprotected (alignment same as above)
 * @param sd_array		Base address of array holding the AC secure domain ID
 *				(APP-S or APP-NS or both) for which unlock has to be done
 * @param count			Number of elements passed in sdUnlockQAD array
 * @param blanket		Blanket has to be set TRUE for unlocking complete range
 *				spun across multiple RGs.
 *				Checks the static RGs as well.
 *				If Blanket is FALSE looks for exact match in single RG.
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_unprotect_region(ac_xpu4_priv_info *xpu_info,
				  uint64_t start,
				  uint64_t size,
				  ac_sec_domain_id *sd_array,
				  uint32_t count,
				  bool blanket);

/*
 * API to enable or disable error reporting
 *
 * @param xpu_info	xPU info structure for which configuration has to be done
 * @param sd_array	Base address of array holding the AC secure domain ID
 *			(APP-S or APP-NS or both) for which error reporting has to be disabled
 * @param count		Number of elements passed in sdUnlockQAD array
 * @param enable	Field to identify error has to be enabled or disabled
 *
 * @return Success/error otherwise
 *
 * @details From start/end address, xPU is identified and error reporting is enabled or
 *          disabled for that xPU. It has to be noted error reporting is at xPU level,
 *          which means if particular address is passed for this API in IMEM, then it disables
 *          error reporting for the whole IMEM MPU.
 */
AC_ERROR ac_xpu_configure_client_error_reporting(ac_xpu4_priv_info *xpu_info,
						  ac_sec_domain_id *sd_array,
						  uint32_t count,
						  bool enable);

#ifdef xPU_4_2_SUPPORT
/*
 * API to set unmapped regions permmsions (xPU 4.2 feature)
 *
 * @param xpu_info	xPU info structure for which configuration has to be done
 * @param xpu_cfg	Static configuration which has to be applied for this xPU
 *
 * @return Success/error otherwise
 *
 * @details This API sets the access permssions of tyhe unmapped xPU regions in QAD format
 *          The unmapped regions are the regions that are not covered by any exsiting RG
 */
AC_ERROR ac_xpu_set_unmapped_region_perms(ac_xpu4_priv_info *xpu_info,
					   const ac_xpu_cfg *xpu_cfg);

/*
 * API to set the config owner of the xPU (xPU 4.2 feature)
 *
 * @param xpu_info	xPU info structure for which configuration has to be done
 * @param xpu_cfg	Static configuration which has to be applied for this xPU
 *
 * @return Success/error otherwise
 *
 * @details This API sets the CONFIG OWNER in QAD format. CONFIG OWNER is the QAD
 *          that will do all of the configurations for the RG's
 */
AC_ERROR ac_xpu_set_cfg_owner(ac_xpu4_priv_info *xpu_info,
			       const ac_xpu_cfg *xpu_cfg);

/*
 * API to apply the express config for the xPU (xPU 4.2 feature)
 *
 * @param xpu_info	xPU info structure for which configuration has to be done
 * @param xpu_cfg	Static configuration which has to be applied for this xPU
 *
 * @return Success/error otherwise
 *
 * @details This API will set the approprate bits to apply the xpress config for this
 *          xPU. If any overrides are needed, this function also applies these policies
 */
AC_ERROR ac_xpu_apply_xpress_config(ac_xpu4_priv_info *xpu_info,
				     const ac_xpu_cfg *xpu_cfg);

/*
 * API to Parse the xPU configuration data into data structure
 * used by HW HAL layer
 *
 * @param milestone	Milestone call in which API is invoked
 * @param xpu_cfg	XPU Static configuration which has to be applied for this xPU
 * @param index		RG number for which static config has to be updated
 * @param isMpu		specifies the xpu type APU/RPU/MPU
 * @param rgCfg		XPU RG static configurations
 */
void ac_xpu4_get_rg_static_rg_config(ac_milestone milestone,
				      ac_xpu_cfg *xpu_cfg,
				      uint32_t index,
				      bool isMpu,
				      ac_xpu_rg_static_config_t *rgCfg);

/*
 * API to clear the APNSPE bit of the XPU
 *
 * @param xpu_info	xPU info structure for which static configuration has to be done
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_clear_apnspe(ac_xpu4_priv_info *xpu_info);

/*
 * API to clear the lock bit of current EE
 *
 * @param xpu_info	xPU info structure for which static configuration has to be done
 * @param rg_num	RG Number for which unlock should be done
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_unprotect_by_rg(ac_xpu4_priv_info *xpu_info, uint32_t rg_num);

/*
 * API to Update CfgOwner with given QAD
 *
 * @param xpu_info	xPU info structure for which static configuration has to be done
 * @param cfgOwner	CFG owner
 *
 * @return Success/error otherwise
 */
AC_ERROR ac_xpu_update_cfg_owner(ac_xpu4_priv_info *xpu_info,
				  const ac_xpu4_qad_vector cfgOwner);

/*
 * API to Read CfgOwner with given Xpuinfo
 *
 * @return QAD vector
 */
ac_xpu4_qad_vector ac_xpu_read_cfg_owner(ac_xpu4_priv_info *xpu_info);

#endif /* xPU_4_2_SUPPORT */

/*
 * Read XPU error syndrome registers
 * Ported from qtiseclib HALxPU4.c ACXpuReadSyndrome function
 */
void ac_xpu_read_syndrome(ac_xpu4_priv_info *xpu_info, void *syndrome);

/*
 * Log XPU violation details with comprehensive syndrome information
 * Ported from qtiseclib HALxPU4.c ACXpuLogViolation function
 */
void ac_xpu_log_violation(ac_xpu4_priv_info *xpu_info);

/*
 * Clear XPU violation status registers
 * Ported from qtiseclib HALxPU4.c ACXpuClearViolation function
 */
void ac_xpu_clear_violation(ac_xpu4_priv_info *xpu_info);

/*
 * Append silent log for XPU violation
 * Ported from qtiseclib HALxPU4.c ACXpuAppendSilentLog function
 */
void ac_xpu_append_silent_log(ac_xpu4_priv_info *xpu_info);

#endif /* HAL_XPU4_H */
