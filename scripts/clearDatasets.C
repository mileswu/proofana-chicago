// Macro to clear PROOF datasets.
// Usage root -l clearDatasets("username@atlprf01","mydsetname")
// mydsetname can contain regular expression wildcards like .*
#include <TEnv.h>
#include <TProof.h>
#include <TString.h>
#include "getProof.C"

void clearDatasets(TString url, TString dataset)
{
	dataset.Prepend("^.*/");
	dataset.Append("$");
	TRegexp regexp(dataset);

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

	TMap* datasets = proof->GetDataSets();
	TMapIter *iter = new TMapIter(datasets);
	TObjString* name = (TObjString*)iter->Next();
	int* len = new int(); 
	while(name) {
		if(regexp.Index(name->GetString(),len)==0) {
			proof->RemoveDataSet(name->GetString());
		}
		name = (TObjString*)iter->Next();
	}
	delete len;

	proof->ShowDataSets();
}
