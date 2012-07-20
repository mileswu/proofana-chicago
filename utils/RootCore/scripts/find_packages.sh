#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

DIR=`pwd`
echo looking for packages in $DIR
PACKAGES=
for file in `find -L $DIR -type f -name Makefile.RootCore | grep cmt/Makefile.RootCore$`
do
    pkg=`dirname $file | xargs dirname`
    PACKAGES="$pkg $PACKAGES"
    name1=`basename $pkg`
    name2=`$ROOTCOREDIR/scripts/get_field.sh $pkg/cmt/Makefile.RootCore PACKAGE`
    if test "$name2" == ""
    then
	echo failed to read package name from $pkg/cmt/Makefile.RootCore
	exit 1
    fi
    if test "$name1" != "$name2"
    then
	echo package $pkg should have name $name2
	echo please rename it to `dirname $pkg`/$name2 by typing
	echo   mv \"$pkg\" \"`dirname $pkg`/$name2\"
	exit 1
    fi
done

PKGFILE=$ROOTCOREBIN/packages
rm -f $PKGFILE
touch $PKGFILE

for pkg in $PACKAGES
do
    echo $pkg >>$PKGFILE
done

echo packages found:
cat $PKGFILE

echo
echo sorted packages:
list=`sed 's/.*\///' < $PKGFILE` || exit $?
list=`$ROOTCOREDIR/scripts/get_dependency.sh $list` || exit $?
list=`$ROOTCOREDIR/scripts/get_location.sh $list` || exit $?
rm -f $PKGFILE
touch $PKGFILE
for pkg in $list
do
    echo $pkg >>$PKGFILE
done
cat $PKGFILE
