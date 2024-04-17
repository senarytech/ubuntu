#!/usr/bin/python
#last update: 2020/04/17

import os
import sys
import shutil
import datetime

COMMON_PREFIX = '{"name": "'

CUSTKSTORE_CHECK_OFFSET = 4
CUSTKSTORE_CHECK_PATTERN = 0x37c20013
CUSTKSTORE_CHECK_BYTES = 4

def usage():
	print 'Usage:'
	print '  %s ROMK_TSM\n' % sys.argv[0]
	print '  ROMK_TSM: input, ROMK encrypted TSM'

def check_custk_store_image(input_image):
    """
    Check custk_store image format
    """
    with file(input_image, 'r+b') as input_file:
        input_file.seek(CUSTKSTORE_CHECK_OFFSET)
        check_bytes = input_file.read(CUSTKSTORE_CHECK_BYTES)

        if int(check_bytes.encode('hex'), 16) != CUSTKSTORE_CHECK_PATTERN:
            print 'Check "' + input_image + '" is invalid!'
            print 'TSM is wrong format or not ROMK'
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
	check_custk_store_image(filename)
	with open('./imglst.json', 'w+') as jsonfile:
		jsonfile.write('[\n')
		output = COMMON_PREFIX + filename + '", "type": "tsm",  "path": "./"},\n'
		jsonfile.write(output)
		jsonfile.seek(-2, 2)
		jsonfile.write('\n]')
		jsonfile.close()

	outzip = 'romk_images.' + datetime.datetime.today().strftime('%Y%m%d%H%M') + '.zip'
	os.system('zip -q ' + outzip + ' imglst.json ' + filename)
	os.remove(filename)
	os.remove('imglst.json')
