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

#ADD_IMG_TO_TARGET_FILES = 'build/make/tools/releasetools/add_img_to_target_files.py'
ADD_IMG_TO_TARGET_FILES = 'out/host/linux-x86/bin/add_img_to_target_files'

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

        # substitute root signk
        overwrite_file(key_data['root_signk_store'],  signk_file)

        # reassemble the parts
        with open(orig_image, 'w') as out:
            with open(signk_file) as inp:
                shutil.copyfileobj(inp, out, BLOCK_SIZE)
            with open(payload_file) as inp:
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

        # patch the bootloader partition
        patch_bootloader(temp_dir, key_data)

        if os.path.exists(temp_dir + '/VENDOR/boot/preboot'):
            # update the copy of the recovery nw bootloader that is stored in the system partition
            shutil.copy(os.path.join(temp_dir, 'RADIO', 'preboot.subimg'),
                        os.path.join(temp_dir, 'VENDOR', 'boot', 'preboot'))

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
        #shutil.rmtree(temp_dir)
        print "finall"

def main():

    # this is the list of keys we need to perform the resigning
    input_keys = { 'root_signk_store': 'Store containing the new root RSA signk7 store that will be replaced in preboot image' }

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
