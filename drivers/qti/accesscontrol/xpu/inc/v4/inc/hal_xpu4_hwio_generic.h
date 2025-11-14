/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: ISC
 */

#ifndef HALXPU4HWIOGENERIC_H
#define HALXPU4HWIOGENERIC_H

#define HWIO_XPU4_IDR0_ADDR(x)					((x) + 0x0)
#define HWIO_XPU4_IDR0_OFFS					(0x0)
#define HWIO_XPU4_IDR0_RMSK					0x3ff007f
#define HWIO_XPU4_IDR0_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR0_ADDR(x), HWIO_XPU4_IDR0_RMSK)
#define HWIO_XPU4_IDR0_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR0_ADDR(x), m)
#define HWIO_XPU4_IDR0_NRG_BMSK					0x3ff0000
#define HWIO_XPU4_IDR0_NRG_SHFT					16
#define HWIO_XPU4_IDR0_BLED_BMSK				0x40
#define HWIO_XPU4_IDR0_BLED_SHFT				6
#define HWIO_XPU4_IDR0_CLIENT_HALTREQACK_EN_BMSK		0x20
#define HWIO_XPU4_IDR0_CLIENT_HALTREQACK_EN_SHFT		5
#define HWIO_XPU4_IDR0_CLIENT_PIPELINE_EN_BMSK			0x10
#define HWIO_XPU4_IDR0_CLIENT_PIPELINE_EN_SHFT			4
#define HWIO_XPU4_IDR0_XPRESSCFGMULTIDIE_BMSK			0x8
#define HWIO_XPU4_IDR0_XPRESSCFGMULTIDIE_SHFT			3
#define HWIO_XPU4_IDR0_XPRESSCFGEN_BMSK				0x4
#define HWIO_XPU4_IDR0_XPRESSCFGEN_SHFT				2
#define HWIO_XPU4_IDR0_XPU_TYPE_BMSK				0x3
#define HWIO_XPU4_IDR0_XPU_TYPE_SHFT				0

#define HWIO_XPU4_IDR1_ADDR(x)					((x) + 0x4)
#define HWIO_XPU4_IDR1_OFFS					(0x4)
#define HWIO_XPU4_IDR1_RMSK					0x3f3f3f3f
#define HWIO_XPU4_IDR1_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR1_ADDR(x), HWIO_XPU4_IDR1_RMSK)
#define HWIO_XPU4_IDR1_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR1_ADDR(x), m)
#define HWIO_XPU4_IDR1_CLIENT_ADDR_WIDTH_BMSK			0x3f000000
#define HWIO_XPU4_IDR1_CLIENT_ADDR_WIDTH_SHFT			24
#define HWIO_XPU4_IDR1_CONFIG_ADDR_WIDTH_BMSK			0x3f0000
#define HWIO_XPU4_IDR1_CONFIG_ADDR_WIDTH_SHFT			16
#define HWIO_XPU4_IDR1_ADDR_MSB_BMSK				0x3f00
#define HWIO_XPU4_IDR1_ADDR_MSB_SHFT				8
#define HWIO_XPU4_IDR1_ADDR_LSB_BMSK				0x3f
#define HWIO_XPU4_IDR1_ADDR_LSB_SHFT				0

#define HWIO_XPU4_IDR2_ADDR(x)					((x) + 0x8)
#define HWIO_XPU4_IDR2_OFFS					(0x8)
#define HWIO_XPU4_IDR2_RMSK					0x3f1f
#define HWIO_XPU4_IDR2_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR2_ADDR(x), HWIO_XPU4_IDR2_RMSK)
#define HWIO_XPU4_IDR2_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR2_ADDR(x), m)
#define HWIO_XPU4_IDR2_SYNCMODEEN_BMSK				0x2000
#define HWIO_XPU4_IDR2_SYNCMODEEN_SHFT				13
#define HWIO_XPU4_IDR2_PARITYEN_BMSK				0x1000
#define HWIO_XPU4_IDR2_PARITYEN_SHFT				12
#define HWIO_XPU4_IDR2_USEQSICFGINTF_BMSK			0x800
#define HWIO_XPU4_IDR2_USEQSICFGINTF_SHFT			11
#define HWIO_XPU4_IDR2_USEQSICLIENTINTF_BMSK			0x400
#define HWIO_XPU4_IDR2_USEQSICLIENTINTF_SHFT			10
#define HWIO_XPU4_IDR2_USEAHBWRAPPER_BMSK			0x200
#define HWIO_XPU4_IDR2_USEAHBWRAPPER_SHFT			9
#define HWIO_XPU4_IDR2_USELEGACYINTF_BMSK			0x100
#define HWIO_XPU4_IDR2_USELEGACYINTF_SHFT			8
#define HWIO_XPU4_IDR2_NQAD_BMSK				0x1f
#define HWIO_XPU4_IDR2_NQAD_SHFT				0

#define HWIO_XPU4_REV_ADDR(x)					((x) + 0xc)
#define HWIO_XPU4_REV_OFFS					(0xc)
#define HWIO_XPU4_REV_RMSK					0xffffffff
#define HWIO_XPU4_REV_IN(x)		\
	in_dword_masked(HWIO_XPU4_REV_ADDR(x), HWIO_XPU4_REV_RMSK)
#define HWIO_XPU4_REV_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_REV_ADDR(x), m)
#define HWIO_XPU4_REV_MAJOR_BMSK				0xf0000000
#define HWIO_XPU4_REV_MAJOR_SHFT				28
#define HWIO_XPU4_REV_MINOR_BMSK				0xfff0000
#define HWIO_XPU4_REV_MINOR_SHFT				16
#define HWIO_XPU4_REV_STEP_BMSK					0xffff
#define HWIO_XPU4_REV_STEP_SHFT					0

#define HWIO_XPU4_GCR_ADDR(x)					((x) + 0x100)
#define HWIO_XPU4_GCR_OFFS					(0x100)
#define HWIO_XPU4_GCR_RMSK					0x1
#define HWIO_XPU4_GCR_IN(x)		\
	in_dword_masked(HWIO_XPU4_GCR_ADDR(x), HWIO_XPU4_GCR_RMSK)
#define HWIO_XPU4_GCR_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_GCR_ADDR(x), m)
#define HWIO_XPU4_GCR_OUT(x, v)		\
	out_dword(HWIO_XPU4_GCR_ADDR(x), v)
#define HWIO_XPU4_GCR_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_GCR_ADDR(x), m, v, HWIO_XPU4_GCR_IN(x))
#define HWIO_XPU4_GCR_APNSPE_BMSK				0x1
#define HWIO_XPU4_GCR_APNSPE_SHFT				0

#define HWIO_XPU4_RSR_ADDR(x)					((x) + 0x104)
#define HWIO_XPU4_RSR_OFFS					(0x104)
#define HWIO_XPU4_RSR_RMSK					0xffffffff
#define HWIO_XPU4_RSR_IN(x)		\
	in_dword_masked(HWIO_XPU4_RSR_ADDR(x), HWIO_XPU4_RSR_RMSK)
