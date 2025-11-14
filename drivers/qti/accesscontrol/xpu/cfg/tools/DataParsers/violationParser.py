# # # #
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: ISC
#

####################################################################################################################
# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------

import xml.etree.ElementTree as ET
import os
from copy import deepcopy
import re

from DataParsers import xPUParser, len_correct

xpresscfg_milestones = {"TME_ROM" : 0, "TME_FW" : 1, "xBL_SC" : 2, "TZ" : 3}
UINT64_ALIGN = 0xFFFFFFFFFFFFFFFFFFFFF000
SIZE_4KB = 0x1000

####################################################################################################################
class policyPatch:
	def __init__(self, patch):
		# Expected format [xPU-Name, rg_index, start_address, end_address, is_patch]
		self.xpu_name = patch[0]
		self.rg = patch[1]
		self.start_address = patch[2]
		self.end_address = patch[3]
		self.is_patch = patch[4]


	def __str__(self):
		ret = 'xpu_name: %s,\n' % self.xpu_name
		ret += 'start_address: %s,\n' % len_correct(hex(self.start_address))
		ret += 'end_address: %s,\n' % len_correct(hex(self.end_address))
		ret += 'is_patch: %s,\n' % self.is_patch
		return ret

####################################################################################################################
class xPUViolation:
	def __init__(self, vio, qad_values, domain_to_bit_mappings):
		# Expected format [xPU-Addr, xPU-Name, Address, Error, MultiErr, R/W, Inst, Burst, BID, MID, PID, QAD-ID, NSSignal, ErrCount]
		self.xpu_address = len_correct(str(hex(int(vio[0], 16))))
		self.name = vio[1]
		self.address = len_correct(str(hex(int(vio[2], 16))))
		self.error = vio[3]
		self.is_multi = True if vio[4] == '1' else False
		self.is_write = True if vio[5] == 'Write' else False
		self.inst = int(vio[6], 16)
		self.burst = int(vio[7], 16)
		self.bid = int(vio[8], 16)
		self.mid = int(vio[9], 16)
		self.pid = int(vio[10], 16)

		self.qad = ''
		if int(vio[11], 16) == 0:
			self.qad = 'TZ' if vio[12] == '0x0' else 'HYP'
		elif qad_values and domain_to_bit_mappings:
			for key, val in qad_values.items():
				if val != int(vio[11], 16):
					continue
				for domain, bit in domain_to_bit_mappings.items():
					if key in bit and domain != 'ALL_ROT':
						self.qad = domain
		else:
			raise Exception("Cannot parse the given silent log")

		if self.qad == '':
			raise Exception("Cannot parse the QAD in the given silent log")
		self.count = int(vio[13], 16)

	def __str__(self):
		ret = 'xpu_address: %s,\n' % self.xpu_address
		ret += 'name: %s,\n' % self.name
		ret += 'address: %s,\n' % self.address
		ret += 'error: %s,\n' % self.error
		ret += 'is_multi: %s,\n' % self.is_multi
		ret += 'is_write: %s,\n' % self.is_write
		ret += 'inst: %s,\n' % self.inst
		ret += 'burst: %s,\n' % self.burst
		ret += 'BID: %s,\n' % self.bid
		ret += 'MID: %s,\n' % self.mid
		ret += 'PID: %s,\n' % self.pid
		ret += 'QAD: %s,\n' % self.qad
		ret += 'count: %s\n' % self.count
		return ret

####################################################################################################################

