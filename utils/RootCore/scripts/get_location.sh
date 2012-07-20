#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

for package in $*
do
    dir=`grep \/$package\$ $pkgfile`
    if test "$dir" = ""
    then
	echo package $package not known 1>&2
	exit -1
    fi
    echo $dir
done
