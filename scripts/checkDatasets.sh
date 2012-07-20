#!/bin/bash

# This script should be used in conjunction with getFilesFromGridXrootd.sh, as
# it checks the download log files produced by it. It must be run from the datasets
# directory

echo
echo "Starting checking datasets"
echo

datasets="`ls -d */`"

prev_dir=${PWD}

## check datasets
for ds in ${datasets}; do
    cd ${ds}
    ds_name=$(echo ${ds%/})
    files="`grep -c File: get_${ds_name}_missed.log`"
    successful="`grep -c SUCCESSFUL get_${ds_name}_missed.log`"
    exception="`grep -c \"LFC exception\" get_${ds_name}_missed.log`"
    finished="`grep -c Finished get_${ds_name}_missed.log`"
    if [[ ! ${files} -eq ${successful} || ${finished} -lt 1 || ${exception} -gt 0 ]] 
    then
	echo "${ds} not completely downloaded."
    fi
    cd $prev_dir
done

