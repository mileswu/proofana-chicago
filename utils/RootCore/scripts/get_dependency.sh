#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

fulldep=""

function add_dep {
    for mydep in $fulldep
    do
	if test "$1" = "$mydep"
	then
	    return 0
	fi
    done

    dir=`grep \/$1\$ $pkgfile`
    if test "$dir" = ""
    then
	echo package $1 not known 1>&2
	return -1
    fi
    dep=`$ROOTCOREDIR/scripts/get_field.sh $dir/cmt/Makefile.RootCore PACKAGE_DEP`
    for mydep in `$ROOTCOREDIR/scripts/get_field.sh $dir/cmt/Makefile.RootCore PACKAGE_TRYDEP`
    do
	dep="$dep `grep \/$mydep\$ $pkgfile | sed 's/.*\///'`"
    done
    for mydep in $dep
    do
	add_dep $mydep || exit $?
    done

    for mydep in $fulldep
    do
	if test "$1" = "$mydep"
	then
	    echo package $1 has cyclical dependency 1>&2
	    return -1
	fi
    done

    fulldep="$fulldep $1"
    return 0
}



for package in $*
do
    add_dep $package || exit $?
done

echo $fulldep