#define HWIO_XPU4_RSR_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_RSR_ADDR(x), m)
#define HWIO_XPU4_RSR_NXTFRG_BMSK				0xffff0000
#define HWIO_XPU4_RSR_NXTFRG_SHFT				16
#define HWIO_XPU4_RSR_NUMFRG_BMSK				0xffff
#define HWIO_XPU4_RSR_NUMFRG_SHFT				0

#define HWIO_XPU4_IDR3_ADDR(x)					((x) + 0x120)
#define HWIO_XPU4_IDR3_OFFS					(0x120)
#define HWIO_XPU4_IDR3_RMSK					0xffffffff
#define HWIO_XPU4_IDR3_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR3_ADDR(x), HWIO_XPU4_IDR3_RMSK)
#define HWIO_XPU4_IDR3_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR3_ADDR(x), m)
#define HWIO_XPU4_IDR3_XPRESSQADEN_BMSK				0xffffffff
#define HWIO_XPU4_IDR3_XPRESSQADEN_SHFT				0

#define HWIO_XPU4_IDR4_ADDR(x)					((x) + 0x124)
#define HWIO_XPU4_IDR4_OFFS					(0x124)
#define HWIO_XPU4_IDR4_RMSK					0xffffffff
#define HWIO_XPU4_IDR4_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR4_ADDR(x), HWIO_XPU4_IDR4_RMSK)
#define HWIO_XPU4_IDR4_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR4_ADDR(x), m)
#define HWIO_XPU4_IDR4_SYNDQADEN_BMSK				0xffffffff
#define HWIO_XPU4_IDR4_SYNDQADEN_SHFT				0

#define HWIO_XPU4_IDR5_ADDR(x)					((x) + 0x128)
#define HWIO_XPU4_IDR5_OFFS					(0x128)
#define HWIO_XPU4_IDR5_RMSK					0x11f
#define HWIO_XPU4_IDR5_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR5_ADDR(x), HWIO_XPU4_IDR5_RMSK)
#define HWIO_XPU4_IDR5_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR5_ADDR(x), m)
#define HWIO_XPU4_IDR5_WDIEID_BMSK				0x100
#define HWIO_XPU4_IDR5_WDIEID_SHFT				8
#define HWIO_XPU4_IDR5_USEWAKEUPIDLE_BMSK			0x10
#define HWIO_XPU4_IDR5_USEWAKEUPIDLE_SHFT			4
#define HWIO_XPU4_IDR5_LOCKALIASEN_BMSK				0x8
#define HWIO_XPU4_IDR5_LOCKALIASEN_SHFT				3
#define HWIO_XPU4_IDR5_RECORDRGUPDEN_BMSK			0x4
#define HWIO_XPU4_IDR5_RECORDRGUPDEN_SHFT			2
#define HWIO_XPU4_IDR5_TMEERRCTRLEN_BMSK			0x2
#define HWIO_XPU4_IDR5_TMEERRCTRLEN_SHFT			1
#define HWIO_XPU4_IDR5_LOCKDELEGATEEN_BMSK			0x1
#define HWIO_XPU4_IDR5_LOCKDELEGATEEN_SHFT			0

#define HWIO_XPU4_IDR6_ADDR(x)					((x) + 0x12c)
#define HWIO_XPU4_IDR6_OFFS					(0x12c)
#define HWIO_XPU4_IDR6_RMSK					0xffffffff
#define HWIO_XPU4_IDR6_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR6_ADDR(x), HWIO_XPU4_IDR6_RMSK)
#define HWIO_XPU4_IDR6_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR6_ADDR(x), m)

#define HWIO_XPU4_IDR7_ADDR(x)					((x) + 0x130)
#define HWIO_XPU4_IDR7_OFFS					(0x130)
#define HWIO_XPU4_IDR7_RMSK					0xffffffff
#define HWIO_XPU4_IDR7_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR7_ADDR(x), HWIO_XPU4_IDR7_RMSK)
#define HWIO_XPU4_IDR7_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR7_ADDR(x), m)

#define HWIO_XPU4_IDR8_ADDR(x)					((x) + 0x134)
#define HWIO_XPU4_IDR8_OFFS					(0x134)
#define HWIO_XPU4_IDR8_RMSK					0xffffffff
#define HWIO_XPU4_IDR8_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR8_ADDR(x), HWIO_XPU4_IDR8_RMSK)
#define HWIO_XPU4_IDR8_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR8_ADDR(x), m)

#define HWIO_XPU4_IDR9_ADDR(x)					((x) + 0x138)
#define HWIO_XPU4_IDR9_OFFS					(0x138)
#define HWIO_XPU4_IDR9_RMSK					0xffffffff
#define HWIO_XPU4_IDR9_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR9_ADDR(x), HWIO_XPU4_IDR9_RMSK)
#define HWIO_XPU4_IDR9_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR9_ADDR(x), m)

#define HWIO_XPU4_IDR10_ADDR(x)					((x) + 0x13c)
#define HWIO_XPU4_IDR10_OFFS					(0x13c)
#define HWIO_XPU4_IDR10_RMSK					0xffffffff
#define HWIO_XPU4_IDR10_IN(x)		\
	in_dword_masked(HWIO_XPU4_IDR10_ADDR(x), HWIO_XPU4_IDR10_RMSK)
#define HWIO_XPU4_IDR10_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_IDR10_ADDR(x), m)

#define HWIO_XPU4_CFGERE_ADDR(x)				((x) + 0x208)
#define HWIO_XPU4_CFGERE_OFFS					(0x208)
#define HWIO_XPU4_CFGERE_RMSK					0xffffffff
#define HWIO_XPU4_CFGERE_IN(x)		\
	in_dword_masked(HWIO_XPU4_CFGERE_ADDR(x), HWIO_XPU4_CFGERE_RMSK)
#define HWIO_XPU4_CFGERE_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_CFGERE_ADDR(x), m)
#define HWIO_XPU4_CFGERE_OUT(x, v)		\
	out_dword(HWIO_XPU4_CFGERE_ADDR(x), v)
#define HWIO_XPU4_CFGERE_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_CFGERE_ADDR(x), m, v, \
			    HWIO_XPU4_CFGERE_IN(x))
#define HWIO_XPU4_CFGERE_CFGERE_S_BMSK				0x80000000
#define HWIO_XPU4_CFGERE_CFGERE_S_SHFT				31
#define HWIO_XPU4_CFGERE_CFGERE_NS_BMSK				0x40000000
#define HWIO_XPU4_CFGERE_CFGERE_NS_SHFT				30
#define HWIO_XPU4_CFGERE_CFGERE_QAD_BMSK			0xfff
#define HWIO_XPU4_CFGERE_CFGERE_QAD_SHFT			0

#define HWIO_XPU4_CLERE_ADDR(x)					((x) + 0x20c)
#define HWIO_XPU4_CLERE_OFFS					(0x20c)
#define HWIO_XPU4_CLERE_RMSK					0xffffffff
#define HWIO_XPU4_CLERE_IN(x)		\
	in_dword_masked(HWIO_XPU4_CLERE_ADDR(x), HWIO_XPU4_CLERE_RMSK)
