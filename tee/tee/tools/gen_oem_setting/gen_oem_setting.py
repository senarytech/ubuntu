#!/usr/bin/env python3

import json
import sys
import struct


oem_setting = {}
with open(sys.argv[1]) as f:
    oem_setting = json.load(f)

oem_setting_len = len(oem_setting)
memory_size = int(sys.argv[2], 16)

data = struct.pack('4s7I', b'INFO', 0x1, 32 + oem_setting_len * 44, 32, oem_setting_len, memory_size, 0, 0)
print(data)

for key in oem_setting:
    name = key.ljust(32, '\0').encode()
    address = int(oem_setting[key]['Address'], 16)
    size = int(oem_setting[key]['Size'], 16)
    item = struct.pack('32s3I', name, address, size, 0)
    data += item
    print(item)

with open(sys.argv[3], "wb") as f:
    f.write(data)

