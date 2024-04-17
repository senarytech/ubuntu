These scripts are for the customer to change the OEM keys used to resign OTA/eMMC images for VS680, please follow the steps below:

 NOTE:
    resign_from_target_files.py is designed to apply on a release SDK;
    if you are working on ROMK development SDK, please replace it with 'resign_from_target_files_ROMK.py'.


 1. prepare the keys and stores below that are used to resign Android target file and eMMC image.
    new oem_custk            : File containing the new raw OEM_CUSTK key to be used reencrypt images
    new oem_custk_store      : File containing the new OEM_CUSTK store will be injected in key images
    new oem_ext_rsa          : PEM file containing new OEM_EXTRSA private key to be used resign images
    new oem_ext_rsa_store    : File containing the new OEM_EXTRSA singk7 store that will be injected in key images
    new root_signk_store     : Store containing the new root RSA signk7 store that will be replaced in preboot image
    old oem_custk            : File containing the raw OEM_CUSTK key used to encrypt the images in input target files zip
                               Be aware of this key is same as synaptics-sdk/security/keys/chip/vs680/A0/mid-00000001/oem/codetype_5/custk.bin
    new Android avb_key      : Store containing the new Android public AVB key (Optinal, if you want to replace with new AVB_key)

 2. check out/dist/XXX-target_files-XXX.zip is existing, if not please build Android codebase first.

 3. For Android A/B (Seamless) update system, please substitute resign_from_target_files_ab.py for resign_from_target_files.py, resign_from_target_files_ab_ROMK.py for resign_from_target_files_ROMK.py and rebuild_emmc_from_target_files_ab.py for rebuild_emmc_from_target_files.py in the following steps.

 4. copy all scripts and binaries to Android root directory
    resign_from_target_files.py
    rebuild_emmc_from_target_files.py
    resign_from_target_files_ROMK.py
    resign_image_v4
    sign_image_v4
    simg2simg

    For A/B system:
    resign_from_target_files_ab.py
    rebuild_emmc_from_target_files_ab.py
    resign_from_target_files_ab_ROMK.py
    resign_image_v4
    sign_image_v4
    simg2simg

 5. Before running any python scripts, run following commands first in Android ROOT directory:
    $ . build/envsetup.sh
    $ lunch [profile]

 6. Resigning target files and APKs:
    Choose one to fit your condition:

    (a) If avb_key is remain unchanged (as SDK).
        $ ./build/tools/releasetools/sign_target_files_apks.py  out/dist/*-target_files-*.zip  signed-target_files.zip

    (b) Android Q, API level = 28+ (avb_enable = true)
        $ ./build/tools/releasetools/sign_target_files_apks.py -v -o -d $KEY_FOLDER                               \
                                --avb_boot_key    $KEY_FOLDER/RSA4096.pem  --avb_boot_algorithm    SHA256_RSA4096 \
                                --avb_system_key  $KEY_FOLDER/RSA4096.pem  --avb_system_algorithm  SHA256_RSA4096 \
                                --avb_vendor_key  $KEY_FOLDER/RSA4096.pem  --avb_vendor_algorithm  SHA256_RSA4096 \
                                --avb_dtbo_key    $KEY_FOLDER/RSA4096.pem  --avb_dtbo_algorithm    SHA256_RSA4096 \
                                --avb_vbmeta_key  $KEY_FOLDER/RSA4096.pem  --avb_vbmeta_algorithm  SHA256_RSA4096 \
                                out/dist/*-target_files-*.zip                                                     \
                                signed-target_files.zip

            The above command is an example, you can use individual keys for boot/system/vendor/dtbo/vbmeta images.
            Remember to pass vbmeta' key in Step 8. as parameter: --avb_key $KEY_FOLDER/RSA4096.pem

 7. Resigning target files to change the OEM keys:
    run resign_from_target_files.py script to resign Android target files
    $ ./resign_from_target_files.py --oem-ext-rsa-store new_extrsa.keystore \
                                    --new-oem-custk-store new_custk.keystore \
                                    --oem-ext-rsa new_extrsa.pem \
                                    --new-oem-custk new_custk.bin \
                                    --root-signk-store rootrsa_signk7.keystore \
                                    --old-oem-custk old_custk.bin \
                                    signed-target_files.zip \
                                    resigned_target_files.zip

    For ROMK SDK:
    $ ./resign_from_target_files_ROMK.py
      (parameters almost the same as above resign_from_target_files.py, except no need '--root-signk-store' parameter)

    For A/B system:
    $ ./resign_from_target_files_ab.py --oem-ext-rsa-store new_extrsa.keystore \
                                       --new-oem-custk-store new_custk.keystore \
                                       --oem-ext-rsa new_extrsa.pem \
                                       --new-oem-custk new_custk.bin \
                                       --root-signk-store rootrsa_signk7.keystore \
                                       --old-oem-custk old_custk.bin \
                                       signed-target_files.zip \
                                       resigned_target_files.zip

 8. Generating a resigned OTA image from the resigned target files:
    run ota_from_target_files ( Android standalone tool provided in [android_root]/build/tools/releasetools ) to generate resigned OTA package
    $ ./build/tools/releasetools/ota_from_target_files.py resigned_target_files.zip resigned_OTA_file.zip

 9. Generating an eMMC image from the resigned target files:
    run rebuild_emmc_from_target_files.py to generate resigned eMMC images
    $ ./rebuild_emmc_from_target_files.py --oem-ext-rsa new_extrsa.pem \
                                          --new-oem-custk new_custk.bin \
                                          --oem-ext-rsa-store new_extrsa.keystore \
                                          --new-oem-custk-store new_custk.keystore \
                                          --old-oem-custk old_custk.bin \
                                          -o resigned_eMMCimg \
                                          original_eMMCimg \
                                          resigned_target_files.zip \
                                          --avb_key avb_key (optinal)
    For A/B system:
    $ ./rebuild_emmc_from_target_files_ab.py --oem-ext-rsa new_extrsa.pem \
                                             --new-oem-custk new_custk.bin \
                                             --oem-ext-rsa-store new_extrsa.keystore \
                                             --new-oem-custk-store new_custk.keystore \
                                             --old-oem-custk old_custk.bin \
                                             -o resigned_eMMCimg \
                                             original_eMMCimg \
                                             resigned_target_files.zip \
                                             --avb_key avb_key (optinal)