#define HWIO_XPU4_CLERE_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_CLERE_ADDR(x), m)
#define HWIO_XPU4_CLERE_OUT(x, v)		\
	out_dword(HWIO_XPU4_CLERE_ADDR(x), v)
#define HWIO_XPU4_CLERE_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_CLERE_ADDR(x), m, v, \
			    HWIO_XPU4_CLERE_IN(x))
#define HWIO_XPU4_CLERE_CLERE_S_BMSK				0x80000000
#define HWIO_XPU4_CLERE_CLERE_S_SHFT				31
#define HWIO_XPU4_CLERE_CLERE_NS_BMSK				0x40000000
#define HWIO_XPU4_CLERE_CLERE_NS_SHFT				30
#define HWIO_XPU4_CLERE_CLERE_QAD_BMSK				0xfff
#define HWIO_XPU4_CLERE_CLERE_QAD_SHFT				0

#define HWIO_XPU4_DBGAR_ADDR(x)					((x) + 0x304)
#define HWIO_XPU4_DBGAR_OFFS					(0x304)
#define HWIO_XPU4_DBGAR_RMSK					0xffffffff
#define HWIO_XPU4_DBGAR_IN(x)		\
	in_dword_masked(HWIO_XPU4_DBGAR_ADDR(x), HWIO_XPU4_DBGAR_RMSK)
#define HWIO_XPU4_DBGAR_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_DBGAR_ADDR(x), m)
#define HWIO_XPU4_DBGAR_OUT(x, v)		\
	out_dword(HWIO_XPU4_DBGAR_ADDR(x), v)
#define HWIO_XPU4_DBGAR_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_DBGAR_ADDR(x), m, v, \
			    HWIO_XPU4_DBGAR_IN(x))
#define HWIO_XPU4_DBGAR_DBGA_S_BMSK				0x80000000
#define HWIO_XPU4_DBGAR_DBGA_S_SHFT				31
#define HWIO_XPU4_DBGAR_DBGA_NS_BMSK				0x40000000
#define HWIO_XPU4_DBGAR_DBGA_NS_SHFT				30
#define HWIO_XPU4_DBGAR_DBGA_QAD_BMSK				0xfff
#define HWIO_XPU4_DBGAR_DBGA_QAD_SHFT				0

#define HWIO_XPU4_GCR2_ADDR(x)					((x) + 0x400)
#define HWIO_XPU4_GCR2_OFFS					(0x400)
#define HWIO_XPU4_GCR2_RMSK					0x77
#define HWIO_XPU4_GCR2_IN(x)		\
	in_dword_masked(HWIO_XPU4_GCR2_ADDR(x), HWIO_XPU4_GCR2_RMSK)
#define HWIO_XPU4_GCR2_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_GCR2_ADDR(x), m)
#define HWIO_XPU4_GCR2_OUT(x, v)		\
	out_dword(HWIO_XPU4_GCR2_ADDR(x), v)
#define HWIO_XPU4_GCR2_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_GCR2_ADDR(x), m, v, HWIO_XPU4_GCR2_IN(x))
#define HWIO_XPU4_GCR2_SYN_CGC_EN_BMSK				0x40
#define HWIO_XPU4_GCR2_SYN_CGC_EN_SHFT				6
#define HWIO_XPU4_GCR2_CLPIPE_CGC_EN_BMSK			0x20
#define HWIO_XPU4_GCR2_CLPIPE_CGC_EN_SHFT			5
#define HWIO_XPU4_GCR2_RSR_CGC_EN_BMSK				0x10
#define HWIO_XPU4_GCR2_RSR_CGC_EN_SHFT				4
#define HWIO_XPU4_GCR2_ERROR_CLK_EN_BMSK			0x4
#define HWIO_XPU4_GCR2_ERROR_CLK_EN_SHFT			2
#define HWIO_XPU4_GCR2_DYNAMIC_CLK_EN_BMSK			0x2
#define HWIO_XPU4_GCR2_DYNAMIC_CLK_EN_SHFT			1
#define HWIO_XPU4_GCR2_RG_CFG_RD_AC_EN_BMSK			0x1
#define HWIO_XPU4_GCR2_RG_CFG_RD_AC_EN_SHFT			0

#define HWIO_XPU4_CFGOWNER_ADDR(x)				((x) + 0x404)
#define HWIO_XPU4_CFGOWNER_OFFS					(0x404)
#define HWIO_XPU4_CFGOWNER_RMSK					0x1f
#define HWIO_XPU4_CFGOWNER_IN(x)		\
	in_dword_masked(HWIO_XPU4_CFGOWNER_ADDR(x), HWIO_XPU4_CFGOWNER_RMSK)
#define HWIO_XPU4_CFGOWNER_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_CFGOWNER_ADDR(x), m)
#define HWIO_XPU4_CFGOWNER_OUT(x, v)		\
	out_dword(HWIO_XPU4_CFGOWNER_ADDR(x), v)
#define HWIO_XPU4_CFGOWNER_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_CFGOWNER_ADDR(x), m, v, \
			    HWIO_XPU4_CFGOWNER_IN(x))
#define HWIO_XPU4_CFGOWNER_CFG_QAD_BMSK				0x1f
#define HWIO_XPU4_CFGOWNER_CFG_QAD_SHFT				0

#define HWIO_XPU4_UMRPERMREG_ADDR(x)				((x) + 0x408)
#define HWIO_XPU4_UMRPERMREG_OFFS				(0x408)
#define HWIO_XPU4_UMRPERMREG_RMSK				0xffffffff
#define HWIO_XPU4_UMRPERMREG_IN(x)		\
	in_dword_masked(HWIO_XPU4_UMRPERMREG_ADDR(x), HWIO_XPU4_UMRPERMREG_RMSK)
#define HWIO_XPU4_UMRPERMREG_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_UMRPERMREG_ADDR(x), m)
#define HWIO_XPU4_UMRPERMREG_OUT(x, v)		\
	out_dword(HWIO_XPU4_UMRPERMREG_ADDR(x), v)
#define HWIO_XPU4_UMRPERMREG_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_UMRPERMREG_ADDR(x), m, v, \
			    HWIO_XPU4_UMRPERMREG_IN(x))
#define HWIO_XPU4_UMRPERMREG_UMR_S_BMSK				0x80000000
#define HWIO_XPU4_UMRPERMREG_UMR_S_SHFT				31
#define HWIO_XPU4_UMRPERMREG_UMR_NS_BMSK			0x40000000
#define HWIO_XPU4_UMRPERMREG_UMR_NS_SHFT			30
#define HWIO_XPU4_UMRPERMREG_UMR_QAD_BMSK			0xffffffff
#define HWIO_XPU4_UMRPERMREG_UMR_QAD_SHFT			0

#define HWIO_XPU4_RG_MODIFIEDr_ADDR(base, r)			((base) + 0X420 + (0x4*(r)))
#define HWIO_XPU4_RG_MODIFIEDr_OFFS(r)				(0X420 + (0x4*(r)))
#define HWIO_XPU4_RG_MODIFIEDr_RMSK				0xffffffff
#define HWIO_XPU4_RG_MODIFIEDr_MAXr				1
#define HWIO_XPU4_RG_MODIFIEDr_INI(base, r)	\
	in_dword_masked(HWIO_XPU4_RG_MODIFIEDr_ADDR(base, r), \
			HWIO_XPU4_RG_MODIFIEDr_RMSK)
