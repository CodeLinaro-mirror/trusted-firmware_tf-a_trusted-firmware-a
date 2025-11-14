#
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: ISC
#

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------

import os
import sys
import argparse
import math

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from DataParsers import xPUParser
from DataParsers import violationParser
from DataParsers import FileIntegrity

DEVPRGPOLICY = False
def write_rg_override_array(xpu):
	"""
	Writes the RG Override bitmap for a given xPU

	Returns:
		RG Override Array source code as a string
	"""
	reg_count = math.ceil(xpu.nrg / 32)
	reg_vals = [0] * reg_count
	for rg_idx in xpu.rg_override_array:
		reg_idx = math.floor(int(rg_idx) / 32)
		reg_vals[reg_idx] |= 1 << (int(rg_idx) % 32)
	ret = f'/* {xpu.name}_IGNORED\n   RGs with override: {[int(idx) for idx in xpu.rg_override_array]} */\nstatic const ac_rg_ignored {xpu.name.lower()}_IGNORED[] = {{\n\t'
	for val in reg_vals:
		ret += f"{val:#0{10}x}, "
	ret += '\n};\n\n'
	return ret

def write_xpu_header(attributes, xpu):
	attribute = attributes['RO'] if xpu.dynamic else attributes['Reclaim']
	if DEVPRGPOLICY:
		xpu_def_type = ' ac_mpu_rg_dev_prog ' if xpu.type == 'MPU' else ' ac_rpu_rg_dev_prog '
	else:
		xpu_def_type = ' ac_mpu_rg ' if xpu.type == 'MPU' else ' ac_rpu_rg '
	return '/* ' + xpu.name + ' */\n' + attribute + 'static const' + xpu_def_type + xpu.name.lower() + '[] = {\n%s};\n\n'

def write_rg(xpu, rg):
	vals_to_ignore = ['', '0xffffffff']
	if not rg.static and (rg.start is not None and rg.start.lower() in vals_to_ignore):
		return ''
	#Some MPUs are missing all info, they should be disabled instead
	if rg.start == '' or (rg.start is not None and rg.start.lower() == '0xffffffff'):
		return ''
	# if the profile contains XPRESSCFG, then the policy is handled by hardware
	if rg.is_xpresscfg:
		return ''
	rvmids, wvmids = xpu.getRGVmids(rg)
	rdomains, wdomains = xpu.getRGDomains(rg)
	mpu_start_end = '.start        = ' + rg.start + ',\n\t\t.end          = ' + rg.end + ',\n\t\t' if xpu.type == 'MPU' else ''

	read = ' | '.join(rvmids + rdomains) if rdomains or rvmids else 'AC_DOMAIN_NONE'
	write = ' | '.join(wvmids + wdomains) if wdomains or wvmids else 'AC_DOMAIN_NONE'

	profile = ' | '.join(['AC_PROFILE_' + prof for prof in rg.writable_profiles])
	if profile == 'AC_PROFILE_' or profile == '':
		profile = 'AC_PROFILE_DEFAULT'

	if DEVPRGPOLICY:
		if 'AC_DOMAIN_AP_NS_BIT' not in read and 'AC_DOMAIN_AP_NS_BIT' not in write:
			rwvalue = 0x0
		elif 'AC_DOMAIN_AP_NS_BIT' in read and 'AC_DOMAIN_AP_NS_BIT' not in write:
			rwvalue = 0x1
		elif 'AC_DOMAIN_AP_NS_BIT' in read and 'AC_DOMAIN_AP_NS_BIT' in write:
			rwvalue = 0x3

		consolidated_value = (rwvalue << 16) | (int(rg.index) & 0xFFFF)
		return ("\t{\n\t\t%s.rg_num        = %s\n\t},\n") % (mpu_start_end, hex(consolidated_value))
	return ("\t{\n\t\t.rg_num        = %s,"
			"\n\t\t%s.profile_flags = %s,"
			"\n\t\t.read_qads     = %s,"
			"\n\t\t.write_qads    = %s\n\t},\n") % \
			(rg.index, mpu_start_end , profile, read, write)

