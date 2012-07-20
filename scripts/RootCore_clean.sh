#!/bin/bash

cd utils/RootCore
./configure
cd ..
source RootCore/scripts/setup.sh
sh RootCore/scripts/clean.sh

