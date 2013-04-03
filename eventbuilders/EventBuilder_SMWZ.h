/**************************************************************************
 **
 **   File:         EventBuilder_SMWZ.h
 **
 **   Description:  EventBuilder class for analyzing the v7 GRJETS D3PDs. Use
 **                 cases include but are not limited to the following:
 **
 **   Authors:      D. W. Miller and M. Swiatlowski
 **
 **   Created:      2012-02-22
 **   Modified:     2012-03-01 - DWM
 **
 **************************************************************************/

#ifndef EventBuilder_SMWZ_h
#define EventBuilder_SMWZ_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "EventBuilderBase.h"
#include <TLorentzVector.h>

namespace Root {class TPileupReweighting;}

class EventBuilder_SMWZ : public EventBuilderBase {
public :
	 // Standard required methods
   Bool_t   CopyEvent(AnalysisBase* evt);
   void  WriteAuxTrees(TDirectory* outfile);
   void Initialize();
   Bool_t  Notify();
 
	 // Copy methods
   Bool_t   CopyJets();
   Bool_t   CopyMuons();
   Bool_t   CopyMuonsTruth();
   Bool_t   CopyTruth();
   Bool_t   CopyTruthMET();
   Bool_t   CopyElectrons();
	 Bool_t		CopyMET();
   Bool_t   CopyTracks();
   Bool_t   CopyClusters();
   /*Bool_t   CopyPhotons();
   Bool_t   CopyVertices();	
   Bool_t   CopyJets();
   Bool_t   CopyJet(TString jetType = "", TString jetClass = "");
   Bool_t   CopySubjets();*/


	 Bool_t AddMuonMiniIsolation();
/*
   Bool_t   AddTrackJetLink(TString recoType = "");
   Bool_t   AddParentLinks();
   Bool_t   AddParentLink(TString parentJetType = "");
   Bool_t   AddConstituents();
   Bool_t   AddTruthLinks();
   Bool_t   AddTruthJetLinks();
   Bool_t   AddTruthJetLink(TString recoType = "");
   Bool_t   AddTrackingInfo();
   Bool_t   AddTrackInJetInfo(TString recoType = "");
   Bool_t   AddTriggers();
   Bool_t   AddBadJetType();
   Bool_t   AddBtags();
   Bool_t   AddBtag(TString jetType = "", TString jetClass = "");*/

   bool doJetStructure;
   bool doParentChild;
   bool doConstituents;
   bool doTruthConstituents;
   bool doTrack;
   bool doPhotons;
   bool doElectrons;
   bool doMuons;
	 bool doMET;
	 bool doMiniIsolation;
   bool doJets;
   bool doLCCluster;
   bool doEMCluster;
   bool doTruth;
   bool doVertex;
   bool doBasic;
   bool doTruthLinks;
   bool doTruthJetLinks;
   bool doTrackJetLinks;
   bool doTrigger;
   bool doSubjets;

	 // Constructor and destructor
   EventBuilder_SMWZ(TTree * /*tree*/ =0);
   virtual ~EventBuilder_SMWZ();

   AnalysisBase* fEvt;

   ClassDef(EventBuilder_SMWZ,0);

private:

};

#endif

#ifdef EventBuilder_SMWZ_cxx

Bool_t EventBuilder_SMWZ::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}


#endif // #ifdef EventBuilder_SMWZ_cxx
