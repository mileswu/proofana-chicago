#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

rm -f $ROOTCOREDIR/scripts/load_packages_C.*
for pkg in `cat $pkgfile`
do
    echo cleaning $pkg
    cd $pkg
    rm -rf bin obj StandAlone `$ROOTCOREDIR/scripts/get_field.sh cmt/Makefile.RootCore PACKAGE_CLEAN`
done
