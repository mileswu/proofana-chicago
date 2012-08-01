//This histblock method of routines works to siplify creating and filling the same
//histograms many times and allows you to define blocks of hsitograms that can easily
//be filled in your main analysis after making another selection criteria

//Initialize : define a map of <TString, TH1D> and fill this map with pairs
//Fill       : fill each histogram after getting it from the associated TString key
//Write      : loop through the entire map and write out all histograms in it

//The maps are defined in the main analysis header file for each block

//NOTE: using this assumes that each of your blocks will be uniquely defined

#include <iostream>
#include "HistogramBlock.h"

using namespace std;



///////////////////
void InitializeRegionBlock(TString blockname, std::map<TString, TH1D>& Block1D, std::map<TString, TH2D>& Block2D){
  cout<<"Initializing block: "<<blockname<<endl;

  DefineHist1D(Block1D,blockname,"ptmu1",100,0,100);
  
  DefineHist2D(Block2D,blockname,"ptmu1_vs_etamu1",100,0,100,80,-4,4);


}




void FillRegionBlock(TString blockname,  std::map<TString, TH1D>& Block1D, std::map<TString, TH2D>& Block2D, AnalysisBase *A, float weight=1){
  cout<<"Filling block: "<<blockname<<endl;


  cout<<"Nmuons "<<A->muons()<<endl;
  if(A->muons()>0){
    FillHist1D(Block1D, blockname, "ptmu1", A->muon(0).p.Pt(), 1);

    FillHist2D(Block2D, blockname, "ptmu1_vs_etamu1", A->muon(0).p.Pt(), A->muon(0).p.Eta(), 1);
  }

}
/////////////////////



void WriteRegionBlock(TString blockname, std::map<TString, TH1D>& Block1D, std::map<TString, TH2D>& Block2D, TFile *file){
  cout<<"Writing block: "<<blockname<<endl;

  //Make new directory in which to write block of histograms
  file->mkdir(blockname);
  //Move into new directory
  file->cd(blockname);
  //Loop over histogram map and write to directory
  
  std::map<TString, TH1D>::iterator itr1D;
  for(itr1D = Block1D.begin(); itr1D!=Block1D.end(); itr1D++){
    cout<<"Map Element: "<<itr1D->first<<endl;
    (itr1D->second).Write( itr1D->first );
    
  }
  
  std::map<TString, TH2D>::iterator itr2D;
  for(itr2D = Block2D.begin(); itr2D!=Block2D.end(); itr2D++){
    cout<<"Map Element: "<<itr2D->first<<endl;
    (itr2D->second).Write( itr2D->first );
    
  }
  //Move back to top directory
  file->cd();


}




void DefineHist1D(std::map<TString, TH1D>& Block1D, TString blockname, TString title, int nbinsx, double xlow, double xhigh){
  TString name = blockname+title;
  TH1D temphist( name, title, nbinsx, xlow, xhigh);
  Block1D[name] = temphist;
}

void FillHist1D(std::map<TString, TH1D>& Block1D, TString blockname, TString title, double xval, double weight=1){
  TString name = blockname+title;
  if (Block1D.find( name.Data() ) != Block1D.end()) {
      Block1D[name.Data()].Fill(xval, weight);
  } 
  else {
    cout<<"No 1D histogram exists with name: "<<name<<endl;
  }
}




void DefineHist2D(std::map<TString, TH2D>& Block2D, TString blockname, TString title, int nbinsx, double xlow, double xhigh, int nbinsy, double ylow, double yhigh){
  TString name = blockname+title;
  TH2D temphist( name, title, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  Block2D[name] = temphist;
}

void FillHist2D(std::map<TString, TH2D>& Block2D, TString blockname, TString title, double xval, double yval, double weight=1){
  TString name = blockname+title;
  if (Block2D.find( name.Data() ) != Block2D.end()) {
      Block2D[name.Data()].Fill(xval, yval, weight);
  } 
  else {
    cout<<"No 2D histogram exists with name: "<<name<<endl;
  }
}


