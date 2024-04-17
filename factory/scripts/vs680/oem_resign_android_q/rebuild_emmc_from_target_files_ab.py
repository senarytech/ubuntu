#!/usr/bin/env python

"""

This script is used to rebuild an eMMC image flashable with a Synaptics U-BOOT dongle

The inputs are an existing eMMC image and a target_files package as created by the
Android build system.

"""

from argparse import ArgumentParser
import os
import traceback
import tempfile
import shutil
import subprocess

BLOCK_SIZE = 1024 * 1024
EXTRSA_STORE_SIZE = 3112
CUSTK_EXTRSA_SIZE = 5120
CUSTK_STORE_SIZE = 1024
KEY_IMAGE_KEY_STORE_SIZE = 16384
KEY_IMAGE_OEM_KEY_SIZE = 16384
KEY_IMAGE_USER_KEY_SIZE = 16384

RESIGN_TOOL = 'resign_image_v4'
SIGN_TOOL = 'sign_image_v4'
IMG_FROM_TARGET_FILES = 'build/tools/releasetools/img_from_target_files.py'
BUILD_SUPER_IMAGE = 'build/tools/releasetools/build_super_image.py'
SPARSE2RAW_TOOL = 'out/host/linux-x86/bin/simg2img'
VERBOSE = True

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

def copy_non_android_partitions(dst_emmc, target_files):
    """
    Extract and copy to the target directory all subimages that are not built by android build system
    """

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    try:

        # uncompress the RADIO partitions from the target_files zip
        with open(os.devnull) as out:
            subprocess.check_call(['unzip', target_files, 'RADIO/*'], cwd=temp_dir, stdout=out, stderr=out)

        # copy the preboot partitions to the output directory
        shutil.copy(os.path.join(temp_dir, 'RADIO', 'preboot.subimg'), dst_emmc)
        if os.path.isfile(os.path.join(temp_dir, 'RADIO', 'fastlogo.img')):
           shutil.copy(os.path.join(temp_dir, 'RADIO', 'fastlogo.img'), os.path.join(dst_emmc, 'fastlogo.subimg'))

        # copy the boot partitions to the output directory
        images = [ 'bl.img', 'tzk.img', 'fastboot.img' ]

        for image in images:
            dst_emmc_img_a = os.path.join(dst_emmc, image[:-4] + "_a.subimg")
            dst_emmc_img_b = os.path.join(dst_emmc, image[:-4] + "_b.subimg")

            shutil.copy(os.path.join(temp_dir, 'RADIO', image), dst_emmc_img_a)
            shutil.copy(os.path.join(temp_dir, 'RADIO', image), dst_emmc_img_b)

    finally:
        # delete the temporary files
        shutil.rmtree(temp_dir)

def copy_android_partitions(src_emmc, dst_emmc, target_files):
    """
    Copy files and subimages that are Android partitions
    """
    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    try:
        # create a folder to store the unzipped target_files
        #zip_dir = os.path.join(temp_dir, 'zip')

        #os.makedirs(zip_dir)

        # unzip the target_files zip
        #with open(os.devnull) as out:
        #    subprocess.check_call(['unzip', target_files], cwd=zip_dir, stdout=out, stderr=out)

        # recompress the target_files zip without the unnecessary files
        #zip_out = os.path.abspath(os.path.join(temp_dir, 'new.zip'))

        #with open(os.devnull) as out:
        #    subprocess.check_call(['zip', '--symlinks', '-r', zip_out, '.'], cwd=zip_dir, stdout=out, stderr=out)

        # create the signed android partition images from the new target_files
        out_images = os.path.join(temp_dir, 'update.zip')

        try:
          subprocess.check_call([IMG_FROM_TARGET_FILES, '-v', target_files, out_images])
        except subprocess.CalledProcessError:
            print "ERROR: run img_from_target_files failed"
            exit(-1)

       # uncompress the IMAGES partitions from the target_files zip
        with open(os.devnull) as out:
           subprocess.check_call(['unzip', 'update.zip'], cwd=temp_dir, stdout=out, stderr=out)
           images = ['boot', 'dtbo','vbmeta', 'vbmeta_system']

           for image in images:
               src_name = os.path.join(temp_dir, image + ".img")
               dest_name = os.path.join(dst_emmc, image + ".subimg")
               shutil.copy(src_name, dest_name)

        try:
           # re-generate super image from target files
           super_image = os.path.join(dst_emmc, "super.subimg")
           subprocess.check_call([BUILD_SUPER_IMAGE, '-v', target_files, super_image])
        except subprocess.CalledProcessError:
           print "ERROR: run build_super_image.py failed"
           exit(-1)

    finally:
        # delete the temporary files
        shutil.rmtree(temp_dir)

