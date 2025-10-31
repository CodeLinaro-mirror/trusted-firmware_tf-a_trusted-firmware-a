#ifndef __NOC_ERROR_H__
#define __NOC_ERROR_H__

/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#include "stdint.h"
#include "stdbool.h"
#include "qtiseclib_cb_interface.h"

#define CHIPINFO_VERSION(major, minor)  (((major) << 16) | (minor))

#define BIT_FLAG(x) ((uint32_t)(1 << x))

/* Register offset reserved value for non-applicable hardware versions */
#define REGISTER_NOT_APPLICABLE 0xFFFF

/* Interrupt vector reserved value for no interrupt registration */
#define NO_INTERRUPT UINTPTR_MAX

/* Module definitions */
#define MODULE_BASE_RSVD                 0
#define DEF_MODULE(ID)                   (MODULE_BASE_RSVD + ID)
#define KRNL                             DEF_MODULE(0x07)

/* Error Code Bit Field Definitions */
#define EC_MSK_NEGATIVE_BIT_SHFT         (31)
#define EC_MSK_RSVD_BIT_SHFT             (30)
#define EC_MSK_MODULE_BIT_SHFT           (20)

/* Error Code Masks */
#define EC_MSK_NEGATIVE_MSK              (UINT32_C(0x1) << EC_MSK_NEGATIVE_BIT_SHFT)
#define EC_MSK_RSVD_MSK                  (UINT32_C(0x1) << EC_MSK_RSVD_BIT_SHFT)
#define EC_MSK_MODULE_MSK                (UINT32_C(0x3FF) << EC_MSK_MODULE_BIT_SHFT)
#define EC_MSK_CODE_MSK                  (0xFFFFF)

/* TFA Error Code Construction Macro */
#define TFA_ERR_CODE(EC_MODULE, CODE) \
	(((EC_MSK_MODULE_MSK & (EC_MODULE << EC_MSK_MODULE_BIT_SHFT)) | \
	  (EC_MSK_CODE_MSK & (CODE))) | \
	 (EC_MSK_RSVD_MSK & (MODULE_BASE_RSVD << EC_MSK_RSVD_BIT_SHFT)))

/* NOC Error Code Definitions */
/*
 * NOC_NAME: %s ERRLOG0_LOW = 0x%08x, ERRLOG0_HIGH = 0x%08x,
 * ERRLOG1_LOW = 0x%08x, ERRLOG1_HIGH = 0x%08x,
 * ERRLOG2_LOW = 0x%08x, ERRLOG2_HIGH = 0x%08x,
 * ERRLOG3_LOW = 0x%08x, ERRLOG3_HIGH = 0x%08x,
 * ERRLOG2_1_LOW = 0x%08x, ERRLOG2_1_HIGH = 0x%08x,
 * ERRLOG4_3_LOW = 0x%08x, ERRLOG4_3_HIGH = 0x%08x,
 * ERRLOG6_5_LOW = 0x%08x, ERRLOG6_5_HIGH = 0x%08x,
 * ERRLOG8_HIGH = 0x%08x
 */
#define NOC_ERR_FATAL_SYNDROME_REG       TFA_ERR_CODE(KRNL, 572)

/*
 * NOC_NAME: %s SBM:%d, FAULTINSTATUS0_LOW = 0x%08x,
 * FAULTINSTATUS1_HIGH = 0x%08x, FAULTINSTATUS1_LOW = 0x%08x,
 * FAULTINSTATUS1_HIGH = 0x%08x
 */
#define NOC_FAULT_NAME_SBMS              TFA_ERR_CODE(KRNL, 573)

/*
 * NOC_NAME: %s POS:%x, ERRLOG_LOW = 0x%08x, ERRLOG_HIGH = 0x%08x
 */
#define NOC_POS_NAME_SYNDROME_REG        TFA_ERR_CODE(KRNL, 628)

