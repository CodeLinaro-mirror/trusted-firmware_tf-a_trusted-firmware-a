# # # #
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: ISC
#

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------

import xml.etree.ElementTree as ET
import os
from copy import deepcopy
import re
from datetime import datetime
from DataParsers import jsonConfigParser

xpu_types = {'0' : 'MPU', '1' : 'APU', '2' : 'RPU'}
xpresscfg_milestones = {"TME_ROM" : 0, "TME_FW" : 1, "xBL_SC" : 2, "TZ" : 3}
# SOCCP_QAD_ALLOW is a custom profile for SOCCP MS MPU, which means a policy will be applied by TME_FW
# during SOCCP PIL, which is essentially a milestone and means we don't want the policy programmed at DEFAULT milestone
milestones = ["DEFAULT", "DEBUG_SDI_PASS2", "DEBUG_POLICY_READY", "xBL_SC_EXIT", "SOCCP_QAD_ALLOW"]
all_milestones = ['TME_PBL', 'TME_ROM', 'TME_FW', 'xBL_SC', 'DEBUG_POLICY_READY', 'xBL_SC_EXIT', 'TZ', 'DEBUG_SDI_PASS2']
ee_global_profiles = ['CFGOWNER', 'UMRPERM'] # XPU Global Profiles which may be programmed at specific EEs
ee_global_profiles_milestones = ['TME_FW', 'xBL_SC', 'TZ'] # milestones to apply xPU Global Profiles *IN ORDER OF BOOT MILESTONE*
valid_offsets = [0x01F00000, 0x20000000, 0x0B000000, 0x0ED10000, 0x0C400000, 0x00400000]
ee_rules_dict = {'TME_FW'        : lambda rg: True if 'TME_FW' in rg.profiles else False,
                 'xBL_SC'        : lambda rg: True if 'xBL_SC' in rg.profiles or ('TME_FW' in rg.profiles and (bool(set(['TZ', 'HYP']) & set(rg.wdomains)) or bool(set(['TZ', 'HYP']) & set(rg.rdomains)))) else False,
                 'xBL_SC_DEVPRG' : lambda rg: True if 'xBL_SC' in rg.profiles and ('DEFAULT' in rg.writable_profiles) else False,
                 'TZ'            : lambda rg: True if 'TZ' in rg.profiles else False}

def outline_string(s):
	return f"\n\n{'=' * 80}\n\n{s}\n\n{'=' * 80}\n\n"


def len_correct(val):
	if val == '':
		return val
	if len(val) < 11:	#minimum len is 8 hex bits, 10 including leading 0x
		return "{0:#0{1}x}".format(int(val, 16),10)
	if len(val) < 15:
		return "{0:#0{1}x}".format(int(val, 16),14)
	if len(val) < 19:
		return "{0:#0{1}x}".format(int(val, 16),18)
	return val

####################################################################################################################
class RG:
	def __init__(self, rg=None):
		if rg:
			self.index = rg['index']
			self.order = rg['order']
			self.start = len_correct(rg['start']) if 'start' in rg else None
			self.end = len_correct(rg['end']) if 'end' in rg else None
			self.addressRange = [{'start' : len_correct(each['start']), 'end' : len_correct(each['end'])} for each in rg['AddressRange']] if 'AddressRange' in rg else None
			self.enabled = True if rg['enabled'] == 'true' else False
			self.profiles = [rg['profile']] if isinstance(rg['profile'], str) else rg['profile']
			if not set(['TME_PBL', 'TME_ROM', 'TME_FW', 'xBL_SC', 'MSA']) & set(self.profiles):
				self.profiles = list(set(['TZ']).union(set([each if each != '' else 'TZ' for each in self.profiles])))
			self.is_xpresscfg = True if 'XPRESSCFG' in self.profiles else False
			self.writable_profiles = list(set(self.profiles) - set(['TME_PBL', 'TME_FW', 'TME_ROM', 'xBL_SC', 'TZ', 'XPRESSCFG', 'RG_OVERRIDE']))
			# if there is no milestone present, we will add DEFAULT to apply at cold boot
			if len(set(self.writable_profiles) & set(milestones)) == 0:
				self.writable_profiles.insert(0, 'DEFAULT') # insert at the beginning instead of append for readability
			self.static = False if rg['static'] == 'false' else True
			self.rdomains = rg['rdomains'].replace(' ', '').split(',') if isinstance(rg['rdomains'], str) else rg['rdomains']
			self.wdomains = rg['wdomains'].replace(' ', '').split(',') if isinstance(rg['wdomains'], str) else rg['wdomains']
			self.rvmids = rg['rvmids'].replace(' ', '').split(',') if isinstance(rg['rvmids'], str) else rg['rvmids']
			self.wvmids = rg['wvmids'].replace(' ', '').split(',') if isinstance(rg['wvmids'], str) else rg['wvmids']
			#append to gloabl set self.rdomains + self.wdomains
			#log during gen if any ROT got dropped (wasn't present in the policy)

	def __str__(self):
		ret = 'index: %s,\n' % self.index
		ret += 'order: %s,\n' % self.order
		ret += 'start: %s,\n' % self.start
		ret += 'end: %s,\n' % self.end
		if self.addressRange:
			ret += 'addressRange: %s,\n' % ','.join(['\n\tstart: %s, end: %s' % (each['start'], each['end']) for each in self.addressRange])
		ret += 'enabled: %s,\n' % self.enabled
		ret += 'profiles: %s,\n' % ', '.join(self.profiles)
		ret += 'static: %s,\n' % self.static
		ret += 'rdomains: %s,\n' % ', '.join(self.rdomains)
		ret += 'wdomains: %s,\n' % ', '.join(self.wdomains)
		ret += 'rvmids: %s,\n' % ', '.join(self.rvmids)
		ret += 'wvmids: %s,\n' % ', '.join(self.wvmids)
		return ret