def write_intr_mapping(xpu_interrupt_data):
	bit_mapping = ''
	cur_xpu_idx = 0
	bit_map = xpu_interrupt_data['bit_mapping']
	all_clock_group = [each['clock'] for each in bit_map]
	all_xpuId = [each['enum'] for each in bit_map]
	clk_size = max(len(s) for s in all_clock_group) if all_clock_group else 0
	ljust_sz = clk_size + len('\t\tCLOCK_XPU_ENTRY( ,')
	rjust_sz = max(len(s) for s in all_xpuId) if all_xpuId else 0 + 2

	for i in range(int(xpu_interrupt_data["num_regs"])):
		bit_mapping += '\t{\n'
		for j in range(int(xpu_interrupt_data["num_per"])):
			if cur_xpu_idx < len(bit_map) and i == int(bit_map[cur_xpu_idx]['reg']) and j == int(bit_map[cur_xpu_idx]['bit']):
				cur_entry = bit_map[cur_xpu_idx]
				cur_xpu_idx += 1
			else:
				cur_entry = {"clock" : "CLOCK_GROUP_XPU_TOTAL", "enum" : "HAL_XPU2_COUNT", "reg" : "%02d" % i, "bit" : "%02d\t- Reserved" % j}
			if clk_size == 0:
				# Generate new structure format with both xpu_id and name
				xpu_enum = cur_entry["enum"]
				xpu_name = f'"{xpu_enum}"'
				bit_mapping += f'\t\t{{.xpu_id = {xpu_enum}, .name = {xpu_name}}}, //Bit {j}\n'
			else:
				bit_mapping += f'\t\tCLOCK_XPU_ENTRY({cur_entry["clock"]},'.ljust(ljust_sz)
				bit_mapping += f'{cur_entry["enum"]:<{rjust_sz}}),'
				bit_mapping += f' //Bit {cur_entry["bit"]}\n'
		bit_mapping += '\t},\n'
	return bit_mapping

def write_xpu_array(xpuInfo, image, xpu, has_static_rgs):
	"""
	Creates an xPU Global Array entry for a single xPU.

	Args:
		xpuInfo (xpuInfo): xpuParser object.
		image (string): name of the image for which to create the array entry.
		xPU (xPU): specific xpu for which to create array entry.
		has_static_rgs (bool): True if there is a static xPU array to program.

	Returns:
		xPU Global Array entry as a string.
	"""
	xpu_disabled = 'XPU_DISABLED' if xpu.disabled or xpu.dynamic else 'XPU_ENABLED'
	profile = ' | '.join(['AC_PROFILE_' + prof for prof in xpu.writable_profiles])
	if profile == 'AC_PROFILE_' or profile == '':
		profile = 'AC_PROFILE_DEFAULT'
	nrg_str = f'ARRAY_SIZE({xpu.name.lower()})' if has_static_rgs else '0'
	rg_str = f'{{{xpu.name.lower()}}}' if has_static_rgs else '{NULL}'

	if image == 'TME_FW':
		default_umr_perm = ' | '.join(xpuInfo.domain_to_bit_mappings['ALL_ROT'])
	else:
		default_umr_perm = ' | '.join(xpuInfo.domain_to_bit_mappings['HYP'] + xpuInfo.domain_to_bit_mappings['TZ'])

	umr_perm_list = xpu.get_umr_perms(image)
	umr_perm = ' | '.join(umr_perm_list) if umr_perm_list else default_umr_perm
	cfg_owner = xpuInfo.domain_to_qad_mappings[xpu.cfg_owner[image][0]] if image in xpu.cfg_owner else xpuInfo.domain_to_qad_mappings['APPS']
	rg_ignored = f'{xpu.name}_IGNORED' if image == xpu.rg_override_ee else 'NULL'
	if xpuInfo.supportsXpuRev("4.2.0"):
		addl_fields = (f"\n\t\t.cfg_owner     = {cfg_owner},"
					   f"\n\t\t.umr_perm      = {umr_perm},"
					   f"\n\t\t.rg_ignored    = {rg_ignored},")
	else:
		addl_fields = ''
	return (f"\t{{\t/* {xpu.name} */"
		 	f"\n\t\t.base_addr     = {xpu.address},"
			f"\n\t\t.profile_flags = {profile},"
			f"\n\t\t.xpu_id        = {xpu.enum},"
			f"\n\t\t.status       = {xpu_disabled},"
			f"\n\t\t.nrg          = {nrg_str},"
			f"\n\t\t.rg           = {rg_str},"
			f"{addl_fields}\n\t}},\n")

def get_policy(options, info, xpu_type=None):
	"""
	Gets the xpu policy for the given image

	Args:
		options: cli options (including for which image to generate policy)
		info (xpuParser): xPU parser object with policy data
		xpu_type (string): gets only a subset of xPU data (ra and mpu supported)

	Returns:
		A dictionary with xPU and RG data
	"""
	policy = {}
	if options.image in ['TME_FW']:
		xpus = info.getAllxPU()
	elif xpu_type is not None:
		if xpu_type == 'ra':
			xpus = info.getAllRAxPU(image=options.image)
		elif xpu_type == 'mpu':
			xpus = info.getAllMpu(image=options.image)
		else:
			raise Exception('what policy xPU type?')
	else:
		xpus = info.getAllxPU(image=options.image)
	for xPU in xpus:
		if DEVPRGPOLICY:
			selected_image = 'xBL_SC_DEVPRG'
		else:
			selected_image = options.image
		for Index, RGs in xPU.getPolicyRGs(selected_image).items():
			for RG in RGs:
				if not RG.enabled:
					continue
				if xPU.name not in policy:
					policy[xPU.name] = {'xPU' : None, 'RGs' : []}
				policy[xPU.name]['RGs'].append(RG)
			if xPU.name in policy:
				policy[xPU.name]['xPU'] = xPU
	if options.image in ['TME_FW']:
		for xPU in xpus:
			if xPU.name not in policy:
				# For TME_FW only, we need to append all xPUs to the policy even if they aren't programmed by TME_FW,
				# so that TME_FW can have knowledge of each xPU for interrupt handling
				policy[xPU.name] = {'xPU' : xPU, 'RGs' : []}
	return policy

