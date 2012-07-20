#!/bin/bash

if [ -z $2 ]; then
    echo "Usage: sh makeFileListBulk.sh regexp path [--autoname type] [treename]"
    echo "Makes separate file lists and config files for each directory in \"path\" matching \"regexp\"."
    echo "If no tree name is provided, config files will not be produced."
    exit 1
fi

if [[ ! -d "$2" ]]; then
    echo "\"${2}\" is not a directory"
    exit 1
fi

dirlist=`ls -d ${2}/*/ | grep -e ${1}`

shift
shift

autoname=0
type=""
if [[ ! -z $2 ]]; then
	if [[ ${1} == "--autoname" ]]; then
		autoname=1
		shift
		type=$1
		shift
	fi
fi

for dir in $dirlist; do
	dataset=""
	
	if [[ autoname -eq 0 ]]; then
    		echo "Directory: $dir"
    		echo "Enter dataset name without .txt suffix in the form identifier.type:"
    		read dataset
    	else
		dataset=""

		# match to various regexps
		ds=`expr ${dir} : '.*data[0-1][0-9]_[0-9]TeV\.00\([0-9]*\).physics_.*'`
		if [[ "${ds}" != "" ]]; then
			channel=`expr ${dir} : '.*data[0-1][0-9]_[0-9]TeV\.00[0-9]*.physics_\([a-zA-Z]*\)\..*'`
			dataset=`echo "${channel}_${ds}.${type}"`
		fi

		ds=`expr ${dir} : '.*mc[0-1][0-9]_[0-9]TeV\.[0-9]*\.simple_\(.*\)_herwigpp.*'`
                if [[ "${ds}" != "" ]]; then
			# Rename Gb, Gt
                        ds=`echo ${ds//Gt_/Gtt_}`
                        ds=`echo ${ds//Gb_/Gbb_}`

			# Remove undesirable sections
                        ds=`echo ${ds//_/}`
			ds=`echo ${ds//T1200/}`
			ds=`echo ${ds//B1200/}`
			ds=`echo ${ds//T2000/}`
			ds=`echo ${ds//B2000/}`
			chargino=`expr ${dir} : '.*mc[0-1][0-9]_[0-9]TeV\.[0-9]*\.simple_.*_\(C[0-9]*\)_.*_herwigpp.*'`
			ds=`echo ${ds//${chargino}/}`

                        dataset=`echo "${ds}.${type}"`
                fi

                ds=`expr ${dir} : '.*mc[0-1][0-9]_[0-9]TeV\.[0-9]*\.gl[0-9]*_b\([0-9]*\)_t[0-9]*_c[0-9]*_n060_sl000_hwppsusy.*'`
                if [[ "${ds}" != "" ]]; then
			gluino=`expr ${dir} : '.*mc[0-1][0-9]_[0-9]TeV\.[0-9]*\.gl\([0-9]*\)_b[0-9]*_t[0-9]*_c[0-9]*_n060_sl000_hwppsusy.*'`
			if [[ "${ds}" != "000" ]]; then
				dataset=`echo "GBG${gluino}B${ds}.${type}"`
			else
				ds=`expr ${dir} : '.*mc[0-1][0-9]_[0-9]TeV\.[0-9]*\.gl[0-9]*_b[0-9]*_t\([0-9]*\)_c[0-9]*_n060_sl000_hwppsusy.*'`
				dataset=`echo "GTG${gluino}T${ds}.${type}"`
			fi
                fi

		echo ${dataset}
	fi
	

    # remove old files if exist
    rm ${dataset}.txt >& /dev/null
    rm ${dataset}.config >& /dev/null

    sh ../scripts/makeFileList.sh ${dataset}.txt ${dir}

    # run the automatic file list config script
    if [ ! -z $1 ]; then
		sh ../scripts/makeFileListConfig.sh ${dataset}.txt ${1}
	fi
done
