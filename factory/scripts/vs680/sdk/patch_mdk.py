#!/usr/bin/python
#last update: 2018/12/05

import os
import sys
import shutil

def usage():
	print 'Usage:'
	print '  %s MDK_IMG_ZIP ROMK_SDK_TGZ\n' % sys.argv[0]
	print '  MDK_IMG_ZIP: input, MDK images zip file from signing server'
	print '  ROMK_SDK_TGZ: input, ROMK syna-release.tgz'

if __name__ == '__main__':
	if len(sys.argv) != 3:
		usage()
		exit()

	if not os.path.isfile(sys.argv[1]):
		print 'can not find ' + sys.argv[1]
		exit()

	if not os.path.isfile(sys.argv[2]):
		print 'can not find ' + sys.argv[2]
		exit()

	curdir = os.getcwd() + '/'
	tmpdir = curdir + '/tmp/'
	if os.path.isdir(curdir + '/syna-release/'):
		shutil.rmtree(curdir + '/syna-release/')
	elif os.path.isdir(curdir + '/synaptics-sdk/'):
		shutil.rmtree(curdir + '/synaptics-sdk/')
	if os.path.isdir(tmpdir):
		shutil.rmtree(tmpdir)
	os.makedirs(tmpdir)

	os.system('tar -zxf ' + sys.argv[2] + ' -C ./')
	#VSSDK 0.2 folder structure
	if os.path.isdir(curdir + '/synaptics-sdk/') or os.path.isdir(curdir + '/syna-release/release_prebuilt/'):
		if os.path.isdir(curdir + '/synaptics-sdk/'):
			sdkdir = curdir + '/synaptics-sdk/'
			zipdir = tmpdir + '/synaptics-sdk/'
			outtgz = curdir + 'synaptics-sdk.mdk.tgz'
		else:
			sdkdir = curdir + '/syna-release/'
			zipdir = tmpdir + '/syna-release/'
			outtgz = curdir + 'syna-release.mdk.tgz'

		if os.path.isdir(sdkdir + '/release_prebuilt/'):
			projectlist = os.listdir(sdkdir + '/release_prebuilt/')
			if len(projectlist) != 1:
				print 'wrong project number'
				exit(1)
		else:
			print 'not a SDK'
			exit(1)
		projectname = str(projectlist[0])
		chipname = projectname[:5]
		#path of erom, tsm, sysinit and miniloader
		prebootzipdir = zipdir + '/release_prebuilt/' + projectname + '/MBOOT/preboot/'
		prebootdir = sdkdir + '/release_prebuilt/' + projectname + '/MBOOT/preboot/'
	#VSSDK 1.0 folder structure
	elif os.path.isdir(curdir + '/syna-release/'):
		sdkdir = curdir + '/syna-release/'
		zipdir = tmpdir + '/syna-release/'
		outtgz = curdir + 'syna-release.mdk.tgz'
		prebootzipdir = zipdir + '/boot/preboot/'
		prebootdir = sdkdir + '/boot/preboot/'

	else:
		print sys.argv[2] + ' is not a SDK'
		exit()
	os.system('unzip -qo ' + sys.argv[1] + ' -d ' + tmpdir)
	if not os.path.isdir(zipdir):
		print sys.argv[1] + ' is not a mdk zip'
	os.system('cp -rf ' +  zipdir + ' ' + curdir)

	found_erom = False
	found_tsm = False
	found_eromf = False
	found_tsmf = False
	for dirpath, dirs, files in os.walk(prebootzipdir):
		for filename in files:
			fname = os.path.join(dirpath, filename)
			if filename == 'erom.bin' and found_erom == False:
				found_erom = True
				mdk_erom = fname
			elif filename == 'tsm.bin' and found_tsm == False:
				found_tsm = True
				mdk_tsm = fname
			elif filename == 'erom.factory' and found_eromf == False:
				found_eromf = True
				mdk_eromf = fname
			elif filename == 'tsm.factory' and found_tsmf == False:
				found_tsmf = True
				mdk_tsmf = fname

	#if found_erom == True or found_tsm == True or found_eromf == True or found_tsmf == True:
	#	for dirpath, dirs, files in os.walk(prebootdir):
	#		for filename in files:
	#			fname = os.path.join(dirpath, filename)
	#			if filename == 'erom.bin' and found_erom == True:
	#				shutil.copy(mdk_erom, fname)
	#				esmtdir = os.path.dirname(fname) + '/'
	#			elif filename == 'tsm.bin' and found_tsm == True:
	#				shutil.copy(mdk_tsm, fname)
	#			elif filename == 'erom.factory' and found_eromf == True:
	#				shutil.copy(mdk_eromf, fname)
	#			elif filename == 'tsm.factory' and found_tsmf == True:
	#				shutil.copy(mdk_tsmf, fname)

	#VSSDK 0.2 preboot_esmt
	if os.path.isdir(curdir + '/synaptics-sdk/') or os.path.isdir(curdir + '/syna-release/release_prebuilt/'):
		if chipname == 'bg4ct':
			if os.path.exists(esmtdir + 'erom.bin') and os.path.exists(esmtdir + 'tsm.bin') \
			and os.path.exists(esmtdir + 'sysinit_en.bin') and os.path.exists(esmtdir + 'miniloader_en.bin'):
				padding_0 = 184320 - os.path.getsize(esmtdir + 'erom.bin') \
				- os.path.getsize(esmtdir + 'sysinit_en.bin') - os.path.getsize(esmtdir + 'tsm.bin')
				os.system('dd if=/dev/zero of=' + tmpdir + 'padding_0.bin bs=1 count=' + str(padding_0))
				os.system('cat ' + esmtdir + 'erom.bin ' + esmtdir + 'sysinit_en.bin ' + esmtdir + 'tsm.bin ' \
				+ tmpdir + 'padding_0.bin ' + esmtdir + 'miniloader_en.bin > ' + esmtdir + 'preboot_esmt_mdk.bin')
				os.system('cp ' + esmtdir + 'preboot_esmt_mdk.bin ' + esmtdir + 'preboot_esmt.bin')
			if os.path.exists(esmtdir + 'erom.factory') and os.path.exists(esmtdir + 'tsm.factory') \
			and os.path.exists(esmtdir + 'sysinit_en.bin') and os.path.exists(esmtdir + 'miniloader_en.bin'):
				padding_0 = 184320 - os.path.getsize(esmtdir + 'erom.factory') \
				- os.path.getsize('sysinit_en.bin') - os.path.getsize(esmtdir + 'tsm.factory')
				os.system('cat ' + esmtdir + 'erom.factory ' + esmtdir + 'sysinit_en.bin ' + esmtdir + 'tsm.factory ' \
				+ tmpdir + 'padding_0.bin ' + esmtdir + 'miniloader_en.bin > ' + esmtdir + 'preboot_esmt_otp.bin')
		elif chipname == 'bg5ct':
			if os.path.exists(esmtdir + 'erom.bin') and os.path.exists(esmtdir + 'tsm.bin') \
			and os.path.exists(esmtdir + 'sysinit_en.bin') and os.path.exists(esmtdir + 'miniloader_en.bin'):
				os.system('cat ' + esmtdir + 'erom.bin ' + esmtdir + 'sysinit_en.bin ' \
				+ esmtdir + 'miniloader_en.bin ' + esmtdir + 'tsm.bin > ' + esmtdir + 'preboot_esmt_mdk.bin')
				os.system('cp ' + esmtdir + 'preboot_esmt_mdk.bin ' + esmtdir + 'preboot_esmt.bin')
			if os.path.exists(esmtdir + 'erom.factory') and os.path.exists(esmtdir + 'tsm.factory') \
			and os.path.exists(esmtdir + 'sysinit_en.bin') and os.path.exists(esmtdir + 'miniloader_en.bin'):
				os.system('cat ' + esmtdir + 'erom.factory ' + esmtdir + 'sysinit_en.bin ' \
				+ esmtdir + 'miniloader_en.bin ' + esmtdir + 'tsm.factory > ' + esmtdir + 'preboot_esmt_otp.bin')

		if os.path.isdir(curdir + '/synaptics-sdk/'):
			os.system('tar -zcf synaptics-sdk.mdk.tgz synaptics-sdk')
		else:
			os.system('tar -zcf syna-release.mdk.tgz syna-release')
	elif os.path.isdir(curdir + '/syna-release/'):
		os.system('tar -zcf syna-release.mdk.tgz syna-release')
	shutil.rmtree(sdkdir)
	shutil.rmtree(tmpdir)