def generate_target_code(options, info):
	target_out_file = ''

	#tme details
	if options.image in ["TME_FW"]:
		#AC_SD's that have a QAD associated
		target_out_file += "const ACSecDomainId g_acValidDomainIds[] =\n{\n" + ''.join(['\t' + each + ',\n' for each in info.valid_qad_bits])
		target_out_file += '};\nconst uint32_t g_acValidDomainIdsCount = ARRAY_SIZE(g_acValidDomainIds);\n\n'

	#xPU_protected ranges
	if options.image in ["TME_FW", "xBL_SC", "TZ"]:
		target_out_file += "const uint32_t g_ac_enabled = %s;\n\n" % ("true" if info.ac_enabled else "false")
		target_out_file += "const ac_xpu_protected_range g_ac_xpu_protected_ranges[] = {\n"
		for each in sorted(info.xpu_protected_ranges, key=lambda x: int(x['ranges'][0]['start'], 16)):
			cur_xpu = each['xpuId']
			if 'BROADCAST' in cur_xpu and options.image == 'TZ':
				continue
			xpu = info.getxPUByName(cur_xpu)[0]
			if isinstance(xpu, str):
				print(f'{xpu} is in protected range list put in XML')
				continue
			dyn_start, dyn_end = xpu.getDynamicRGRange(options.image)
			if dyn_start is None or dyn_end is None:
				# This xPU has no dynamic RGs for this EE
				continue
			for addr_range in each["ranges"]:
				target_out_file += '\t{\n\t\t// %s\n\t\t.start = %s,\n\t\t.end   = %s,\n\t\t.xpu_id = %s,\n\t},\n' % (addr_range['comment'], addr_range['start'], addr_range['end'], cur_xpu)
		target_out_file += '};\nconst uint32_t g_ac_xpu_protected_ranges_count = ARRAY_SIZE(g_ac_xpu_protected_ranges);\n\n'

	#ms protected ranges
	if options.image in ["TZ"]:
		target_out_file += 'const ACXpuProtectedRangeMS g_ac_xpu_protected_ranges_ms[] = {\n};\nconst uint32_t g_ac_xpu_protected_ranges_ms_count = ARRAY_SIZE(g_ac_xpu_protected_ranges_ms);\n\n'

	#xpu_hw_addr_mask
	if options.image in ["TME_FW", "xBL_SC", "TZ"]:
		if options.image in ["TME_FW"]:
			target_out_file += "const ac_xpu_hw_addr_mask_offset g_ac_xpu_hw_addr_mask_offset[] = {\n"
		else:
			target_out_file += "const ac_xpu_hw_addr_mask_offset g_ac_xpu_hw_addr_mask_offset[] = {\n"
		ljust_sz = max([len(xpu) for entry in info.xpu_hw_addr_mask for xpu in entry['xpuId']]) + 4
		for each in info.xpu_hw_addr_mask:
			for xpu in each["xpuId"]:
				target_out_file += f"\t{{{xpu},".ljust(ljust_sz)
				target_out_file += f"{each['mask']:<10}, {each['offset']:<10}}},\n"
		if options.image in ["TME_FW"]:
			target_out_file += '};\nconst uint32_t g_ac_xpu_hw_addr_mask_offset_count = ARRAY_SIZE(g_ac_xpu_hw_addr_mask_offset);\n\n'
		else:
			target_out_file += '};\nconst uint32_t g_ac_xpu_hw_addr_mask_offset_count = ARRAY_SIZE(g_ac_xpu_hw_addr_mask_offset);\n\n'

	#ra_static_signals
	if options.image in ["xBL_SC", "TZ"]:
		ra_xpus = get_policy(options, info, 'ra')
		ra_xpu_enums = [ra_xpus[each]['xPU'].enum for each in ra_xpus if not ra_xpus[each]['xPU'].disabled]
		global_ra = 'const ac_ra_xpu_info g_ac_ra_xpu_info[] = {\n'
		intr_desc = '\n\t\t.intrDesc = "",\n\t\t.intrNum = 0xFFFFFFFF,'
		for ra_xpu in ra_xpu_enums:
			if ra_xpu not in info.ra_xpu_interrupt_data['static_signals']:
				raise Exception("Missing ra xpu static signals for: " + ra_xpu)
			each = info.ra_xpu_interrupt_data['static_signals'][ra_xpu]
			sle_name = None
			others_name = None
			others_arr = None
			if 'other' in each or 'SLE'in each:
				if 'SLE'in each:
					sle_name = 's_RAXpuStaticSignalSilentLogging_' + each['name']
					target_out_file += 'static ac_addr_value_tuple ' + sle_name + ' =\n'
					target_out_file += '{\n\t.addr = %s,\n\t.mask = %s,\n\t.value = %s,\n};\n\n' % (each['SLE']['addr'], each['SLE']['mask'], each['SLE']['value'])
				others_name = 's_RAXpuStaticSignalOthers_' + each['name']
				target_out_file += 'static ac_addr_value_tuple ' + others_name + '[] =\n{\n'
				for other in each['other']:
					target_out_file += '\t{\n\t\t.addr = %s,\n\t\t.mask = %s,\n\t\t.value = %s,\n\t},\n' % (other['addr'], other['mask'], other['value'])
				others_arr = 'ARRAY_SIZE(' + others_name + ')'
				target_out_file += '};\n\n'
			global_ra += '\t{%s\n\t\t.xpuId = %s,\n\t\t.staticSignalSilentLogging = %s,\n\t\t.staticSignalOthers = %s,\n\t\t.staticSignalOthersCount = %s,\n\t},\n' % \
						(intr_desc, ra_xpu, '&' + sle_name if sle_name is not None else 'NULL', others_name if others_name is not None else 'NULL', others_arr if others_arr is not None else '0')
		global_ra += '};\nconst uint32_t g_ac_ra_xpu_info_count = ARRAY_SIZE(g_ac_ra_xpu_info);\n\n'
		target_out_file += global_ra

	#ra interrupt mapping
	if options.image in ["TZ"]:
		target_out_file += 'const HAL_xpu2_XPU2Type g_ac_ra_xpu_err_pos_to_xpu_map[][RA_XPU_ERR_NUM_PER_REG] = {\n%s};\n\n' % write_intr_mapping(info.ra_xpu_interrupt_data)

	#xpu_interrupt_data
	if options.image in ["TME_FW"]:
		enable_reg = 'ACRegisterTuple g_acXpuIntrEnReg[XPU_ERR_INT_REG_NUM] =\n{\n'
		status_reg = 'ACRegisterTuple g_acXpuIntrStatusReg[XPU_ERR_INT_REG_NUM] =\n{\n'
		bit_mapping = "//TODO: Resolve clock dependencies and correct the XPU positions\n"
		bit_mapping += 'const ACClockXpuTuple g_acXpuErrPosToXpuMap[XPU_ERR_INT_REG_NUM][XPU_ERR_NUM_PER_REG] =\n{\n'

		if len(info.er_xpu_interrupt_data["enable_regs"]) != len(info.er_xpu_interrupt_data["status_regs"]):
			raise Exception("Error in target_info.json: The number of TCSR XPU Interrupt enable and status registers should be equal")
		for intr_reg in info.er_xpu_interrupt_data["enable_regs"]:
			enable_reg += '\t{\n\t\t.addr = %s,\n\t\t.mask = %s,\n\t},\n' % (intr_reg["addr"],intr_reg["mask"])
		for intr_reg in info.er_xpu_interrupt_data["status_regs"]:
			status_reg += '\t{\n\t\t.addr = %s,\n\t\t.mask = %s,\n\t},\n' % (intr_reg["addr"],intr_reg["mask"])

		enable_reg += '};\n\n'
		status_reg += '};\n\n'
		bit_mapping += write_intr_mapping(info.er_xpu_interrupt_data) + '};\n\n'
		target_out_file += enable_reg
		target_out_file += status_reg
		target_out_file += ('#define CLOCK_XPU_ENTRY(clock, xpu) \ \n\t{.clockId = (uint16_t)(clock), .xpuId = (uint16_t)(xpu)}\n\n'
							'//static_assert(CLOCK_GROUP_XPU_TOTAL <= UINT16_MAX, "overflows .clockId");\n'
							'//static_assert(HAL_XPU2_COUNT <= UINT16_MAX, "overflows .xpuId");\n\n')
		target_out_file += bit_mapping

	if options.image in ["TZ"]:
		enable_reg = 'ac_register_tuple g_ac_xpu_intr_en_reg[XPU_ERR_INT_REG_NUM] = {\n'
		status_reg = 'ac_register_tuple g_ac_xpu_intr_status_reg[XPU_ERR_INT_REG_NUM] = {\n'
		bit_mapping = 'const ac_xpu_err_mapping_t g_ac_xpu_err_pos_to_xpu_map[XPU_ERR_INT_REG_NUM][XPU_ERR_NUM_PER_REG] = {\n'

		if len(info.er_xpu_interrupt_data["enable_regs"]) != len(info.er_xpu_interrupt_data["status_regs"]):
			raise Exception("Error in target_info.json: The number of TCSR XPU Interrupt enable and status registers should be equal")
		for intr_reg in info.er_xpu_interrupt_data["enable_regs"]:
			enable_reg += '\t{\n\t\t.addr = %s,\n\t\t.mask = %s,\n\t},\n' % (intr_reg["addr"],intr_reg["mask"])
		for intr_reg in info.er_xpu_interrupt_data["status_regs"]:
			status_reg += '\t{\n\t\t.addr = %s,\n\t\t.mask = %s,\n\t},\n' % (intr_reg["addr"],intr_reg["mask"])

		enable_reg += '};\n\n'
		status_reg += '};\n\n'
		bit_mapping += write_intr_mapping(info.er_xpu_interrupt_data) + '};\n\n'
		target_out_file += enable_reg
		target_out_file += status_reg
		target_out_file += bit_mapping

    #ddr_slice_xpu_daisy_chain_interrupt_data
	if hasattr(info, 'ddr_slice_xpus') and options.image in ["TME_FW"]:
		ddr_slice_xpu = 'HAL_xpu2_XPU2Type g_acDdrSliceXpus[] = \n{\n'
		for sublist in info.ddr_slice_xpus['slice_xpu_details']:
			ddr_slice_xpu += '\t' + sublist + ',\n'
		ddr_slice_xpu += '};\n\n'

		ddr_slice_xpu_count = 'ARRAY_SIZE(g_acDdrSliceXpus);\n\n'
		ddr_slice_xpu += 'const uint32_t g_acDdrSliceXpusCount =' + ddr_slice_xpu_count
		ddr_slice_xpu += 'const uint8_t g_numOfSlices = ' + str(info.ddr_slice_xpus['number_of_slices']) + ';\n'
		ddr_slice_xpu += 'const HAL_xpu2_XPU2Type g_acDdrSliceXpusInterrupt[][XPU_DDR_SLICE_NUM] =\n{\n'
		for sublist in info.ddr_slice_xpus['slice_xpu_details']:
			ddr_slice_xpu += "\t{\n"
			for list in info.ddr_slice_xpus['slice_xpu_details'][str(sublist)]:
				ddr_slice_xpu += "\t\t" + list + ",\n"
			ddr_slice_xpu += "\t},\n"
		ddr_slice_xpu += "};\n\n"
		target_out_file += ddr_slice_xpu
	target_out_file += 'const HAL_xpu2_XPU2Type g_xpu_enum_count = HAL_XPU2_COUNT;\n\n'


	#g_acOemSpareFuseInfos
	if options.image in ["TME_FW"]:
		target_out_file += "ACOemSpareFuseInfo g_acOemSpareFuseInfos[] =\n{\n"
		region_id = 0
		for region in info.oem_spare_fuses:
			target_out_file +=	(f'\t{{\n'
								 f'\t\t.start         = {region["start"]},\n'
								 f'\t\t.end           = {region["end"]},\n'
								 f'\t\t.xpuId         = HAL_XPU2_TME_QFPROM_APU,\n'
								 f'\t\t.rgNum         = {region["rg_num"]},\n'
								 f'\t\t.regionId      = {region_id},\n'
								 f'\t\t.writePermMask = 0,\n'
								 f'\t}},\n')
			region_id += 1
		target_out_file += "};\n\n"
		target_out_file += ('const uint32_t g_acOemSpareFuseInfosCount = ARRAY_SIZE(g_acOemSpareFuseInfos);\n'
							'//static_assert(g_acOemSpareFuseInfosCount <= AC_OEM_SPARE_FUSES_MAX_COUNT,\n'
							'//             \"too many OEM spare fuses\");\n\n')

	wa_supported = 'true' if info.supportsWAs() else 'false'
	target_out_file += f"const bool g_was_supported = {wa_supported};\n\n"

	return info.getHeader(options.image) + target_out_file