####################################################################################################################

class xPU:
	def __init__(self, xpu_info, name, target_info, d2b_mappings, d2q_mappings):
		self.target_info = target_info
		self.d2b_mappings = d2b_mappings
		self.d2q_mappings = d2q_mappings
		self.fqname = xpu_info['fqname']
		self.name = name
		self.rgs = xpu_info['RGs']
		self.nrg = int(xpu_info['HWConfig']['XPU4_IDR0:nRG'])
		self.type = xpu_types[xpu_info['HWConfig']['XPU_TYPE']]
		self.HWConfig = xpu_info['HWConfig']
		self.enum = xpu_info['enum']
		self.address = len_correct(xpu_info['HWConfig']['FF_ADDRESS'])
		self.dbgar_address = len_correct(str(hex(int(self.address, 16) + 0x304)))

		write_ignore = set(['XPU_RA', 'DYNAMIC_INITIALIZE', 'XPU_DISABLED', 'XPU_ADDRESS_OFFSET', 'RG_OVERRIDE', 'CFGOWNER', 'UMRPERM'])

		global_profiles = self.parse_xpu_global_profiles(xpu_info)

		self.disabled = True if 'XPU_DISABLED'in global_profiles else False
		self.dynamic = True if 'DYNAMIC_INITIALIZE'in global_profiles else False
		self.is_ra = True if 'XPU_RA'in global_profiles else False
		self.profiles = global_profiles.keys()
		self.cfg_owner = global_profiles['CFGOWNER']
		self.umr_perm = global_profiles['UMRPERM']
		self.writable_profiles = list(set(self.profiles) - write_ignore)
		self.rg_override_array = xpu_info['xpresscfg_info']['rg_override_array']
		self.rg_override_ee = xpu_info['xpresscfg_info']['ee']
		self.skip_debugar = True if xpu_info['skip_xpu_debugar'] or self.disabled or self.is_ra else False

	def __str__(self):
		ret = 'fqname: %s\n' % self.fqname
		ret += 'name: %s\n' % self.name
		ret += 'type: %s\n' % self.type
		ret += 'address: %s\n' % self.address
		ret += 'nrg: %s\n' % self.nrg
		ret += 'enum: %s\n' % self.enum
		ret += 'profiles: %s\n' % ' ,'.join(self.profiles)
		ret += 'is_ra: %s\n' % str(self.is_ra)
		ret += 'dynamic: %s\n' % str(self.dynamic)
		ret += 'disabled: %s\n' % str(self.disabled)
		return ret

	def getMinMaxAddressRange(self):
		if self.type == 'MPU':
			return None, None
		min_address = self.rgs[0].addressRange[0]['start']
		max_address = self.rgs[-1].addressRange[-1]['end']
		if self.type != 'RPU':
			for rg in self.rgs:
				for addr in rg.addressRange:
					min_address = addr['start'] if int(addr['start'], 16) < int(min_address, 16) else min_address
					max_address = addr['end'] if int(addr['end'], 16) > int(max_address, 16) else max_address
		return int(min_address, 16), int(max_address, 16)

	def getRGs(self):
		ret = {}
		for rg in self.rgs:
			if rg.index not in ret:
				ret[rg.index] = []
			ret[rg.index].append(rg)
		for rg_index in ret:
			ret[rg_index] = sorted(ret[rg_index], key=lambda x:x.order)
		return ret

	def getPolicyRGs(self, image):
		ret = {}
		for rg in self.rgs:
			try:
				rule_found = ee_rules_dict[image](rg)
				if not rule_found:
					continue
			except KeyError as e:
				raise Exception("ERROR : No policy rules for EE: %s" % image)
			if rg.index not in ret:
				ret[rg.index] = []
			ret[rg.index].append(rg)
		for rg_index in ret:
			ret[rg_index] = sorted(ret[rg_index], key=lambda x:x.order)
		return ret

	def getDynamicRGRange(self, profile):
		start = None
		end = None
		for rg in self.rgs:
			if not rg.static:
				rg_idx = int(rg.index)

				# if non static RG is having XPRESSCFG, we should ignore these RG to make sure every EE has only one entry.
				if 'XPRESSCFG' in rg.profiles:
					break
				if start is None and (profile in rg.profiles or 'ALL_EE' in rg.profiles):
					start = rg_idx
					end = start + 1
				elif profile in rg.profiles or 'ALL_EE' in rg.profiles:
					if rg_idx != end:
						print("profile=%s rg.profiles=%s rg_idx=%s" % (profile, rg.profiles, rg_idx))
						print(rg)
						raise Exception('Found gap in dynamic RGs for xPU: %s' % self.name)
					end += 1
		return start, end

	def getMatchingProfiledRGs(self, profiles, static=None, is_subset=False, is_exact_match=False):
		"""
		gets rgs matching the given profiles

		Args:
			profiles(string or list): A single profile as a string or a list of profiles as strings
			static (bool): If not specified, matches any RG. If True, matches static RGs. If False, matches dynammic (Non-static) RGs
			is_subset (bool): If True, only matches an RG if the given profiles are a subset of the RGs profiles
			is_exact_match (bool): If True, only matches an RG if the given profiles are an exact match of the RGs profiles
		"""
		ret = {}
		check_data = [profiles] if isinstance(profiles, str) else profiles
		all_rgs = self.getRGs()
		for rg_index in all_rgs:
			for rg in all_rgs[rg_index]:
				if static is not None:
					if static and not rg.static:
						continue
					if not static and rg.static:
						continue
				if not bool(set(rg.profiles) & set(check_data)):
					continue
				if is_subset and not bool(set(check_data).issubset(rg.profiles)):
					continue
				if is_exact_match and set(rg.profiles) != set(check_data):
					continue
				if rg.index not in ret:
					ret[rg.index] = []
				ret[rg.index].append(rg)
		return ret

	def getOrderMatchedRG(self, rg_arr, profiles):
		"""
		Returns the first RG policy which matches the given profiles
		"""
		check_data = [profiles] if isinstance(profiles, str) else profiles

		xpresscfg_rg = None
		for rg in rg_arr:
			if rg.is_xpresscfg:
				xpresscfg_rg = rg
			if 'RG_OVERRIDE' in rg.profiles:
				# If there is an RG_OVERRIDE, we expect to see this programmed
				return rg
		if xpresscfg_rg:
			# If there is an XPRESSCFG and no RG_OVERRIDE, then this should be programmed
			return xpresscfg_rg

		for rg in reversed(rg_arr):
			milestone_matched = True
			vals_to_ignore = ['', '0xffffffff']
			if rg.start in vals_to_ignore and rg.end in vals_to_ignore:
				#ideally the rg should be disabled in policy, but for now, this also means disabled
				continue
			if set(rg.profiles).issubset(set(check_data)):
				#if the rg doesn't match the current milestone, continue
				for profile in rg.profiles:
					if profile in all_milestones and profile not in check_data:
						milestone_matched = False
				if milestone_matched:
					return rg
		return None

	def getRGVmids(self, rg):
		vmids = {'read' : rg.rvmids, 'write' : rg.wvmids}
		ret = {'read' : [], 'write' : []}
		name = self.name
		ra_domains = self.target_info['ra_xpu_vmid_mapping']
		for perm in vmids:
			if len(vmids[perm]) == 1 and vmids[perm][0] == '':
				vmids[perm] = []
				continue
			for vmid in vmids[perm]:
				if name in ra_domains and vmid in ra_domains[name]:
					ret[perm].append(ra_domains[name][vmid])
				elif 'VMID_' in vmid:
					ret[perm].append('AC_VMIDMT_BIT' + re.search(r'\d+$', vmid).group())
				# if len(ret[perm]) and ret[perm][-1] == 'AC_VMIDMT_BIT0':
				# 	raise Exception(f"VMID_0 found in xPU: {self.name}. This is not a valid VMID, please use TZ/HYP")
		return ret['read'], ret['write']

	def getRGDomains(self, rg):
		read = [item for each in rg.rdomains for item in self.d2b_mappings[each]]
		write = [item for each in rg.wdomains for item in self.d2b_mappings[each]]
		if len(read) == 1 and read[0] == 'AC_DOMAIN_NONE':
			read = []
		if len(write) == 1 and write[0] == 'AC_DOMAIN_NONE':
			write = []
		return read, write

	def get_umr_perms(self, image):
		"""Gets umr_perm for a particular for a particular EE

		Args:
			image (string): EE {TME_FW/xBL_SC/TZ} to apply the umr_perm

		Details:
			Logic is written to support ER xPUs which will have only valid
			domain names which can be mapped to QAD bits, or RA xPUs which may
			have a combination of domain names and VMID bits specified.
		"""
		ret = []
		if image in self.umr_perm:
			for perm in self.umr_perm[image]:
				if 'VMID' in perm:
					ret.append('AC_VMIDMT_BIT' + re.search(r'\d+$', perm).group())
				else:
					bits = self.d2b_mappings[perm]
					for bit in bits:
						ret.append(bit)
		return ret

	def getRGAttribute(self, device, attrib):
		rg_regs = self.target_info['DEBUG']['rg_regs']
		if attrib not in list(rg_regs.values()):
			raise Exception('unknown rg attrib')
		for reg in rg_regs:
			if rg_regs[reg] == attrib:
				return device[int(reg)]
		return None

	def parse_xpu_global_profiles(self, xpu_info):
		"""Parses xPU Global data entries from XML.

		Args:
			xpu_info (xPUParser): xPUParser object.

		Returns:
			A dict mapping global profiles to which EE will program them and with which values.

		Raises:
			ValueError: An invalid profile/milestone/value combo was specified in policy.
		"""
		global_profiles = {profile: {} for profile in ee_global_profiles}
		for entry in xpu_info['Global']:
			profile = entry['profile'].replace(' ', '').split(',')
			value = entry.get('value', '').replace(' ', '').split(',')
			if not profile:
				continue
			if set(profile) & set(ee_global_profiles):
				# if the current profile is in ee_global_profiles, then there is some additional parsing to do to determine what values to write at which milestone
				milestone = list(set(profile) & set(ee_global_profiles_milestones))
				if not milestone:
					milestone = ['TZ']
				elif len(milestone) > 1:
					raise ValueError(f'Multiple milestones specified for {self.name}: {profile}. Only one milestone should be specified for xPU EE Global Profiles.')
				profile = [item for item in profile if item not in ee_global_profiles_milestones]
				if len(profile) != 1:
					raise ValueError(outline_string(f'INVALID POLICY: The following xPU EE Global Profiles must be specified as their own entry:\n'
									f'{self.name}: profile={profile} milestone={milestone}.\n'
									f'EE Global Profiles: {ee_global_profiles} (1 only per entry). Valid milestones {ee_global_profiles_milestones}.'))
				profile, milestone = profile[0], milestone[0]
				if profile == 'CFGOWNER':
					if len(value) != 1:
						raise ValueError(outline_string(f'INVALID POLICY: Only one CFGOWNER may be specified.\n'
														f'{self.name}: profile={profile} milestone={milestone} value={value}'))
					valid_cfg_owners = [key for key in self.d2q_mappings.keys() if key not in {'TZ', 'HYP'}]
					if value[0] not in valid_cfg_owners:
						raise ValueError(outline_string(f'INVALID POLICY: Incorrect CFGOWNER specified.\n'
														f'{self.name}: profile={profile} milestone={milestone} value={value}\n'
														f'Valid CFGOWNER values: {valid_cfg_owners}'))

				# Update the dictionary so that subsequent EEs will know the programming of previous EEs.
				# 	Ex: If TME sets unmapped permissions to be 'TME_FW', then xBL and TZ both need to know not to change this
				# If a previous EE sets it, it can still be valid for a policy to update it in the next EE.
				# 	Ex: If TME sets unmapped permissions to be 'TME_FW, policy in xBL could change unmapped permissions to be "TME_FW,MSA",
				# 		so TZ should expect to see the most recently programmed value from xBL ("TME_FW,MSA")
				# Note: This is needed for both code gen and xPU validation, as both should know if there are any previously programmed values
				milestone_index = ee_global_profiles_milestones.index(milestone)
				global_profiles[profile].update({m: value for m in ee_global_profiles_milestones[milestone_index:]})
			else:
				# since the profile is not in ee_global_profiles there is no additional data to parse
				# (Basic profiles such as XPU_DISABLED have no special data to interpret from the policy)
				global_profiles.update({each: '' for each in profile})

		return global_profiles


