#!/bin/sh
set -e

#LD_LIBRARY_PATH="/usr/lib:/opt/syna/lib"
LD_LIBRARY_PATH="/usr/lib:/usr/lib64:/opt/syna/lib:/opt/syna/lib64:/usr/local/lib":$LD_LIBRARY_PATH
AMP_TA_PATH="/opt/syna/ta"
AMP_FW_PATH="/opt/syna/fw"
#PATH="/bin:/sbin:/usr/bin:/usr/sbin:/opt/syna/bin"
PATH="/bin:/sbin:/usr/bin:/usr/sbin:/opt/syna/bin:/usr/local/bin"
export LD_LIBRARY_PATH
export AMP_TA_PATH
export AMP_FW_PATH
export PATH

. /lib/lsb/init-functions
mkdir -p /data/tee
tee_daemon &
ampservice -l1 2>&1 &
ampdiag log off
test_disp hdcp disable
test_disp setformat 0 24 0 2 1 0 0
ampclient_alpha 15 -t 16 &
ampclient_samples 18 &
#hciattach -n -s 115200 /dev/ttyS2 any &
