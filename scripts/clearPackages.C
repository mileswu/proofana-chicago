// Macro to clear PROOF package cache.
#include <TEnv.h>
#include <TProof.h>
#include <TString.h>
#include "getProof.C"

void clearPackages(TString url)
{

  	// Temp dir
  	TString tempdir = Form("%s/.proofanalysis", gSystem->TempDirectory());
  	if (gSystem->AccessPathName(tempdir)) {
    	Printf("runProof: creating the temporary directory"
	   		" (%s) ... ", tempdir.Data());
    	if (gSystem->mkdir(tempdir, kTRUE) != 0) {
      		Printf("runProof: could not assert / create the temporary directory"
	    		" (%s)", tempdir.Data());
      		return;
    	}
  	}
  
	// Get the PROOF Session
	TProof *proof = getProof(url, -1, tempdir.Data(), "ask");
	if (!proof) {
		Printf("runProof: could not start/attach a PROOF session");	
		return;
	}

	proof->ClearPackages();
	proof->ShowPackages();
}
