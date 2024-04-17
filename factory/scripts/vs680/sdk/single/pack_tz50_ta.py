#!/usr/bin/python
#last update: 2020/04/17

import os
import sys
import shutil
import datetime

COMMON_PREFIX = '{"name": "'

TA_CHECK_OFFSET = 36
TA50_CHECK_MAGIC_NUM = '05AT'
CUSTKSTORE_CHECK_PATTERN = 0x37c20113
CUSTKSTORE_CHECK_BYTES = 4

def usage():
	print 'Usage:'
	print '  %s ROMK_TA\n' % sys.argv[0]
	print '  ROMK_TA: input, ROMK encrypted TZ50 TA'

def check_ta_image(ta_image):
    """
    Check TA image format
    """

    with file(ta_image, 'r+b') as ta_file:
        ta_file.seek(0)
        header = ta_file.read(4)
        ta_file.seek(TA_CHECK_OFFSET)
        check_bytes = ta_file.read(CUSTKSTORE_CHECK_BYTES)

        if header != TA50_CHECK_MAGIC_NUM or int(check_bytes.encode('hex'), 16) != CUSTKSTORE_CHECK_PATTERN:
            print 'Check "' + ta_image + '" is invalid!'
            print 'TZ50 TA is wrong format or not ROMK'
            print 'header: ' + header
            print 'Checked bytes: ' + check_bytes.encode('hex')
            exit(1)

if __name__ == '__main__':
	if len(sys.argv) != 2:
		usage()
		exit()

	if not os.path.isfile(sys.argv[1]):
		print 'can not find ' + sys.argv[1]
		exit()

	os.system('cp ' + sys.argv[1] + ' ./')
	filename = os.path.basename(sys.argv[1])
	check_ta_image(filename)
	with open('./imglst.json', 'w+') as jsonfile:
		jsonfile.write('[\n')
		output = COMMON_PREFIX + filename + '", "type": "ta50",  "path": "./"},\n'
		jsonfile.write(output)
		jsonfile.seek(-2, 2)
		jsonfile.write('\n]')
		jsonfile.close()

	outzip = 'romk_images.' + datetime.datetime.today().strftime('%Y%m%d%H%M') + '.zip'
	os.system('zip -q ' + outzip + ' imglst.json ' + filename)
	os.remove(filename)
	os.remove('imglst.json')
