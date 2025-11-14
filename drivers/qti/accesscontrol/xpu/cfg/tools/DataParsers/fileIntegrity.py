# #
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
# SPDX-License-Identifier: ISC
#

import hashlib
import re
import os

md5_info_re = re.compile("(/\*MD5:)([0-9a-fA-F]+)(\*/)")
md5_info_len = len("\n/*MD5:*/\n") + len(hashlib.md5().hexdigest())

class FileIntegrity:
	def __init__(self, file_paths=None):
		self.hash = {}
		if file_paths is not None:
			self.updateHashs(file_paths)

	def updateHashs(self, file_paths):
		if isinstance(file_paths, str):
			file_paths = [file_paths]
		if not isinstance(file_paths, list):
			raise Exception("Unknown python type for file paths")

		for file_path in file_paths:
			if file_path is None:
				raise Exception("File path passed is None")
			if file_path is not None and not os.path.isfile(file_path):
				raise Exception("File path passed is not a file: " + file_path)
			self.hash[file_path] = {}
			with open(file_path, "r") as fd:
				lines = fd.readlines()
				content = ""
				for line in lines:
					content += line.rstrip() + "\n"
				text = content[:-md5_info_len] if 'MD5' in content else content
				md5_info = content[-md5_info_len:].strip() if 'MD5' in content else content
				self.hash[file_path]['content'] = content.encode('utf-8')
				self.hash[file_path]['text'] = text.encode('utf-8')
				self.hash[file_path]['md5_info'] = md5_info

	def checkIntegrity(self, file=None):
		if file is not None and file not in self.hash:
			self.updateHashs(file)
		files = self.hash if file is None else [file]
		for file in files:
			hash_md5 = hashlib.md5()
			m = md5_info_re.match(self.hash[file]['md5_info'])
			if m is None:
				print(("Error: MD5 info not found in " + file))
				return False
			md5_hex = m.group(2)
			hash_md5.update(self.hash[file]['text'])
			if md5_hex != hash_md5.hexdigest():
				print("Error: MD5 mismatch")
				print(("	   Found : " + md5_hex))
				print(("	   Actual: " + hash_md5.hexdigest()))
				return False
		return True

	def writeHash(self, file=None):
		if file is not None and file not in self.hash:
			self.updateHashs(file)
		files = self.hash if file is None else [file]
		for file in files:
			m = md5_info_re.match(self.hash[file]['md5_info'])
			if m is None:
				self.hash[file]['text'] = self.hash[file]['content']
			hash_md5 = hashlib.md5()
			hash_md5.update(self.hash[file]['text'])
			self.hash[file]['md5_info'] = "\n/*MD5:" + hash_md5.hexdigest() + "*/\n"
			with open(file, "w") as fd:
				fd.write(self.hash[file]['text'].decode())
				fd.write(self.hash[file]['md5_info'])