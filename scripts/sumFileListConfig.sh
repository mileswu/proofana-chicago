#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: sh sumFileListConfig.sh input_file_list.config"
    exit 1
fi

if [ `echo ${1} | grep -c .config` -lt 1 ]; then
    echo "Input file needs to end in .config"
    exit 1
fi

sum=0

for line in `cat $1`; do
   prefix=`echo ${line} | awk -F"_" '{print $1}'`
   if [ "${prefix}" == "n" ]; then
	echo "${line}"
	evts=`echo ${line} | awk -F"=" '{print $2}'`
	sum=$[${sum}+${evts}]
   fi
done

echo "Sum: ${sum}"
