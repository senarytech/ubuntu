#!/usr/bin/env python3

import json
import sys
import getopt
import os
import struct
import warnings
import re

def usage():
	"""
The tz memory region config tool to generate memory zone
Usage: gen_memzone.py [-s|--size total_memory_size] -i|--input <inputfile> -d|--data oem_setting.cfg [-o|--output <outputfile>]

Description
	-h or --help                            Display help information
	-v or --version                         Display version number
	-s or --size	<integer_hex>		Config the memory space size
	-d or --data	<string>		Input default oem_setting.cfg
	-i or --input	<string>		Input tz_boot_param_value.c file,
                                                    this option is required to get SYNA settings !
	-o or --output	<string>		The output zone cfg file name,
                                                    this is optional for generated zone file !
"""

MIN_REGION_SIZE = 12	# Min region size 4KB
MAX_REGION_SIZE = 43	# Max region size 8TB
MAX_ZONE_NUM	= 16
MAX_REGION_NUM	= 8
TOOL_VERSION = "2018v02.1126"

def generate_securezone(start, end, memzone, zone_num):
	region_size = 1 << 12 # 4KB initial length
	shift = 12

	try:
		if (zone_num >= 16 or zone_num < 0):
			raise Exception("illegal existing zone nums !")
		if (start > end or start < 0 or end <= 0):
			raise Exception("illegal existing secure region range !")
		elif start == end:
			raise Exception("no space for OEM secure memory region !")
	except Exception as err:
		print(err)
		return -1, zone_num

	left_size = end - start
	i = zone_num
	for i in range(zone_num, 16):
		shift = MIN_REGION_SIZE
		region_size = (0x1 << shift)
		while (not start & (region_size - 0x1)):
			shift += 0x1
			region_size = (region_size << 0x1)
			if region_size > left_size:
				break

		region_size = (region_size >> 0x1)
		shift -= 0x1

		memzone[i] = {}
		memzone[i]["Start"] = hex(start)
		start += region_size

		try:
			if (shift < MIN_REGION_SIZE or shift > MAX_REGION_SIZE):
				raise Exception("Could not find right region size with current start address !")
			if start > end:
				raise Exception("illegal zone size calculated !")
		except Exception as err:
			print(err)
			return -1, zone_num
		if start == end:
			memzone[i]["Size"] = hex(region_size)
			break
		elif start < end:
			memzone[i]["Size"] = hex(region_size)
			left_size = end - start

	if start < end:
		print("Error: could NOT find valid zone for current address !")
		return -1, zone_num
	else:
		zone_num = i + 1

        #print("INFO: zone_num: %d\n"%zone_num, memzone)
	return 0, zone_num


