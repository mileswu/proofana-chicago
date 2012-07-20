source /afs/slac/g/atlas/packages/gcc432/setup.sh
export ROOTSYS=/afs/slac.stanford.edu/g/atlas/packages/root/proof
export XPDUTILS=$ROOTSYS/etc/proof/utils
export PATH=${ROOTSYS}/bin:${PATH}
export LD_LIBRARY_PATH=${ROOTSYS}/lib:${ROOTSYS}/bin/root:${LD_LIBRARY_PATH}
export DYLD_LIBRARY_PATH=${ROOTSYS}/lib:${ROOTSYS}/bin/root:${DYLD_LIBRARY_PATH}
export PYTHONPATH=$ROOTSYS/lib:${PYTHONPATH}
alias python=/usr/bin/python
