#include <TList.h>
#include <TString.h>
#include <TFileCollection.h>
#include <TFile.h>
#include <TDataSetManagerFile.h>
#include "../scripts/helperFunc.C"

int runGridRuntime()
{

  LoadLibraries();

  std::string argStr;
  std::ifstream ifs("input.txt");
  std::getline(ifs,argStr);

  std::string prefix = "../../";

  // split by ','
  std::vector<std::string> fileList;
  for (size_t i=0,n; i <= argStr.length(); i=n+1)
  {
    n = argStr.find_first_of(',',i);
    if (n == string::npos)
      n = argStr.length();
    string tmp = argStr.substr(i,n-i);
    fileList.push_back(tmp);
  }

  TFile* optionsFile = new TFile("options.root");

  TString treename = "";

  TKey *key;
  TIter nextkey(optionsFile->GetListOfKeys());
  while ((key = (TKey*)nextkey())) {
    TString name = key->GetName();
    cout << name << endl;
    if(name.CompareTo("ProofAna")==0){
      Config* options = (Config*) key->ReadObj();
      treename = options->String("TREENAME");
    }
  }

  if(treename.CompareTo("")==0){
    cout << "Treename not set! I don't know what to do!" << endl;
    return;
  }
  TChain fChain(treename);
  for (int iFile=0; iFile<fileList.size(); ++iFile)
  {
    if(gSystem->AccessPathName(fileList[iFile].c_str())){
      fileList[iFile] = prefix + fileList[iFile];

      if(gSystem->AccessPathName(fileList[iFile].c_str())){
        cout << "input root file " << fileList[iFile] << " not found, exiting"<< endl;
        return 1;
      }
    }
  
    std::cout << "open " << fileList[iFile].c_str() << std::endl;
    fChain.Add(fileList[iFile].c_str());
  }




  ProofAna* ana = new ProofAna();

  //Need to provide a TList for local running
  TList* list = new TList();
  ana->SetInputList(list);

  // Process TChain
  int result = fChain.Process(ana);
  if(result!=-1){ 
    // if not an error, move the file to the right place
    // if there was an error, grid will complain
    gSystem->Exec("mv *.root* ../../");
    return 0;
  }
  // if we get here, we had an error!
  cout << "Error processing TChain!" << endl;
  return 1;
}

