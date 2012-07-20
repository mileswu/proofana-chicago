#!/bin/bash

# Run all datasets found in ../filelists/ that match a regular expression with the specified run file

if [ -z $4 ]; then
	echo "Usage: sh runBatch.sh regexp runfile identifier username [test]"
	exit 1
fi

datasets="`ls ../filelists/*.txt | xargs -n1 basename | grep ${1}`"




## Get datasets
for ds in ${datasets}; do
	ds=`echo ${ds//.txt/}` # Remove .txt extension
	echo "Running dataset ${ds}..."
	if [ -z $5 ]; then
		root -l -b -q ${2}\(\"cluster\",\"${3}\",\"${ds}\",\"${4}\"\)
	fi
done

