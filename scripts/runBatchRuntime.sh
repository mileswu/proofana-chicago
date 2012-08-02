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
localSetupROOT

echo `pwd`
date

ls -lh /atlas
ls /atlas/uct3/data/users/antonk/NTUPLE/v1_29d/user.kapliy.UCNTUP.mc11_7TeV.106047.PythiaZmumu_no_filter.merge.AOD.e815_s1272_s1274_r3043_r2993.j.mc11c.v1_29d.120218115907/

DATASET=`sed -n '1p' < options.batch`
TREE=`sed -n '2p' < options.batch`
FILESNUM=`sed -n '3p' < options.batch`

echo "Data set " $DATASET
echo "Tree " $TREE
echo "Num files " $FILESNUM
echo "Array id " $ARRAYID

root -b -x "../scripts/runBatchRuntime.C(\"$DATASET\", \"$TREE\", $FILESNUM, $ARRAYID)"
