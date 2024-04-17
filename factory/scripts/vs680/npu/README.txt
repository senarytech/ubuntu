How to use TZ 5.0 NPU signing tool

Note: 1) TA vendor need submit "GenX TZ50 Trusted Application Permission Request Form" to Synaptics
      	2) All input parameters are stored in Genx_npu_ControllingData.csv and ./keys/kdf_npu/config. User can update it before generate npu image. Such as, VendorGID, UUID,TA_Permission_group_name_list, NPU_BGM_Len, etc

      3) The default VendorGID is 0x1. If need update, please modify
         a. TACERT_VendorGID, TACERT_PermVendorGID in .csv file
         b. NPU_VendorGID in .csv file
         c. TA_VENDOR_GID  in ./keys/kdf_ta/config
         The above parmeters should have same value.

      4) The default configuration is dev configuration, if need enable produciton configuration. Please modify
         a. TACERT_ProdImgFlag,0x00000001 in .csv file
         b. NPU_ProdImgFlag,0x00000001 in .csv file
         c. DEV_IMAGE_FLAG=0x01 in ./keys/kdf_ta/config

      5) If need update SegID and SegIDMask, please modify
         a. TACERT_SegID, TACERT_SegIDMask in .csv file
         b. NPU_SegID, NPU_SegIDMask in .csv file

      6) The TA_Permission_group_name_list is stored in ./input/config
      7) The raw bin file is stored in ./input folder，user can modify name in .csv file
      8) If TA_Root_Cert.rcert isn't generated, it will exit with error report. The ta root cert is in ./input folder
      9) The npu file is generated in ./output folder. User can modifiy the name in .csv file if needed

1. Generate keys
 Note: For testing purpose, user can use the flowing commands to generate test keys at his/her own discretion. The keys in ./keys folder is generated randomly by the following commands. User may need update the key before using the tool.
    1) TA_K0_RSA_Key generating
       openssl genrsa  -out $KeyDIR/TA_K0_RSA.priv.pem 2048
       openssl rsa -in $KeyDIR/TA_K0_RSA.priv.pem  -pubout -out $KeyDIR/TA_K0_RSA.pub.pem

    2) TA_K0_AES_Key generating
       dd if=/dev/random of=$KeyDIR/TA_K0_AES_Key.bin bs=1 count=16

    3) TA_K1_RSA_Key generating
       openssl genrsa -out $KeyDIR/TA_K1_RSA.priv.pem 2048
       openssl rsa -in $KeyDIR/TA_K1_RSA.priv.pem  -pubout -out $KeyDIR/TA_K1_RSA.pub.pem

    4) TA_K1_AES_key generating
       dd if=/dev/random of=$KeyDIR/TA_K1_AES_Key.bin bs=1 count=16

    5) Use KDF (refer to ./keys/kdf_npu) shell script to derive TA_K0_AES_Key_TA_CERT.bin from TA_K0_AES_Key. The input file is TA_K0_AES_Key.bin, and output file is TA_K0_AES_Key_TA_CERT.bin.
       ./npu_kdf.sh  TA_K0_AES_Key.bin
    6) Use KDF (refer to ./keys/kdf_npu) shell script to derive TA_K1_AES_Key_MODEL.bin from TA_K1_AES_Key.bin. The input file is TA_K1_AES_Key.bin, and output file is TA_K1_AES_Key_MODEL.bin
       ./npu_kdf.sh  TA_K1_AES_Key.bin

2. Get Root Cert from Synaptics

 Note:1) TA vendor need provide TA_K0_RSA.pub.pem, TA_K0_AES_Key.bin and TA Permission Superset to Synaptics, then Synaptics provides TA Root Certificate to TA vendor. TA Vendor need put the root cert into ./input folder and modify the name in .csv file if needed
    2)  Please make sure the transition process is in a security environment, such as, use PGP encryption/signature method

3. Generate TA Cert
  ./gen_ta_cert.sh Genx_npu_ControllingData.csv
 Note: This step can be ignored if execute Step4 as below.

4. Gen Full Image
  ./gen_full_ta_img.sh Genx_npu_ControllingData.csv

