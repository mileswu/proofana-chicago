#!/bin/bash

# use command:
# nohup sh getFilesFromGridXrootd.sh pattern &
# to start

if [ -z $1 ]; then
    echo "Usage: sh getFilesFromGridXrootd.sh \"pattern\"" [test]
    exit 1
fi

echo
echo "Starting dq2-get download"
echo

pattern=$1 #"user.bcbutler.susySlim12.v0*/"

datasets="`dq2-ls \"${pattern}\" | sort`"

## Get datasets
for ds in ${datasets}; do
	if [ ! -z $2 ]; then
		echo "${ds}"
		continue
	fi
    prev_dir=${PWD}
    echo "Current directory: ${prev_dir}"
    echo "Starting download of dataset: ${ds}"
	echo "Making a new directory"
	mkdir ${ds}
	cd ${ds}
	echo "Downloading to directory: ${PWD}"
	ds_name=$(echo ${ds%/})
	echo "Downloading dataset: ${ds_name}" 
	dq2-get -T 6,6 --ignore -D ${ds} &> get_${ds_name}_missed.log&
       #echo "done downloading.." 
	sleep 60
	me=`whoami`
	while [ `ps -u ${me} | grep -c lcg-cp` -gt 6 ]; do
		sleep 60
	done
    cd $prev_dir
done