/*
 * NOC_NAME: %s POC:%x, ERRLOGSTATUS_LOW = 0x%08x,
 * ERRLOGMAIN_LOW = 0x%08x, ERRLOGMAIN_HIGH = 0x%08x,
 * ERRLOGADDR_LOW = 0x%08x, ERRLOGADDR_HIGH = 0x%08x,
 * ERRLOGUSER_LOW = 0x%08x, ERRLOGUSER_HIGH = 0x%08x
 */
#define NOC_POC_NAME_SYNDROME_REG        TFA_ERR_CODE(KRNL, 913)

/*
 * NOC_NAME: %s SFTY:%x, CFTLA_LOW = 0x%08x, CFLTA_HIGH = 0x%08x,
 * UFLTA_LOW = 0x%08x, UFLTA_HIGH = 0x%08x
 */
#define NOC_SFTY_NAME_SYNDROME_REG       TFA_ERR_CODE(KRNL, 914)

/* NOC Register Interface */
typedef struct {
	uint16_t swid_low;
	uint16_t swid_high;
	uint16_t main_ctl_low;
	uint16_t err_valid_low;
	uint16_t err_clear_low;
	uint16_t errlog0_low;
	uint16_t errlog0_high;
	uint16_t errlog1_low;
	uint16_t errlog1_high;
	uint16_t errlog2_low;
	uint16_t errlog2_high;
	uint16_t errlog3_low;
	uint16_t errlog3_high;
	/* Gladiator NoC error fields */
	uint16_t errlog2_1_low;
	uint16_t errlog2_1_high;
	uint16_t errlog4_3_low;
	uint16_t errlog4_3_high;
	uint16_t errlog6_5_low;
	uint16_t errlog6_5_high;
	uint16_t errlog8_high;
} noc_hw_type;

/* NOC SidebandManager Register Interface */
typedef struct {
	uint16_t swid_low;
	uint16_t swid_high;
	uint16_t faultin_en0_low;
	uint16_t faultin_en0_high;
	uint16_t faultin_status0_low;
	uint16_t faultin_status0_high;
	uint16_t faultin_en1_low;
	uint16_t faultin_en1_high;
	uint16_t faultin_status1_low;
	uint16_t faultin_status1_high;
} noc_sideband_hw_type;

/* NOC Point-Of-Serialization Register Interface */
typedef struct {
	uint16_t swid_low;
	uint16_t swid_high;
	uint16_t errlog_low;
	uint16_t errlog_high;
	uint16_t errlogclr_low;
} noc_pos_hw_type;

/* NOC Point-Of-Coherency Register Interface */
typedef struct {
	uint16_t swid_low;
	uint16_t swid_high;
	uint16_t errset_low;
	uint16_t errstatus_low;
	uint16_t errack_low;
	uint16_t errlogmain_low;
	uint16_t errlogmain_high;
	uint16_t errlogaddr_low;
	uint16_t errlogaddr_high;
	uint16_t errloguser_low;
	uint16_t errloguser_high;
	/* New PoC version fields */
} noc_poc_hw_type;

/* NOC Error Syndrome Register Data Types */
typedef struct {
	uint32_t FAULTINSTATUS0_LOW;
	uint32_t FAULTINSTATUS0_HIGH;
	uint32_t FAULTINSTATUS1_LOW;
	uint32_t FAULTINSTATUS1_HIGH;
} nocerr_sbm_syndrome_type;

typedef struct {
	uint32_t ERRLOG_LOW;
	uint32_t ERRLOG_HIGH;
} nocerr_pos_syndrome_type;

typedef struct {
	uint32_t ERRLOGSTATUS_LOW;
	uint32_t ERRLOGMAIN_LOW;
	uint32_t ERRLOGMAIN_HIGH;
	uint32_t ERRLOGADDR_LOW;
	uint32_t ERRLOGADDR_HIGH;
	uint32_t ERRLOGUSER_LOW;
	uint32_t ERRLOGUSER_HIGH;
} nocerr_poc_syndrome_type;

