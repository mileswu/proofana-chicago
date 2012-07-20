#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

dependency=`$ROOTCOREDIR/scripts/get_dependency.sh $*` || exit $?

for dep in $dependency
do
    if pkg=`grep -E /$dep\$ $pkgfile`
    then
	true
    else
	error=$?
	echo failed to find package $dep
	exit $error
    fi
    inc=`$ROOTCOREDIR/scripts/get_field.sh $pkg/cmt/Makefile.RootCore PACKAGE_OBJFLAGS`
    result="$result $inc"
done
echo $result
