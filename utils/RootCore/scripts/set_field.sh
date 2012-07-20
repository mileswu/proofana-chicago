#!/bin/bash

file=$1
tmpfile=$1-
name=`echo $2 | sed 's/\//\\\\\\//g'`
value=`echo $3 | sed 's/\//\\\\\\//g'`

test \! -e $file && echo file not found: $file && exit 1
test -e $tmpfile && echo file is in the way $tmpfile && exit 2

if test "`grep -E "^[ \t]*$2[ \t]*=" $file`" != ""
then
    cat $file | sed "s/^[ \t]*$name[ \t]*=.*/$name = $value/" >$tmpfile
else
    echo "$2 = $3" >$tmpfile
    cat $file >>$tmpfile
fi

mv -f $tmpfile $file
