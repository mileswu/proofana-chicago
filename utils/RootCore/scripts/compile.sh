#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

rm -rf $ROOTCOREBIN/bin
mkdir $ROOTCOREBIN/bin
rm -rf $ROOTCOREBIN/include
mkdir $ROOTCOREBIN/include
rm -rf $ROOTCOREBIN/python
mkdir $ROOTCOREBIN/python
rm -rf $ROOTCOREBIN/user_scripts
mkdir $ROOTCOREBIN/user_scripts
rm -rf $ROOTCOREBIN/data
mkdir $ROOTCOREBIN/data
rm -rf $ROOTCOREBIN/lib
mkdir $ROOTCOREBIN/lib

rm -f $ROOTCOREBIN/preload
touch $ROOTCOREBIN/preload
rm -f $ROOTCOREBIN/load
touch $ROOTCOREBIN/load

parallel=""
test "$ROOTCORE_NCPUS" != "" && parallel="-j $ROOTCORE_NCPUS"

for pkg in `cat $pkgfile`
do
    name=`basename $pkg`
    echo compiling $pkg
    if cd $pkg/cmt
    then
	true
    else
	echo "failed to enter package $pkg"
	exit 2
    fi
    if test -f precompile.RootCore
    then
	if test \! -x precompile.RootCore
	then
	    echo execute flag not set on precompile.RootCore, please run
	    echo   chmod +x $pkg/cmt/precompile.RootCore
	    exit 5
	fi
	if ./precompile.RootCore
	then
	    true
	else
	    echo failed to precompile package $pkg
	    exit 6
	fi
    fi
    if make $parallel -f Makefile.RootCore $*
    then
	true
    else
	echo "failed to compile package $pkg"
	exit 3
    fi
    $ROOTCOREDIR/scripts/link_package.sh $pkg
    test "`$ROOTCOREDIR/scripts/get_field.sh $pkg/cmt/Makefile.RootCore PACKAGE_NOCC`" != "1" && \
	echo $pkg | sed 's/.*\///' >>$ROOTCOREBIN/load
    for lib in `$ROOTCOREDIR/scripts/get_field.sh Makefile.RootCore PACKAGE_PRELOAD`
    do
	echo lib$lib >>$ROOTCOREBIN/preload
    done
done

if test "x$ROOTCOREGRID" != "x1"
then
    if root -l -b -q $ROOTCOREDIR/scripts/load_packages.C+
    then
	true
    else
	echo "failed to test load libraries"
	exit 4
    fi
fi