/* Safety Manager Register Data Types */
typedef struct {
	uint32_t STATUS_LOW;
	uint32_t CFLTA_LOW;
	uint32_t CFLTA_HIGH;
	uint32_t UFLTA_LOW;
	uint32_t UFLTA_HIGH;
} nocerr_sfty_ctl_syndrome_type;

typedef struct {
	uint32_t STATUS_LOW;
	uint32_t CCNT_LOW;
	uint32_t UCNT_LOW;
} nocerr_sfty_agg_syndrome_type;

typedef struct {
	uint32_t ERRLOG0_LOW;
	uint32_t ERRLOG0_HIGH;
	uint32_t ERRLOG1_LOW;
	uint32_t ERRLOG1_HIGH;
	uint32_t ERRLOG2_LOW;
	uint32_t ERRLOG2_HIGH;
	uint32_t ERRLOG3_LOW;
	uint32_t ERRLOG3_HIGH;
	uint32_t ERRLOG2_1_LOW;
	uint32_t ERRLOG2_1_HIGH;
	uint32_t ERRLOG4_3_LOW;
	uint32_t ERRLOG4_3_HIGH;
	uint32_t ERRLOG6_5_LOW;
	uint32_t ERRLOG6_5_HIGH;
	uint32_t ERRLOG8_HIGH;
	nocerr_sbm_syndrome_type *sbms;
	nocerr_pos_syndrome_type *pos;
	nocerr_poc_syndrome_type *poc;
	nocerr_sfty_ctl_syndrome_type *sfty_ctl;
	nocerr_sfty_agg_syndrome_type *sfty_agg;
} nocerr_syndrome_type;

/* Safety Manager Register Interface */
typedef struct {
	uint16_t swid_low;
	uint16_t swid_high;
	uint16_t bistctl_low;
	uint16_t status_low;
	uint16_t outen_low;
	uint16_t cflta_low;
	uint16_t cflta_high;
	uint16_t uflta_low;
	uint16_t uflta_high;
} noc_sfty_ctl_hw_type;

typedef struct {
	uint16_t swid_low;
	uint16_t swid_high;
	uint16_t mainctl_low;
	uint16_t status_low;
	uint16_t ccnt_low;
	uint16_t ucnt_low;
} noc_sfty_agg_hw_type;

typedef struct {
	void                 *addr;      /* NOC SFTY_CTL base addresses */
	noc_sfty_agg_hw_type *hw;        /* NOC SFTY_CTL register offsets */
	uint32_t              aggregid;  /* NOC SFTY_CTL AggregID for BIST */
} noc_sfty_agg_type;

/* Internal NOC error Info data type */
typedef struct {
	char                  *name;            /* NOC name */
	noc_hw_type           *hw;              /* NOC register offsets */
	void                  *base_addr;       /* NOC base address */
	uintptr_t              intr_vector;     /* NOC interrupt vector */
	uint32_t               num_sbms;        /* Number of NoC sideband managers */

	noc_sideband_hw_type **sb_hw;           /* NOC sideband register offsets */
	void                 **sb_base_addrs;   /* NOC sideband base addresses */
	uint32_t               num_tos;         /* Number of timeout enable registers */
	void                 **to_addrs;        /* NOC timeout enable registers */
	nocerr_syndrome_type   syndrome;        /* Error syndrome */
	uint32_t               num_pos;         /* Number of PoS blocks. */

	noc_pos_hw_type      **pos_hw;          /* NOC PoS register offsets */
	void                 **pos_base_addrs;  /* NOC PoS base addresses */
	uint32_t               num_poc;         /* Number of PoC blocks. */

	noc_poc_hw_type      **poc_hw;          /* NOC PoC register offsets */
	void                 **poc_base_addrs;  /* NOC PoC base addresses */
	uint32_t               num_sfty_ctl;    /* Number of safety controllers */

	noc_sfty_ctl_hw_type **sfty_ctl_hw;     /* NOC SFTY_CTL register offsets */
	void                 **sfty_ctl_addrs;  /* NOC SFTY_CTL base addresses */
	uint32_t              *num_sfty_aggs;   /* Number of safety aggregators per ctl */

	noc_sfty_agg_type    **sfty_aggs;       /* NOC SFT_CTL aggregators per ctl*/
} nocerr_info_type;

