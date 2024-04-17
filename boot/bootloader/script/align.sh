#!/bin/bash
if [ $# != "2" ];then
        echo "Usage: $0 input_file alignment_size"
        exit 1
fi
IN_FILE=$1
ALIGN_SIZE=$2

SIZE=`stat -c %s $IN_FILE`
APPEND_SIZE=`expr $ALIGN_SIZE - $SIZE % $ALIGN_SIZE`

if [ $APPEND_SIZE -lt $ALIGN_SIZE ]; then
dd if=/dev/zero of=$IN_FILE bs=1 seek=$SIZE count=$APPEND_SIZE conv=notrunc || exit 1
fi
