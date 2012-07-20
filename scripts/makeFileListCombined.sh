#!/bin/bash

if [ -z $2 ]; then
    echo "Usage: sh makeFileListCombined.sh regexp path [treename]"
    echo "Makes a single combined file list and config file for all directories in \"path\" matching \"regexp\"."
    echo "If no tree name is provided, a config file will not be produced."
    exit 1
fi

if [[ ! -d "$2" ]]; then
    echo "\"${2}\" is not a directory"
    exit 1
fi

dirlist=`ls -d ${2}/*/ | grep ${1}`

echo "Enter dataset name without .txt suffix in the form identifier.fileevent:"
read dataset

# remove old files if exist
rm ${dataset}.txt >& /dev/null
rm ${dataset}.config >& /dev/null

for dir in $dirlist; do
    echo "Directory: $dir"
	sh ../scripts/makeFileList.sh ${dataset}.txt ${dir}
done

# run the automatic file list config script
if [ ! -z $3 ]; then
	sh ../scripts/makeFileListConfig.sh ${dataset}.txt ${3}
fi
