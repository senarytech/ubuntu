Here is the guide to show you how to generate factory image / production OTA / recovery eMMC
1. Copy below keys to synaptics-sdk/security/keys/chip/<CHIP_NAME>/<CHIP_REV>/mid-00000007/oem/codetype_5 folder:
   custk.bin
   custk.keystore
   extrsa.pem
   extrsa.keystore

2. Start to build pre-production eMMC image and OTA package in Android:
   $ ./vendor/synaptics/build/build_androidtv -p <PRODUCT> -m <PATH_TO_SYNAPTICS_SDK>

3. Change pre-production eMMC image or OTA package to factory image / production OTA / recovery eMMC.
    a). Fill all configs in config file, and put it in the same folder with scripts.
        CONFIG_ANDROID_PATH           : Path to Android code tree
        CONFIG_SDK_PATH               : Path to synaptics-sdk
        CONFIG_ANDROID_OTA_PKG_SOURCE : Path to input OTA package file
        CONFIG_ANDROID_PRODUCT_NAME   : TARGET_PRODUCT name of your Android SDK, e.g. "dolphin"
        CONFIG_SDK_PRODUCT_NAME       : Product name of synaptics-sdk, e.g. "vs680_z1_android_q_launched_stb"
        CONFIG_SIGNK7_FILE_PATH       : File path of your rootrsa_signk7.keystore
        CONFIG_CUSTK_FILE_PATH        : File path of your custk_aesk0.keystore
        CONFIG_EXTRSA_KEY_FILE_PATH   : File path of your extrsa_signk7.keystore
        CONFIG_SKIP_FACTORY_SETTING   : Skip or not to erase Application key stores in /vendor/factory ('Y/y' not erase)

    b). Generate factory eMMC image:
        $ cd [ANDROID_CODEBASE]
        $ . build/envsetup.sh
        $ lunch [PRODUCT]
        $ cd [WORKING_DIR]
        $ ./gen_factory_img.sh

    c). Generate production OTA: (For Android Q OTA package, please check #5 below. For Android S OTA package, please check #6 below.)
        $ cd [WORKING_DIR]
        $ ./gen_OTA.sh

    d). Generate recovery eMMC image:
        $ cd [WORKING_DIR]
        $ ./gen_recovery_img.sh

4. Resign OTA package with new custk and extrsa.
    a). File 6 configs in config file, and put it in the same folder with script.
        CONFIG_ANDROID_PATH             : Path to Android code tree
        CONFIG_SIGNK7_FILE_PATH         : File path of your rootrsa_signk7.keystore
        CONFIG_RESIGN_OTA_PKG_SOURCE    : Path to input OTA package file that will be resigned
        CONFIG_OLD_OEM_CUSTK_FILE_PATH  : File containing the original OEM_CUSTK key
        CONFIG_NEW_OEM_CUSTK_FILE_PATH  : File containing the new raw OEM_CUSTK key to be used
        CONFIG_NEW_EXTRSA_KEY_FILE_PATH : PEM file containing OEM_EXTRSA key
    b). Resign OTA:
        $ ./resign_OTA.sh

5. Generate production OTA for Android Q
   a).  Run resign_from_target_files_Q.py to replace OEM’s rootrsa_signk7 into preboot image.
        ./resign_from_target_files_Q.py --root-signk-store rootrsa_signk7.keystore out/dist/dolphin_ab-target_files.zip out_target.zip

        rootrsa_signk7.keystore: is generated from KMS, you can find it in KMS keys folder
        out/dist/dolphin_ab-target_files.zip: is your Android target files, that usually resides in out/dist folder.
        out_target.zip: is your new target file with preboot modification.

   b). Run ota_from_target_files.py to pack the OTA package from target files.
       ./build/tools/releasetools/ota_from_target_files.py out_target.zip out_ota.zip

       out_target.zip: is the target files that output from resign_from_target_files_Q.py
       out_ota.zip: OTA package for production boards.

6. Generate production OTA for Android S
   a).  Build Android codebase and check ./out/host/linux-x86/bin/add_img_to_target_files is existed. Copy resign_from_target_files_S.py into Android root directory.
   b).  Run resign_from_target_files_S.py to replace OEM’s rootrsa_signk7 into preboot imag
        ./resign_from_target_files_S.py --root-signk-store rootrsa_signk7.keystore out/dist/dolphin_ab-target_files.zip out_target.zip

        rootrsa_signk7.keystore: is generated from KMS, you can find it in KMS keys folder
        out/dist/dolphin_ab-target_files.zip: is your Android target files, that usually resides in out/dist folder.
        out_target.zip: is your new target file with preboot modification.

   c). Run ota_from_target_files.py to pack the OTA package from target files.
       ./build/tools/releasetools/ota_from_target_files.py out_target.zip out_ota.zip

       out_target.zip: is the target files that output from resign_from_target_files_S.py
       out_ota.zip: OTA package for production boards.
