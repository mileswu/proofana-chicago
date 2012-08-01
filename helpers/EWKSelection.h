#include "AnalysisBase.h"

#define ELECTRON_PARAM_QUALITY_TIGHTPP 0
#define ELECTRON_PARAM_QUALITY_MEDIUMPP 1

bool passesMCPQualityCuts(Particle &muon);
bool EWKMuonSelectionPasses(Particle &muon, double ptcut, double etacut);
void EWKMuonSelectionFilter(AnalysisBase *analysis, const MomKey& key, double ptcut=15, double etacut=2.5);
bool EWKElectronSelectionPasses(Particle &electron, double ptcut, double etacut, int quality);
void EWKElectronSelectionFilter(AnalysisBase *analysis, const MomKey& key, double ptcut=15, double etacut=2.5, int quality=ELECTRON_PARAM_QUALITY_TIGHTPP);
