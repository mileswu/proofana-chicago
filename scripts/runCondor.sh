#!/bin/bash

echo -n "Time is: "
date
echo -n "Running on: "
hostname

echo -n "Current working directory is"
export JOBDIR=`pwd`
echo $JOBDIR
mkdir home
export HOME=$JOBDIR/home

echo "Contents is"
ls -lh

echo "Extracting"
mkdir ProofAna
tar xvf proofana-condor.tar -C ProofAna
rm proofana-condor.tar

export LD_LIBRARY_PATH="/opt/rhel/lib"
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
ls -lh /
ls -lh /cvmfs
#sleep 60


ls $ATLAS_LOCAL_ROOT_BASE
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh
export X509_USER_PROXY=`pwd`/gridproxy.cert
export XrdSecGSISRVNAMES="*"
localSetupROOT --rootVersion=5.34.07-x86_64-slc5-gcc4.3

cd $JOBDIR/ProofAna/utils/RootCore
./configure
cd $JOBDIR/ProofAna

mkdir run
cd run
tar xvzf $JOBDIR/joboptions*.tar.gz
rm $JOBDIR/joboptions*.tar.gz
mv filelists ..

echo "We are node $1, using $NUMPERNODE files"
root -b -x "../scripts/runCondorRuntime.C($NUMPERNODE, $1)"

echo "Finished; moving this:"
ls -lh
rm options.root
mv * $JOBDIR/

echo "Clean up"
cd $JOBDIR
rm $X509_USER_PROXY
rm -rf ProofAna
rm -rf home
echo "Check is all gone:"
ls -lh
