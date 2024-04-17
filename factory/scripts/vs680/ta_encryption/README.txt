Goal: This package is used to encrypt Vendor TA image.
--------------------------------------------------------------------------------
Prepare:

     1. First, you have to submit "TZ50 Trusted Application Permission Request Form" to Synaptics.
        There are some fields to fill:
            TA vendor name
            TA vendor ID
            TA UUID
            TA Root Cert Version and TA Root Cert Version Mask

        You also need to generate an AES and an RSA key: TA CERT CustKey and TA CERT ExtRsaKey.

     2. Deliver the TA CERT CustKey and the public key of TA CERT ExtRsaKey to Synaptics.
        Then, Synaptics will generate the TA Root Cert for your TAs.
        And, Synaptics also give you the Segmentation ID (SegID).

        Note.
            One vendor may have some TAs, and the high 14-bytes of UUID for these TAs are the same.
            The lowest 2 bytes are used to distinguish these TAs.
            And, these TAs share the same TA Root Cert.

     3. Modify the signing configurations.
        Modify the configuration file in config/signing.cfg, and make your changes accordingly.
        For example,
            VNDR_TACERT_VendorID
            VNDR_TACERT_VendorName
            VNDR_TACERT_SegID
            VNDR_TACERT_Ver
            VNDR_TA_VendorID
            VNDR_TA_VendorName
            VNDR_TA_SegID
            VNDR_TA_Ver
            TAROOTPERM_UUID
            TAPERM_UUID
            TAPERM_GROUP

     4. Compile your TA to ELF format, and rename it to [TA_NAME].ta.clear.

--------------------------------------------------------------------------------
How to encrypt Vendor TA?

     1. Copy TA CERT CustKey and TA CERT ExtRsaKey into keys/ folder and rename them as below:
        TA CERT CustKey   -> TA_RootCustKey.bin
        TA CERT ExtRsaKey -> TA_RootExtRsaKey.pem

     2. Copy TA Root Cert that get from Synaptics into TEE_root_cert/ folder.

     3. Run run_all.sh to encrypt your clear TA
        Usage: ./run_all.sh <TA_File>

               <TA_File>       : clear TA file (ELF)

        Example:
              ./run_all.sh sample/libtest.ta.clear

        We also provide a sample for your verification, you can run above command directly to encrypt sample TA.

--------------------------------------------------------------------------------
Files and Directories

    /bin                binary tools
    /config             signing configurations
    /keys               keys and stores
    /log                log files
    /out_ta             encrypted TA out folder
    /out_tacert         TA cert out folder
    /sample             sample clear TA (ELF)
    /TEE_root_cert      TA root cert folder
