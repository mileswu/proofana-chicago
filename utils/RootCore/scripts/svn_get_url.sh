#!/bin/sh

# this script takes a package location and converts it into a full URL
# that is understood by SVN.  it can be passed the current URL as a
# second argument to allow for modifying the SVN root to contain the
# user name.

pkg=$1

# pkg=`echo $pkg | svn 's/^svn+ssh:\/\/.*svn.cern.ch/svn+ssh:\/\/svn.cern.ch`

if test "`echo $pkg | grep '^atlas'`" != ""
then
    pkg="svn+ssh://svn.cern.ch/reps/$pkg"
fi

if test "$2" != ""
then
    if test "`echo $2 | grep svn.cern.ch`" != "" -a  "`echo $pkg | grep svn.cern.ch`" != ""
    then
	pkg="`echo $2 | sed 's/svn.cern.ch.*//'`svn.cern.ch`echo $pkg | sed 's/.*svn.cern.ch//'`"
    fi
elif test "x$CERN_USER" != "x"
then
    pkg=`echo $pkg | sed "s/svn+ssh:\/\/svn.cern.ch/svn+ssh:\/\/${CERN_USER}@svn.cern.ch/"`
fi

if test "`echo $pkg | grep '/tags$'`" != ""
then
    pkg=$pkg/`svn ls $pkg | tail -n 1 | sed 's/\///'` || exit $?
    pkg=`echo $pkg | sed 's/tags\/$/trunk/'`
fi

echo $pkg
