#!/usr/bin/env python

"""

This script is used to change OEM_EXTRSA, OEM_CUSTK and SIGNK7
used in a target_files zip file.

From the target_files zip file it is possible to then generate
OTAs and eMMC images with the new OEM keys.

"""

import os
import shutil
import tempfile
import subprocess
import argparse
import traceback
import struct

BLOCK_SIZE = 1024 * 1024

RESIGN_TOOL = 'resign_image_v4'
SIGN_TOOL = 'sign_image_v4'

CUSTK_EXTRSA_SIZE = 5120
EXTRSA_STORE_SIZE = 3112

BOOTLOADER_SIGNK_SIZE = 2068
FASTBOOT_HEADER_SIZE = 64
NW_BOOTLOADER_HEADER_SIZE = 64

ADD_IMG_TO_TARGET_FILES = 'build/make/tools/releasetools/add_img_to_target_files.py'

VERBOSE = False

def check_call(*args, **kwargs):
    """
    Utility method to call an external program and raise an exception if its exit value is not 0
    """

    if VERBOSE:
        print 'calling subprocess: %s' % ' '.join(args[0])

    # we want to capture stdout and stderr with communicate later
    kwargs['stderr'] = subprocess.PIPE
    kwargs['stdout'] = subprocess.PIPE

    p = subprocess.Popen(*args, **kwargs)

    normal, err = p.communicate()

    if p.returncode != 0:
        print normal, err
        raise Exception('Error while executing command %s (return code = %d)' % (' '.join(args[0]), p.returncode))

    if VERBOSE:
        print normal, err

def slice_file(input_file, output_file, skip=None, count=None):
    """
    Cuts a part of a file and stores it in a new file
    """

    cmd = ['dd', 'if=' + input_file, 'of=' + output_file, 'bs=1']

    if skip :
        cmd.append('skip=' + str(skip))

    if count :
        cmd.append('count=' + str(count))

    check_call(cmd)

def overwrite_file(input_file, output_file, seek=None):
    """
    Overwrites the beginning of a file with the content of another file
    """

    if os.path.getsize(input_file) > os.path.getsize(output_file):
        raise Exception("Input file is bigger than output file")

    cmd = ['dd', 'if=' + input_file, 'of=' + output_file, 'conv=notrunc', 'bs=1']

    if seek :
        cmd.append('seek=' + str(seek))

    check_call(cmd,)

def resign_and_reencrypt(file_name, key_data, keep_headers=True):
    """
    Resign and re-encrpyt an image with different OEM_CUSTK and OEM_EXTRSA keys
    """

    params = ['new_oem_custk', 'new_oem_custk_store', 'old_oem_custk',
              'oem_ext_rsa', 'oem_ext_rsa_store']

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    try:

        # copy the original file to the temporary location
        orig_file = os.path.join(temp_dir, 'orig')
        shutil.copy(file_name, orig_file)

        # copy all key data to the temporary location
        for param in params :
            param_file = os.path.join(temp_dir, param)
            shutil.copy(key_data[param], param_file)

        # make sure the OEM_EXT_RSA store doesn't have padding at the end
        oem_ext_rsa_store = os.path.join(temp_dir, 'oem_ext_rsa_store')
        if os.path.getsize(oem_ext_rsa_store) != EXTRSA_STORE_SIZE:
            with open(oem_ext_rsa_store, 'r+') as fp:
                fp.truncate(EXTRSA_STORE_SIZE)

        # look for the resign tool in the same folder where the script is
        my_dir = os.path.dirname(os.path.abspath(os.path.realpath(__file__)))
        full_path = os.path.join(my_dir, RESIGN_TOOL)

        if not os.path.exists(full_path):
            raise Exception('Cannot find %s' % RESIGN_TOOL)

        # copy the resign tool to the temporary folder
        temp_tool = os.path.join(temp_dir, 'tool')
        shutil.copy(full_path, temp_tool)

        cmd = [ './tool',
                '-T', '0',
                '-t', '2',
                '-S', 'oem_ext_rsa',
                '-i', 'orig',
                '-o', 'out',
                '-C', 'old_oem_custk',
                '-N', 'new_oem_custk']

        if keep_headers:
            cmd.append([ '-j', 'new_oem_custk_store',
                         '-k', 'oem_ext_rsa_store'])

        # resign the image in place
        check_call(cmd, cwd=temp_dir)

        out_file = os.path.join(temp_dir, 'out')

        # overwrite the original file
        shutil.copy(out_file, file_name)

    finally:
        shutil.rmtree(temp_dir)

