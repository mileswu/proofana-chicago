#include <TList.h>
#include <TString.h>
#include <TFileCollection.h>
#include <TDataSetManagerFile.h>

void runGrid(TString outDSprefix, TString extraFlags = "--nGBPerJob=10")
{
  if(!gSystem->Which(gSystem->Getenv("PATH"),"prun")) {
    cout << "prun not found in path. Have you loaded the GRID tools? Exiting." << endl;
    return;
  }

  TString symString = "ln -s ../lib/libProofAna.par libProofAna.par";
  TString tarString = "tar -zchf gridProofAna.tar.gz libProofAna.par options.root";
  gSystem->Exec(symString);
  gSystem->Exec(tarString);
  gSystem->Unlink("libProofAna.par");

  TString baseCommand = "prun --athenaTag=17.2.2.4 --inTarBall=gridProofAna.tar.gz --bexec \"tar -xvzf libProofAna.par; rm libProofAna.par; mkdir libProofAna/run; mv options.root libProofAna/run; mv libProofAna/scripts/runGridRuntime.C libProofAna/run; cd libProofAna; make solib\" --exec \"cd libProofAna; source scripts/RootCore_relink.sh; cd run; echo %IN > input.txt; root -l -b -q runGridRuntime.C\" " + extraFlags; 

  // get the options.root file with options in it
  TFile* optionsFile = new TFile("options.root");

  TKey *key;
  TIter nextkey(optionsFile->GetListOfKeys());
  while ((key = (TKey*)nextkey())) {
    TString name = key->GetName();
    if(name.CompareTo("ProofAna")==0){
      Config* options = (Config*) key->ReadObj();

      if(!options->Exists("TREENAME")) {
        cout << "TREENAME missing in ProofAna config object, this likely means it is missing in your dataset config file. Exiting." << endl;
        return;
      }

      vector<TString> moreOutputs = GetMoreOutputs(options);

      TString outputsString = options->String("IDENTIFIER") + "." + options->String("DATASET") + ".root";
      vector<TString>::iterator iterMore = moreOutputs.begin();
      for(; iterMore != moreOutputs.end(); iterMore++){
        outputsString += "," + *iterMore;
      }
      cout << "outputs are " << outputsString << endl;
 
 
      vector<TString> datasets = GetDatasets(options);


      vector<TString>::iterator iter = datasets.begin();
      for(; iter != datasets.end(); iter++){
        TString inDS = *iter;
        TString outDS = outDSprefix+"."+inDS;
        TString command = baseCommand + " --outputs="+outputsString + " --inDS="+inDS + " --outDS="+outDS;

        cout << "--------------------------------------------------------------------------------------" << endl;
        cout << command << endl;
        cout << "--------------------------------------------------------------------------------------" << endl;
        gSystem->Exec(command);
      }
    }
  }
  gSystem->Unlink("gridProofAna.tar.gz");

  cout << "Finished submitting all! Check panda to see if things went through properly!" << endl;
}

