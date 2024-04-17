#!/usr/bin/python
#last update: 2020/07/09

import os
import sys
import shutil
import datetime
import time
import sys

EROM_PREFIX = '{"name": "erom.bin", "type": "erom", "path": "'
TSM_PREFIX = '{"name": "tsm.bin", "type": "tsm", "path": "'
EROMF_PREFIX = '{"name": "erom.factory", "type": "eromf", "path": "'
TSMF_PREFIX = '{"name": "tsm.factory", "type": "tsmf", "path": "'
SYSINIT_PREFIX = '{"name": "sysinit_en.bin", "type": "sysinit", "path": "'
MINILOADER_PREFIX = '{"name": "miniloader_en.bin", "type": "miniloader", "path": "'
COMMON_PREFIX = '{"name": "'

CUSTKSTORE_CHECK_OFFSET = 4
CUSTKSTORE_CHECK_PATTERN = 0x37c20013
CUSTKSTORE_CHECK_PATTERN1 = 0x37c20113
CUSTKSTORE_CHECK_BYTES = 4

TA_CHECK_OFFSET = 36
TA50_CHECK_MAGIC_NUM = '05AT'
TA30_CHECK_MAGIC_NUM = '03AT'

SYNA_ROOT_RSA_OFFSET = 0x24AB4
SYNA_ROOT_RSA_PREFIX = 0x50000000

IMAGE_HEADER_CHECK_OFFSET = 4
IMAGE_HEADER_CHECK_PATTERN = 0xdec00201
IMAGE_HEADER_CHECK_BYTES = 4
GEN2_FWDIR_PREFIX = 'arm'

def usage():
	print 'Usage:'
	print '  %s ROMK_SDK_TGZ CHIP\n' % sys.argv[0]
	print '  ROMK_SDK_TGZ: input, ROMK syna-release.tgz'
	print '  CHIP: bg4ct, bg5ct, as370 or vs680'
def update_progress(progress):
    barLength = 40
    status = ""
    if isinstance(progress, int):
        progress = float(progress)
    if not isinstance(progress, float):
        progress = 0
        status = "error: progress var must be float\r\n"
    if progress < 0:
        progress = 0
        status = "Halt...\r\n"
    if progress >= 1:
        progress = 1
        status = "Done...\r\n"
    block = int(round(barLength*progress))
    text = "\rPercent: [{0}] {1}% {2}".format( "="*block + " "*(barLength-block), progress*100, status)
    sys.stdout.write(text)
    sys.stdout.flush()

def check_custk_store_image(input_image, image_type):
    """
    Check custk_store image format
    """
    if image_type == "erom.bin" or image_type == "erom.factory":
        check_offset = int(CUSTKSTORE_CHECK_OFFSET) + 8192
        check_pattern = CUSTKSTORE_CHECK_PATTERN
    elif image_type == "tsm.bin" or image_type == "tsm.factory":
        check_offset = CUSTKSTORE_CHECK_OFFSET
        check_pattern = CUSTKSTORE_CHECK_PATTERN
    else:
        check_offset = CUSTKSTORE_CHECK_OFFSET
        check_pattern = CUSTKSTORE_CHECK_PATTERN1

    with file(input_image, 'r+b') as input_file:
        input_file.seek(check_offset)
        check_bytes = input_file.read(CUSTKSTORE_CHECK_BYTES)

        if int(check_bytes.encode('hex'), 16) != check_pattern:
            update_progress(-1)
            print 'Check "' + input_image + '" is invalid!'
            print image_type + ' is wrong format or not ROMK'
            print 'Checked bytes: ' + check_bytes.encode('hex')
            exit(1)

def check_image_header_image(imageh_image, image_type):
    """
    Check image_header image format
    """

    with file(imageh_image, 'r+b') as imageh_file:
        imageh_file.seek(IMAGE_HEADER_CHECK_OFFSET)
	check_bytes = imageh_file.read(IMAGE_HEADER_CHECK_BYTES)

        if int(check_bytes.encode('hex'), 16) != IMAGE_HEADER_CHECK_PATTERN:
            update_progress(-1)
            print 'Check "' + imageh_image + '" is invalid!'
            print image_type + ' is wrong format or not ROMK'
            print 'Checked bytes: ' + check_bytes.encode('hex')
            exit(1)

