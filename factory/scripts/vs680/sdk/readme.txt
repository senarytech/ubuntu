latest update: 2019/01/29

A. Full SDK
1. ./get_romk_images.py $YOURPATH/syna-release.tgz 

2. a zip file (romk_images.$TIME.zip) is generated which contains synaptics-encrypted ROMK images and imglst.json

3. upload romk_images.$TIME.zip to signing server, and download outputimage.zip

4. ./patch_mdk.py outputimage.zip $YOURPATH/syna-release.tgz

5. you'll get syna-release.mdk.tgz in this folder

-----------------------------------------------------------------------------------------------------------

B. Partial SDK
1. use the same folder structure as SDK
   1) EROM, TSM, SYSINIT, MINILOADER
      syna-release/boot/preboot/

   2) TZK
      syna-release/tee/tee/products/

   3) TA
      syna-release/ta_enc/

   4) FW
      syna-release/fw_enc/

2. tar -zcvf $YOURPATH/syna-release.tgz syna-release

3. ./get_romk_images.py $YOURPATH/syna-release.tgz 

4. a zip file (romk_images.$TIME.zip) is generated which contains synaptics-encrypted ROMK images and imglst.json

5. upload romk_images.$TIME.zip to signing server, and download outputimage.zip

-----------------------------------------------------------------------------------------------------------

C. Single image
1. run the scripts under single folder to pack one image, for example:
   ./pack_erom_factory.py $YOURPATH/erom.bin
   ./pack_erom.py $YOURPATH/erom.bin
   ./pack_fw.py $YOURPATH/zsp_misc.fw
   ./pack_miniloader.py $YOURPATH/miniloader_en.bin
   ./pack_sysinit.py $YOURPATH/sysinit_en.bin
   ./pack_tz30_ta.py $YOURPATH/libdrmse.ta
   ./pack_tz50_ta.py $YOURPATH/libdrmse.ta
   ./pack_tsm_factory.py $YOURPATH/tsm.bin
   ./pack_tsm.py $YOURPATH/tsm.bin
   ./pack_tz30_tzk.py $YOURPATH/tee_en.bin
   ./pack_tz50_tzk.py $YOURPATH/tee_en.bin
   ./pack_bkl.py $YOURPATH/bkl_bk.subimg

2. a zip file (romk_images.$TIME.zip) is generated which contains synaptics-encrypted ROMK image and imglst.json
3. upload romk_images.$TIME.zip to signing server, and download outputimage.zip

-----------------------------------------------------------------------------------------------------------

D. Pack images manually

1. the format of the input for signing server:
   1) a .zip file which contains romk images and imglst.json
   2) you can create any folders/sub-folders for romk images
   3) edit the images types and pathes in imglst.json

2. example
   1) prepare to upload these files:
      libdrmse.ta
      mytee/tee_en.bin
      boot/preboot/erom.bin
   2) edit imglst.json:
[
{"name": "libdrmse.ta", "type": "ta50",  "path": "./"},
{"name": "tee_en.bin", "type": "tzk50",  "path": "mytee/"},
{"name": "erom.bin", "type": "erom", "path": "boot/preboot/"}
]
   3) zip files:
      zip -q romk_images_example.zip imglst.json libdrmse.ta mytee/tee_en.bin boot/preboot/erom.bin

3. upload romk_images_example.zip to signing server, and download outputimage.zip