#define HWIO_XPU4_RG_MODIFIEDr_INMI(base, r, mask)	\
	in_dword_masked(HWIO_XPU4_RG_MODIFIEDr_ADDR(base, r), mask)
#define HWIO_XPU4_RG_MODIFIEDr_MODIFIED_BMSK			0xffffffff
#define HWIO_XPU4_RG_MODIFIEDr_MODIFIED_SHFT			0

#define HWIO_XPU4_RG_MODIFIEDs_ADDR(base, s)			((base) + 0X420 + (0x4*(s)))
#define HWIO_XPU4_RG_MODIFIEDs_OFFS(s)				(0X420 + (0x4*(s)))
#define HWIO_XPU4_RG_MODIFIEDs_RMSK				0x3ffffff
#define HWIO_XPU4_RG_MODIFIEDs_MAXs				2
#define HWIO_XPU4_RG_MODIFIEDs_INI(base, s)	\
	in_dword_masked(HWIO_XPU4_RG_MODIFIEDs_ADDR(base, s), \
			HWIO_XPU4_RG_MODIFIEDs_RMSK)
#define HWIO_XPU4_RG_MODIFIEDs_INMI(base, s, mask)	\
	in_dword_masked(HWIO_XPU4_RG_MODIFIEDs_ADDR(base, s), mask)
#define HWIO_XPU4_RG_MODIFIEDs_MODIFIED_BMSK			0x3ffffff
#define HWIO_XPU4_RG_MODIFIEDs_MODIFIED_SHFT			0

#define HWIO_XPU4_ESR_ADDR(x)					((x) + 0x500)
#define HWIO_XPU4_ESR_OFFS					(0x500)
#define HWIO_XPU4_ESR_RMSK					0xf
#define HWIO_XPU4_ESR_IN(x)		\
	in_dword_masked(HWIO_XPU4_ESR_ADDR(x), HWIO_XPU4_ESR_RMSK)
#define HWIO_XPU4_ESR_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_ESR_ADDR(x), m)
#define HWIO_XPU4_ESR_OUT(x, v)		\
	out_dword(HWIO_XPU4_ESR_ADDR(x), v)
#define HWIO_XPU4_ESR_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_ESR_ADDR(x), m, v, HWIO_XPU4_ESR_IN(x))
#define HWIO_XPU4_ESR_CLMULTI_BMSK				0x8
#define HWIO_XPU4_ESR_CLMULTI_SHFT				3
#define HWIO_XPU4_ESR_CFGMULTI_BMSK				0x4
#define HWIO_XPU4_ESR_CFGMULTI_SHFT				2
#define HWIO_XPU4_ESR_CLERR_BMSK				0x2
#define HWIO_XPU4_ESR_CLERR_SHFT				1
#define HWIO_XPU4_ESR_CFGERR_BMSK				0x1
#define HWIO_XPU4_ESR_CFGERR_SHFT				0

#define HWIO_XPU4_SYNAR0_ADDR(x)				((x) + 0x504)
#define HWIO_XPU4_SYNAR0_OFFS					(0x504)
#define HWIO_XPU4_SYNAR0_RMSK					0xffffffff
#define HWIO_XPU4_SYNAR0_IN(x)		\
	in_dword_masked(HWIO_XPU4_SYNAR0_ADDR(x), HWIO_XPU4_SYNAR0_RMSK)
#define HWIO_XPU4_SYNAR0_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_SYNAR0_ADDR(x), m)
#define HWIO_XPU4_SYNAR0_SYNADDR_BMSK				0xffffffff
#define HWIO_XPU4_SYNAR0_SYNADDR_SHFT				0

#define HWIO_XPU4_SYNAR1_ADDR(x)				((x) + 0x508)
#define HWIO_XPU4_SYNAR1_OFFS					(0x508)
#define HWIO_XPU4_SYNAR1_RMSK					0xffffffff
#define HWIO_XPU4_SYNAR1_IN(x)		\
	in_dword_masked(HWIO_XPU4_SYNAR1_ADDR(x), HWIO_XPU4_SYNAR1_RMSK)
#define HWIO_XPU4_SYNAR1_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_SYNAR1_ADDR(x), m)
#define HWIO_XPU4_SYNAR1_SYNADDR_BMSK				0xffffffff
#define HWIO_XPU4_SYNAR1_SYNADDR_SHFT				0

#define HWIO_XPU4_SYNR0_ADDR(x)					((x) + 0x50c)
#define HWIO_XPU4_SYNR0_OFFS					(0x50c)
#define HWIO_XPU4_SYNR0_RMSK					0xffff9f7f
#define HWIO_XPU4_SYNR0_IN(x)		\
	in_dword_masked(HWIO_XPU4_SYNR0_ADDR(x), HWIO_XPU4_SYNR0_RMSK)
#define HWIO_XPU4_SYNR0_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_SYNR0_ADDR(x), m)
#define HWIO_XPU4_SYNR0_PH_BMSK					0x80000000
#define HWIO_XPU4_SYNR0_PH_SHFT					31
#define HWIO_XPU4_SYNR0_AC_CFG_BMSK				0x40000000
#define HWIO_XPU4_SYNR0_AC_CFG_SHFT				30
#define HWIO_XPU4_SYNR0_AC_CL_BMSK				0x20000000
#define HWIO_XPU4_SYNR0_AC_CL_SHFT				29
#define HWIO_XPU4_SYNR0_BURSTLEN_BMSK				0x10000000
#define HWIO_XPU4_SYNR0_BURSTLEN_SHFT				28
#define HWIO_XPU4_SYNR0_QADERR_BMSK				0x8000000
#define HWIO_XPU4_SYNR0_QADERR_SHFT				27
#define HWIO_XPU4_SYNR0_APUDECERR_BMSK				0x4000000
#define HWIO_XPU4_SYNR0_APUDECERR_SHFT				26
#define HWIO_XPU4_SYNR0_SSIZE_BMSK				0x3800000
#define HWIO_XPU4_SYNR0_SSIZE_SHFT				23
#define HWIO_XPU4_SYNR0_LEN_BMSK				0x7f8000
#define HWIO_XPU4_SYNR0_LEN_SHFT				15
#define HWIO_XPU4_SYNR0_QAD_BMSK				0x1f00
#define HWIO_XPU4_SYNR0_QAD_SHFT				8
#define HWIO_XPU4_SYNR0_REQ_OPC_BMSK				0x78
#define HWIO_XPU4_SYNR0_REQ_OPC_SHFT				3
#define HWIO_XPU4_SYNR0_PRIV_BMSK				0x4
#define HWIO_XPU4_SYNR0_PRIV_SHFT				2
#define HWIO_XPU4_SYNR0_INST_BMSK				0x2
#define HWIO_XPU4_SYNR0_INST_SHFT				1
#define HWIO_XPU4_SYNR0_XPROTNS_BMSK				0x1
#define HWIO_XPU4_SYNR0_XPROTNS_SHFT				0

