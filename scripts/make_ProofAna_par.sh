#!/bin/bash
# If you execute this script directly (not from the makefile), first of all
# 1. Don't do it. Use the makefile, that way it verifies that your code compiles
# 2. If you insist, call it from the top directory of the package, or it won't work
#    sh scripts/make_ProofAna_par.sh

source utils/RootCore/scripts/setup.sh

ORIG=$PWD
DIR=/tmp/ProofAnaBuild-$$
LIBDIR=$DIR/libProofAna

rm -rf $DIR
mkdir -p $LIBDIR

ln -sf $ORIG/Makefile $LIBDIR/
ln -sf $ORIG/analyses $LIBDIR/
ln -sf $ORIG/core $LIBDIR/
ln -sf $ORIG/eventbuilders $LIBDIR/
ln -sf $ORIG/lib $LIBDIR/
ln -sf $ORIG/scripts $LIBDIR/
ln -sf $ORIG/utils $LIBDIR/

mkdir $LIBDIR/PROOF-INF
cd $LIBDIR/PROOF-INF

cat > BUILD.sh <<EOF
#! /bin/bash
# Build libProofAna library.

if [ "$1" = "clean" ]; then
   make clean
   exit 0
fi

make solib
EOF

cat > SETUP.C <<EOF
#include <TString.h>
#include <fstream>
#include <iostream>

int SETUP()
{
    if(!gSystem->AccessPathName("RootCoreEnv.txt")) gSystem->Unlink("RootCoreEnv.txt");
	gSystem->Exec("source utils/RootCore/scripts/setup.sh; echo ROOTCOREDIR=\$ROOTCOREDIR >> RootCoreEnv.txt; echo ROOTCOREBIN=\$ROOTCOREBIN >> RootCoreEnv.txt; echo PATH=\$PATH >> RootCoreEnv.txt; echo LD_LIBRARY_PATH=\$LD_LIBRARY_PATH >> RootCoreEnv.txt; echo DYLD_LIBRARY_PATH=\$DYLD_LIBRARY_PATH >> RootCoreEnv.txt; echo PYTHONPATH=\$PYTHONPATH >> RootCoreEnv.txt;");
	
	char buffer[2000];
	ifstream fh("RootCoreEnv.txt");
   	if (!fh.is_open()) {
     	cout << "LoadLibraries ERROR opening temporary RootCore environment variable dump RootCoreEnv.txt" << endl;
      	return 1;
    }

 	fh.getline(buffer,1995);
	while (!fh.eof()) {
		TString line(buffer);
		if(line.IsNull()) {
			fh.getline(buffer,1995);
			continue;
		}
			
		TObjArray* arr = line.Tokenize("=");
		if(arr->GetEntries()!=2) {
			cout << "LoadLibraries ERROR parsed line into !=2 entries" << endl;
			return 1;
		}

		gSystem->Setenv(((TObjString*)arr->At(0))->String(),((TObjString*)arr->At(1))->String().Data());
		
		delete arr;
		fh.getline(buffer,1995);	
	}
    fh.close();
    	
    gSystem->Unlink("RootCoreEnv.txt");

	gSystem->CompileMacro("utils/RootCore/scripts/load_packages.C","k");
	load_packages();

	if (gSystem->Load("lib/libTriggerMenuNtuple") == -1) return -1;
	if (gSystem->Load("lib/libMctLib") == -1) return -1;
	if (gSystem->Load("lib/libFastjet") == -1) return -1;
	if (gSystem->Load("lib/libFastjetPlugins") == -1) return -1;
	if (gSystem->Load("lib/libqgTag") == -1) return -1;
	if (gSystem->Load("lib/libProofAna") == -1) return -1;
	return 0;
}
EOF

chmod 755 BUILD.sh

cd $DIR

tar -czhf $ORIG/lib/libProofAna.par --exclude RootCore/Makefile-common --exclude RootCore/scripts/setup.csh --exclude RootCore/scripts/setup.sh --exclude TriggerMenuNtupleDict.\* --exclude utils/fastjet/install-dir --exclude utils/fastjet/Makefile --exclude utils/fastjet/libtool --exclude utils/fastjet/\*/Makefile --exclude utils/fastjet/\*/\*/Makefile --exclude utils/fastjet/\*/\*/\*/Makefile --exclude \*Cint\* --exclude \*.log --exclude \*.status --exclude \*/obj/\* --exclude \*/StandAlone/\* --exclude \*.so --exclude \*.lock --exclude \*.d --exclude \*.o --exclude \*.lo --exclude \*~ --exclude .svn --exclude \*.root\* --exclude \*.par libProofAna

cd $ORIG

rm -rf $DIR

exit 0
