#include <TList.h>
#include <TString.h>
#include <TFileCollection.h>
#include <TDataSetManagerFile.h>

void runLocal(TString dataset, TString treename = "", int nfiles = -1)
{
	ProofAna* ana = new ProofAna();
	
	//Need to provide a TList for local running
	TList* list = new TList();
	ana->SetInputList(list);

	// Load dataset manager stuff, uses same local datasets as PROOF-Lite by design
	TString dsetdir(gSystem->HomeDirectory());
	dsetdir.Append("/.proof/datasets");
	dsetdir.Prepend("dir:");
	TDataSetManagerFile mgr(dsetdir);
	
	TString dsetfile(dataset);
	dsetfile.Append(".txt");
	dsetfile.Prepend("../filelists/");

	//Register dataset, if non-existent
	if(!mgr.ExistsDataSet(dataset)) {
		TFileCollection* dset = new TFileCollection(dataset,"",dsetfile, nfiles);
		mgr.RegisterDataSet(dataset,dset,"V"); //This seems to return true regardless of success or failure at the moment
		if(treename.CompareTo("")!=0) {
			TProof* lite = TProof::Open("lite://");
			lite->SetDataSetTreeName(dataset,treename);
			delete lite;
		}
	}
	mgr.ShowDataSets();

	// And yes, all this dataset garbage was to get the default tree name
	TFileCollection* dset = mgr.GetDataSet(dataset);
	TString defaultTree = dset->GetDefaultTreeName();
	
	// Make TChain from TFileCollection...doesn't seem like there is a more direct way
	if(defaultTree.First("/")==0) defaultTree.Remove(0,1);
	TChain* T = new TChain(defaultTree);
	TList* filelist = (TList*)dset->GetList();
	TIter next(filelist);
	TFileInfo* fileinfo = 0;
	while ((fileinfo = (TFileInfo*)next())) T->AddFile(fileinfo->GetCurrentUrl()->GetUrl());
	
	// Process TChain
	T->Process(ana);
}

