#include <map>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TObjArray.h"
#include "TObjString.h"

void dumpSignalXS(TString filename, bool errors = false) {

	TFile file(filename,"READ");
	TTree* tree = (TTree*)file.Get("SignalUncertainties");

	if(!tree) {
		cout << "No SignalUncertainties tree" << endl;
	}

	map<TString, Int_t> intBranches;
	map<TString, Float_t> floatBranches;

	TObjArray* arr = tree->GetListOfBranches();
	for(int i = 0; i<arr->GetEntriesFast(); ++i) {
		TObjString* str = (TObjString*)arr->At(i);
		TString name(str->String());

		TBranch* branch = tree->GetBranch(name);

        	TObjArray* leaves = branch->GetListOfLeaves();
                TObjString* strleaf = (TObjString*)leaves->At(0);
                TString leafname(strleaf->String());
		TLeaf* leaf = branch->GetLeaf(leafname);
		TString leaftype(leaf->GetTypeName());

		if(leaftype.CompareTo("Float_t")==0) branch->SetAddress(&floatBranches[name]);
		else if(leaftype.CompareTo("Int_t")==0) branch->SetAddress(&intBranches[name]);
		else {
			cout << "Unrecognized type " << leaftype << endl;
		}
	}

	for(int i = 0; i<tree->GetEntries(); ++i) {
		tree->GetEntry(i);
		map<TString, Int_t>::iterator intiter = intBranches.begin();
		for(; intiter!=intBranches.end(); ++intiter) {
			if((!errors)&&intiter->first.Contains("error")) continue;
			cout << intiter->first << ": " << intiter->second << ", ";
		}
		map<TString, Float_t>::iterator floatiter = floatBranches.begin();
		for(; floatiter!=floatBranches.end(); ++floatiter) {
			if((!errors)&&floatiter->first.Contains("error")) continue;
			cout << floatiter->first << ": " << floatiter->second << ", ";
		}
		cout << endl;
	}
}