#define HWIO_XPU4_SYNR1_ADDR(x)					((x) + 0x510)
#define HWIO_XPU4_SYNR1_OFFS					(0x510)
#define HWIO_XPU4_SYNR1_RMSK					0xff1fffff
#define HWIO_XPU4_SYNR1_IN(x)		\
	in_dword_masked(HWIO_XPU4_SYNR1_ADDR(x), HWIO_XPU4_SYNR1_RMSK)
#define HWIO_XPU4_SYNR1_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_SYNR1_ADDR(x), m)
#define HWIO_XPU4_SYNR1_TID_BMSK				0xff000000
#define HWIO_XPU4_SYNR1_TID_SHFT				24
#define HWIO_XPU4_SYNR1_DIEID_BMSK				0x180000
#define HWIO_XPU4_SYNR1_DIEID_SHFT				19
#define HWIO_XPU4_SYNR1_TRTYPE_BMSK				0x70000
#define HWIO_XPU4_SYNR1_TRTYPE_SHFT				16
#define HWIO_XPU4_SYNR1_BID_BMSK				0xe000
#define HWIO_XPU4_SYNR1_BID_SHFT				13
#define HWIO_XPU4_SYNR1_PID_BMSK				0x1f00
#define HWIO_XPU4_SYNR1_PID_SHFT				8
#define HWIO_XPU4_SYNR1_MID_BMSK				0xff
#define HWIO_XPU4_SYNR1_MID_SHFT				0

#define HWIO_XPU4_SYNR2_ADDR(x)					((x) + 0x514)
#define HWIO_XPU4_SYNR2_OFFS					(0x514)
#define HWIO_XPU4_SYNR2_RMSK					0x3ffffff
#define HWIO_XPU4_SYNR2_IN(x)		\
	in_dword_masked(HWIO_XPU4_SYNR2_ADDR(x), HWIO_XPU4_SYNR2_RMSK)
#define HWIO_XPU4_SYNR2_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_SYNR2_ADDR(x), m)
#define HWIO_XPU4_SYNR2_APSVIOE_BMSK				0x2000000
#define HWIO_XPU4_SYNR2_APSVIOE_SHFT				25
#define HWIO_XPU4_SYNR2_SLE_BMSK				0x1000000
#define HWIO_XPU4_SYNR2_SLE_SHFT				24
#define HWIO_XPU4_SYNR2_CFG_OWNER_BMSK				0x800000
#define HWIO_XPU4_SYNR2_CFG_OWNER_SHFT				23
#define HWIO_XPU4_SYNR2_APNSEE_BMSK				0x400000
#define HWIO_XPU4_SYNR2_APNSEE_SHFT				22
#define HWIO_XPU4_SYNR2_CESDE_BMSK				0x200000
#define HWIO_XPU4_SYNR2_CESDE_SHFT				21
#define HWIO_XPU4_SYNR2_OPTRW_EN_BMSK				0x100000
#define HWIO_XPU4_SYNR2_OPTRW_EN_SHFT				20
#define HWIO_XPU4_SYNR2_ATOPC_BMSK				0xf0000
#define HWIO_XPU4_SYNR2_ATOPC_SHFT				16
#define HWIO_XPU4_SYNR2_REDIRBITS_BMSK				0xf000
#define HWIO_XPU4_SYNR2_REDIRBITS_SHFT				12
#define HWIO_XPU4_SYNR2_INNERCACHEABLE_BMSK			0x800
#define HWIO_XPU4_SYNR2_INNERCACHEABLE_SHFT			11
#define HWIO_XPU4_SYNR2_MEMTYPE_BMSK				0x700
#define HWIO_XPU4_SYNR2_MEMTYPE_SHFT				8
#define HWIO_XPU4_SYNR2_TRANSIENT_BMSK				0x80
#define HWIO_XPU4_SYNR2_TRANSIENT_SHFT				7
#define HWIO_XPU4_SYNR2_NOALLOCATE_BMSK				0x40
#define HWIO_XPU4_SYNR2_NOALLOCATE_SHFT				6
#define HWIO_XPU4_SYNR2_WRITETHROUGH_BMSK			0x20
#define HWIO_XPU4_SYNR2_WRITETHROUGH_SHFT			5
#define HWIO_XPU4_SYNR2_CACHEALLOCATION_BMSK			0x1e
#define HWIO_XPU4_SYNR2_CACHEALLOCATION_SHFT			1
#define HWIO_XPU4_SYNR2_DIRTYINFO_BMSK				0x1
#define HWIO_XPU4_SYNR2_DIRTYINFO_SHFT				0

#define HWIO_XPU4_SYNR3_ADDR(x)					((x) + 0x518)
#define HWIO_XPU4_SYNR3_OFFS					(0x518)
#define HWIO_XPU4_SYNR3_RMSK					0x3
#define HWIO_XPU4_SYNR3_IN(x)		\
	in_dword_masked(HWIO_XPU4_SYNR3_ADDR(x), HWIO_XPU4_SYNR3_RMSK)
#define HWIO_XPU4_SYNR3_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_SYNR3_ADDR(x), m)
#define HWIO_XPU4_SYNR3_TMEERRCTRLAC_BMSK			0x2
#define HWIO_XPU4_SYNR3_TMEERRCTRLAC_SHFT			1
#define HWIO_XPU4_SYNR3_LOCKDELEGATE_BMSK			0x1
#define HWIO_XPU4_SYNR3_LOCKDELEGATE_SHFT			0

#define HWIO_XPU4_TME_GRP0_RG_PRG_ADDR(x)			((x) + 0x600)
#define HWIO_XPU4_TME_GRP0_RG_PRG_OFFS				(0x600)
#define HWIO_XPU4_TME_GRP0_RG_PRG_RMSK				0x1
#define HWIO_XPU4_TME_GRP0_RG_PRG_IN(x)		\
	in_dword_masked(HWIO_XPU4_TME_GRP0_RG_PRG_ADDR(x), \
			HWIO_XPU4_TME_GRP0_RG_PRG_RMSK)
#define HWIO_XPU4_TME_GRP0_RG_PRG_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_TME_GRP0_RG_PRG_ADDR(x), m)
#define HWIO_XPU4_TME_GRP0_RG_PRG_OUT(x, v)		\
	out_dword(HWIO_XPU4_TME_GRP0_RG_PRG_ADDR(x), v)
#define HWIO_XPU4_TME_GRP0_RG_PRG_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_TME_GRP0_RG_PRG_ADDR(x), m, v, \
			    HWIO_XPU4_TME_GRP0_RG_PRG_IN(x))
#define HWIO_XPU4_TME_GRP0_RG_PRG_TME0_RG_PRG_BMSK		0x1
#define HWIO_XPU4_TME_GRP0_RG_PRG_TME0_RG_PRG_SHFT		0

#define HWIO_XPU4_TME_GRP1_RG_PRG_ADDR(x)			((x) + 0x604)
#define HWIO_XPU4_TME_GRP1_RG_PRG_OFFS				(0x604)
#define HWIO_XPU4_TME_GRP1_RG_PRG_RMSK				0x1
#define HWIO_XPU4_TME_GRP1_RG_PRG_IN(x)		\
	in_dword_masked(HWIO_XPU4_TME_GRP1_RG_PRG_ADDR(x), \
			HWIO_XPU4_TME_GRP1_RG_PRG_RMSK)