def encrypt_and_sign_file(file_name, key_data, keep_headers=True):
    """
    Sign and encrpyt an file with OEM_CUSTK and OEM_EXTRSA
    """

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    params = ['new_oem_custk', 'new_oem_custk_store',
              'oem_ext_rsa', 'oem_ext_rsa_store']

    try:

        # copy all key data to the temporary location
        for param in params :
            param_file = os.path.join(temp_dir, param)
            shutil.copy(key_data[param], param_file)

        # copy the original file to the temporary location
        orig_file = os.path.join(temp_dir, 'orig')
        shutil.copy(file_name, orig_file)

        # look for the store creation tool
        my_dir = os.path.dirname(os.path.abspath(os.path.realpath(__file__)))
        full_path = os.path.join(my_dir, SIGN_TOOL)

        if not os.path.exists(full_path):
            raise Exception('Cannot find %s' % SIGN_TOOL)

        # copy the sign tool to the temporary folder
        temp_tool = os.path.join(temp_dir, 'tool')
        shutil.copy(full_path, temp_tool)

        cmd = [ './tool',
                '-H', '0x20000000',
                '-s', '32',
                '-z', '32',
                '-T', '0',
                '-d', '5',
                '-l', '0x0',
                '-t', '2',
                '-m', '0x1',
                '-M', '0xffffffff',
                '-e', '0x0',
                '-E', '0xff',
                '-y', '0',
                '-g', '-f',
                '-S', 'oem_ext_rsa',
                '-i', 'orig',
                '-o', 'out',
                '-C', 'new_oem_custk',
                '-j', 'new_oem_custk_store',
                '-k', 'oem_ext_rsa_store']

        # encrypts the file
        check_call(cmd, cwd=temp_dir)

        out_file = os.path.join(temp_dir, 'out')

        # strip the re-signed file of the CUSTK and EXTRSA headers if necessary
        if not keep_headers:
            stripped_out = os.path.join(temp_dir, 'stripped')
            slice_file(out_file, stripped_out, CUSTK_EXTRSA_SIZE)
            out_file = stripped_out

        # overwrite the original file
        shutil.copy(out_file, file_name)
    finally:
        shutil.rmtree(temp_dir)

def patch_bootloader(input_dir, key_data):
    """
    Re-signs and re-encrypts preboot image
    """
    # look for the original image
    orig_image = os.path.join(input_dir, 'RADIO', 'preboot.subimg')

    if not os.path.exists(orig_image):
        raise Exception('Cannot find RADIO/preboot.subimg')

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    try:
        # split the original fastboot in the header and payload parts
        signk_file = os.path.join(temp_dir, 'signk')
        payload_file = os.path.join(temp_dir, 'payload')

        slice_file(orig_image, signk_file, count=BOOTLOADER_SIGNK_SIZE)
        slice_file(orig_image, payload_file, skip=BOOTLOADER_SIGNK_SIZE)

        # reassemble the parts
        with open(orig_image, 'w') as out:
            with open(signk_file) as inp:
                shutil.copyfileobj(inp, out, BLOCK_SIZE)
            with open(payload_file) as inp:
                shutil.copyfileobj(inp, out, BLOCK_SIZE)

    finally:
        shutil.rmtree(temp_dir)

def patch_nw_bootloader(input_dir, bl_name, key_data):
    """
    Creates a resigned a NW bootloader
    """

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    try:
        # extract the original NW bootloader partition
        orig_image = os.path.join(input_dir, 'RADIO', bl_name)

        # split the original NW booloader in the header and payload parts
        header_file = os.path.join(temp_dir, 'header')
        payload_file = os.path.join(temp_dir, 'payload')

        slice_file(orig_image, header_file, count=NW_BOOTLOADER_HEADER_SIZE)
        slice_file(orig_image, payload_file, skip=NW_BOOTLOADER_HEADER_SIZE)

        # resign the NW bootloader
        resign_and_reencrypt(payload_file, key_data, keep_headers=False)

        # reassemble the NW bootloader partition
        with open(orig_image, 'w') as out:
            with open(header_file) as inp:
                shutil.copyfileobj(inp, out, BLOCK_SIZE)
            with open(payload_file) as inp:
                shutil.copyfileobj(inp, out, BLOCK_SIZE)

    finally:
        shutil.rmtree(temp_dir)

def patch_fastboot(input_dir, fb_name, key_data):
    """
    Creates a resigned a fastboot
    """

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    try:
        # extract the original fastboot partition
        orig_image = os.path.join(input_dir, 'RADIO', fb_name)

        # split the original fastboot in the header and payload parts
        header_file = os.path.join(temp_dir, 'header')
        payload_file = os.path.join(temp_dir, 'payload')

        slice_file(orig_image, header_file, count=FASTBOOT_HEADER_SIZE)
        slice_file(orig_image, payload_file, skip=FASTBOOT_HEADER_SIZE)

        # resign the fastboot
        resign_and_reencrypt(payload_file, key_data, keep_headers=False)

        # reassemble the NW bootloader partition
        with open(orig_image, 'w') as out:
            with open(header_file) as inp:
                shutil.copyfileobj(inp, out, BLOCK_SIZE)
            with open(payload_file) as inp:
                shutil.copyfileobj(inp, out, BLOCK_SIZE)

    finally:
        shutil.rmtree(temp_dir)

