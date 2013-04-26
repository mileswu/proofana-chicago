#include <TList.h>
#include <TString.h>
#include <TFileCollection.h>
#include <TFile.h>
#include "../scripts/helperFunc.C"

int runCondorRuntime(int numpernode=1, int arrayid=0)
{
	LoadLibraries();
	TString treename, dataset;

	TFile* optionsFile = new TFile("options.root");
	TKey *key;
	TIter nextkey(optionsFile->GetListOfKeys());
	while ((key = (TKey*)nextkey())) {
		TString name = key->GetName();
		cout << name << endl;
		if(name.CompareTo("ProofAna")==0){
			Config* options = (Config*) key->ReadObj();
			treename = options->String("TREENAME");
			dataset = options->String("DATASET");
		}
	}

	if(treename.CompareTo("")==0 || dataset.CompareTo("") == 0){
		cout << "Treename and/or dataset not set! I don't know what to do!" << endl;
		return;
	}


	TString dsetfilename(dataset);
	dsetfilename.Append(".txt");
	dsetfilename.Prepend("../filelists/");

	ifstream dsetfile(dsetfilename.Data());
	if(!dsetfile.is_open()) {
		cout << "Can't find file at: " << dsetfilename.Data() << endl;
		return;
	}

	std::vector<std::string> fileList;
	int linenum = 0;
	int n_files = 0;
	string line;
	while (getline(dsetfile, line)) {
		if(linenum >= arrayid*numpernode) {
			fileList.push_back(line);
			n_files++;
		}
		linenum++;
		if(n_files == numpernode) break;
	}
	dsetfile.close();

	TChain fChain(treename);
	for (int iFile=0; iFile<fileList.size(); ++iFile)
	{
		std::cout << "open " << fileList[iFile].c_str() << std::endl;
		fChain.Add(fileList[iFile].c_str());
	}

	ProofAna* ana = new ProofAna();

	TString suffix = TString::Itoa(arrayid, 10);
	while(suffix.Length() < 5) { // max number is probably 99999 then
		suffix.Prepend("0");
	}
	suffix.Prepend(".");
	ana->filenameSuffix = suffix;

	//Need to provide a TList for local running
	TList* list = new TList();
	ana->SetInputList(list);

	// Process TChain
	int result = fChain.Process(ana);
	if(result!=-1){ 
		return 0;
	}
	// if we get here, we had an error!
	cout << "Error processing TChain!" << endl;
	return 1;
}