def copy_missing(src_emmc, dst_emmc):
    """
    Copy files and subimages that are not contained in the target_files zip
    """

    files = ['emmc_image_list', 'emmc_part_list', 'emmc_image_list_full', 'userdata.subimg', 'boot-debug.subimg']

    for file_name in files:
        shutil.copy(os.path.join(src_emmc, file_name), dst_emmc)

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

def patch_android_key_stores(output_file, key_data, avb_key):
    """
    Substitutes the verity key stored in the key image parition.

    This function acts on a chunk of the key image that contains
    the key (either OEM key or user key). It changes only the
    beginning of the chunk and doesn't touch what is stored after
    the key.
    """

    # check avb_key
    if avb_key is not None:
        if not os.path.exists(avb_key):
            raise Exception('Cannot find avb_key')
        else:
            replace_avb_key=True
    else:
        replace_avb_key=False

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    try:

        orig_file = os.path.join(temp_dir, 'orig')

        if replace_avb_key is True:
            shutil.copy(avb_key, orig_file)
            # encrypt and sign the store with OEM_CUSTK and OEM_EXTRSA
            encrypt_and_sign_file(orig_file, key_data, keep_headers=False)
        else:
            shutil.copy(output_file, orig_file)
            # resign the verity key
            resign_and_reencrypt(orig_file, key_data, keep_headers=False)

        # overwrite the original file, keep the trailing data
        overwrite_file(orig_file, output_file)
    finally:
        shutil.rmtree(temp_dir)

def patch_key_image(input_dir, output_dir, key_image_name, key_data, avb_key):
    """
    Re-signs and re-encrypts key image
    """

    parts = ['kstore', 'oemk', 'usrk']

    parts_sizes = {
             'kstore' : KEY_IMAGE_KEY_STORE_SIZE,
             'oemk': KEY_IMAGE_OEM_KEY_SIZE,
             'usrk': KEY_IMAGE_USER_KEY_SIZE }

    # look for the original image
    orig_image = os.path.join(input_dir, key_image_name)

    if not os.path.exists(orig_image):
        raise Exception('Cannot find key subimage')

    # create a temporary working directory
    temp_dir = tempfile.mkdtemp()

    # copy key subimage to temp
    shutil.copy(orig_image, temp_dir)

    # look for process key image
    key_image = os.path.join(temp_dir, key_image_name)

    try:
        offset = 0

        for name in parts:

            size = parts_sizes[name]

            # cut out the part from the original file
            file_name = os.path.join(temp_dir, name)
            slice_file(key_image, file_name, skip=offset, count=size)

            # update the offset of the next part
            offset = offset + size

        # replace custk and extrsa stores
        overwrite_file(key_data['new_oem_custk_store'], os.path.join(temp_dir, 'kstore'))
        overwrite_file(key_data['oem_ext_rsa_store'], os.path.join(temp_dir, 'kstore'), CUSTK_STORE_SIZE)

        # patches the OEM android keys stores
        patch_android_key_stores(os.path.join(temp_dir, 'oemk'), key_data, avb_key)

        # patches the User android keys stores
        patch_android_key_stores(os.path.join(temp_dir, 'usrk'), key_data, avb_key)

        # reassemble the parts
        with open(key_image, 'w') as out:

            # append all the sections we cut out previously
            for name in parts:
                with open(os.path.join(temp_dir, name)) as inp:
                    shutil.copyfileobj(inp, out, length=BLOCK_SIZE)

        # copy key subimage gz file to output dir
        shutil.copy(os.path.join(temp_dir, key_image_name), output_dir)

    finally:
        shutil.rmtree(temp_dir)