def generate_target_header(options, info):
	xpu_target_header = "#ifndef AC_XPU_TARGET_INFO_H\n#define AC_XPU_TARGET_INFO_H\n\n"

	if info.supportsXpuRev("4.2.0"):
		xpu_target_header +=  "#define xPU_4_2_SUPPORT\n"

	if options.image in ["xBL_SC", "TZ"]:
		xpu_target_header += "#define TME_SLE_ENABLED %s\n\n" % str(hex(info.sle_enabled_val))
		xpu_target_header += '#define RA_XPU_ERR_INT_REG_NUM\t' + str(info.ra_xpu_interrupt_data['num_regs']) + '\n'
		xpu_target_header += '#define RA_XPU_ERR_NUM_PER_REG\t' + str(info.ra_xpu_interrupt_data['num_per']) + '\n\n'

	if options.image in ["TME_FW", "TZ"]:
		xpu_target_header += f"#define XPU_ERR_INT_REG_NUM {info.er_xpu_interrupt_data['num_regs']}\n"
		xpu_target_header += f"#define XPU_ERR_NUM_PER_REG {info.er_xpu_interrupt_data['num_per']}\n"
		xpu_target_header += f"#define ROM_VERSION_SUPPORTED(ver) ((ver) == {info.rom_version})\n"
		if info.ac_entry != 0:
			xpu_target_header += f"#define ENTRY_POINT (ACGlobalData_t*){hex(info.ac_entry)}\n\n"
		if hasattr(info, 'ddr_slice_xpus'):
			xpu_target_header += f"#define XPU_DDR_SLICE_NUM {info.ddr_slice_xpus['number_of_slices']}\n"

	xpu_target_header += f"#define VALID_QAD_MASK {info.qad_mask}\n"

	xpu_target_header += "\n#endif"
	return info.autogen_header + xpu_target_header

