#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

DIR=$1
test "x$DIR" = "x" && DIR=`pwd`

mkdir -p "$DIR/cmt"
mkdir -p "$DIR/`basename $DIR`"
mkdir -p "$DIR/Root"

MAKEFILE=$DIR/cmt/Makefile.RootCore
if test \! -f "$MAKEFILE"
then
    echo >>"$MAKEFILE" '# this makefile also gets parsed by shell scripts'
    echo >>"$MAKEFILE" '# therefore it does not support full make syntax and features'
    echo >>"$MAKEFILE" '# edit with care'
    echo >>"$MAKEFILE"
    echo >>"$MAKEFILE" "PACKAGE          = `basename $DIR`"
    echo >>"$MAKEFILE" 'PACKAGE_PRELOAD  = '
    echo >>"$MAKEFILE" 'PACKAGE_CXXFLAGS = '
    echo >>"$MAKEFILE" 'PACKAGE_OBJFLAGS = '
    echo >>"$MAKEFILE" 'PACKAGE_LDFLAGS  = '
    echo >>"$MAKEFILE" 'PACKAGE_BINFLAGS = '
    echo >>"$MAKEFILE" 'PACKAGE_DEP      = '
    echo >>"$MAKEFILE" 'PACKAGE_TRYDEP   = '
    echo >>"$MAKEFILE" 'PACKAGE_CLEAN    = '
    echo >>"$MAKEFILE" 'PACKAGE_NOOPT    = 0'
    echo >>"$MAKEFILE" 'PACKAGE_NOCC     = 0'
    echo >>"$MAKEFILE" ''
    echo >>"$MAKEFILE" 'include $(ROOTCOREDIR)/Makefile-common'
fi

LINKDEF=$DIR/Root/LinkDef.h
if test \! -f "$LINKDEF"
then
    echo >>"$LINKDEF" '#ifdef __CINT__'
    echo >>"$LINKDEF" ''
    echo >>"$LINKDEF" '#pragma link off all globals;'
    echo >>"$LINKDEF" '#pragma link off all classes;'
    echo >>"$LINKDEF" '#pragma link off all functions;'
    echo >>"$LINKDEF" '#pragma link C++ nestedclass;'
    echo >>"$LINKDEF" ''
    echo >>"$LINKDEF" '#endif'
fi