def main():
	finput_tz_param = ""
	foutput_zone = ""
	foem_cfg = ""
	memory_size = 0x80000000
	oem_setting = {}
	mem_region = {}
	memzone = {}
	zonenum = 0x0
	predefined_region_name = ("sys_init", "tz", "zsp", "bootloader")
	predefined_mem_region = {"sys_init": {"Address": 0x00000000, "Size": 0x00100000},
				"tz": {"Address": 0x00100000, "Size": 0x03f00000},
				"zsp": {"Address": 0x04000000, "Size": 0x01000000},
				"bootloader": {"Address": 0x05000000, "Size": 0x00800000}}

	try:
		opts, args = getopt.getopt(sys.argv[1:], "i:o:d:s:hv", ["input=","output=","data=","size=","help","version"])
	except getopt.GetoptError as err:
		print(err)
		print(usage.__doc__)
		sys.exit(2)
	for opt, arg in opts:
		if opt in ("-i", "--input"):
			finput_tz_param = arg
		elif opt in ("-o", "--output"):
			foutput_zone = arg
		elif opt in ("-d", "--data"):
			foem_cfg = arg
		elif opt in ("-s", "--size"):
			memory_size = int(arg, 16)
		elif opt in ("-v", "--version"):
			print(TOOL_VERSION)
			sys.exit()
		else:	#"-h", "--help"
			print(usage.__doc__)
			sys.exit()

	if memory_size <= 0:
		print("Error: illegal memory size param !")
		print(usage.__doc__)
		sys.exit(1)
	elif memory_size >> 32:
		memory_size = 0x100000000 		## ??????
		print("Warning: Memory Space was forced to Maximum 4GB !\n")

	if (not len(finput_tz_param)):
		print("INFO: no input tz_boot_param, using default predefined !")
		mem_region = predefined_mem_region
		secure_region_start = 0x0
		secure_region_end = 0x05000000
	else:
		try:
			with open(finput_tz_param, "r") as f:
				for line in f.readlines():
					for region in predefined_region_name:
						if re.match("^{\"%s\""%(region), line.strip()):
							mem_region[region] = {}
							buffer = line.strip().split(",", 3)
							mem_region[region]["Address"] = int(buffer[1].strip(), 16)
							mem_region[region]["Size"] = int(buffer[2].strip(), 16)
							if (mem_region[region]["Size"] <= 0 or (mem_region[region]["Size"] + \
									mem_region[region]["Address"] > memory_size)):
								raise Exception("got illegal region size for SYNA predefined !")
					if len(mem_region) == len(predefined_region_name):
						break
		except (IOError, LookupError, ValueError) as err:
			print("Error: fail to parse tz_boot_param_value\n", err)
			print(usage.__doc__)
			sys.exit(1)
		else:
			# print(mem_region)
			if (len(mem_region) != 4):
				print("Error: Did not got enough memory regions for SYNA prefined !")
				sys.exit(1)

		# Notice: check: predefined secure regions are sequential !!!!
		# Notice: default: predefined secure regions are before oem Secure region !!!!
		sort_region_list = sorted(mem_region, key=lambda k:mem_region[k]["Address"])
		not_default = 0
		secure_region_start = 0x0
		secure_region_end = 0x0
		for i in range(len(sort_region_list) - 1):
			region1 = sort_region_list[i]
			region2 = sort_region_list[i+1]
			if (mem_region[region1]["Address"] + mem_region[region1]["Size"] != mem_region[region2]["Address"]):
				print("INFO: predefined memory region are not sequential !")
				not_default = 1

		if predefined_region_name[3] == sort_region_list[0]:
			secure_region_start = mem_region[sort_region_list[1]]["Address"]
			secure_region_end = mem_region[sort_region_list[1]]["Size"] + \
						mem_region[sort_region_list[2]]["Size"] + \
						mem_region[sort_region_list[3]]["Size"]

		elif predefined_region_name[3] == sort_region_list[3]:
			secure_region_start = mem_region[sort_region_list[0]]["Address"]
			secure_region_end = mem_region[sort_region_list[0]]["Size"] + \
						mem_region[sort_region_list[1]]["Size"] + \
						mem_region[sort_region_list[2]]["Size"]
		else:
			print("Warning: bootloader is in the middle of predefined region !")
			not_default = 1

		if not_default:
			print("Error: got illegal memory region for SYNA prefined !")
			sys.exit(1)

	memzone[0] = {}
	ret, zonenum = generate_securezone(secure_region_start, secure_region_end, memzone, zonenum)
	if (ret or zonenum > 16):
		print("Err: fail to generate zone for predefined memory region, ret= %d, %d !"%(ret, zonenum))
		sys.exit(1)

	customized_region_name = ("NonSecure", "system", "NonSecure-NC", "Secure")

	try:
		with open(foem_cfg) as f:
			oem_setting = json.load(f)

		if (not len(oem_setting)):
			raise Exception("oem_setting is NULL !")

		for i in oem_setting:
			address = int(oem_setting[i]["Address"], 16)
			size = int(oem_setting[i]["Size"], 16)
			if (address < 0 or size < 0):
				raise Exception("invalid oem_setting param for region {} !".format(i))
			elif (i in customized_region_name):
				mem_region[i] = {}
				mem_region[i]["Address"] = address
				mem_region[i]["Size"] = size
			else:
				warnings.warn("unrecognized region in oem_setting: {}{} !".format(i, oem_setting[i]), Warning) #UserWarning

	except (IOError, KeyError, ValueError, Exception) as err:
		print("Fail to parse default oem_setting.cfg, Reasons: ", err)
		oem_setting = {}
		sys.exit(1)

	# print("Finally:", oem_setting)
	## Check buffer overlay for system, NoSecure-NC, Secure
	try:
		region_system = customized_region_name[1]
		region_nonsecure = customized_region_name[0]
		system_end = mem_region[region_system]["Size"] + mem_region[region_system]["Address"]
		nonsecure_end = mem_region[region_nonsecure]["Size"] + mem_region[region_nonsecure]["Address"]
		if (mem_region[region_nonsecure]["Address"] >= mem_region[region_system]["Address"] and nonsecure_end <= system_end):
			print("INFO: %s memory region is in the range of %s region as design !"%(region_nonsecure, region_system))
		else:
			raise Exception("invalid region setting for {} and {} !".format(region_nonsecure, region_system))

		for i in range(1, len(customized_region_name) - 1):
			region1_start = mem_region[customized_region_name[i]]["Address"]
			region1_end = region1_start + mem_region[customized_region_name[i]]["Size"]
			for region in customized_region_name[(i+1):]:
				region2_start = mem_region[region]["Address"]
				region2_end = region2_start + mem_region[region]["Size"]
				if (region2_end > memory_size or region1_end > memory_size):
					raise Exception("region {} or {} exceeds total Memory Space {} !".format(customized_region_name[i], \
										region, memory_size))
				if ((region1_start < region2_start and region1_end <= region2_start) or region1_start >= region2_end):
					print("INFO: %s region is checked passed with %s region !"%(customized_region_name[i], region))
				else:
					raise Exception("Region {} and {} Overlay !".format(customized_region_name[i], region))

	except (KeyError, Exception) as err:
		print("Invalid oem_setting configuration!\n Reasons: ", err)
		sys.exit(1)
	else:
		print("INFO: memory regions are checked pass !")

	region_secure = customized_region_name[3]
	region_start = mem_region[region_secure]["Address"]
	region_end = mem_region[region_secure]["Size"] + mem_region[region_secure]["Address"]
	if region_start == secure_region_end:
		secure_region_end += region_start
		zonenum = 0
	elif region_end == secure_region_start:
		secure_region_start = region_start
		zonenum = 0
	else:
		secure_region_start = region_start
		secure_region_end = region_end
	ret, zonenum = generate_securezone(secure_region_start, secure_region_end, memzone, zonenum)
	if (ret or zonenum > 16):
		print("Err: fail to generate zone for oem memory region, ret= %d, %d !"%(ret, zonenum))
		sys.exit(1)
	else:
		print("Finally generated %d secure zone !\n"%zonenum)

	if foutput_zone:
		with open(foutput_zone, "w") as f:
			json.dump(memzone, f, indent=4)
	else:
		print(json.dumps(memzone, indent=4))

	return 0

if __name__ == "__main__":
	if ( len(sys.argv) == 1 ):
		print(usage.__doc__)
		sys.exit(1)
	else:
		main()