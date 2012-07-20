#!/bin/bash

# This script is a batch script for checkFilelist.C, which compares the total number of
# events claimed in the the config file with the number of events found in the root files
# in the specified file list. These number need not match in the case of slims. Run it in
# the filelists/ directory.

echo
echo "Starting checking filelists"
echo

datasets=`ls -d *.config | awk -F".conf" '{print $1}'`

if [ ! -z $1 ]; then
	datasets=`echo "${datasets}" | grep $1`
fi

echo "${datasets}"

# check filelists/config files
for ds in ${datasets}; do
	treename=`grep TREENAME= ${ds}.config | uniq | awk -F"=" '{print $2}'`
	count=`cat ${ds}.config | uniq | grep -c TREENAME=`
	if [ ${count} -eq 1 ]; then
		root -l -b -q ../scripts/checkFileList.C\(\"${ds}.txt\",\"${treename}\"\)
	else
		echo "${ds}.config does not contain a TREENAME key or has multiple TREENAME keys with different values"
	fi
done

