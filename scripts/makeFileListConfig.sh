#!/bin/bash

if [ -z $2 ]; then
    echo "Usage: sh makeFileListConfig.sh input_file_list.txt treename"
    exit 1
fi

if [ `echo ${1} | grep -c .txt` -lt 1 ]; then
    echo "Input file needs to end in .txt"
    exit 1
fi

echo "If this script fails, be sure you have Athena 16.0.2 loaded as well as a valid GRID proxy."

filelist=${1}
filelistconfig=${1/.txt/.config}
treename=${2}
shift
shift

#source /afs/cern.ch/atlas/software/builds/AtlasCore/15.6.9/Database/Bookkeeping/AMIClients/pyAMI/cmt/setup.sh
source /afs/slac.stanford.edu/g/atlas/d/AtlasCore/16.0.2/Database/Bookkeeping/AMIClients/pyAMI/cmt/setup.sh

prev_ds_num=9999999999

echo "TREENAME=${treename}" | tee -a ${filelistconfig}

for file in `cat ${filelist}`; do
    if [ `echo ${file} | grep -c ${prev_ds_num}` -lt 1 ]; then 
	# parse filename into dataset
	slim=`echo ${file} | awk -F"/" '{ for (i=NF; i>0; i--) printf("%s ", $i)}' | awk '{print $2}'`
	
	isData=0
	# strip off user slim prefix
	ds=`expr ${slim} : '.*\(mc[0-1][0-9].*_[0-9]TeV.*\)'`
	if [[ "${ds}" == "" ]]; then
		ds=`expr ${slim} : '.*\(data[0-1][0-9]_[0-9]TeV.*\)'`
		isData=1
	fi
	
	#check for groupXX prefixes
	groupds=`expr ${slim} : '.*\(group[0-1][0-9].*\)'`
	if [[ "${groupds}" != "" ]]; then
		grjetTest=`expr ${groupds} : '.*\(GRJETS.*\)'`	
		if [[ "${grjetTest}" != "" ]]; then
		    newds=${groupds%%\.GRJETS.*GRJETS}
		    ds=`expr ${newds} : '.*\(mc[0-1][0-9].*_[0-9]TeV.*\)'`
	    else
		    ds="${groupds}"
		fi
	fi	
	
	#check for _AANT suffixes
	aantds=`expr ${ds} : '\(.*\)_AANT'`
	if [[ "${aantds}" != "" ]]; then
		ds="${aantds}"
	fi	
	
	if [[ "${ds}" == "" ]]; then 
	    echo "Dataset name does not match regexp"
	    exit 1
	fi

	#mcyear=`expr ${slim} : '.*mc\(.*\)_[0-9]TeV.*'` #for k-factor paths

	echo
	echo "Getting cross section information from AMI for dataset \"${ds}\""
	echo

	output=`amiCommand GetDatasetInfo -logicalDatasetName=${ds}`

	echo "AMI query result:"
	echo $output
	echo

	ds_num=`echo ${output} | awk -F"=" 'BEGIN { RS = "->" } ; /datasetNumber/ { print $2 }'`
	if [ ${isData} -eq 1 ]; then
		ds_num=`echo ${output} | awk -F"=" 'BEGIN { RS = "->" } ; /runNumber/ { print $2 }'`	
	fi
	n=`echo ${output} | awk -F"=" 'BEGIN { RS = "->" } ; /totalEvents/ { print $2 }'`
	xs=`echo ${output} | awk -F"=" 'BEGIN { RS = "->" } ; /approx_crossSection/ { print $2 }'`

	if [[ "$n" == "" ]]; then
	    echo "Dataset ${ds} not found in AMI."
	    exit 1
	fi

	if [ ${isData} -eq 0 ]; then
		# convert scientific notation, units to pb
		exp=`expr "${xs}" : ".*E\(.*\)"`
		fac=`expr "${xs}" : "\(.*\)E.*"`
		# pb from fb

		exp=`echo ${exp} | sed 's/\(-*\)0*\([0-9]\)/\1\2/'` # remove leading zeros so bash doesn't think exp is octal...
		exp=$((${exp}+3))
		scale=0
		if [[ ${exp} -lt 4 ]]; then
	    	scale=$((4-${exp})) # decimal places for bc
		fi
		# take care of scientific notation
		xs=`echo "scale=${scale}; ${fac}E${exp}" | sed 's/E/\*10\^/' | sed 's/+//' | bc`
	fi

	name="name_${ds_num}=${ds}/"
	xs="xs_${ds_num}=${xs}"
	n="n_${ds_num}=${n}"

	#do k-factor retrieval for some Alpgen samples, often does not work!!
	#pyfilename=`elinks http://alxr.usatlas.bnl.gov/lxr/source/atlas/Generators/MC${mcyear}JobOptions/share/ | grep ${ds_num} | awk '{print $2}' | awk -F"]" '{print $2}'`
	#pyfile=`elinks http://alxr.usatlas.bnl.gov/lxr/source/atlas/Generators/MC${mcyear}JobOptions/share/${pyfilename}` 
	#kfactor=`echo "${pyfile}" | awk -F" = " '/7 TeV - MLM matching efficiency/ {eff7=$2;  k7=1/eff7} \
		#						/8 TeV - MLM matching efficiency/ {eff8=$2;  k8=1/eff8   } \
		#						/10 TeV - MLM matching efficiency/ {eff10=$2; k10=1/eff10  } \
		#						/MLM matching efficiency/ {eff=$2; k=1/eff  } \
		#						END { if (eff7 !=0) { print k7 } \
		#							if (eff7 == 0) { print k } }'`
	#if [[ "${kfactor}" == "" ]]; then
	    kfactor=1.0
	#fi

	k="k_${ds_num}=${kfactor}"

	echo ${name// /} | tee -a ${filelistconfig}
	echo ${n// /} | tee -a ${filelistconfig}
	if [[ ${isData} -eq 0 ]]; then
		echo ${xs// /} | tee -a ${filelistconfig}
		echo ${k// /} | tee -a ${filelistconfig}
		echo "Please manually check cross sections and k-factors"
	fi
	
	prev_ds_num=${ds_num}
    fi
done