def generate_target_data(options, info):
	dbgar_str = 'const uint32_t g_ac_xpu_dbgars[] = {\n'
	dyn_rg_str = ''
	if options.image in ['TME_FW']:
		xpu_info_str = 'ac_xpu4_priv_info g_ac_xpu_infos[] = {\n'
	else:
		xpu_info_str = 'ac_xpu4_priv_info g_ac_xpu_infos[] = {\n'
	policy_xpus = get_policy(options, info)
	xpus = [policy_xpus[each]['xPU'] for each in policy_xpus]
	for xpu in xpus:
		if xpu.enum == 'HAL_XPU2_XPU_UNSUPPORTED':
			continue

		#debug_ar array
		if not info.skip_dbgar_programming and not xpu.skip_debugar and policy_xpus[xpu.name]['RGs']:
			dbgar_str += '\t%s, \t// %s\n' % (xpu.dbgar_address, xpu.name)

		#shadow permissions and dynamic RG data
		dyn_start, dyn_end = xpu.getDynamicRGRange(options.image)
		dyn_rgs = None
		if dyn_start is not None and dyn_end is not None:
			sdw_read = xpu.enum.replace('HAL', 's_shadow_read_perm').replace('_XPU2','')
			sdw_write = xpu.enum.replace('HAL', 's_shadow_write_perm').replace('_XPU2','')
			dyn_rgs = xpu.enum.replace('HAL', 's_ac_xpu_dynamic_rgs').replace('_XPU2','')

			sdw_perm_arr_sz = str(dyn_end - dyn_start) if options.image not in ['TME_FW'] else f"((({str(dyn_end - dyn_start)})*2+31)/32)"
			dyn_rg_str += 'static uint32_t %s[%s];\nstatic uint32_t %s[%s];\nstatic const ac_xpu_dynamic_rgs %s = {\n' % \
									(sdw_read, sdw_perm_arr_sz, sdw_write, sdw_perm_arr_sz, dyn_rgs)
			dyn_rg_str += '\t.rg_start = %s,\n\t.rg_count = %s,\n\t.shadow_read_perm = %s,\n\t.shadow_write_perm = %s,\n};\n\n' % \
									(str(dyn_start), str(dyn_end - dyn_start), sdw_read, sdw_write)
		# This is unnecessary because ACTmeInit correctly updates this field with TME addresses for each XPU, but still we can keep
		# the hardcoded value NULL or 'uninitialized' to avoid confusion when looking at values on TME
		address = xpu.address if options.image not in ['TME_FW'] else "0x00000000"
		#xPU info array
		xpu_info_str += '\t{\n\t\t.soc_addr = %s,\n\t\t.addr = %s,\n\t\t.xpu_id = %s,\n\t\t.idr = %s,\n\t\t.rev = %s,\n\t\t.dyn_rgs = %s,\n\t},\n' % \
								(xpu.address, address, xpu.enum, '{0' + '}', '0', 'NULL' if dyn_rgs is None else '&' + dyn_rgs)

	dbgar_str += '};\nconst uint32_t g_ac_xpu_dbgars_count = ARRAY_SIZE(g_ac_xpu_dbgars);\n\n'
	xpu_info_str += '};\nconst uint32_t g_ac_xpu_infos_count = ARRAY_SIZE(g_ac_xpu_infos);\n'

	addl_info = ''
	if options.image == 'TZ':
		addl_info += "const ac_register_tuple g_ac_ra_xpu_intr_status_reg[RA_XPU_ERR_INT_REG_NUM] = {\n"
		enable = ''
		status = ''
		for intr_reg in info.ra_xpu_interrupt_data["enable_regs"]:
			enable += '\t{\n\t\t.addr = %s,\n\t\t.mask = %s,\n\t},\n' % (intr_reg["addr"],intr_reg["mask"])
		for intr_reg in info.ra_xpu_interrupt_data["status_regs"]:
			status += '\t{\n\t\t.addr = %s,\n\t\t.mask = %s,\n\t},\n' % (intr_reg["addr"],intr_reg["mask"])
		addl_info += status + "};\n\nconst ac_register_tuple g_ac_ra_xpu_intr_enable_reg[RA_XPU_ERR_INT_REG_NUM] = {\n" + enable + "};\n\n"
	if options.image == 'TME_FW':
		addl_info += "ACRegisterTuple g_acXpuTcsrReg[] = {\n"
		bits_to_write = [0,0,0,0]
		bits = info.tcsr_config['GCR']['Bit']
		values = info.tcsr_config['GCR']['Value']
		for each in values:
			if values[each] == 'enabled':
				bits_to_write[bits[each]] = 1
			else:
				bits_to_write[bits[each]] = 0
		value_to_write = hex(int("".join([str(a) for a in reversed(bits_to_write)]), 2))
		addl_info += '	{\n	.addr = %s,\n	.mask = %s,\n	},\n' % (info.tcsr_config['GCR']['addr'], str(value_to_write))
		values = 0
		viocr_contents = info.tcsr_config['VIOCR']
		if 'APSVIOE' in viocr_contents and info.tcsr_config['VIOCR']['APSVIOE'] == 'enabled' :
			values = 1
		if 'APSVIOE_XPU' in viocr_contents and info.tcsr_config['VIOCR']['APSVIOE_XPU'] == 'enabled' :
			values = 1
		if 'APSVIOE_VMIDMT' in viocr_contents and info.tcsr_config['VIOCR']['APSVIOE_VMIDMT'] == 'enabled' :
			values = values + 2
		addl_info += '	{\n	.addr = %s,\n	.mask = %s,\n	},\n};\n\n' % (info.tcsr_config['VIOCR']['addr'], str(hex(values)))
		addl_info += 'const uint32_t g_acXpuTcsrRegCount = ARRAY_SIZE(g_acXpuTcsrReg);\n\n'
		addl_info += 'const uintptr_t g_acXpuTcsrSleAddr = %s;\n' % (info.tcsr_config['DCR']['addr'])
		addl_info += 'const uint32_t g_acXpuTcsrSleEnable = 0x5;\n'
		addl_info += 'const uint32_t g_acXpuTcsrSleDisable = 0xA;\n\n'
		addl_info += 'const bool g_acSilentLoggingEnabled = %s;\n\n' % ('true' if info.sle else 'false')

	return info.getHeader(options.image) + addl_info + dbgar_str + dyn_rg_str + xpu_info_str


