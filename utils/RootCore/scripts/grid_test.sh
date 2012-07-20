#!/bin/bash

test "x$ROOTCOREDIR" != "x" && source $ROOTCOREDIR/scripts/unsetup.sh

NOBUILD=
if test "$1" == "--nobuild"
then
    NOBUILD=--nobuild
    shift
fi

test \! -f "$1/scripts/setup.sh" && echo rootcore not found at $1 && exit 2
test "x$2" == "x" && echo no space to run specified && exit 3

dir_submit="$2/grid_submit"
dir_compile="$2/grid_compile"
dir_run="$2/grid_run"
for dir in "$dir_submit" "$dir_compile" "$dir_run"
do
    test -e $dir && echo please remove $dir && echo \ \ rm -rf $dir && exit 4
done


if test "$NOBUILD" = ""
then
    (source $1/scripts/setup.sh && $ROOTCOREDIR/scripts/grid_submit.sh "$dir_submit") || exit 10
else
    (source $1/scripts/setup.sh && $ROOTCOREDIR/scripts/grid_submit_nobuild.sh "$dir_submit") || exit 10
fi

mv "$dir_submit" "$dir_compile" || exit 11

if test "$NOBUILD" = ""
then
    (source "$dir_compile/RootCore/scripts/grid_compile.sh" "$dir_compile") || exit 20
fi

mv "$dir_compile" "$dir_run" || exit 21

if test "$NOBUILD" = ""
then
    (source "$dir_run/RootCore/scripts/grid_run.sh" "$dir_run" && root -l -b -q $ROOTCOREDIR/scripts/load_packages.C+) || exit 30
else
    (source "$dir_run/RootCore/scripts/grid_run_nobuild.sh" "$dir_run" && root -l -b -q $ROOTCOREDIR/scripts/load_packages.C+) || exit 30
fi

rm -rf "$dir_run"

echo it appears this RootCore installation is grid compatible