#define HWIO_XPU4_TME_GRP1_RG_PRG_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_TME_GRP1_RG_PRG_ADDR(x), m)
#define HWIO_XPU4_TME_GRP1_RG_PRG_OUT(x, v)		\
	out_dword(HWIO_XPU4_TME_GRP1_RG_PRG_ADDR(x), v)
#define HWIO_XPU4_TME_GRP1_RG_PRG_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_TME_GRP1_RG_PRG_ADDR(x), m, v, \
			    HWIO_XPU4_TME_GRP1_RG_PRG_IN(x))
#define HWIO_XPU4_TME_GRP1_RG_PRG_TME1_RG_PRG_BMSK		0x1
#define HWIO_XPU4_TME_GRP1_RG_PRG_TME1_RG_PRG_SHFT		0

#define HWIO_XPU4_APPS_GRP0_RG_PRG_ADDR(x)			((x) + 0x608)
#define HWIO_XPU4_APPS_GRP0_RG_PRG_OFFS				(0x608)
#define HWIO_XPU4_APPS_GRP0_RG_PRG_RMSK				0x1
#define HWIO_XPU4_APPS_GRP0_RG_PRG_IN(x)		\
	in_dword_masked(HWIO_XPU4_APPS_GRP0_RG_PRG_ADDR(x), \
			HWIO_XPU4_APPS_GRP0_RG_PRG_RMSK)
#define HWIO_XPU4_APPS_GRP0_RG_PRG_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_APPS_GRP0_RG_PRG_ADDR(x), m)
#define HWIO_XPU4_APPS_GRP0_RG_PRG_OUT(x, v)		\
	out_dword(HWIO_XPU4_APPS_GRP0_RG_PRG_ADDR(x), v)
#define HWIO_XPU4_APPS_GRP0_RG_PRG_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_APPS_GRP0_RG_PRG_ADDR(x), m, v, \
			    HWIO_XPU4_APPS_GRP0_RG_PRG_IN(x))
#define HWIO_XPU4_APPS_GRP0_RG_PRG_APPS0_RG_PRG_BMSK		0x1
#define HWIO_XPU4_APPS_GRP0_RG_PRG_APPS0_RG_PRG_SHFT		0
#define HWIO_XPU4_APPS_GRP1_RG_PRG_ADDR(x)			((x) + 0x60c)
#define HWIO_XPU4_APPS_GRP1_RG_PRG_OFFS				(0x60c)
#define HWIO_XPU4_APPS_GRP1_RG_PRG_RMSK				0x1
#define HWIO_XPU4_APPS_GRP1_RG_PRG_IN(x)		\
	in_dword_masked(HWIO_XPU4_APPS_GRP1_RG_PRG_ADDR(x), \
			HWIO_XPU4_APPS_GRP1_RG_PRG_RMSK)
#define HWIO_XPU4_APPS_GRP1_RG_PRG_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_APPS_GRP1_RG_PRG_ADDR(x), m)
#define HWIO_XPU4_APPS_GRP1_RG_PRG_OUT(x, v)		\
	out_dword(HWIO_XPU4_APPS_GRP1_RG_PRG_ADDR(x), v)
#define HWIO_XPU4_APPS_GRP1_RG_PRG_OUTM(x, m, v) \
	out_dword_masked_ns(HWIO_XPU4_APPS_GRP1_RG_PRG_ADDR(x), m, v, \
			    HWIO_XPU4_APPS_GRP1_RG_PRG_IN(x))
#define HWIO_XPU4_APPS_GRP1_RG_PRG_APPS1_RG_PRG_BMSK		0x1
#define HWIO_XPU4_APPS_GRP1_RG_PRG_APPS1_RG_PRG_SHFT		0

#define HWIO_XPU4_BOOT_CFG_STS_ADDR(x)				((x) + 0x610)
#define HWIO_XPU4_BOOT_CFG_STS_OFFS				(0x610)
#define HWIO_XPU4_BOOT_CFG_STS_RMSK				0xff
#define HWIO_XPU4_BOOT_CFG_STS_IN(x)		\
	in_dword_masked(HWIO_XPU4_BOOT_CFG_STS_ADDR(x), \
			HWIO_XPU4_BOOT_CFG_STS_RMSK)
#define HWIO_XPU4_BOOT_CFG_STS_INM(x, m)		\
	in_dword_masked(HWIO_XPU4_BOOT_CFG_STS_ADDR(x), m)
#define HWIO_XPU4_BOOT_CFG_STS_POLICY_IGNORED_WO_BMSK		0xf0
#define HWIO_XPU4_BOOT_CFG_STS_POLICY_IGNORED_WO_SHFT		4
#define HWIO_XPU4_BOOT_CFG_STS_APPS_GRP1_WO_BMSK		0x8
#define HWIO_XPU4_BOOT_CFG_STS_APPS_GRP1_WO_SHFT		3
#define HWIO_XPU4_BOOT_CFG_STS_APPS_GRP0_WO_BMSK		0x4
#define HWIO_XPU4_BOOT_CFG_STS_APPS_GRP0_WO_SHFT		2
#define HWIO_XPU4_BOOT_CFG_STS_TME_GRP1_WO_BMSK			0x2
#define HWIO_XPU4_BOOT_CFG_STS_TME_GRP1_WO_SHFT		1
#define HWIO_XPU4_BOOT_CFG_STS_TME_GRP0_WO_BMSK			0x1
#define HWIO_XPU4_BOOT_CFG_STS_TME_GRP0_WO_SHFT		0

#define HWIO_XPU4_HW_POLICY_IGNOREDr_ADDR(base, r)		((base) + 0X620 + (0x4*(r)))
#define HWIO_XPU4_HW_POLICY_IGNOREDr_OFFS(r)			(0X620 + (0x4*(r)))
#define HWIO_XPU4_HW_POLICY_IGNOREDr_RMSK			0xffffffff
#define HWIO_XPU4_HW_POLICY_IGNOREDr_MAXr			2
#define HWIO_XPU4_HW_POLICY_IGNOREDr_INI(base, r)	\
	in_dword_masked(HWIO_XPU4_HW_POLICY_IGNOREDr_ADDR(base, r), \
			HWIO_XPU4_HW_POLICY_IGNOREDr_RMSK)
#define HWIO_XPU4_HW_POLICY_IGNOREDr_INMI(base, r, mask)	\
	in_dword_masked(HWIO_XPU4_HW_POLICY_IGNOREDr_ADDR(base, r), mask)
#define HWIO_XPU4_HW_POLICY_IGNOREDr_OUTI(base, r, val)	\
	out_dword(HWIO_XPU4_HW_POLICY_IGNOREDr_ADDR(base, r), val)
#define HWIO_XPU4_HW_POLICY_IGNOREDr_OUTMI(base, r, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_HW_POLICY_IGNOREDr_ADDR(base, r), \
			    mask, val, HWIO_XPU4_HW_POLICY_IGNOREDr_INI(base, r))
