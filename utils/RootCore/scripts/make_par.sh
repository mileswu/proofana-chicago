#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

DIR=/tmp/RootCoreBuild-$$

NOBUILD=
if test "$1" == "--nobuild"
then
    NOBUILD=--nobuild
    shift
fi
LITE=
if test "$1" == "--lite"
then
    LITE=--lite
    shift
fi

OUTFILE=`dirname $ROOTCOREDIR`/RootCore.par
test "$1" != "" && OUTFILE=$1
test "${OUTFILE:0:1}" != "/" && OUTFILE=`pwd`/$OUTFILE

MYDIR=$DIR/`basename $OUTFILE | sed 's/.par$//'`

rm -rf $DIR
if mkdir -p $MYDIR
then
    true
else
    echo failed to create directory $MYDIR
    exit 1
fi
cd $DIR

if test "$LITE" != ""
then
    mkdir $MYDIR/PROOF-INF
    cat $ROOTCOREDIR/PROOF-INF-LITE/SETUP.C | sed "s/%ROOTCOREDIR%/`echo $ROOTCOREDIR | sed 's/\//\\\\\//'g`/" | sed "s/%ROOTCOREBIN%/`echo $ROOTCOREBIN | sed 's/\//\\\\\//'g`/" >$MYDIR/PROOF-INF/SETUP.C
    cat $ROOTCOREDIR/PROOF-INF-LITE/BUILD.sh | sed "s/%ROOTCOREDIR%/`echo $ROOTCOREDIR | sed 's/\//\\\\\//'g`/" | sed "s/%ROOTCOREBIN%/`echo $ROOTCOREBIN | sed 's/\//\\\\\//'g`/" >$MYDIR/PROOF-INF/BUILD.sh
    chmod +x $MYDIR/PROOF-INF/BUILD.sh
else
    $ROOTCOREDIR/scripts/grid_copy.sh $MYDIR $ROOTCOREDIR

    for package in `cat $ROOTCOREBIN/packages`
    do
	echo using package $package
	$ROOTCOREDIR/scripts/grid_copy.sh $NOBUILD $MYDIR $package
	basename $package >>$MYDIR/packages
    done

    ln -s RootCore/PROOF-INF `basename $MYDIR`/PROOF-INF
fi

tar -czf $OUTFILE `basename $MYDIR`

cd
rm -rf $DIR