def patch_emmc(src_emmc, dst_emmc, target_files, key_data, avb_key):
    """
    Create a new emmc using the files in the target_files zip and the partition table and extra files from a template emmc
    """

    # check that tools are available
    if not os.path.exists(IMG_FROM_TARGET_FILES):
        raise Exception('Cannot find android release tool %s (are you in the root of the build tree?)' % IMG_FROM_TARGET_FILES)

    if not os.path.exists(BUILD_SUPER_IMAGE):
        raise Exception('Cannot find android release tool %s (are you in the root of the build tree?)' % BUILD_SUPER_IMAGE)

    if not os.path.exists(SPARSE2RAW_TOOL):
        raise Exception('Cannot find android release tool %s (did you run make otatools ?)' % SPARSE2RAW_TOOL)

    # check that the inputs valid
    if not os.path.exists(target_files):
        raise Exception('Cannot find target_files zip')

    if not os.path.isdir(src_emmc):
        raise Exception('Cannot find input emmc')

    if os.path.exists(dst_emmc):
        raise Exception('Destination directory already exists')

    # transform path to absolute so that we can use it in different working directories
    target_files = os.path.abspath(target_files)

    done = False

    try :
        # create the output directory
        os.makedirs(dst_emmc)

        # copy the android partitions to the emmc
        copy_android_partitions(src_emmc, dst_emmc, target_files)

        # copy all the non-android partitions
        copy_non_android_partitions(dst_emmc, target_files)

        # patch key images
        patch_key_image(src_emmc, dst_emmc, 'key_a.subimg', key_data, avb_key)
        patch_key_image(src_emmc, dst_emmc, 'key_b.subimg', key_data, avb_key)

        # copy the remaining metadata and partitions from the original eMMC
        copy_missing(src_emmc, dst_emmc)

        done = True

    finally:

        # remove partially created output in case of exceptions
        if not done:
            shutil.rmtree(dst_emmc)

def main():

    # this is the list of keys we need to perform the resigning
    input_keys = { 'new_oem_custk' : 'File containing the new raw OEM_CUSTK key to be used reencrypt images',
                   'new_oem_custk_store' : 'File containing the new OEM_CUSTK store will be injected in key images',
                   'old_oem_custk' : 'File containing the raw OEM_CUSTK key used to encrypt the images in input target zip files',
                   'oem_ext_rsa' : 'PEM file containing new OEM_EXTRSA private key to be used resign images',
                   'oem_ext_rsa_store' : 'File containing the new OEM_EXTRSA singk7 store that will be injected in key images' }

    parser = ArgumentParser()

    # add arguments to receive all key paths
    for key, desc in input_keys.iteritems():
        argument = '--' + key.replace('_', '-')
        parser.add_argument(argument, required=True, help=desc)

    # add arguments to receive avb_key
    parser.add_argument('--avb_key', help='Store containing the new Android AVB key (Optinal, if you want to replace it)')

    parser.add_argument('src_emmc_dir', help='eMMC to be used as model')
    parser.add_argument('src_target_files', help='input target_files package')
    parser.add_argument('-o', dest='output', help='directory where the patched eMMC should be written')

    args = parser.parse_args()

    # store in key_data the paths to all the keys
    key_data = {}
    for key in input_keys:
        key_data[key] = getattr(args, key)

    try:
        patch_emmc(args.src_emmc_dir, args.output, args.src_target_files, key_data, args.avb_key)

    except Exception as ex:
        print "\nError: %s" % ex

        print "\nDetails:\n"
        traceback.print_exc()
        exit(1)

if __name__ == "__main__":
    main()
