#!/usr/bin/python
#last update: 2019/01/07

import os
import sys
import shutil
import datetime

COMMON_PREFIX = '{"name": "'
SYNA_ROOT_RSA_offset = 0x24AB4
SYNA_ROOT_RSA_PREFIX = 0x50000000

def usage():
	print 'Usage:'
	print '  %s ROMK_BKL\n' % sys.argv[0]
	print '  ROMK_BKL: input, ROMK encrypted BKL'

if __name__ == '__main__':
	if len(sys.argv) != 2:
		usage()
		exit()

	if not os.path.isfile(sys.argv[1]):
		print 'can not find ' + sys.argv[1]
		exit()

	os.system('cp ' + sys.argv[1] + ' ./')
	filename = os.path.basename(sys.argv[1])

        # Check bkl_bk.subimg is from released SDK package
        with file(filename, 'r+b') as bkl_file:
            bkl_file.seek(SYNA_ROOT_RSA_offset)
            check_bytes = bkl_file.read(4)

            if  int(check_bytes.encode('hex'), 16) != SYNA_ROOT_RSA_PREFIX:
                    print 'Error: Please get bkl_bk.subimg from released SDK package.'
                    #print 'ROOT_RSA_PREFIX: ' + check_bytes.encode('hex')
                    os.remove(filename)
                    exit()

	with open('./imglst.json', 'w+') as jsonfile:
		jsonfile.write('[\n')
		output = COMMON_PREFIX + filename + '", "type": "bkl",  "path": "./"},\n'
		jsonfile.write(output)
		jsonfile.seek(-2, 2)
		jsonfile.write('\n]')
		jsonfile.close()

	outzip = 'romk_images.' + datetime.datetime.today().strftime('%Y%m%d%H%M') + '.zip'
	os.system('zip -q ' + outzip + ' imglst.json ' + filename)
	os.remove(filename)
	os.remove('imglst.json')
