#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

test \! -f "$1" && echo could not find list file $1 && exit 1

for package in `grep -v '^#' $1`
do
    package=`$ROOTCOREDIR/scripts/svn_get_url.sh $package`
    if test "`basename $package`" == "trunk"
    then
	name=`dirname $package | xargs basename`
    elif test "`basename $package`" == "tags"
    then
	name=`dirname $package | xargs basename`
    elif test "`dirname $package | xargs basename`" == "tags"
    then
	name=`dirname $package | xargs dirname | xargs basename`
    elif test "`dirname $package | xargs basename`" == "branches"
    then
	name=`dirname $package | xargs dirname | xargs basename`
    else
	name=`basename $package`
    fi
    if test \! -d $name
    then
	echo svn co $package $name
	svn co $package $name
    fi
done
