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
void EWKMuonSelection(AnalysisBase *analysis, double ptcut, double etacut) {
	analysis->AddVec("muonsEWK");
	for(int iMu=0; iMu<analysis->muons(); iMu++) {
		Particle &muon = analysis->muon(iMu);

		// Quality cuts
		if(muon.Int("author") != MUON_PARAM_AUTHOR_STACO) continue;
		if(muon.Int("isCombined") != true) continue;
		if(passesMCPQualityCuts(muon) != true) continue; 

		// Kinematic cuts
		if(fabs(muon.p.Eta()) > etacut) continue;
		if(muon.p.Pt() < ptcut) continue;

		// Isolation
		if(muon.Float("Etcone20") / muon.p.Pt() > 0.3) continue;
		if(muon.Float("Ptcone20") / muon.p.Pt() > 0.15) continue;

		// Impact parameter cuts
		if(fabs(muon.Float("z0")) > 2) continue;
		if(fabs(muon.Float("d0") / muon.Float("d0_err")) > 3.5) continue;

		analysis->Add("muonsEWK", &muon);
	}
}


