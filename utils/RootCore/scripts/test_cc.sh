#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

if test "$ROOTCORETEST_DIR" == ""
then
    DIR=/tmp/RootCoreTest.$$
else
    DIR="$ROOTCORETEST_DIR"
fi

rm -rf $DIR
mkdir $DIR || exit 1
cat >$DIR/test.cxx

make -f $ROOTCOREDIR/Makefile-single $1 INPUT=$DIR 2>$DIR/err >$DIR/log && RESULT=success

if test "$ROOTCORETEST_DIR" == ""
then
    rm -rf $DIR
fi

test "$RESULT" == "success"
