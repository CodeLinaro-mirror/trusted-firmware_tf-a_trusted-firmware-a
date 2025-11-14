# # # #
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: ISC
#

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------

import os
import json

class jsonConfigParser:
	"""json configuration file parser
	"""
	def __init__(self, json_paths, set_attr=False):
		"""Initializes the json config parser

		Args:
			json_paths (string or array): Path(s) to the json config file to initialize
			set_attr (bool): If True, the jsonConfigParser object will have all json attributes

		Raises:
			IOError: If the specified json file does not exist

		"""
		self.json = {}
		local_json_paths = [json_paths] if isinstance(json_paths, str) else json_paths
		for each in local_json_paths:
			if not os.path.isfile(each):
				raise IOError(f'File doesn\'t exist {each}')
			with open(each) as json_config_file:
				loaded_json_config = json.load(json_config_file, strict=False)
			if 'tcsr_config' in loaded_json_config:
				loaded_json_config['sle'] = loaded_json_config['tcsr_config']['DCR']['SLE']
			if 'ac_entry' in loaded_json_config:
				loaded_json_config['ac_entry'] = int(loaded_json_config['ac_entry'], 16)
			if 'xPU' in loaded_json_config:
				self.json.update(loaded_json_config['xPU'])
			self.json.update(loaded_json_config)

		if set_attr:
			for key, val in self.json.items():
				setattr(self, key, val)

	def get_json(self):
		"""
		Get the json file parsed by this instance
		"""
		return self.json

