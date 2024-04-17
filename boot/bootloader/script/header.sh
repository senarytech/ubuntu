#!/bin/bash
{
 if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ] ; then
	name=`basename $0`
	echo
	echo "Usage: $name input_file output_file pattern"
	echo
	echo "This batch file convert hardward memory map header file into software format."
	echo "In general case, the input is mem_map.h; the output is memmap.h"
	echo
	echo "Example: $name mem_map.h memmap.h MEMMAP_"
	echo
	exit 1
 elif [ ! -e $1 ] ; then
	echo
	echo "The input header file $1 does NOT exist."
	echo
	exit
 else
	echo "/*" > $2
	echo " * Copyright Marvell Semiconductor, Inc. 2006. All rights reserved." >> $2
	echo " *" >> $2
	echo " * Register address mapping configure file for rom testing code." >> $2
	echo " */" >> $2
	echo >> $2
	echo "#ifndef	__$3_H__" >> $2
	echo "#define	__$3_H__" >> $2
	echo >> $2
#	grep "#define	$3" $1 >> $2
	grep '#define[ 	]' $1 | sed 's/\/\*.*\*\/[ 	]*#define[ 	]*/#define	/' | sed 's/\/\*.*\*\///' | grep '#define[ 	]*[(0-9)(a-z)(A-Z)(_)]*[ 	][ 	]*[0-9].*$' >> $2

	echo >> $2
	echo "#endif" >> $2
	echo >> $2
 fi
}
