//This histblock method of routines works to siplify creating and filling the same
//histograms many times and allows you to define blocks of hsitograms that can easily
//be filled in your main analysis after making another selection criteria

//Initialize : define a map of <TString, TH1D> and fill this map with pairs
//Fill       : fill each histogram after getting it from the associated TString key

//The maps are defined in the main analysis header file for each block

//NOTE: using this assumes that each of your blocks will be uniquely defined

#include <iostream>
#include "HistogramBlock.h"

using namespace std;

void InitializeRegionBlock(AnalysisBase *A, TString blockname) {	
	A->OutputDir()->Dir()->mkdir(blockname.Data());
	A->OutputDir()->cd(blockname.Data());
	new TH1F("ptmu1", "", 100, 0, 100);
	new TH2F("ptmu1_vs_etamu1", "", 100, 0, 100, 80, -4, 4);
	A->OutputDir()->cd("..");
}

void FillRegionBlock(AnalysisBase *A, TString blockname){
	A->SetOutputDir(A->OutputDir()->Dir()->GetDirectory(blockname.Data()));
	if(A->muons() > 0) {
		A->Fill("ptmu1", A->muon(0).p.Pt());
		A->Fill("ptmu1_vs_etamu1", A->muon(0).p.Pt(), A->muon(0).p.Eta());
	}
	A->SetOutputDir(A->OutputDir()->Dir()->GetDirectory(".."));
}