#define HWIO_XPU4_HW_POLICY_IGNOREDr_POLICY_IGNORED_BMSK	0xffffffff
#define HWIO_XPU4_HW_POLICY_IGNOREDr_POLICY_IGNORED_SHFT	0

#define HWIO_XPU4_HW_POLICY_IGNOREDs_ADDR(base, s)		((base) + 0X620 + (0x4*(s)))
#define HWIO_XPU4_HW_POLICY_IGNOREDs_OFFS(s)			(0X620 + (0x4*(s)))
#define HWIO_XPU4_HW_POLICY_IGNOREDs_RMSK			0x3fffffff
#define HWIO_XPU4_HW_POLICY_IGNOREDs_MAXs			3
#define HWIO_XPU4_HW_POLICY_IGNOREDs_INI(base, s)	\
	in_dword_masked(HWIO_XPU4_HW_POLICY_IGNOREDs_ADDR(base, s), \
			HWIO_XPU4_HW_POLICY_IGNOREDs_RMSK)
#define HWIO_XPU4_HW_POLICY_IGNOREDs_INMI(base, s, mask)	\
	in_dword_masked(HWIO_XPU4_HW_POLICY_IGNOREDs_ADDR(base, s), mask)
#define HWIO_XPU4_HW_POLICY_IGNOREDs_OUTI(base, s, val)	\
	out_dword(HWIO_XPU4_HW_POLICY_IGNOREDs_ADDR(base, s), val)
#define HWIO_XPU4_HW_POLICY_IGNOREDs_OUTMI(base, s, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_HW_POLICY_IGNOREDs_ADDR(base, s), \
			    mask, val, HWIO_XPU4_HW_POLICY_IGNOREDs_INI(base, s))
#define HWIO_XPU4_HW_POLICY_IGNOREDs_POLICY_IGNORED_BMSK	0x3fffffff
#define HWIO_XPU4_HW_POLICY_IGNOREDs_POLICY_IGNORED_SHFT	0

#define HWIO_XPU4_RGCR0n_ADDR(base, n)				((base) + 0X1000 + (0x40*(n)))
#define HWIO_XPU4_RGCR0n_OFFS(n)				(0X1000 + (0x40*(n)))
#define HWIO_XPU4_RGCR0n_RMSK					0x1
#define HWIO_XPU4_RGCR0n_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_RGCR0n_ADDR(base, n), HWIO_XPU4_RGCR0n_RMSK)
#define HWIO_XPU4_RGCR0n_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_RGCR0n_ADDR(base, n), mask)
#define HWIO_XPU4_RGCR0n_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_RGCR0n_ADDR(base, n), val)
#define HWIO_XPU4_RGCR0n_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_RGCR0n_ADDR(base, n), mask, val, \
			    HWIO_XPU4_RGCR0n_INI(base, n))
#define HWIO_XPU4_RGCR0n_RGWOWP_BMSK				0x1
#define HWIO_XPU4_RGCR0n_RGWOWP_SHFT				0

#define HWIO_XPU4_RGCR1n_ADDR(base, n)				((base) + 0X1004 + (0x40*(n)))
#define HWIO_XPU4_RGCR1n_OFFS(n)				(0X1004 + (0x40*(n)))
#define HWIO_XPU4_RGCR1n_RMSK					0x1
#define HWIO_XPU4_RGCR1n_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_RGCR1n_ADDR(base, n), HWIO_XPU4_RGCR1n_RMSK)
#define HWIO_XPU4_RGCR1n_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_RGCR1n_ADDR(base, n), mask)
#define HWIO_XPU4_RGCR1n_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_RGCR1n_ADDR(base, n), val)
#define HWIO_XPU4_RGCR1n_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_RGCR1n_ADDR(base, n), mask, val, \
			    HWIO_XPU4_RGCR1n_INI(base, n))
#define HWIO_XPU4_RGCR1n_RGE_BMSK				0x1
#define HWIO_XPU4_RGCR1n_RGE_SHFT				0

#define HWIO_XPU4_RGCSAR1n_ADDR(base, n)			((base) + 0x1008 + (0x40*(n)))
#define HWIO_XPU4_RGCSAR1n_OFFS(n)				(0X1008 + (0x40*(n)))
#define HWIO_XPU4_RGCSAR1n_RMSK					0xffffffff
#define HWIO_XPU4_RGCSAR1n_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_RGCSAR1n_ADDR(base, n), \
			HWIO_XPU4_RGCSAR1n_RMSK)
#define HWIO_XPU4_RGCSAR1n_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_RGCSAR1n_ADDR(base, n), mask)
#define HWIO_XPU4_RGCSAR1n_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_RGCSAR1n_ADDR(base, n), val)
#define HWIO_XPU4_RGCSAR1n_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_RGCSAR1n_ADDR(base, n), mask, val, \
			    HWIO_XPU4_RGCSAR1n_INI(base, n))
#define HWIO_XPU4_RGCSAR1n_RGSA_BMSK				0xffffffff
#define HWIO_XPU4_RGCSAR1n_RGSA_SHFT				0

#define HWIO_XPU4_RGCSAR0n_ADDR(base, n)			((base) + 0X100C + (0x40*(n)))
#define HWIO_XPU4_RGCSAR0n_OFFS(n)				(0X100C + (0x40*(n)))
#define HWIO_XPU4_RGCSAR0n_RMSK					0xffffffff
#define HWIO_XPU4_RGCSAR0n_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_RGCSAR0n_ADDR(base, n), \
			HWIO_XPU4_RGCSAR0n_RMSK)
#define HWIO_XPU4_RGCSAR0n_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_RGCSAR0n_ADDR(base, n), mask)
#define HWIO_XPU4_RGCSAR0n_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_RGCSAR0n_ADDR(base, n), val)
#define HWIO_XPU4_RGCSAR0n_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_RGCSAR0n_ADDR(base, n), mask, val, \
			    HWIO_XPU4_RGCSAR0n_INI(base, n))
#define HWIO_XPU4_RGCSAR0n_RGSA_BMSK				0xffffffff
#define HWIO_XPU4_RGCSAR0n_RGSA_SHFT				0

#define HWIO_XPU4_RGCEAR1n_ADDR(base, n)			((base) + 0x1010 + (0x40*(n)))
#define HWIO_XPU4_RGCEAR1n_OFFS(n)				(0X1010 + (0x40*(n)))
#define HWIO_XPU4_RGCEAR1n_RMSK					0xffffffff
#define HWIO_XPU4_RGCEAR1n_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_RGCEAR1n_ADDR(base, n), \
			HWIO_XPU4_RGCEAR1n_RMSK)
#define HWIO_XPU4_RGCEAR1n_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_RGCEAR1n_ADDR(base, n), mask)
#define HWIO_XPU4_RGCEAR1n_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_RGCEAR1n_ADDR(base, n), val)
#define HWIO_XPU4_RGCEAR1n_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_RGCEAR1n_ADDR(base, n), mask, val, \
			    HWIO_XPU4_RGCEAR1n_INI(base, n))
#define HWIO_XPU4_RGCEAR1n_RGEA_BMSK				0xffffffff
#define HWIO_XPU4_RGCEAR1n_RGEA_SHFT				0

