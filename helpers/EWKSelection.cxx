#include <iostream>
#include "EWKSelection.h"

using namespace std;

#define MUON_PARAM_AUTHOR_STACO 6

bool passesMCPQualityCuts(Particle &muon) {
  if(muon.Int("expectBLayerHit") == 1 && muon.Int("nBLayerHits") == 0) return false;
  if(muon.Int("nPixHits") + muon.Int("nPixDead") == 0) return false;
  if(muon.Int("nSCTHits") + muon.Int("nSCTDead") <= 4) return false;
  if(muon.Int("nPixHoles") + muon.Int("nSCTHoles") >= 3) return false;
  
  int n = muon.Int("nTRTHits") + muon.Int("nTRTOutliers");
  double eta = fabs(muon.p.Eta());
  if(eta > 0.1 && eta < 1.9) {
    if(n <= 5) return false;
    if(muon.Int("nTRTOutliers") > 0.9*((float)n)) return false;
  }
  else {
    if(n > 5) {
      if(muon.Int("nTRTOutliers") > 0.9*((float)n)) return false;
    }
  }
  
  return true;
}

// Reference: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZElectroweakCommonTopics2012#Muon_Selection
bool EWKMuonSelectionPasses(Particle &muon, double ptcut, double etacut, TH1* cutflow, TH1* objselection) {
	bool pass = true;

  int iCount=0;
  cutflow->Fill(iCount); iCount++;
	objselection->Fill(0);
  
  // Quality cuts
  if(muon.Int("author") != MUON_PARAM_AUTHOR_STACO) pass = false;
	else objselection->Fill(1);
  cutflow->Fill(iCount);
	iCount++;

  if(muon.Int("isCombined") != true) pass = false;
	else objselection->Fill(2);
  cutflow->Fill(iCount);
	iCount++;

  if(passesMCPQualityCuts(muon) != true) pass = false; 
	else objselection->Fill(5);
  cutflow->Fill(iCount);
	iCount++;
  
  // Kinematic cuts
  if(fabs(muon.p.Eta()) > etacut) pass = false;
	else objselection->Fill(8);
  cutflow->Fill(iCount);
	iCount++;
  if(muon.p.Pt() < ptcut) pass = false;
	else objselection->Fill(7);
  cutflow->Fill(iCount);
	iCount++;
  
  // Isolation
  if(muon.Float("Etcone20") / muon.p.Et() > 0.3) pass = false;
	else objselection->Fill(9);
  cutflow->Fill(iCount);
	iCount++;
  if(muon.Float("Ptcone20") / muon.p.Pt() > 0.15) pass = false;
	else objselection->Fill(10);
  cutflow->Fill(iCount);
	iCount++;
  
  // Impact parameter cuts
  if(fabs(muon.Float("z0") * sin(muon.Float("tracktheta"))) > 1) pass = false;
	else objselection->Fill(4);
  cutflow->Fill(iCount);
	iCount++;
  if(fabs(muon.Float("d0") / muon.Float("d0_err")) > 6) pass = false;
  //if(fabs(muon.Float("d0") / muon.Float("d0_err")) > 3.5) pass = false;
	else objselection->Fill(3);
  cutflow->Fill(iCount);
	iCount++;

	//if(fabs(muon.Float("id_qoverp") - muon.Float("me_qoverp")) / sqrt(pow(muon.Float("id_qoverp_err"), 2.0) + pow(muon.Float("me_qoverp_err"), 2.0)) > 5) pass = false;
	if(fabs(muon.Float("id_qoverp") - muon.Float("me_qoverp")) / sqrt(pow(muon.Float("id_qoverp_err"), 2.0) + pow(muon.Float("me_qoverp_err"), 2.0)) > 0.5) pass = false;
	else objselection->Fill(6);
	cutflow->Fill(iCount);
	iCount++;
	
	if(pass) objselection->Fill(11);
  
  return pass;
}

void EWKMuonSelectionFilter(AnalysisBase *analysis, const MomKey& key, double ptcut, double etacut) {

  //Histogram to track cutflow for muon selection
  if(!analysis->OutputDir()->Get("EWK_muon_selection_cutflow")) {
    analysis->OutputDir()->cd();
    new TH1F("EWK_muon_selection_cutflow","EWK_muon_selection_cutflow",20,0,20);
  }
  if(!analysis->OutputDir()->Get("EWK_muon_selection_objselection")) {
    analysis->OutputDir()->cd();
    new TH1F("EWK_muon_selection_objselection","EWK_muon_selection_objselection",20,0,20);
  }

  TH1 *cutflow = dynamic_cast<TH1*>(analysis->OutputDir()->Get("EWK_muon_selection_cutflow"));
  TH1 *objselection = dynamic_cast<TH1*>(analysis->OutputDir()->Get("EWK_muon_selection_objselection"));

  for(int iE=analysis->Objs(key)-1; iE >= 0; iE--) { // reverse loop
    Particle &muon = *dynamic_cast<Particle *>(analysis->Obj(key, iE));
    
    if(EWKMuonSelectionPasses(muon, ptcut, etacut, cutflow, objselection) == false) {
      analysis->Remove(key, iE);
    }
  }
}

// Reference: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZElectroweakCommonTopics2012#Electron_Selection
bool EWKElectronSelectionPasses(Particle &electron, double ptcut, double etacut, int quality) {
	// Author
	if(electron.Int("author") != 1 && electron.Int("author") != 3) return false;
	if((electron.Int("OQ") & 1446) != 0) return false;

	// ECAL Crack
	double cluster_eta = fabs(electron.Float("clusterEta"));
	if(cluster_eta > 2.47) return false;
	if(cluster_eta > 1.37 && cluster_eta < 1.52) return false;

	// Quality
	if(quality == ELECTRON_PARAM_QUALITY_MEDIUMPP)
		if(electron.Int("mediumPP") != 1) return false;
	if(quality == ELECTRON_PARAM_QUALITY_TIGHTPP)
		if(electron.Int("tightPP") != 1) return false;

	// Impact parameter cuts
	if(fabs(electron.Float("z0")) > 2) return false;
	if(fabs(electron.Float("d0") / electron.Float("d0_err")) > 6) return false;

	// Isolation
	if(electron.Float("Etcone20_corrected") / electron.p.Et() > 0.3) return false;
	if(electron.Float("Ptcone20") / electron.p.Et() > 0.15) return false;

	// Kinematic cut
	if(fabs(electron.p.Eta()) > etacut) return false;
	if(electron.p.Pt() < ptcut) return false;

	return true;
}

void EWKElectronSelectionFilter(AnalysisBase *analysis, const MomKey& key, double ptcut, double etacut, int quality) {
	for(int iE=analysis->Objs(key)-1; iE >= 0; iE--) { // reverse loop
		Particle &electron = *dynamic_cast<Particle *>(analysis->Obj(key, iE));

		if(EWKElectronSelectionPasses(electron, ptcut, etacut, quality) == false) {
			analysis->Remove(key, iE);
		}
	}
}
