#!/usr/bin/python
#last update: 2020/04/17

import os
import sys
import shutil
import datetime

COMMON_PREFIX = '{"name": "'

IMAGE_HEADER_CHECK_OFFSET = 4
IMAGE_HEADER_CHECK_PATTERN = 0xdec00201
IMAGE_HEADER_CHECK_BYTES = 4

def usage():
	print 'Usage:'
	print '  %s ROMK_MINILOADER\n' % sys.argv[0]
	print '  ROMK_MINILOADER: input, ROMK encrypted MINILOADER'

def check_image_header_image(imageh_image):
    """
    Check image_header image format
    """

    with file(imageh_image, 'r+b') as imageh_file:
        imageh_file.seek(IMAGE_HEADER_CHECK_OFFSET)
	check_bytes = imageh_file.read(IMAGE_HEADER_CHECK_BYTES)

        if int(check_bytes.encode('hex'), 16) != IMAGE_HEADER_CHECK_PATTERN:
            print 'Check "' + imageh_image + '" is invalid!'
            print 'Miniloader is wrong format or not ROMK'
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
	check_image_header_image(filename)
	with open('./imglst.json', 'w+') as jsonfile:
		jsonfile.write('[\n')
		output = COMMON_PREFIX + filename + '", "type": "miniloader",  "path": "./"},\n'
		jsonfile.write(output)
		jsonfile.seek(-2, 2)
		jsonfile.write('\n]')
		jsonfile.close()

	outzip = 'romk_images.' + datetime.datetime.today().strftime('%Y%m%d%H%M') + '.zip'
	os.system('zip -q ' + outzip + ' imglst.json ' + filename)
	os.remove(filename)
	os.remove('imglst.json')
