#!/bin/bash

if [ "$PBS_O_WORKDIR" ]; then
	echo "Running as PBS"
	cd $PBS_O_WORKDIR
	ARRAYID=$PBS_ARRAYID
elif [ "$LS_SUBCWD" ]; then
	echo "Running on LXBatch"
	cd $LS_SUBCWD
	ARRAYID=$LSB_JOBINDEX
else
	echo "Unknown batch system"
	exit
fi

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh > /dev/null
export X509_USER_PROXY=${HOME}/.globus/gridproxy.cert
export XrdSecGSISRVNAMES="*"
localSetupROOT --rootVersion=5.34.03-x86_64-slc5-gcc4.3

echo `pwd`
date

DATASET=`sed -n '1p' < options.batch`
TREE=`sed -n '2p' < options.batch`
FILESNUM=`sed -n '3p' < options.batch`

echo "Data set " $DATASET
echo "Tree " $TREE
echo "Num files " $FILESNUM
echo "Array id " $ARRAYID

root -b -x "../scripts/runBatchRuntime.C(\"$DATASET\", \"$TREE\", $FILESNUM, $ARRAYID)"