/* OEM NOC error Info data type */
typedef struct {
	uint32_t faultin_en0_low;
	uint32_t faultin_en0_high;
	uint32_t faultin_en1_low;
	uint32_t faultin_en1_high;
} nocerr_sbm_info_type_oem;

typedef struct {
	bool enable;
} nocerr_pos_info_type_oem;

typedef struct {
	uint32_t outen_low;
} nocerr_sfty_ctl_info_type_oem;

typedef struct {
	char                          *name;         /* NOC name */
	bool                           intr_enable;  /* NOC int enable */
	bool                           error_fatal;  /* NOC int fatal */
	nocerr_sbm_info_type_oem      *sbms;         /* NOC SBM cfgs */
	nocerr_sbm_info_type_oem      *obs_mask;     /* NOC SBM obs cfg */
	uint32_t                      *to_reg_vals;  /* NOC TO reg vals */
	nocerr_pos_info_type_oem      *pos;          /* NOC PoS enables */
	nocerr_pos_info_type_oem      *poc;          /* NOC PoC enables */
	nocerr_sfty_ctl_info_type_oem *sfty_ctl;     /* NOC Safety cfg */
} nocerr_info_type_oem;

/* Filter types */
typedef struct {
	uint32_t  num_extids;   /* Number of ExtIDs */
	uint32_t *extids;       /* Array of ExtIDs */
	uint32_t  num_errcodes; /* Number of errcodes */
	uint32_t *errcodes;     /* Array of errcodes */
	bool      non_fatal;    /* Non-fatal NoC errors flag */
	bool      delay_fatal;  /* Delayed fatal errors flag */
} nocerr_filter_type;

typedef struct {
	bool enable;      /* Enable this filter */
	bool delay_fatal; /* Enable delayed fatal */
} nocerr_filter_type_oem;

typedef struct {
	/* Chip version information for this device data. */
	uint32_t              family;
	bool                  match;
	uint32_t              version;

	/* Associated target data */
	uint32_t          len;             /* cfgdata array length */
	nocerr_info_type *noc_info_list;   /* Internal cfgdata array */
	uint32_t          num_clock_regs;  /* External timeout clock regs count */
	void            **clock_reg_addrs; /* External timeout clock regs */
	uint32_t          num_filters;     /* Error filters count */
	nocerr_filter_type *filters;         /* Array of error filters */

	/* Register/fuse configuration */
	uint8_t  *reg_addr; /* Register or fuse */
	uint32_t  reg_mask; /* Register mask */
	uint32_t  reg_val;  /* Register value */
} nocerr_propdata_type;

typedef struct {
	uint32_t              num_configs;
	nocerr_propdata_type *configs;
} nocerr_config_info_type;

/* NOC Error OEM Property Data type */
typedef struct {
	/* Chip version information for this device data. */
	uint32_t               family;
	bool                   match;
	uint32_t               version;

	/* Associated target data */
	uint32_t              len;            /* cfgdata array length */
	nocerr_info_type_oem *noc_info_oem_list;   /* OEM cfgdata */
	uint32_t             *clock_reg_vals; /* NOC TO ext reg vals */
	nocerr_filter_type_oem *filters;        /* Error filters (OEM) */

	uint8_t  *reg_addr; /* Register or fuse */
	uint32_t  reg_mask; /* Register mask */
	uint32_t  reg_val;  /* Register value */
} nocerr_propdata_type_oem;

typedef struct {
	uint32_t                  num_configs;
	nocerr_propdata_type_oem *configs;
} nocerr_config_info_type_oem;

#endif /* __NOC_ERROR_H__ */