class violationParser:
	def __init__(self, logs, parser):
		violations = {}
		self.parser = parser
		#Convert each violation to its own class object
		with open(logs, 'r') as f:
			lines = f.readlines()
			start_idx = -1
			#find starting index
			for i in range(0, len(lines)):
				if 'xPU-Name' in lines[i]:
					start_idx = i + 2
					break
			if start_idx == -1:
				raise Exception("Couldnt find start of silent logs, please check the formatting")
			for i in range(start_idx, len(lines)):
				if lines[i] == '\n':
					break
				raw_data = lines[i].split('|')
				data = [each.strip() for each in raw_data][1:-1]	#ignore first and last in array, they will always be empty due to splitting on '|'
				data[1] = parser.getxPUNameByAddr(data[0])
				if not data[1] or int(data[2], 16) == 0:
					print(f"Cannot find xPU name via address {data[0]} in logs or invalid violation address, skipping this entry")
					continue
				cur_vio = xPUViolation(data, parser.qad_bit_to_val_mappings, parser.domain_to_bit_mappings)
				if cur_vio.name not in violations:
					violations[cur_vio.name] = []
				violations[cur_vio.name].append(cur_vio)

		#Iterate over each and create/patch the policy
		#To allow the access described by the violation
		patches = []
		for xpu_name in violations:
			cur_xpu = parser.xPUS[xpu_name]
			for vio in violations[xpu_name]:
				found_static_rg = False
				found_free_static_rg = False
				in_protected_range = False
				updated_patch = False

				vio_address = self.getSoCVioAddress(vio, cur_xpu, parser)

				#check if any existing patch covers this address
				for patch in patches:
					if patch.xpu_name != xpu_name or patch.start_address > vio_address or patch.end_address < vio_address:
						continue
					#found an existing patch that matches this violations address range
					#just update the rg and continue
					patch_idx = parser.xPUS[xpu_name].rgs.index(patch.rg)
					cur_rdomains = parser.xPUS[xpu_name].rgs[patch_idx].rdomains + [vio.qad]
					parser.xPUS[xpu_name].rgs[patch_idx].rdomains = list(set(cur_rdomains))
					if vio.is_write:
						cur_wdomains = parser.xPUS[xpu_name].rgs[patch_idx].wdomains + [vio.qad]
						parser.xPUS[xpu_name].rgs[patch_idx].wdomains = list(set(cur_wdomains))
					updated_patch = True

				if updated_patch: continue

				#Find the static policy to patch
				for rg in cur_xpu.rgs:
					if found_static_rg: break
					if rg.start and rg.end and vio_address >= int(rg.start, 16) and vio_address < int(rg.end, 16):
						idx, is_patch = self.patchRG(rg, xpu_name, vio)
						patches.append(policyPatch([xpu_name, cur_xpu.rgs[idx], int(cur_xpu.rgs[idx].start, 16), int(cur_xpu.rgs[idx].end, 16), is_patch]))
						found_static_rg = True
					elif rg.addressRange:
						for each in rg.addressRange:
							if vio_address >= int(each['start'], 16) and vio_address <= int(each['end'], 16):
								idx, is_patch = self.patchRG(rg, xpu_name, vio)
								addr_rang_idx = rg.addressRange.index(each)
								patches.append(policyPatch([xpu_name, cur_xpu.rgs[idx], int(cur_xpu.rgs[idx].addressRange[addr_rang_idx]['start'], 16), int(cur_xpu.rgs[idx].addressRange[addr_rang_idx]['end'], 16), is_patch]))
								found_static_rg = True
								break
				if found_static_rg: continue

				#skip any range that is part of protected ranges
				for each in parser.xpu_protected_ranges:
					if in_protected_range: break
					if each['xpuId'] != cur_xpu.enum: continue
					for addr_range in each["ranges"]:
						if int(addr_range['start'] , 16) <= vio_address and int(addr_range['end'] , 16) >= vio_address:
							in_protected_range = True
							break
				if in_protected_range:
					print(f"\n\nSkipping current violation {vio.name} as it is in the xPU Protected Ranges Array, addressing may cause dynamic conflicts...")
					print(vio)
					continue

				#If we didn't find a static RG for an APU/RPU, we need to fail here
				if cur_xpu.type != 'MPU':
					raise Exception(f"Cannot find matching RG in {vio.name} with calculated address {vio_address}")

				#if we can't find a match for an MPU, try and find an unused RG
				for rg in reversed(cur_xpu.rgs):
					if (rg.start == '' or rg.start == '0xffffffff') and rg.static:
						if 'XPRESSCFG' in rg.profiles and rg.index in cur_xpu.rg_override_array:
							continue
						idx, is_patch = self.patchRG(rg, xpu_name, vio)
						#Because this falls in unmapped space, we need to add TZ + HYP to the r/w domains
						umr_perm = cur_xpu.umr_perm if cur_xpu.umr_perm else ['TZ', 'HYP']
						cur_xpu.rgs[idx].rdomains = list(set(cur_xpu.rgs[idx].rdomains + umr_perm))
						cur_xpu.rgs[idx].wdomains = list(set(cur_xpu.rgs[idx].wdomains + umr_perm))
						cur_xpu.rgs[idx].start = str(hex(vio_address & UINT64_ALIGN))
						cur_xpu.rgs[idx].end = str(hex((vio_address & UINT64_ALIGN) + SIZE_4KB))
						patches.append(policyPatch([xpu_name, cur_xpu.rgs[idx], int(cur_xpu.rgs[idx].start, 16), int(cur_xpu.rgs[idx].end, 16), is_patch]))
						found_free_static_rg = True
						break

				if not found_static_rg and not found_free_static_rg:
					raise Exception("Cannot find matching RG or free static RG")
		self.patches = patches


	def patchRG(self, rg, xpu_name, vio):
		idx = None
		cur_xpu = self.parser.xPUS[xpu_name]
		is_patch = True
		if 'XPRESSCFG' in rg.profiles:
			#see if we can patch exsisting override
			if rg.index in cur_xpu.rg_override_array:
				for each in cur_xpu.rgs:
					if 'RG_OVERRIDE' in each.profiles and each.index == rg.index:
						idx = cur_xpu.rgs.index(each)
						if not (set(cur_xpu.rgs[idx].profiles) & set(['xBL_SC', 'TME_FW', 'TME_ROM'])):
							cur_xpu.rgs[idx].profiles = [each for each in cur_xpu.rgs[idx].profiles if each != 'TZ'] + ['xBL_SC']
						break
			if idx is None:
				is_patch = False
				idx = cur_xpu.rgs.index(rg) + 1
				cur_xpu.rgs.insert(idx, deepcopy(rg))
				cur_xpu.rgs[idx].profiles = [each for each in rg.profiles if each not in ['XPRESSCFG', 'TZ', 'xBL_SC', 'TME_FW']]
				new_profile = ['TME_FW'] if vio.qad in ['TME_FW', 'TME_ROM'] else ['xBL_SC']
				cur_xpu.rgs[idx].profiles += ['RG_OVERRIDE'] + new_profile
				cur_xpu.rgs[idx].is_xpresscfg = False
				cur_xpu.rg_override_array.append(rg.index)
				for xpcfg_profile in cur_xpu.rgs[idx].profiles:
					if xpcfg_profile not in xpresscfg_milestones:
						continue
					if not cur_xpu.rg_override_ee or (xpresscfg_milestones[xpcfg_profile] < xpresscfg_milestones[cur_xpu.rg_override_ee]):
						cur_xpu.rg_override_ee = xpcfg_profile
		else:
			idx = cur_xpu.rgs.index(rg)
			if not (set(cur_xpu.rgs[idx].profiles) & set(['xBL_SC', 'TME_FW', 'TME_ROM'])):
				cur_xpu.rgs[idx].profiles = [each for each in cur_xpu.rgs[idx].profiles if each != 'TZ'] + ['xBL_SC']

		cur_rdomains = self.parser.xPUS[xpu_name].rgs[idx].rdomains + [vio.qad]
		self.parser.xPUS[xpu_name].rgs[idx].rdomains = [each for each in set(cur_rdomains) if each in self.parser.domain_to_qad_mappings or each in ['ALL_ROT']]
		if vio.is_write:
			cur_wdomains = self.parser.xPUS[xpu_name].rgs[idx].wdomains + [vio.qad]
			self.parser.xPUS[xpu_name].rgs[idx].wdomains =  [each for each in set(cur_wdomains) if each in self.parser.domain_to_qad_mappings or each in ['ALL_ROT']]
		cur_xpu.rgs[idx].writable_profiles = list(set(cur_xpu.rgs[idx].writable_profiles + ['WA_PRE_ES']))
		cur_xpu.rgs[idx].profiles = list(set(cur_xpu.rgs[idx].profiles + ['WA_PRE_ES']))
		return idx, is_patch

	def writePatchedXML(self):
		for each in self.patches:
			patched_xml = False
			for xpu in self.parser.root:
				if patched_xml:
					break
				if xpu.tag == 'Version' or xpu.attrib['name'] != each.xpu_name:
					continue
				sw_config = xpu.find('SWConfig')
				idx = 0
				for rg in sw_config:
					if rg.tag in ['PolicyParameters', 'Global']:
						continue

					if not each.is_patch and rg.attrib['index'] == each.rg.index:
						temp = deepcopy(sw_config[idx])
						if not each.rg.addressRange:
							temp.attrib['start'] = each.rg.start
							temp.attrib['end'] = each.rg.end
						temp.attrib['index'] = each.rg.index
						temp.attrib['profile'] = ','.join(each.rg.profiles)
						temp.attrib['rdomains'] = ','.join(each.rg.rdomains)
						temp.attrib['wdomains'] = ','.join(each.rg.wdomains)
						sw_config.insert(idx, temp)
						patched_xml = True
						break

					raw_profiles = deepcopy(rg.attrib['profile'])
					cur_profiles = list(set(['WA_PRE_ES'] + (raw_profiles.replace(' ', '').split(',') if ',' in raw_profiles else [raw_profiles])))
					if each.is_patch and rg.attrib['index'] == each.rg.index and (set(cur_profiles) & set(each.rg.profiles)) == set(each.rg.profiles):
						if not each.rg.addressRange:
							rg.attrib['start'] = each.rg.start
							rg.attrib['end'] = each.rg.end
						rg.attrib['index'] = each.rg.index
						rg.attrib['profile'] = ','.join(each.rg.profiles)
						rg.attrib['rdomains'] = ','.join(each.rg.rdomains)
						rg.attrib['wdomains'] = ','.join(each.rg.wdomains)
						patched_xml = True
						break
					idx += 1
				if not patched_xml:
					raise Exception(f"Failed to patch XML for vio: {each}")

		tree = ET.ElementTree(self.parser.root)
		ET.indent(tree, space="\t", level=0)
		patched_out_path = self.parser.xml_path[:-4] + 'Patched.xml'
		tree.write(patched_out_path, encoding="utf-8")
		print(f'\n\nPlaced patches in an updated XML - {patched_out_path}\n\n')

	def getSoCVioAddress(self, vio, cur_xpu, parser):
		#convert violation address to correct SOC address
		vio_address = int(vio.address, 16)
		if cur_xpu.type != 'MPU':
			min_addr, max_add = cur_xpu.getMinMaxAddressRange()
			if vio_address < min_addr or vio_address > max_add:
				vio_address = int(vio.address, 16) | int(cur_xpu.rgs[0].addressRange[0]['start'], 16)
		else:
			for each in parser.xpu_hw_addr_mask:
				if cur_xpu.enum in each['xpuId']:
					vio_address += int(each['offset'], 16)
					break
		return vio_address