####################################################################################################################

class xPUParser:
	"""Parser for xPU related configuration data

	"""
	def __init__(self, xml, json_paths):
		"""Initializes the instance based on the specified target and configuration files

		Args:
			xml (string): path to <target>AccessControl.xml (IPCAT XPU config)
			json_paths (string): path to each json config file
		"""
		if not os.path.isfile(xml):
			raise Exception("Error: XML file doesn't exist: " + xml)

		self.root = ET.parse(xml).getroot()
		self.xml_path = xml
		self.max_qad = None
		self.qad_mask = (1 << 31) | (1 << 30)
		#init json info
		#json data should be parsed only by this class into class members,
		#not used directly by clients
		json_config = jsonConfigParser(json_paths)

		#init static vars
		for key, val in json_config.get_json().items():
			setattr(self, key, val)

		#get xpu hw rev info
		self.xpu_rev = {}
		self.xpu_rev["major"], self.xpu_rev["minor"], self.xpu_rev["step"] = [int(n) for n in self.xpu_rev_raw.split('.')]

		#Parse xPU data
		self.xPUS = {}
		for xpu in self.root:
			if xpu.tag == 'Version':
				continue
			xpu_info = {'HWConfig' : {}, 'Global' : [], 'RGs' : [], 'xpresscfg_info' : {"ee" : "", "rg_override_array" : []}}
			for each in xpu.find('HWConfig'):
				for param in each:
					xpu_info['HWConfig'].update({param.attrib['name'] : param.attrib['value']})
			if self.use_clean_names:
				xpu_name = xpu.attrib['name']
			else:
				xpu_name = xpu.attrib['fqname']
				xpu_name = xpu_name.replace(".", "_")
				if xpu_name[-5:] == '_XPU4':
					xpu_name = xpu_name[:-5]
			xpu_prefixes_to_ignore = ['DIE_0', 'DIE_1']
			if any(xpu_name.startswith(prefix) for prefix in xpu_prefixes_to_ignore):
				continue
			for rg in xpu.find('SWConfig'):
				if rg.tag in ['PolicyParameters']:
					continue
				if rg.tag == 'Global':
					xpu_info['Global'].append(deepcopy(rg.attrib))
				else:
					cur_rg = deepcopy(rg.attrib)
					cur_rg.update(rg.find('XPU_RGn_FLAGS').attrib)
					for attrib in cur_rg:
						cur_rg[attrib] = cur_rg[attrib].replace(' ', '').split(',') if ',' in cur_rg[attrib] else cur_rg[attrib]
					if xpu.tag != 'MPU':
						cur_rg['AddressRange'] = []
						arpu_ranges = [deepcopy(arpu_range.attrib) for arpu_range in rg.find('AddressRange')]
						for each in arpu_ranges:
							if each['end'][-1:].lower() == 'f':
								each['end'] = str(hex(int(each['end'], 16) + 1))
							cur_rg['AddressRange'].append(deepcopy(each))
						#Some APUs are missing RG info, ignore these and proceed but should be addressed by AC Admins
						if len(cur_rg['AddressRange']) == 0:
							continue
					xpu_info['RGs'].append(RG(cur_rg))
				if 'RG_OVERRIDE' in xpu_info['RGs'][-1].profiles:
					if len(set(xpu_info['RGs'][-1].profiles) & set(xpresscfg_milestones)) != 1:
						raise Exception(f"Improper Policy for {xpu_name}: Expected a valid XPRESSCFG milestone to listed in the above RG's profiles. RG {xpu_info['RGs'][-1]}")
					xpu_info['xpresscfg_info']['rg_override_array'].append(xpu_info['RGs'][-1].index)
					for profile in xpu_info['RGs'][-1].profiles:
						if profile not in xpresscfg_milestones:
							continue
						if not xpu_info['xpresscfg_info']['ee'] or (xpresscfg_milestones[profile] < xpresscfg_milestones[xpu_info['xpresscfg_info']['ee']]):
							xpu_info['xpresscfg_info']['ee'] = profile if profile != 'TME_ROM' else 'TME_FW'

			xpu_info['name'] = xpu.attrib['name']
			xpu_info['fqname'] = xpu.attrib['fqname']
			if xpu_name not in self.xpu_names:
				raise Exception('xPU name %s not found in common_info.json -> xpu_names. Please add it to the list.' %xpu_name)
			xpu_info['enum'] = self.xpu_names[xpu_name]
			xpu_info['skip_xpu_debugar'] = True if xpu_info['name'] in self.skip_xpu_debugar else False
			self.xPUS[xpu_name] = xPU(xpu_info, xpu_name, self.xPU, self.domain_to_bit_mappings, self.domain_to_qad_mappings)
			if self.max_qad is None and not self.xPUS[xpu_name].is_ra:
				self.max_qad = int(xpu_info['HWConfig']['XPU4_IDR2:nQAD'])
				self.domain_to_bit_mappings['ALL_ROT'] = [key for key, val in self.qad_bit_to_val_mappings.items() if val < self.max_qad]
				for val in self.domain_to_bit_mappings['ALL_ROT']:
					self.qad_mask |= 1 << self.qad_bit_to_val_mappings[val]
				#remove the debug QAD from ALL_ROT
				self.domain_to_bit_mappings['ALL_ROT'] = [each for each in self.domain_to_bit_mappings['ALL_ROT'] if each != 'AC_DOMAIN_DEBUG_BIT']
				self.qad_mask = hex(self.qad_mask)
			elif self.max_qad != int(xpu_info['HWConfig']['XPU4_IDR2:nQAD']) and not self.xPUS[xpu_name].is_ra:
				raise Exception("MAX_QAD Value changes per xPU, not supported by AC")

	def getxPUNameByAddr(self, address):
		for each in self.xPUS:
			if int(self.xPUS[each].address, 16) == int(address, 16):
				return each
		return None

	def getHeader(self, image, arpu = False):
		ret = self.autogen_header
		if image == 'TZ':
			if arpu:
				ret += self.apurpu_include
			else:
				ret += self.file_include_tz
		elif image == 'xBL_SC':
			ret += self.file_include_xbl
		elif image == "TME_FW":
			ret += self.file_include_tmefw
		else:
			ret += self.file_include_default
		ret += self.pragma_defs
		return ret

	def getAllxPU(self, image=None):
		ret = []
		for xpu in self.xPUS:
			enum = self.xPUS[xpu].enum
			if image is not None and enum in self.EE_exclusive and image not in self.EE_exclusive[enum]:
				continue
			ret.append(self.xPUS[xpu])
		return ret

	def getAllERxPU(self):
		ret = []
		for xpu in self.xPUS:
			if not self.xPUS[xpu].is_ra:
				ret.append(self.xPUS[xpu])
		return ret

	def getAllRAxPU(self, image=None):
		ret = []
		for xpu in self.xPUS:
			enum = self.xPUS[xpu].enum
			if image is not None and enum in self.EE_exclusive and image in self.EE_exclusive[enum]:
				continue
			if self.xPUS[xpu].is_ra:
				ret.append(self.xPUS[xpu])
		return ret

	def getAllxPUType(self, xpu_type):
		if xpu_type not in list(xpu_types.values()):
			raise Exception("ERROR : Unknown xPU type: %s" % xpu_type)
		return [self.xPUS[each] for each in self.xPUS if self.xPUS[each].type == xpu_type]

	def getAllApu(self, image=None):
		#TODO: filter image xPUs
		return self.getAllxPUType('APU')

	def getAllRpu(self, image=None):
		#TODO: filter image xPUs
		return self.getAllxPUType('RPU')

	def getAllMpu(self, image=None):
		#TODO: filter image xPUs
		return self.getAllxPUType('MPU')

	def getxPUByName(self, names):
		"""
		Returns:
			A list of xPU objects
		"""
		ret = [names] if isinstance(names, str) else names
		for xpu in self.xPUS:
			if self.xPUS[xpu].enum in ret:
				ret[ret.index(self.xPUS[xpu].enum)] = self.xPUS[xpu]
			if self.xPUS[xpu].name in ret:
				ret[ret.index(self.xPUS[xpu].name)] = self.xPUS[xpu]
		# TODO: validate this returns an array of XPUs (no strings)
		return ret

	def getXpuAttributes(self, image):
		if image not in self.attributes:
			raise Exception("ERROR : Unknown image type: %s" % image)
		return self.attributes[image]

	def supportsXpuRev(self, rev):
		"""
		Returns True if given revision string is supported by XPU HW

		Args:
			rev (string): xPU rev string (ex: "4.2.0" means major=4, minor=2, step=0)
		"""
		major, minor, step = [int(n) for n in rev.split('.')]
		if self.xpu_rev['major'] < major:
			return False
		elif self.xpu_rev['major'] == major and \
			self.xpu_rev['minor'] <  minor:
			return False
		elif self.xpu_rev['major'] == major and \
			self.xpu_rev['minor'] == minor and \
			self.xpu_rev['step']  <  step:
			return False
		else:
			return True

	def supportsWAs(self):
		#first check current data and cutoff
		day = self.xPU_WA_Cutoff['day']
		month = self.xPU_WA_Cutoff['month']
		year = self.xPU_WA_Cutoff['year']

		if 'TBD' in set([day, month, year]):
			if len(set([day, month, year])) != 1:
				raise Exception(f"Invalid timeline for xPU_WA_Cutoff, please correct this in target_info.json: {day} {month} {year} ")
			return True

		time_format = '%m/%d/%Y'
		date_check = '/'.join([month, day, year])
		cutoff_date = datetime.strptime(date_check, time_format)
		wa_supported = True if datetime.today() < cutoff_date else False

		if not wa_supported:
			print('\n\n\n**********************************************************\n\n\n')
			print('WA_PRE_ES FLAG NOT ENABLED DUE TO CUTOFF DATE IN JSON')
			print('\n\n\n**********************************************************\n\n\n')

		return wa_supported

