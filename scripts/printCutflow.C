#include <TROOT.h>
#include <TFile.h>
#include <map>
#include <set>
#include <TObjArray.h>
#include <TObjString.h>

void printAnalysisCutflow(TDirectory* dir)
{
	map<int, int> cutorder;
	vector<TString> name;
	vector<double> n;
	vector<double> nerror;
	vector<double> nwgt;
	vector<double> nwgterror;

	cout << "Analysis " << dir->GetName() << " cut flow:" << endl;

	TKey *key;
	TIter nextkey(dir->GetListOfKeys());
	int i = 0;
	while ((key = (TKey*)nextkey())) {
	  const char *classname = key->GetClassName();
		TClass *cl = gROOT->GetClass(classname);
		if (!cl) continue;
		if(!TString(key->GetName()).Contains("cutflow")) continue;
		if(!cl->InheritsFrom(TH1D::Class())) continue;

		//if(!TString(key->GetName()).Contains("0lepton") && !TString(key->GetName()).Contains("4jet") && !TString(key->GetName()).Contains("6jet")) continue;
		
		//if(!TString(key->GetName()).Contains("grl") &&
		//   !TString(key->GetName()).Contains("1electron_") &&
		//   !TString(key->GetName()).Contains("1electrontopbkg") &&
		//   !TString(key->GetName()).Contains("1electronSR_") &&
		//   !TString(key->GetName()).Contains("tightmuveto")) continue;
		
		//if(!TString(key->GetName()).Contains("grl") &&
		//   !TString(key->GetName()).Contains("1muon_") &&
		//   !TString(key->GetName()).Contains("1muontopbkg") &&
		//   !TString(key->GetName()).Contains("1muonSR_") &&
		//   !TString(key->GetName()).Contains("tightelveto")) continue;
	       	
	
		TH1D* histo = (TH1D*)dir->Get(key->GetName());
		name.push_back(TString(key->GetName()).ReplaceAll("_cutflow","").ReplaceAll("cutflow","All Events"));
		cout << "";		
		n.push_back((double)histo->GetEntries());
		nerror.push_back(TMath::Sqrt((double)histo->GetEntries()));
		nwgt.push_back(histo->GetBinContent(1));
		nwgterror.push_back(histo->GetBinError(1));	
		int entries = histo->GetEntries();		
		while(cutorder.find(entries)!=cutorder.end()) entries--; //ensure key uniqueness
	
		pair<int, int> mapentry(entries, i);
		cutorder.insert(mapentry);	
		i++;
	}

	cout << "****************************************************************************************************************************" << endl;
	cout << "* Cut                                  | Entries            | Weighted Entries   | Abs. Eff.          | Weighted Abs. Eff. *" << endl;	
	cout << "*--------------------------------------------------------------------------------------------------------------------------*" << endl;
	map<int, int>::reverse_iterator p = cutorder.rbegin();
	for(; p!=cutorder.rend(); p++) {
		TString first = name.at(p->second);
		TString second = TString::Format("%.0f +/- %.1f", n.at(p->second), nerror.at(p->second)); 
		TString third = TString::Format("%.2f +/- %.2f", nwgt.at(p->second), nwgterror.at(p->second));
		TString fourth = TString::Format("%.1f%%", 100.*n.at(p->second)/n.at(cutorder.rbegin()->second));
		TString fifth = TString::Format("%.1f%%", 100.*nwgt.at(p->second)/nwgt.at(cutorder.rbegin()->second));
		while(first.Length()<36) {first = TString(first + " ");}
		while(second.Length()<18) {second = TString(second + " ");}
		while(third.Length()<18) {third = TString(third + " ");}	
		while(fourth.Length()<18) {fourth = TString(fourth + " ");}	
		while(fifth.Length()<18) {fifth = TString(fifth + " ");}	
		cout << "* " << first << " | " << second << " | " << third << " | " << fourth << " | " << fifth << " *" << endl;
	}
	cout << "****************************************************************************************************************************" << endl;
}

void printCutflow(TString filename, TString analyses = "")
{
	TFile* file;
	if(filename.Contains("root://")) file = new TXNetFile(filename,"READ");
	else file = new TFile(filename,"READ");

	bool subset = false;
	set<TString> analysisList; 
	if(!analyses.IsNull()) {
		subset = true;
		
		TObjArray* arr = analyses.Tokenize(",");
		for(int i = 0; i<arr->GetEntries(); i++) analysisList.insert(((TObjString*)arr->At(i))->GetString());
		
		arr->SetOwner(kTRUE);
		delete arr;
	}

	//loop on all analysis TDirectoryFiles in this directory
	TKey *key;
	TIter nextkey(file->GetListOfKeys());
	while ((key = (TKey*)nextkey())) {
		const char *classname = key->GetClassName();
		TClass *cl = gROOT->GetClass(classname);
		if (!cl) continue;
		if (cl->InheritsFrom(TDirectory::Class())) {
			if(subset&&(analysisList.find(key->GetName())==analysisList.end())) continue;
			file->cd(key->GetName());
			TDirectory *subdir = gDirectory;
			printAnalysisCutflow(subdir);
		} 
	}
	
}


