#!/bin/bash

test "x$ROOTCOREDIR" \!= "x" && echo "ROOTCOREDIR set, please use clean shell" && exit 1

test "x$1" = "$1" && echo "no path argument supplied" && exit 1
test \! -d "$1" && echo "path $1 doesn't exist" && exit 1

DIR=$1

if cd $1/RootCore
then
    true
else
    echo "failed to enter $1/RootCore"
    exit 1
fi
./configure
source scripts/setup.sh

rm -rf bin data include lib python user_scripts
mkdir  bin data include lib python user_scripts

rm -f packages
touch packages
for package in `cat grid_packages`
do
    echo $1/$package >> packages
    $ROOTCOREDIR/scripts/link_package.sh $1/$package
done