def generate_io(options, info):
	ret_str = 'const ac_xpu_info* ac_apu_rpu_partitions_info_group[] =\n{\n%s};\nconst uint8_t u_num_apu_rpu_io_mappings = ARRAY_SIZE(ac_apu_rpu_partitions_info_group);\n'
	xpu_str = ''
	rg_str = ''
	apurpu_strcut_str = ''
	all_arpu = info.getAllApu() + info.getAllRpu()
	for xpu in all_arpu:
		min_address = xpu.rgs[0].addressRange[0]['start']
		max_address = xpu.rgs[-1].addressRange[-1]['end']
		num_partitions = len(xpu.rgs)
		if xpu.type != 'RPU':
			rg_str += 'static ac_partition PARTITION_INFO_' + xpu.enum + '[] =\n{\n'
			num_partitions = 0
			for rg in xpu.rgs:
				num_partitions += len(rg.addressRange)
				for addr in rg.addressRange:
					rg_str += '\t{ .start = %s, .end = %s, .u_rg_num = %s},\n' % (addr['start'], addr['end'], rg.index)
					min_address = addr['start'] if int(addr['start'], 16) < int(min_address, 16) else min_address
					max_address = addr['end'] if int(addr['end'], 16) > int(max_address, 16) else max_address
			rg_str += '};\n\n'
		apurpu_strcut_str += '\t&AC_%s,\n' % xpu.enum
		xpu_str += 'static const ac_xpu_info AC_%s =\n{\n\t.e_xpu_id = %s,\n\t.e_xpu_id_type = %s,\n\t.u_range_start = %s,\n\t.u_range_end = %s,\n\t.u_num_partitions = %s,\n' %\
				(xpu.enum, xpu.enum, 'HAL_XPU2_DEVICE_' + xpu.type, min_address, max_address, str(num_partitions))
		xpu_str += '\t.partition_info = %s,\n};\n\n' % ('PARTITION_INFO_' + xpu.enum if xpu.type == 'APU' else 'NULL')

	ret_str = ret_str % (apurpu_strcut_str)

	return info.getHeader(options.image, True) + rg_str + xpu_str + ret_str

