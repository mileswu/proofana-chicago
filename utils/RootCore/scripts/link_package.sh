#!/bin/sh

source "`dirname $0`/preamble.sh" "$0"

pkg=$1
name=`basename $pkg`

test -d $pkg/$name && ln -sf $pkg/$name $ROOTCOREBIN/include/
test -d $pkg/python && ln -sf $pkg/python $ROOTCOREBIN/python/$name
test -d $pkg/scripts && ln -sf $pkg/scripts $ROOTCOREBIN/user_scripts/$name
(test -d $pkg/data && ln -sf $pkg/data $ROOTCOREBIN/data/$name) || (test -d $pkg/share && ln -sf $pkg/share $ROOTCOREBIN/data/$name)
test -f $pkg/StandAlone/lib${name}.so && ln -sf $pkg/StandAlone/lib${name}.so $ROOTCOREBIN/lib/
if test -d $pkg/bin
then
    for file in $pkg/bin/*
    do
	test -f $file && ln -sf $file $ROOTCOREBIN/bin/
    done
fi
