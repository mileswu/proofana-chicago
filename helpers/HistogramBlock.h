#include "AnalysisBase.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TROOT.h"
#include <vector>

void InitializeRegionBlock(TString blockname, std::map<TString, TH1D>& Block1D, std::map<TString, TH2D>& Block2D);
void FillRegionBlock      (TString blockname, std::map<TString, TH1D>& Block1D, std::map<TString, TH2D>& Block2D, AnalysisBase *analysis, float weight);
void WriteRegionBlock     (TString blockname, std::map<TString, TH1D>& Block1D, std::map<TString, TH2D>& Block2D, TFile *file);

void DefineHist1D(std::map<TString, TH1D>& Block1D, TString blockname, TString title, int nbinsx, double xlow, double xhigh);
void FillHist1D  (std::map<TString, TH1D>& Block1D, TString blockname, TString title, double xval, double weight);

void DefineHist2D(std::map<TString, TH2D>& Block2D, TString blockname, TString title, int nbinsx, double xlow, double xhigh,int nbinsy, double ylow, double yhigh);
void FillHist2D  (std::map<TString, TH2D>& Block2D, TString blockname, TString title, double xval, double yval, double weight);