def generate_policy(options, info):
	"""Creates the xPU Static Policy.

	Args:
		options : command line options.
		info : xPUParser object.

	Returns:
		string: The xPU Static Policy C file.
	"""
	cur_policy = get_policy(options, info)
	mem_attrib = info.getXpuAttributes(options.image)
	xpu_str = ''
	global_str = ''
	rg_override_array = ''
	ret_str = mem_attrib['RO'] + 'const ac_xpu_cfg g_ac_xpu_cfg[] = {\n%s};\n' + mem_attrib['RO'] + 'const uint32_t g_ac_xpu_cfg_count = ARRAY_SIZE(g_ac_xpu_cfg);'
	for name, xpu_info in cur_policy.items():
		rg_str = ''
		has_dyn_rg, has_xpresscfg, has_xpu_global_cfg = False, False, False
		if options.image in xpu_info['xPU'].cfg_owner or options.image in xpu_info['xPU'].umr_perm:
			has_xpu_global_cfg = True
		for rg in xpu_info['RGs']:
			if options.image not in rg.profiles:
				pass
			if rg.is_xpresscfg:
				has_xpresscfg = True
			if not rg.static:
				has_dyn_rg = True
			rg_str += write_rg(xpu_info['xPU'], rg)
		#Only add if the xPU has static, dynamic or xpresscfg RGs to program, or xPU global registers to program
		has_static_rgs = rg_str != ''
		if has_static_rgs or has_dyn_rg or has_xpresscfg or has_xpu_global_cfg:
			if options.image == xpu_info['xPU'].rg_override_ee:
				rg_override_array += write_rg_override_array(xpu_info['xPU'])
			if has_static_rgs:
				# If the xPU has no static RGs, we won't create an xPU array for it in the static policy
				xpu_str += write_xpu_header(mem_attrib, xpu_info['xPU'])
				xpu_str = xpu_str % rg_str
			global_str += write_xpu_array(info, options.image,  xpu_info['xPU'], has_static_rgs)
	return info.getHeader(options.image) + info.qad_defines + rg_override_array + xpu_str + (ret_str % global_str)


