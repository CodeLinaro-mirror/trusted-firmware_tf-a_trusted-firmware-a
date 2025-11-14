# # # #
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: ISC
#

"""
Helper functions utilized by both CodeGen and T32 sources
"""
import copy

def convert_offsets_to_int(offsets):
	"""
	converts the hex offset strings into int

	Args:
		offsets (list): TME offset configuration for the given chipset

	Returns:
		list of offsets as integers
	"""
	ret = copy.deepcopy(offsets)
	for i in range(len(ret)):
		for each in ret[i]:
			if each != "region_name":
				ret[i][each] = int(ret[i][each], 16)
	return ret


def get_tme_addr(address, offsets):
	"""
	converts SoC address to TME address

	Args:
		address (int) : address to be translated to TME view
		offsets (list): TME offset configuration for the given chipset

	Returns:
		TME view of SoC address as an int

	Note:
		Mirrors implementation in /ssg.tmefw.3.1/sources/memorymap/src/rom/address_translation.c
	"""
	ret = address
	offsets_int = convert_offsets_to_int(offsets)
	for offset in offsets_int:
		if address >= offset['soc_addr'] and address < (offset['soc_addr'] + offset['size']):
			if offset['tme_addr'] > offset['soc_addr']:
				ret = address + (offset['tme_addr'] - offset['soc_addr'])
			else:
				ret = address - (offset['soc_addr'] - offset['tme_addr'])
			break
	return ret

def get_soc_addr(tme_address, offsets):
	"""
	converts TME address to SoC address

	Args:
		address (int) : address to be translated to SoC view
		offsets (list): TME offset configuration for the given chipset

	Returns:
		SoC view of TME address as an int

	Note:
		Mirrors implementation in /ssg.tmefw.3.1/sources/memorymap/src/rom/address_translation.c
	"""
	ret = tme_address
	offsets_int = convert_offsets_to_int(offsets)
	for offset in offsets_int:
		if tme_address >= offset['tme_addr'] and tme_address < (offset['tme_addr'] + offset['size']):
			if offset['tme_addr'] > offset['soc_addr']:
				ret = tme_address - (offset['tme_addr'] - offset['soc_addr'])
			else:
				ret = tme_address + (offset['soc_addr'] - offset['tme_addr'])
			break
	return ret