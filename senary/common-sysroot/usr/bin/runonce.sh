#!/bin/sh

resize2fs /dev/mmcblk0p16
resize2fs /dev/mmcblk1p16

mv /usr/bin/runonce.sh  /usr/bin/runonce_bk.sh
