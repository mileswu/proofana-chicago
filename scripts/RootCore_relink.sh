#!/bin/bash

ORIGDIR=$PWD
cd utils/RootCore
./configure
cd ..
source RootCore/scripts/setup.sh
sh RootCore/scripts/find_packages.sh

PKGFILE=$ROOTCOREDIR/packages

rm -rf $ROOTCOREDIR/bin
mkdir $ROOTCOREDIR/bin
rm -rf $ROOTCOREDIR/include
mkdir $ROOTCOREDIR/include
rm -rf $ROOTCOREDIR/python
mkdir $ROOTCOREDIR/python
rm -rf $ROOTCOREDIR/user_scripts
mkdir $ROOTCOREDIR/user_scripts
rm -rf $ROOTCOREDIR/data
mkdir $ROOTCOREDIR/data
rm -rf $ROOTCOREDIR/lib
mkdir $ROOTCOREDIR/lib

for pkg in `cat $PKGFILE`
do
    name=`basename $pkg`
    echo relinking $pkg
    if cd $pkg/cmt
    then
	true
    else
	echo "failed to enter package $pkg"
	exit 2
    fi
    $ROOTCOREDIR/scripts/link_package.sh $pkg
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

cd $ORIGDIR
