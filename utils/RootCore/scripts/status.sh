#!/bin/sh

source "`dirname $0`/preamble.sh" "$0"

for pkg in $ROOTCOREDIR `cat $pkgfile`
do
    echo $pkg:
    cd $pkg
    if test -d .svn
    then
	svn status | grep -v '^?       StandAlone$' | grep -v '^?       obj$' | grep -v '^?       bin$' | grep -v '^?       test-bin$'
    else
	echo '   not in SVN'
    fi
done