def check_ta_image(ta_image, ta_type):
    """
    Check TA image format
    """

    with file(ta_image, 'r+b') as ta_file:
        ta_file.seek(0)
        header = ta_file.read(4)
        ta_file.seek(TA_CHECK_OFFSET)
        check_bytes = ta_file.read(CUSTKSTORE_CHECK_BYTES)

    if ta_type == 'ta50':
        if header != TA50_CHECK_MAGIC_NUM or int(check_bytes.encode('hex'), 16) != CUSTKSTORE_CHECK_PATTERN1:
            update_progress(-1)
            print 'Check "' + ta_image + '" is invalid!'
            print 'TZ50 TA is wrong format or not ROMK'
            print 'header: ' + header
            print 'Checked bytes: ' + check_bytes.encode('hex')
            exit(1)

    else:
        if header != TA30_CHECK_MAGIC_NUM or int(check_bytes.encode('hex'), 16) != CUSTKSTORE_CHECK_PATTERN1:
            update_progress(-1)
            print 'Check "' + ta_image + '" is invalid!'
            print 'TZ30 TA is wrong format or not ROMK'
            print 'header: ' + header
            print 'Checked bytes: ' + check_bytes.encode('hex')
            exit(1)

def check_bkl_image(bkl_image):
    """
    Check BCM Kernel image format
    """
    with file(bkl_image, 'r+b') as bkl_file:
        bkl_file.seek(SYNA_ROOT_RSA_OFFSET)
        check_bytes = bkl_file.read(4)
        print 'Checked bytes: ' + check_bytes.encode('hex')

        if int(check_bytes.encode('hex'), 16) != SYNA_ROOT_RSA_PREFIX:
            update_progress(-1)
            print 'Check "' + bkl_image + '" is invalid!'
            print 'BKL is wrong format or not ROMK'
            print 'Checked bytes: ' + check_bytes.encode('hex')
            exit(1)