def patch_tzk(input_dir, tzk_name, key_data):
    """
    Creates a resigned a tzk
    """

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    try:
        # extract the original fastboot partition
        orig_image = os.path.join(input_dir, 'RADIO', tzk_name)

        file = open(orig_image)
        skip = file.read(128)
        magic_number = struct.unpack('<4s', file.read(4))[0]
        image_offset = struct.unpack('<I', file.read(4))[0]
        image_size = struct.unpack('<I', file.read(4))[0]

        if magic_number == 'TZOP':
            print 'image_offset = ' + str(image_offset)
            print 'image_size = ' + str(image_size)
            image_offset += 64

            # split the original fastboot in the header and payload parts
            header_file = os.path.join(temp_dir, 'header')
            payload_file = os.path.join(temp_dir, 'payload')
            tail_file = os.path.join(temp_dir, 'tail')

            slice_file(orig_image, header_file, 0, image_offset)
            slice_file(orig_image, payload_file, image_offset, image_size)
            slice_file(orig_image, tail_file, image_offset + image_size)

            # resign the tzk
            resign_and_reencrypt(payload_file, key_data, keep_headers=False)

            # reassemble the tzk partition
            with open(orig_image, 'w') as out:
                with open(header_file) as inp:
                    shutil.copyfileobj(inp, out, BLOCK_SIZE)
                with open(payload_file) as inp:
                    shutil.copyfileobj(inp, out, BLOCK_SIZE)
                with open(tail_file) as inp:
                    shutil.copyfileobj(inp, out, BLOCK_SIZE)

    finally:
        shutil.rmtree(temp_dir)

def patch_zip(input_zip, output_zip, key_data):

    # validate that all key data is available
    for name, file_name in key_data.iteritems():
        if not os.path.exists(file_name):
            raise Exception('The key %s is not valid' % name)

    # parameters are valid
    if os.path.exists(output_zip):
        raise Exception('The output zip already exists')

    if not os.path.exists(input_zip):
        raise Exception('Cannot find input zip')

    # check that tools are available
    if not os.path.exists(ADD_IMG_TO_TARGET_FILES):
        raise Exception('Cannot find android tool %s (are you in the root of the build tree?)' % ADD_IMG_TO_TARGET_FILES)

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    input_zip = os.path.abspath(input_zip)
    output_zip = os.path.abspath(output_zip)

    try:

        # decompress the target files
        check_call(['unzip', input_zip, '-d', temp_dir])

        # patch the preboot partition
        patch_bootloader(temp_dir, key_data)

        # patch the normal world bootloaders
        patch_nw_bootloader(temp_dir, 'bl.img', key_data)

        # patch fastboot
        patch_fastboot(temp_dir, 'fastboot.img', key_data)

        # update the copy of the recovery nw bootloader that is stored in the system partition
        shutil.copy(os.path.join(temp_dir, 'RADIO', 'preboot.subimg'),
                    os.path.join(temp_dir, 'VENDOR', 'boot', 'preboot'))

        # patch tzk
        patch_tzk(temp_dir, 'tzk.img', key_data)

        # remove the prebuilt images because they are out of sync
        shutil.rmtree(os.path.join(temp_dir, 'IMAGES'))

        # re-compress the patched target files
        check_call(['zip', '--symlinks', '-r', output_zip, '.'], cwd=temp_dir)

        # add deleted IMAGE to resigned target file
        if subprocess.check_call([ADD_IMG_TO_TARGET_FILES, '-a', '--is_signing', '-r', '-v',  output_zip]) == 0:
            print "Resigned TARGET FILES is done: %s" % output_zip
        else:
            print "Oops, something error"

    finally:
        shutil.rmtree(temp_dir)

def main():

    # this is the list of keys we need to perform the resigning
    input_keys = { 'new_oem_custk' : 'File containing the new raw OEM_CUSTK key to be used reencrypt images',
                   'new_oem_custk_store' : 'File containing the new raw OEM_CUSTK store will be injected in key images',
                   'old_oem_custk' : 'File containing the raw OEM_CUSTK key used to encrypt the images in input target files zip',
                   'oem_ext_rsa' : 'PEM file containing new OEM_EXTRSA private key to be used resign images',
                   'oem_ext_rsa_store' : 'File containing the new OEM_EXTRSA singk7 store that will be injected in key images', }

    # create a parser for the command line arguments
    parser = argparse.ArgumentParser()

    # add arguments to receive all key paths
    for key, desc in input_keys.iteritems():
        argument = '--' + key.replace('_', '-')
        parser.add_argument(argument, required=True, help=desc)

    # add arguments to receive input and output files
    parser.add_argument('input_zip', help='The XXX-target_files-XXX.zip file generated during build')
    parser.add_argument('output_zip', help='The name of the patched target_files.zip to be generated')

    # add parameter to increase verbosity
    parser.add_argument('--verbose', action='store_true', help='Output more details about what is being done')

    # parse the received arguments
    args = parser.parse_args()

    # update the verbose flag
    global VERBOSE
    VERBOSE = args.verbose

    # store in key_data the paths to all the keys
    key_data = {}
    for key in input_keys:
        key_data[key] = getattr(args, key)

    try:
        patch_zip(args.input_zip, args.output_zip, key_data)
    except Exception as ex:
        print "\nError: %s" % ex

        if VERBOSE:
            print "\nDetails:\n"
            traceback.print_exc()
        exit(1)

if __name__ == "__main__":
    main()
