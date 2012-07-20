#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

export ROOTCORE_AUTO_UT=1

if test "$1" == "--fast"
then
    export ROOTCORE_FAST_UT=1
    shift
fi

count=0
success=0
fails=""
for pkg in `cat $ROOTCOREBIN/packages`
do
    for test in $pkg/test-bin/ut_*
    do
	if test -f $test -a -x $test
	then
	    count=`expr $count + 1`
	    echo running test $test
	    if time $test
	    then
		success=`expr $success + 1`
	    else
		fails="$fails $test"
	    fi
	fi
    done
done

echo $success tests out of $count passed
if test "$fails" != ""
then
    echo failed tests:
    for test in $fails
    do
	echo $test
    done
fi