if __name__ == '__main__':
	if len(sys.argv) != 3:
		usage()
		exit(1)

	if not os.path.isfile(sys.argv[1]):
		print 'can not find ' + sys.argv[1]
		exit(1)

	chip = sys.argv[2]
	if chip != 'bg4ct' and chip != 'bg5ct' and chip != 'as370' and chip != 'vs680':
		print 'wrong chip!'
		exit(1)

	update_progress(0)
	curdir = os.getcwd() + '/'
	tmpdir = curdir + 'tmp/'
	if os.path.exists(tmpdir):
		shutil.rmtree(tmpdir)
	os.makedirs(tmpdir)
	if os.path.exists(curdir + '/syna-release/'):
		shutil.rmtree(curdir + '/syna-release/')
	elif os.path.exists(curdir + '/synaptics-sdk/'):
		shutil.rmtree(curdir + '/synaptics-sdk/')

	update_progress(0.05)
	os.system('tar -zxf ' + sys.argv[1] + ' -C ' + tmpdir)
	update_progress(0.4)

	#VSSDK 0.2 folder structure
	if os.path.isdir(tmpdir + '/synaptics-sdk/') or os.path.isdir(tmpdir + '/syna-release/release_prebuilt/'):
		if os.path.isdir(tmpdir + '/synaptics-sdk/'):
			sdkdir = tmpdir + '/synaptics-sdk/'
			sdkname = 'synaptics-sdk'
		else:
			sdkdir = tmpdir + '/syna-release/'
			sdkname = 'syna-release'
		if os.path.isdir(sdkdir + '/release_prebuilt/'):
			projectlist = os.listdir(sdkdir + '/release_prebuilt/')
			if len(projectlist) != 1:
				print 'wrong project number'
				exit(1)
		else:
			print 'not a SDK'
			exit(1)
		projectdir = sdkdir + '/release_prebuilt/' + str(projectlist[0]) + '/'
		#path of erom, tsm, sysinit and miniloader
		prebootdir = projectdir + '/MBOOT/preboot'
		preboot_abspath = sdkname + '/release_prebuilt/' + str(projectlist[0]) + '/MBOOT/preboot'
		#path/type of tzk
		teedir = projectdir + '/TEE'
		tee_abspath = sdkname + '/release_prebuilt/' + str(projectlist[0]) + '/TEE'
		tee_type = 'tzk30'
		#path/type of ta
		tadir = projectdir + '/TEE/tee_ta'
		ta_abspath = sdkname + '/release_prebuilt/' + str(projectlist[0]) + '/TEE/tee_ta'
		ta_type = 'ta30'
		#path/type of fw
		fwdir = projectdir + '/AMP/fw_a1'
		fw_abspath = sdkname + '/release_prebuilt/' + str(projectlist[0]) + '/AMP/fw_a1'
	#for VSSDK 1.0 folder structure
	elif os.path.isdir(tmpdir + '/syna-release/'):
		sdkdir = tmpdir + '/syna-release/'
		#path of erom, tsm, sysinit and miniloader
		prebootdir = sdkdir + '/boot/preboot/'
		preboot_abspath = 'syna-release/boot/preboot/'
		#path/type of tzk
		teedir = sdkdir + '/tee/tee/products/'
		tee_abspath = 'syna-release/tee/tee/products/'
		tee_type = 'tzk50'
		#path/type of ta
		tadir = sdkdir + '/ta_enc/'
		ta_abspath = 'syna-release/ta_enc/'
		ta_type = 'ta50'
		#path of fw
		fwdir = sdkdir + '/fw_enc/'
		fw_abspath = 'syna-release/fw_enc/'

	else:
		print 'not a SDK'
		exit(1)

	with open(curdir + 'imglst.json', 'w+') as jsonfile:
		update_progress(0.45)
		jsonfile.write('[\n')
		#find erom, tsm, sysinit, miniloader
		for dirpath, dirs, files in os.walk(prebootdir):
			for filename in files:
				fname = os.path.join(dirpath, filename)
				abspath = preboot_abspath + dirpath.split(prebootdir)[-1]
				if abspath.find(chip) != -1:
					if filename == "erom.bin":
						check_custk_store_image(fname, filename)
						output = EROM_PREFIX + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.5)
					elif filename == "tsm.bin":
						check_custk_store_image(fname, filename)
						output = TSM_PREFIX + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.55)
					if filename == "erom.factory":
						check_custk_store_image(fname, filename)
						output = EROMF_PREFIX + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.6)
					elif filename == "tsm.factory":
						check_custk_store_image(fname, filename)
						output = TSMF_PREFIX + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.65)
					elif filename == "sysinit_en.bin":
						check_custk_store_image(fname, filename)
						output = SYSINIT_PREFIX + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.7)
					elif filename == "miniloader_en.bin":
                                                check_image_header_image(fname, filename)
						output = MINILOADER_PREFIX + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.75)

		#find tzk
		for dirpath, dirs, files in os.walk(teedir):
			for filename in files:
				fname = os.path.join(dirpath, filename)
				abspath = tee_abspath + dirpath.split(teedir)[-1]
				if abspath.find(chip) != -1:
					if filename == "bootparam_en.bin" or filename == "bootparam_recovery_en.bin" or filename == "tz1_en.bin" or filename == "tz2_en.bin":
                                                check_custk_store_image(fname, filename)
						output = COMMON_PREFIX + filename + '", "type": "' + tee_type + '",  "path": "' + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.8)

		#find ta
		for dirpath, dirs, files in os.walk(tadir):
			for filename in files:
				fname = os.path.join(dirpath, filename)
				abspath = ta_abspath + dirpath.split(tadir)[-1]
				if abspath.find(chip) != -1:
					if filename.split(".")[-1] == "ta":
						check_ta_image(fname, ta_type)
						output = COMMON_PREFIX + filename + '", "type": "' + ta_type + '",  "path": "' + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.85)

		#find fw
		for dirpath, dirs, files in os.walk(fwdir):
			for filename in files:
				fname = os.path.join(dirpath, filename)
				abspath = fw_abspath + dirpath.split(fwdir)[-1]
				if abspath.find(GEN2_FWDIR_PREFIX) != -1 or abspath.find(chip) != -1:
					if filename.split(".")[-1] == "fw":
						check_custk_store_image(fname, filename)
						output = COMMON_PREFIX + filename + '", "type": "fw",  "path": "' + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.9)

		#find bkl
		fwdir = sdkdir + '/boot/security/images/chip/'
		for dirpath, dirs, files in os.walk(fwdir):
			for filename in files:
				fname = os.path.join(dirpath, filename)
				abspath = 'syna-release/boot/security/images/chip/' + dirpath.split(fwdir)[-1]
				if abspath.find(chip) != -1:
					if filename == "bkl_bk.subimg":
						check_bkl_image(fname)
						output = COMMON_PREFIX + filename + '", "type": "bkl",  "path": "' + abspath + '/"},\n'
						jsonfile.write(output)
						if not os.path.exists(curdir + abspath):
							os.makedirs(curdir + abspath)
						shutil.copy(fname, curdir + abspath)
						update_progress(0.95)

		jsonfile.seek(-2, 2)
		jsonfile.write('\n]')
		jsonfile.close()

	outzip = 'romk_images.' + datetime.datetime.today().strftime('%Y%m%d%H%M') + '.zip'
	if os.path.isdir(curdir + '/syna-release/'):
		os.system('zip -qr ' + outzip + ' syna-release imglst.json')
		shutil.rmtree(curdir + '/syna-release/')
	elif os.path.isdir(curdir + '/synaptics-sdk/'):
		os.system('zip -qr ' + outzip + ' synaptics-sdk imglst.json')
		shutil.rmtree(curdir + '/synaptics-sdk/')
	shutil.rmtree(tmpdir)
	os.remove(curdir + 'imglst.json')
	update_progress(1)