################### DEVICE SPECIFIC FUNCTIONS MEANT FOR T32 INTERFACE ###########################################
	def convertDomainsToBitMapping(self, domains):
		"""
		converts domain names to corresponding bit mappings defined in C source files

		Args:
			domains(list or string): list individual string name of domains for which to get associated bit mappings
		"""
		bit_mappings = ''
		# if there is only 1 domain it will be string, but we expect a list
		if isinstance(domains, str):
			domains = [domains]
		first = True
		for domain in domains:
			if first == False:
				bit_mappings += ' | '
			bit_mappings += ' | '.join(self.common_info['domain_to_bit_mappings'][domain])
			first = False
		return bit_mappings

	def getRGAttribute(self, device, attrib):
		"""
		Gets a particular atributes of an xPU rg. i.e. Start Address, read perms, etc.

		Args:
			device: T32 RG instance
			attrib (string): RG Attribute (Start, End, Read, Write, etc)
		"""
		rg_regs = self.rg_regs
		if attrib not in list(rg_regs.values()):
			raise Exception('unknown rg attrib')
		for reg in rg_regs:
			if rg_regs[reg] == attrib:
				return device[int(reg)]
		return None

	def rgIsEnabled(self, device):
		"""
		Returns true if the rg is enabled else false

		Args:
			device: T32 RG instance
		"""
		return bool(self.getRGAttribute(device, 'enable'))

	def deviceToRG(self, xpu, rg_index, device):
		"""
		Converts register values from device to RG values

		Args:
			xpu: (xPU) xPU instance which we are interpreting values from
			rg_index: (int) rg index which we are interepreting values from
			device: (array) array of register values read from the device

		Returns:
			RG object with values interpretted from device

		"""
		rg_regs = self.rg_regs
		domain_to_value = self.domain_to_value
		ret_rg = RG()

		#init info that we can't get from device
		ret_rg.index = rg_index
		ret_rg.order = '0'
		ret_rg.static = False
		ret_rg.profiles = []
		ret_rg.rvmids = []
		ret_rg.wvmids = []

		#init lists to be populated later
		ret_rg.rdomains = []
		ret_rg.wdomains = []
		ret_rg.lock = []
		ret_rg.start = '0x0'
		ret_rg.end = '0x0'
		ret_rg.addressRange = None

		for reg in rg_regs:
			idx = int(reg)
			#TODO: Check WOWP
			if rg_regs[reg] == 'enable':
				ret_rg.enabled = True if bool(device[idx]) else False
			elif rg_regs[reg] == 'start':
				if xpu.type != 'MPU' and ret_rg.addressRange is None:
					ret_rg.addressRange = self.rgs[rg_index].addressRange
				else:
					ret_rg.start = hex(device[idx] | device[idx - 1])
					if ret_rg.start == '0xfffff000':
						ret_rg.enabled = False
						return ret_rg
			elif rg_regs[reg] == 'end':
				if xpu.type != 'MPU' and ret_rg.addressRange is None:
					ret_rg.addressRange = self.rgs[rg_index].addressRange
				else:
					ret_rg.end = hex(device[idx] | device[idx - 1])
					if ret_rg.end == '0xfffff000':
						ret_rg.enabled = False
						return ret_rg
			elif rg_regs[reg] == 'rdomains' or rg_regs[reg] == 'wdomains' or rg_regs[reg] == 'lock':
				for qad_val in domain_to_value:
					if (domain_to_value[qad_val] & device[idx]) == domain_to_value[qad_val]:
						if rg_regs[reg] == 'rdomains':
							ret_rg.rdomains.append(domain_to_value[qad_val])
						elif rg_regs[reg] == 'wdomains':
							ret_rg.wdomains.append(domain_to_value[qad_val])
						else:
							ret_rg.lock.append(domain_to_value[qad_val])
		return ret_rg

	def compareDeviceRG(self, device, rg):
		"""
		Compares the RG info read on device to the RG info from the policy XML

		Args:
			device: T32 RG instance
			rg: RG information from the policy
		"""
		if (rg.start == '0xffffffff' and rg.end == '0xffffffff'):
			return True

		rg_regs = self.rg_regs
		domain_to_value = self.domain_to_value
		rg_profiles = [rg.profiles] if isinstance(rg.profiles, str) else rg.profiles
		match = True
		mismatch_regs = []
		for reg in rg_regs:
			idx = int(reg)
			cur_case = False
			if rg_regs[reg] =='wowp':
					check = True if 'WP' in rg_profiles else False
					cur_case = True if check == bool(device[idx]) else False
			elif rg_regs[reg] == 'enable':
					cur_case = True if bool(device[idx]) else False
			elif rg_regs[reg] == 'start':
					device[idx] = device[idx] | device[idx - 1]
					cur_case = True if device[idx] == device[idx] else False
					if not cur_case:
						cur_case = True if (int(rg.start,16) - device[idx]) in valid_offsets else False
			elif rg_regs[reg] == 'end':
					device[idx] = device[idx] | device[idx - 1]
					cur_case = True if device[idx] == device[idx] else False
					if not cur_case:
						cur_case = True if (int(rg.end,16) - device[idx]) in valid_offsets else False
			elif rg_regs[reg] == 'rdomains' or rg_regs[reg] == 'wdomains':
					check = ['']
					rg_policy = rg.rdomains if rg_regs[reg] == 'rdomains' else rg.wdomains
					if 'ALL_ROT' in rg_policy:
						rg_policy += self.domain_to_bit_mappings['ALL_ROT']
						rg_policy.remove('ALL_ROT')
					# TODO: add [r/w]vmids permissions to rg_policy. We are making assumption on correctness of policy (each policy should have only domains or vmids specified)

					for qad_val in domain_to_value:
						if (domain_to_value[qad_val] & device[idx]) == domain_to_value[qad_val]:
							check.append(domain_to_value[qad_val])
							if '' in check: check.remove('')
					#if rg.rvmids or rg.wvmids:
					#	print(f"{rg.rvmids} {rg.wvmids}")
					#	print(f'{check=} {rg_policy=}')
					cur_case = True if set(check) == set(rg_policy) else False
			elif rg_regs[reg] == 'lock':
					cur_case = True
			else:
					raise Exception('oops')
			if not cur_case:
				match = False
				mismatch_regs.append(rg_regs[reg])
		if not match:
				print('Device policy mismatch')
				print("XML (IPCAT) Policy")
				print(rg)
				print("Device RG registers")
				print([hex(each) for each in device])
				print('Mismatched registers:')
				print(mismatch_regs)

		return match