def main():
	global DEVPRGPOLICY
	parser = argparse.ArgumentParser()

	parser.add_argument("-x", "--xml", type=str, required=True,
	help="Access control XML xPU policy file")
	parser.add_argument("-o", "--out_dir", type=str, required=True,
	help="Output path where files will be generated")
	parser.add_argument("-j", '--json_config', action='append', required=True,
	help='list of paths for config info')
	parser.add_argument("-i", "--image", type=str, required=True, choices=["TME_FW", "xBL_SC", "TZ", "AOP", "MSA", "xBL_SC_DEVPRG"],
	help="Image to generate policy for")
	parser.add_argument("-p", '--vio_patch', type=str, required=False, default="",
	help='Path to silent logs text file to patch policy with')
	parser.add_argument("-w", '--write_patch', action='store_true', required=False, default=False,
	help='Write the created patches to a new XML')

	options = parser.parse_args()
	for path in options.json_config:
		if not os.path.isfile(path):
			raise Exception("Error: File doesn't exist %s" % path)

	if not os.path.exists(options.out_dir):
		try:
			print("DEBUG: Output path does not exist, trying to make it ...")
			os.makedirs(options.out_dir)
			print(f"DEBUG: Output path: {options.out_dir}")
		except:
			raise Exception("Error: Path doesn't exist %s" % options.out_dir)

	if not os.path.isfile(options.xml):
		raise Exception("Error: File doesn't exist %s" % options.xml)

	if options.vio_patch != "":
		if not os.path.isfile(options.vio_patch):
			print("\n-------------------- SILENT LOG FILE MISSING, SKIPPING PATCH --------------------\n")
			options.vio_patch = ""
		else:
			print("\n---------------------- PATCHING AC POLICY WITH SILENT LOGS ----------------------\n")

	if options.image == 'xBL_SC_DEVPRG':
		DEVPRGPOLICY = True
		options.image = 'xBL_SC'

	xpuInfo = xPUParser(options.xml, options.json_config)
	if options.vio_patch != "":
		vioParser = violationParser(options.vio_patch, xpuInfo)
		if options.write_patch:
			vioParser.writePatchedXML()
	policy = generate_policy(options, xpuInfo)
	target_data = generate_target_data(options, xpuInfo)
	xpu_data_header = generate_target_header(options, xpuInfo)
	target_code = generate_target_code(options, xpuInfo)

	file_hash = FileIntegrity()

	file_type = '.c'
	if options.image == 'TZ':
		io = generate_io(options, xpuInfo)
		io_file = os.path.join(options.out_dir, 'ac_apu_rpu_io_mapping.c')
		with open(io_file, 'w+') as f:
			f.write(io)
		file_hash.writeHash(io_file)

	target_data_file = os.path.join(options.out_dir, 'ac_target_data' + file_type)
	with open(target_data_file, 'w+') as f:
		f.write(target_data)
	file_hash.writeHash(target_data_file)

	xpu_data_header_file = os.path.join(options.out_dir, 'ac_xpu_target_info.h')
	with open(xpu_data_header_file, 'w+') as f:
		f.write(xpu_data_header)
	file_hash.writeHash(xpu_data_header_file)

	policy_file = os.path.join(options.out_dir, 'ac_xpu_static_config' + file_type)
	with open(policy_file, 'w+') as f:
		f.write(policy)
	file_hash.writeHash(policy_file)

	target_code_file = os.path.join(options.out_dir, 'ac_target' + file_type)
	with open(target_code_file, 'w+') as f:
		f.write(target_code)
	file_hash.writeHash(target_code_file)

if __name__ == "__main__":
	main()
