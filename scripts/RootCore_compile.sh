#!/bin/bash

unset ROOTCOREDIR
unset ROOTCOREBIN
cd utils/RootCore
./configure
cd ..
source RootCore/scripts/setup.sh
sh RootCore/scripts/find_packages.sh
sh RootCore/scripts/compile.sh
cd ..
