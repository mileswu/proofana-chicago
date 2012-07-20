#!/bin/bash

if [ -z $2 ]; then
    echo "Usage: sh makeFileList.sh filelist.txt path"
    exit 1
fi

if [ `echo ${1} | grep -c .txt` -lt 1 ]; then
    echo "Output file needs to end in .txt"
    exit 1
fi

if [[ ! -d "$2" ]]; then
    echo "\"${2}\" is not a directory"
    exit 1
fi

filelist=`/bin/ls ${2} | grep .root`

path="`cd ${2}; pwd`/"

# do substitution to xrootd URLs at SLAC
if [[ "${path}" == \/atlas* ]]; then 
    path=`echo ${2/\/atlas/root:\/\/atlprf01.slac.stanford.edu:1094\/\/atlas}`
fi
if [[ "${path}" == \/xrootd\/atlas* ]]; then 
    path=`echo ${2/\/xrootd\/atlas/root:\/\/atl-xrdr.slac.stanford.edu:11094\/\/atlas\/xrootd}`
fi

echo "Writing file list ${1}..."
echo

for file in $filelist; do
    echo ${path}${file} | tee -a ${1}
done

