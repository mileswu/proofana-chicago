#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

NOBUILD=
if test "$1" == "--nobuild"
then
    NOBUILD=--nobuild
    shift
fi

test "x$1" = "x" && echo "no path argument supplied" && exit 1
test -e "$1" && echo "path $1 already exists" && exit 1

DIR=$1

mkdir -p $DIR
echo copying RootCore
$ROOTCOREDIR/scripts/grid_copy.sh $NOBUILD $DIR $ROOTCOREDIR || exit 10

touch $DIR/RootCore/grid_packages

for pkg in `cat $pkgfile`
do
    name=`basename $pkg`
    echo copying $pkg
    $ROOTCOREDIR/scripts/grid_copy.sh $NOBUILD $DIR $pkg || exit 10
    echo $name >>$DIR/RootCore/grid_packages
done