#define HWIO_XPU4_RGCEAR0n_ADDR(base, n)			((base) + 0X1014 + (0x40*(n)))
#define HWIO_XPU4_RGCEAR0n_OFFS(n)				(0X1014 + (0x40*(n)))
#define HWIO_XPU4_RGCEAR0n_RMSK					0xffffffff
#define HWIO_XPU4_RGCEAR0n_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_RGCEAR0n_ADDR(base, n), \
			HWIO_XPU4_RGCEAR0n_RMSK)
#define HWIO_XPU4_RGCEAR0n_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_RGCEAR0n_ADDR(base, n), mask)
#define HWIO_XPU4_RGCEAR0n_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_RGCEAR0n_ADDR(base, n), val)
#define HWIO_XPU4_RGCEAR0n_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_RGCEAR0n_ADDR(base, n), mask, val, \
			    HWIO_XPU4_RGCEAR0n_INI(base, n))
#define HWIO_XPU4_RGCEAR0n_RGEA_BMSK				0xffffffff
#define HWIO_XPU4_RGCEAR0n_RGEA_SHFT				0

#define HWIO_XPU4_RGRDRn_ADDR(base, n)				((base) + 0X1018 + (0x40*(n)))
#define HWIO_XPU4_RGRDRn_OFFS(n)				(0X1018 + (0x40*(n)))
#define HWIO_XPU4_RGRDRn_RMSK					0xffffffff
#define HWIO_XPU4_RGRDRn_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_RGRDRn_ADDR(base, n), HWIO_XPU4_RGRDRn_RMSK)
#define HWIO_XPU4_RGRDRn_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_RGRDRn_ADDR(base, n), mask)
#define HWIO_XPU4_RGRDRn_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_RGRDRn_ADDR(base, n), val)
#define HWIO_XPU4_RGRDRn_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_RGRDRn_ADDR(base, n), mask, val, \
			    HWIO_XPU4_RGRDRn_INI(base, n))
#define HWIO_XPU4_RGRDRn_RDA_S_BMSK				0x80000000
#define HWIO_XPU4_RGRDRn_RDA_S_SHFT				31
#define HWIO_XPU4_RGRDRn_RDA_NS_BMSK				0x40000000
#define HWIO_XPU4_RGRDRn_RDA_NS_SHFT				30
#define HWIO_XPU4_RGRDRn_RDA_QAD_BMSK				0xffffffff
#define HWIO_XPU4_RGRDRn_RDA_QAD_SHFT				0

#define HWIO_XPU4_RGWRRn_ADDR(base, n)				((base) + 0X101C + (0x40*(n)))
#define HWIO_XPU4_RGWRRn_OFFS(n)				(0X101C + (0x40*(n)))
#define HWIO_XPU4_RGWRRn_RMSK					0xffffffff
#define HWIO_XPU4_RGWRRn_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_RGWRRn_ADDR(base, n), HWIO_XPU4_RGWRRn_RMSK)
#define HWIO_XPU4_RGWRRn_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_RGWRRn_ADDR(base, n), mask)
#define HWIO_XPU4_RGWRRn_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_RGWRRn_ADDR(base, n), val)
#define HWIO_XPU4_RGWRRn_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_RGWRRn_ADDR(base, n), mask, val, \
			    HWIO_XPU4_RGWRRn_INI(base, n))
#define HWIO_XPU4_RGWRRn_WRA_S_BMSK				0x80000000
#define HWIO_XPU4_RGWRRn_WRA_S_SHFT				31
#define HWIO_XPU4_RGWRRn_WRA_NS_BMSK				0x40000000
#define HWIO_XPU4_RGWRRn_WRA_NS_SHFT				30
#define HWIO_XPU4_RGWRRn_WRA_QAD_BMSK				0xffffffff
#define HWIO_XPU4_RGWRRn_WRA_QAD_SHFT				0

#define HWIO_XPU4_QADRGLn_ADDR(base, n)			((base) + 0X1030 + (0x40*(n)))
#define HWIO_XPU4_QADRGLn_OFFS(n)				(0X1030 + (0x40*(n)))
#define HWIO_XPU4_QADRGLn_RMSK					0xffffffff
#define HWIO_XPU4_QADRGLn_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_QADRGLn_ADDR(base, n), HWIO_XPU4_QADRGLn_RMSK)
#define HWIO_XPU4_QADRGLn_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_QADRGLn_ADDR(base, n), mask)
#define HWIO_XPU4_QADRGLn_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_QADRGLn_ADDR(base, n), val)
#define HWIO_XPU4_QADRGLn_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_QADRGLn_ADDR(base, n), mask, val, \
			    HWIO_XPU4_QADRGLn_INI(base, n))
#define HWIO_XPU4_QADRGLn_RGL_S_BMSK				0x80000000
#define HWIO_XPU4_QADRGLn_RGL_S_SHFT				31
#define HWIO_XPU4_QADRGLn_RGL_NS_BMSK				0x40000000
#define HWIO_XPU4_QADRGLn_RGL_NS_SHFT				30
#define HWIO_XPU4_QADRGLn_RGL_QAD_BMSK				0xffffffff
#define HWIO_XPU4_QADRGLn_RGL_QAD_SHFT				0

#define HWIO_XPU4_QADRGL_ALIASn_ADDR(base, n)			((base) + 0X1034 + (0x40*(n)))
#define HWIO_XPU4_QADRGL_ALIASn_OFFS(n)				(0X1034 + (0x40*(n)))
#define HWIO_XPU4_QADRGL_ALIASn_RMSK				0xffffffff
#define HWIO_XPU4_QADRGL_ALIASn_INI(base, n)	\
	in_dword_masked(HWIO_XPU4_QADRGL_ALIASn_ADDR(base, n), \
			HWIO_XPU4_QADRGL_ALIASn_RMSK)
#define HWIO_XPU4_QADRGL_ALIASn_INMI(base, n, mask)	\
	in_dword_masked(HWIO_XPU4_QADRGL_ALIASn_ADDR(base, n), mask)
#define HWIO_XPU4_QADRGL_ALIASn_OUTI(base, n, val)	\
	out_dword(HWIO_XPU4_QADRGL_ALIASn_ADDR(base, n), val)
#define HWIO_XPU4_QADRGL_ALIASn_OUTMI(base, n, mask, val) \
	out_dword_masked_ns(HWIO_XPU4_QADRGL_ALIASn_ADDR(base, n), mask, val, \
			    HWIO_XPU4_QADRGL_ALIASn_INI(base, n))
#define HWIO_XPU4_QADRGL_ALIASn_RGL_S_BMSK			0x80000000
#define HWIO_XPU4_QADRGL_ALIASn_RGL_S_SHFT			31
#define HWIO_XPU4_QADRGL_ALIASn_RGL_NS_BMSK			0x40000000
#define HWIO_XPU4_QADRGL_ALIASn_RGL_NS_SHFT			30
#define HWIO_XPU4_QADRGL_ALIASn_RGL_QAD_BMSK			0xffffffff
#define HWIO_XPU4_QADRGL_ALIASn_RGL_QAD_SHFT			0

#endif /* HALXPU4HWIOGENERIC_H */
