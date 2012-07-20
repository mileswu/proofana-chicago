#!/bin/bash

if [ -z $2 ]; then
    echo "Usage: sh makeDatasetList.sh datasetlist.txt [filelist1.txt ...]"
    exit 1
fi

if [ `echo ${1} | grep -c .txt` -lt 1 ]; then
    echo "Output file needs to end in .txt"
    exit 1
fi

dsetfilelist="$1"
dsetconfig=`echo ${1/.txt/.config}`
dsetconfigtemp=`echo ${1/.txt/.config.temp}`
shift

if [ -e ${dsetfilelist} ]; then
	rm ${dsetfilelist}
fi

if [ -e ${dsetconfig} ]; then
	rm ${dsetconfig}
fi

if [ -e ${dsetconfigtemp} ]; then
	rm ${dsetconfigtemp}
fi

for entry in $@; do
	if [ `echo ${entry} | grep -c .txt` -lt 1 ]; then
    	echo "Input file ${entry} needs to end in .txt"
    	exit 1
	fi
done

strippedlist=`echo ${@/.txt/}`

# Merging file lists is easy...
echo "Merging file lists..."
cat $@ > ${dsetfilelist}

echo "Processing config files..."
for entry in ${strippedlist}; do
	if [ ! -e ${entry}.config ]; then
		echo "Config file ${entry}.config does not exist"
		exit 1
	fi
	
	cat ${entry}.config >> ${dsetconfigtemp}
	# process the file
	nlines=`grep -e ^n_[0-9]*=[0-9]* ${entry}.config | awk '{print $1}'`
	for nline in ${nlines}; do
		runnumber=`expr "${nline}" : "n_\([0-9]*\)=[0-9]*$"`		
		echo "output_${runnumber}=${entry}" >> ${dsetconfigtemp}
	done
done

cat ${dsetconfigtemp} | grep TREENAME= | uniq > ${dsetconfig}
cat ${dsetconfigtemp} | grep -v TREENAME= >> ${dsetconfig}
rm ${dsetconfigtemp}

