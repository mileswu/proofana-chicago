#!/bin/sh

test -e $1 || exit 1
grep -E "^[ \t]*$2[ \t]*=[ \t]*" $1 | sed "s/^[ \t]*$2[ \t]*=[ \t]*//"
true
