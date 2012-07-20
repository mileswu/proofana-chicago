#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

dependency=`$ROOTCOREDIR/scripts/get_dependency.sh $*` || exit $?

result="-L$ROOTCOREBIN/lib"
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
    lib=`$ROOTCOREDIR/scripts/get_field.sh $pkg/cmt/Makefile.RootCore PACKAGE_BINFLAGS`
    name=`basename $pkg`
    test "`$ROOTCOREDIR/scripts/get_field.sh $pkg/cmt/Makefile.RootCore PACKAGE_NOCC`" != "1" && \
	result="$result -l$name"
    result="$result $lib"
    for preload in `$ROOTCOREDIR/scripts/get_field.sh $pkg/cmt/Makefile.RootCore PACKAGE_PRELOAD`
    do
	result="$result -l$preload"
    done
done
echo $result
