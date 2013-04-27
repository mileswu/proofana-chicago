/**************************************************************************
 **
 **   File:         EventBuilder_SMWZ.cxx
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

#define EventBuilder_SMWZ_cxx

#include "EventBuilder_SMWZ.h"
#include "AnaConfig.h"
#include "AnalysisBase.h"
#include "TMath.h"
#include "TVector3.h"
#include "TKey.h"
#include <iostream>
#include "TPRegexp.h"

#ifdef GOODRUNLIST
	#include "GoodRunsLists/TGoodRunsList.h"
#endif

#ifdef APPLYJETCALIBRATION
	#include "ApplyJetCalibration/ApplyJetCalibration.h"
#endif

#ifdef MUONSMEARING
	#include "MuonMomentumCorrections/SmearingClass.h"
#endif

#ifdef PILEUPREWEIGHTING
#include "PileupReweighting/TPileupReweighting.h"
#endif

#define UNITCONVERSION 0.001

///=========================================
/// Constructor
///=========================================
EventBuilder_SMWZ::EventBuilder_SMWZ(TTree * /*tree*/)
{
}

///=========================================
/// Destructor
///=========================================
EventBuilder_SMWZ::~EventBuilder_SMWZ(){
}

///=========================================
/// Initialize
///=========================================
void EventBuilder_SMWZ::Initialize()
{
#ifdef PILEUPREWEIGHTING
	myPRW = (Root::TPileupReweighting*)fInput->FindObject("myPRW");
#endif

#ifdef GOODRUNLIST
	myGRL = (Root::TGoodRunsList*)fInput->FindObject("myGRL");
#endif

#ifdef APPLYJETCALIBRATION
	myJetCalibrationTool = (JetCalibrationTool*)fInput->FindObject("myJetCalibrationTool");
	myJetCalibrationTool->UseGeV(true);
#endif

#ifdef MUONSMEARING
	myMuonSmear = (MuonSmear::SmearingClass*)fInput->FindObject("myMuonSmear");
#endif
}

///=========================================
/// CopyEvent
///=========================================
Bool_t EventBuilder_SMWZ::CopyEvent(AnalysisBase* evt)
{
  // Copy the event from the TTree to the event class. Return false if you want to reject
  // event at the EventBuilder level using D3PD snippets, etc. 	
	
  static const MomKey mChannelNumber("ChannelNumber");
  static const MomKey misMC("isMC");

  fEvt = evt;

  ///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  /// Get the overall configuration
  ///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ: DEBUG CopyEvent() " << endl;	
  
  /// Setup default options
  doBasic        = true;
  doTruthLinks   = false;
  doTruthJetLinks=false;
  doTrackJetLinks= false;
  doJetStructure = false;
  doConstituents = false;
  doTruthConstituents = false;
  doParentChild  = false;
  doTrack        = false;
  doLCCluster    = false;
  doEMCluster    = false;
  doTruth        = false;
  doVertex       = false;
  doPhotons      = false;
  doSubjets      = false;
  doElectrons    = false;
  doMuons    = false;
  doJets    = false;
	doMET					= false;
	doMiniIsolation = false;

	// Load option overides
  fEvt->Cfg()->Get("DOBASIC"         , doBasic        );
  fEvt->Cfg()->Get("DOTRUTHLINKS"    , doTruthLinks   );
  fEvt->Cfg()->Get("DOTRUTH"         , doTruth        );
  fEvt->Cfg()->Get("DOTRACKJETLINKS" , doTrackJetLinks);
  fEvt->Cfg()->Get("DOJETSTRUCT"     , doJetStructure );
  fEvt->Cfg()->Get("DOCONSTIT"       , doConstituents );
  fEvt->Cfg()->Get("DOTRUTHCONSTIT"  , doTruthConstituents );
  fEvt->Cfg()->Get("DOPARENTCHILD"   , doParentChild  );
  fEvt->Cfg()->Get("DOTRACK"         , doTrack        );
  fEvt->Cfg()->Get("DOLCCLUSTER"     , doLCCluster    );
  fEvt->Cfg()->Get("DOEMCLUSTER"     , doEMCluster    );
  fEvt->Cfg()->Get("DOTRUTH"         , doTruth        );
  fEvt->Cfg()->Get("DOVTX"           , doVertex       );
  fEvt->Cfg()->Get("DOPHOTON"        , doPhotons      );
  fEvt->Cfg()->Get("DOSUBJETS"       , doSubjets      );
  fEvt->Cfg()->Get("DOELECTRONS"     , doElectrons    );
  fEvt->Cfg()->Get("DOMUONS"         , doMuons    );
  fEvt->Cfg()->Get("DOMINIISOLATION"         , doMiniIsolation    );
  fEvt->Cfg()->Get("DOMET"         , doMET    );
  fEvt->Cfg()->Get("DOJETS"         , doJets    );
  fEvt->Cfg()->Get("GRL"         , isGRL);

  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyEvent(): DEBUG Setting up configs" << endl;

  ///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  /// BASIC OPTIONS: Run, Event, etc
  ///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  if (doBasic) {
    if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyEvent(): DEBUG Setting basic event level quantities" << endl;

		fEvt->Set("PassedGRL", true);
#ifdef GOODRUNLIST
		if(myGRL) {
			if(!myGRL)
				Abort("GRL not loaded");
			if(!myGRL->HasRunLumiBlock(Get<UInt_t>("RunNumber"), Get<UInt_t>("lbn"))) {
				cout << "did not pass" << endl;
				fEvt->Set("PassedGRL", false); }
			else {
				fEvt->Set("PassedGRL", true);
			}
		 }
#endif
 
    /// Overall event info
    fEvt->Set("RunNumber"         , (int)   Get<UInt_t>("RunNumber"));
    fEvt->Set("EventNumber"       , (int)   Get<UInt_t>("EventNumber"));
    fEvt->Set("EventWeight"       ,  1.0) ; // default value
    fEvt->Set("grl"               ,  true); // default value
    fEvt->Set("larerror"          ,         Get<int>("larError")); 
    fEvt->Set("averageIntPerXing" ,         Get<Float_t>("averageIntPerXing"));
    fEvt->Set("LBN"               , (int)   Get<UInt_t>("lbn"));
    //fEvt->Set("BunchCrossingID"   , (int)   Get<UInt_t>("bcid"));
		int npv=0;
		for(int i=0; i < Get<Int_t>("vxp_n"); i++) {
			if(Get<vector<int> >("vxp_nTracks")[i] >= 2)
				npv++;
		}
		fEvt->Set("npv", npv);


		fEvt->Set("EF_mu18_tight_mu8_EFFS", Get<Bool_t>("EF_mu18_tight_mu8_EFFS"));
		fEvt->Set("EF_mu24i_tight", Get<Bool_t>("EF_mu24i_tight"));
    
		///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		/// Event level options
		///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

		/// Determine if this is Monte Carlo or not 
		/// by looking for the process ID = mc_channel_number
		if(BranchNames().find("mc_channel_number")!=BranchNames().end()) {
		//if(true) {
			//cout << (float) Get<Float_t>("mc_event_weight") << " or " << Get<vector<vector<double> > >("mcevt_weight")[0][0] << endl;
			fEvt->Set(mChannelNumber,(int)Get<UInt_t>("mc_channel_number"));
			fEvt->Set(misMC,true);	

			float averageIntPerXing = Get<Float_t>("averageIntPerXing");
			averageIntPerXing = (Get<UInt_t>("lbn")==1 && int(averageIntPerXing+0.5)==1) ? 0. : averageIntPerXing;
#ifdef PILEUPREWEIGHTING
			float weight = myPRW->GetCombinedWeight(Get<UInt_t>("RunNumber"), Get<UInt_t>("mc_channel_number"), averageIntPerXing);
			fEvt->Set("PRWweight", weight);
#endif
			fEvt->Set("MCweight", Get<Float_t>("mc_event_weight"));

		}
		else {
			fEvt->Set(misMC,          false);	
			fEvt->Set(mChannelNumber, 0);
		}

		if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyEvent(): DEBUG Set event level quantities" << endl;
	}

	///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	/// BASIC OPTIONS: DEFAULT
	///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	else {
		if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyEvent(): DEBUG doBasic = false. Setting event level quantities to 1" << endl;

		fEvt->Set(misMC            , true); //if basic is off, we're almost certainly in MC only mode
		fEvt->Set("RunNumber"      , 1);
		fEvt->Set("EventNumber"    , 1);
		fEvt->Set("ChannelNumber"  , (int) fEvt->Cfg()->Int("CHANNEL"));
		fEvt->Set("grl"            , true);
		fEvt->Set("mcweight"       , Get<Float_t>("mc_event_weight"));

		if(fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyEvent(): DEBUG doBasic = false. Parameters set" << endl;
	}
 

  ///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  /// BUILD EVENT
  ///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  bool success = true;
  
  if(fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyEvent(): About to build the full event" << endl;
  
	if(doMuons)
		success = success && CopyMuons();
	if(doElectrons)
		success = success && CopyElectrons();
	if(doJets)
		success = success && CopyJets();
	if(doTrack)
		success = success && CopyTracks();
	if(doTrack && doMuons && doMiniIsolation)
		success = success && AddMuonMiniIsolation();
	if(fEvt->isMC() && doTruth)
		success = success && CopyTruth();
	if(fEvt->isMC() && doTruth && doMuons)
	  success = success && CopyMuonsTruth();
	if(doMET)
		success = success & CopyMET();
	if(fEvt->isMC() && doTruth && doMET)
		success = success & CopyTruthMET();
	if(doLCCluster)
	  success = success && CopyClusters();
	/*
  success = success && CopyVertices();
  success = success && AddTruthLinks();
  success = success && CopyPhotons();
  success = success && CopyJets();
  
  success = success && AddTrackingInfo();
  success = success && AddTruthJetLinks();
  success = success && AddParentLinks();
  success = success && AddConstituents();
  success = success && AddBadJetType();
  success = success && AddTriggers();

  success = success && CopySubjets();
  success = success && AddBtags();*/

  /// The above need to be sorted by pt, Truth done by E	
  fEvt->SortPtAll(); 

  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyEvent(): DEBUG Done! Status = " << success << endl;
  return success;
}


Bool_t EventBuilder_SMWZ::CopyJets()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyJets() Begin" << endl;

	TString prefix("jet_AntiKt4LCTopo_");
	//TString prefix("jet_AntiKt4TopoEM_");

	fEvt->AddVec("jets");

	unsigned int nJets = Get<Int_t>(prefix + "n");
	for(unsigned int iJ = 0; iJ < nJets; iJ++) {
		Particle *jet = new Particle();
		jet->p.SetPtEtaPhiM(
			Get<vector<float> >(prefix + "pt")[iJ]*UNITCONVERSION,
			Get<vector<float> >(prefix + "eta")[iJ],
		  Get<vector<float> >(prefix + "phi")[iJ],
			Get<vector<float> >(prefix + "m")[iJ]*UNITCONVERSION);

		int npv = fEvt->Int("npv");

#ifdef APPLYJETCALIBRATION

		jet->p = myJetCalibrationTool->ApplyOffsetEtaJES(
				Get<vector<float> >(prefix + "constscale_E")[iJ]*UNITCONVERSION,
				Get<vector<float> >(prefix + "constscale_eta")[iJ],
				Get<vector<float> >(prefix + "constscale_eta")[iJ],
				Get<vector<float> >(prefix + "constscale_phi")[iJ],
				Get<vector<float> >(prefix + "constscale_m")[iJ]*UNITCONVERSION,
				Get<Float_t>("averageIntPerXing"),
				npv
		);

		/*jet->p = myJetCalibrationTool->ApplyJetAreaOffsetEtaJES(
				Get<vector<float> >(prefix + "constscale_E")[iJ]*UNITCONVERSION,
				Get<vector<float> >(prefix + "constscale_eta")[iJ],
				Get<vector<float> >(prefix + "constscale_phi")[iJ],
				Get<vector<float> >(prefix + "m")[iJ]*UNITCONVERSION,
				Get<vector<float> >(prefix + "ActiveAreaPx")[iJ]*UNITCONVERSION,
				Get<vector<float> >(prefix + "ActiveAreaPy")[iJ]*UNITCONVERSION,
				Get<vector<float> >(prefix + "ActiveAreaPz")[iJ]*UNITCONVERSION,
				Get<vector<float> >(prefix + "ActiveAreaE")[iJ]*UNITCONVERSION,
				Get<Float_t>("Eventshape_rhoKt4LC"),
				Get<Float_t>("averageIntPerXing"),
				npv
		);*/

#endif


		jet->Set("isBadLooseMinus", Get<vector<int> >(prefix + "isBadLooseMinus")[iJ]);
		jet->Set("jvtxf", Get<vector<float> >(prefix + "jvtxf")[iJ]);

		fEvt->Add("jets", jet);
	}


  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyJets() Done" << endl;
	return true;

}

Bool_t EventBuilder_SMWZ::CopyMET()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyMET() Begin" << endl;

	fEvt->Set("MET_Et", Get<Float_t>("MET_RefFinal_et") * UNITCONVERSION);
	fEvt->Set("MET_Phi", Get<Float_t>("MET_RefFinal_phi"));

  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyMET() Done" << endl;
	return true;
}

Bool_t EventBuilder_SMWZ::CopyTruthMET()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyTruthMET() Begin" << endl;

	fEvt->Set("TruthMET_Et", Get<Float_t>("MET_Truth_NonInt_et") * UNITCONVERSION);
	fEvt->Set("TruthMET_Phi", Get<Float_t>("MET_Truth_NonInt_phi"));

  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyTruthMET() Done" << endl;
	return true;
}

Bool_t EventBuilder_SMWZ::CopyMuons()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyMuons() Begin" << endl;

	TString prefix("mu_staco_");

	fEvt->AddVec("muons");

	unsigned int nMuons = Get<Int_t>("mu_staco_n");
	for(unsigned int iMu = 0; iMu < nMuons; iMu++) {
		Particle *muon = new Particle();
		muon->Set("charge", Get<vector<int> >(prefix + "charge")[iMu]);
		muon->p.SetPtEtaPhiM(
			Get<vector<float> >(prefix + "pt")[iMu]*UNITCONVERSION,
			Get<vector<float> >(prefix + "eta")[iMu],
			Get<vector<float> >(prefix + "phi")[iMu],
			Get<vector<float> >(prefix + "m")[iMu]*UNITCONVERSION);

		muon->Set("id_qoverp", Get<vector<float> >(prefix + "id_qoverp")[iMu]/UNITCONVERSION);
		muon->Set("me_qoverp", Get<vector<float> >(prefix + "me_qoverp")[iMu]/UNITCONVERSION);
		muon->Set("id_qoverp_err", sqrt(Get<vector<float> >(prefix + "id_cov_qoverp_exPV")[iMu])/UNITCONVERSION);
		muon->Set("me_qoverp_err", sqrt(Get<vector<float> >(prefix + "me_cov_qoverp_exPV")[iMu])/UNITCONVERSION);
		muon->Set("id_pt", fabs(1.0/Get<vector<float> >(prefix + "id_qoverp")[iMu]) * sin(Get<vector<float> >(prefix + "id_theta")[iMu])*UNITCONVERSION);
		muon->Set("me_pt", fabs(1.0/Get<vector<float> >(prefix + "me_qoverp")[iMu]) * sin(Get<vector<float> >(prefix + "me_theta")[iMu])*UNITCONVERSION);
#ifdef MUONSMEARING
		if(fEvt->Bool("isMC") && Get<vector<int> >(prefix + "isCombinedMuon")[iMu]) {
			myMuonSmear->SetSeed(fEvt->Int("EventNumber"), iMu);
			myMuonSmear->Event(
					muon->Float("id_pt"),
					muon->Float("me_pt"),
					Get<vector<float> >(prefix + "pt")[iMu]*UNITCONVERSION,
					Get<vector<float> >(prefix + "eta")[iMu],
					Get<vector<int> >(prefix + "charge")[iMu]);

			muon->p.SetPtEtaPhiM(
				myMuonSmear->pTCB(),
				Get<vector<float> >(prefix + "eta")[iMu],
				Get<vector<float> >(prefix + "phi")[iMu],
				Get<vector<float> >(prefix + "m")[iMu]*UNITCONVERSION);

			if(myMuonSmear->ChargeFlipCB() == -1)
				muon->Set("charge", -Get<vector<int> >(prefix + "charge")[iMu]);
		}
#endif

		// Impact parameters
		muon->Set("d0", Get<vector<float> >(prefix + "trackd0pvunbiased")[iMu]);
		muon->Set("d0_err", Get<vector<float> >(prefix + "tracksigd0pvunbiased")[iMu]);
		muon->Set("z0", Get<vector<float> >(prefix + "trackz0pvunbiased")[iMu]);
		muon->Set("tracktheta", Get<vector<float> >(prefix + "tracktheta")[iMu]);

		muon->Set("isCombined", Get<vector<int> >(prefix + "isCombinedMuon")[iMu]);
		muon->Set("author", Get<vector<int> >(prefix + "author")[iMu]);

		// Quality
		muon->Set("nPixHits", Get<vector<int> >(prefix + "nPixHits")[iMu]);
		muon->Set("nPixDead", Get<vector<int> >(prefix + "nPixelDeadSensors")[iMu]);
		muon->Set("nPixHoles", Get<vector<int> >(prefix + "nPixHoles")[iMu]);
		muon->Set("nSCTHits", Get<vector<int> >(prefix + "nSCTHits")[iMu]);
		muon->Set("nSCTDead", Get<vector<int> >(prefix + "nSCTDeadSensors")[iMu]);
		muon->Set("nSCTHoles", Get<vector<int> >(prefix + "nSCTHoles")[iMu]);
		muon->Set("expectBLayerHit", Get<vector<int> >(prefix + "expectBLayerHit")[iMu]);
		muon->Set("nBLayerHits", Get<vector<int> >(prefix + "nBLHits")[iMu]);
		muon->Set("nTRTHits", Get<vector<int> >(prefix + "nTRTHits")[iMu]);
		muon->Set("nTRTOutliers", Get<vector<int> >(prefix + "nTRTOutliers")[iMu]);
		
		// Isolation cones
		muon->Set("Ptcone20", Get<vector<float> >(prefix + "ptcone20")[iMu]*UNITCONVERSION);
		muon->Set("Ptcone30", Get<vector<float> >(prefix + "ptcone30")[iMu]*UNITCONVERSION);
		muon->Set("Ptcone40", Get<vector<float> >(prefix + "ptcone40")[iMu]*UNITCONVERSION);
		muon->Set("Etcone20", Get<vector<float> >(prefix + "etcone20")[iMu]*UNITCONVERSION);
		muon->Set("Etcone30", Get<vector<float> >(prefix + "etcone30")[iMu]*UNITCONVERSION);
		muon->Set("Etcone40", Get<vector<float> >(prefix + "etcone40")[iMu]*UNITCONVERSION);
		muon->Set("Nucone20", Get<vector<float> >(prefix + "nucone20")[iMu]);
		muon->Set("Nucone30", Get<vector<float> >(prefix + "nucone30")[iMu]);
		muon->Set("Nucone40", Get<vector<float> >(prefix + "nucone40")[iMu]);

		fEvt->Add("muons", muon);
	}


  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyMuons() Done" << endl;
	return true;
}

Bool_t EventBuilder_SMWZ::CopyElectrons()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyElectrons() Begin" << endl;

	//Constant for mass
	double electron_mass = 0.510998/1000.0;
	TString prefix("el_");
	
	fEvt->AddVec("electrons");

	unsigned int nElectrons = Get<Int_t>(prefix + "n");
	for(unsigned int iE = 0; iE < nElectrons; iE++) {
		Particle *electron = new Particle();
		electron->Set("charge", Get<vector<int> >(prefix + "charge")[iE]);
		
		electron->Set("d0_err", Get<vector<float> >(prefix + "tracksigd0pv")[iE]);
		electron->Set("z0", Get<vector<float> >(prefix + "trackz0pv")[iE]);
		electron->Set("d0", Get<vector<float> >(prefix + "trackd0pv")[iE]);

		// Quality
		electron->Set("nPixHits", Get<vector<int> >(prefix + "nPixHits")[iE]);
		electron->Set("nSCTHits", Get<vector<int> >(prefix + "nSCTHits")[iE]);
		
		electron->Set("mediumPP", (bool)Get<vector<int> >(prefix + "mediumPP")[iE]);
		electron->Set("tightPP", (bool)Get<vector<int> >(prefix + "tightPP")[iE]);
		electron->Set("loosePP", (bool)Get<vector<int> >(prefix + "loosePP")[iE]);/*
		electron->Set("tight", (bool)((quality_bitmask & (1 << 2)) >> 2));
		electron->Set("medium", (bool)((quality_bitmask & (1 << 2)) >> 1));
		electron->Set("loose", (bool)(quality_bitmask & 1));
		*/
		electron->Set("author", Get<vector<int> >(prefix + "author")[iE]);
		electron->Set("OQ", Get<vector<unsigned int> >(prefix + "OQ")[iE]);
		
		// Pt/Eta/Phi
		electron->Set("trackEta", Get<vector<float> >(prefix + "tracketa")[iE]);
		electron->Set("trackPhi", Get<vector<float> >(prefix + "trackphi")[iE]);
		electron->Set("trackPt", Get<vector<float> >(prefix + "trackpt")[iE]*UNITCONVERSION);

		electron->Set("clusterEta", Get<vector<float> >(prefix + "cl_eta")[iE]);
		electron->Set("clusterPhi", Get<vector<float> >(prefix + "cl_phi")[iE]);
		electron->Set("clusterE", Get<vector<float> >(prefix + "cl_E")[iE]*UNITCONVERSION);

		if(electron->Int("nPixHits") + electron->Int("nSCTHits") >= 4) {
			electron->p.SetPtEtaPhiM(
				electron->Float("clusterE") / cosh(electron->Float("trackEta")),
				electron->Float("trackEta"),
				electron->Float("trackPhi"),
				electron_mass);
		} else {
			electron->p.SetPtEtaPhiM(
				electron->Float("clusterE") / cosh(electron->Float("clusterEta")),
				electron->Float("clusterEta"),
				electron->Float("clusterPhi"),
				electron_mass);
		}
		
		// Isolation cones
		electron->Set("Ptcone20", Get<vector<float> >(prefix + "ptcone20")[iE]*UNITCONVERSION);
		electron->Set("Ptcone30", Get<vector<float> >(prefix + "ptcone30")[iE]*UNITCONVERSION);
		electron->Set("Ptcone40", Get<vector<float> >(prefix + "ptcone40")[iE]*UNITCONVERSION);
		electron->Set("Etcone20", Get<vector<float> >(prefix + "Etcone20")[iE]*UNITCONVERSION);
		electron->Set("Etcone20_corrected", Get<vector<float> >(prefix + "Etcone20_pt_corrected")[iE]*UNITCONVERSION);
		electron->Set("Etcone30", Get<vector<float> >(prefix + "Etcone30")[iE]*UNITCONVERSION);
		electron->Set("Etcone40", Get<vector<float> >(prefix + "Etcone40")[iE]*UNITCONVERSION);

		fEvt->Add("electrons", electron);
	}

  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyElectrons() Done" << endl;
	return true;
}

Bool_t EventBuilder_SMWZ::CopyTracks()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyTracks() Begin" << endl;

	fEvt->AddVec("tracks");
	
	unsigned int nTracks = Get<Int_t>("trk_n");
	for(int iT=0; iT < nTracks; iT++) {
		Particle *track = new Particle();
		track->p.SetPtEtaPhiM(
				Get<vector<float> >("trk_pt")[iT] * UNITCONVERSION,
				Get<vector<float> >("trk_eta")[iT],
				Get<vector<float> >("trk_phi_wrtPV")[iT],
				0);

		track->Set("d0", Get<vector<float> >("trk_d0_wrtPV")[iT]);
		track->Set("z0", Get<vector<float> >("trk_z0_wrtPV")[iT]);

		TString prefix("trk_");
		track->Set("nPixHits", Get<vector<int> >(prefix + "nPixHits")[iT]);
		track->Set("nPixDead", Get<vector<int> >(prefix + "nPixelDeadSensors")[iT]);
		track->Set("nPixHoles", Get<vector<int> >(prefix + "nPixHoles")[iT]);
		track->Set("nSCTHits", Get<vector<int> >(prefix + "nSCTHits")[iT]);
		track->Set("nSCTDead", Get<vector<int> >(prefix + "nSCTDeadSensors")[iT]);
		track->Set("nSCTHoles", Get<vector<int> >(prefix + "nSCTHoles")[iT]);
		track->Set("expectBLayerHit", Get<vector<int> >(prefix + "expectBLayerHit")[iT]);
		track->Set("nBLayerHits", Get<vector<int> >(prefix + "nBLHits")[iT]);

		fEvt->Add("tracks", track);
	}

  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyTracks() Done" << endl;
	return true;
}

Bool_t EventBuilder_SMWZ::CopyClusters() 
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ: DEBUG CopyClusters() " << endl;	

	fEvt->AddVec("clustersLCTopo");

	for(unsigned int iCl = 0; iCl < Get<UInt_t>("cl_n"); iCl++) {
		Particle* cl = new Particle();
		float pt  = Get<vector<float> >("cl_pt") .at(iCl);
		float eta = Get<vector<float> >("cl_eta").at(iCl);
		float phi = Get<vector<float> >("cl_phi").at(iCl);
		cl->p.SetPtEtaPhiM(pt*UNITCONVERSION,eta,phi,0);

		fEvt->Add("clustersLCTopo",cl);
	}
  
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyClusters(): scheduled clusters" << endl;
  return kTRUE;
}

Bool_t EventBuilder_SMWZ::AddMuonMiniIsolation()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::AddMuonMiniIsolation() Begin" << endl;

	for(int iMu=0; iMu < fEvt->muons(); iMu++) {
		Particle &mu = fEvt->muon(iMu);
		double miniiso5 = 0;
		double miniiso10 = 0;
		double miniiso15 = 0;
		double miniiso20 = 0;
		double miniiso25 = 0;
		double miniiso30 = 0;
		double miniiso35 = 0;
		double miniiso40 = 0;
		double miniiso45 = 0;
		double miniiso60 = 0;
		double miniiso80 = 0;
		double miniiso100 = 0;
		double miniiso45max30 = 0;
		double iso40 = 0;
		double iso30 = 0;
		double iso20 = 0;

		for(int iT=0; iT < fEvt->tracks(); iT++) {
			if(fEvt->track(iT).p.Pt() < 1.0) continue;
			if(fabs(fEvt->track(iT).Float("d0")) > 1.5) continue;
			if(fabs(fEvt->track(iT).Float("z0")) > 1.0) continue;
			if(fEvt->track(iT).Int("expectBLayerHit") == 1 && fEvt->track(iT).Int("nBLayerHits") == 0) continue;
			if(fEvt->track(iT).Int("nPixHits") + fEvt->track(iT).Int("nPixDead") < 2) continue;
			if(fEvt->track(iT).Int("nPixHits") + fEvt->track(iT).Int("nPixDead") + fEvt->track(iT).Int("nSCTDead") + fEvt->track(iT).Int("nSCTHits") < 7) continue;
			if(fEvt->track(iT).Int("nPixHits") + fEvt->track(iT).Int("nSCTHits") < 3) continue;
			//if(fEvt->track(iT).Int("nPixHoles") > 0) continue;

			double deltar = fEvt->track(iT).p.DeltaR(mu.p);

			if(deltar < 0.004) continue;

			if(deltar < 5.0 / mu.p.Pt()) 
				miniiso5 += fEvt->track(iT).p.Pt();
			if(deltar < 10.0 / mu.p.Pt()) 
				miniiso10 += fEvt->track(iT).p.Pt();
			if(deltar < 15.0 / mu.p.Pt()) 
				miniiso15 += fEvt->track(iT).p.Pt();
			if(deltar < 20.0 / mu.p.Pt()) 
				miniiso20 += fEvt->track(iT).p.Pt();
			if(deltar < 25.0 / mu.p.Pt()) 
				miniiso25 += fEvt->track(iT).p.Pt();
			if(deltar < 30.0 / mu.p.Pt()) 
				miniiso30 += fEvt->track(iT).p.Pt();
			if(deltar < 35.0 / mu.p.Pt()) 
				miniiso35 += fEvt->track(iT).p.Pt();
			if(deltar < 40.0 / mu.p.Pt()) 
				miniiso40 += fEvt->track(iT).p.Pt();
			if(deltar < 45.0 / mu.p.Pt()) 
				miniiso45 += fEvt->track(iT).p.Pt();
			if(deltar < 60.0 / mu.p.Pt()) 
				miniiso60 += fEvt->track(iT).p.Pt();
			if(deltar < 80.0 / mu.p.Pt()) 
				miniiso80 += fEvt->track(iT).p.Pt();
			if(deltar < 100.0 / mu.p.Pt()) 
				miniiso100 += fEvt->track(iT).p.Pt();
			
			if(deltar < (45.0 / mu.p.Pt() > 0.3 ? 0.3 : 45.0 / mu.p.Pt()))
				miniiso45max30 += fEvt->track(iT).p.Pt();
			if(deltar < 0.4)
				iso40 += fEvt->track(iT).p.Pt();
			if(deltar < 0.3)
				iso30 += fEvt->track(iT).p.Pt();
			if(deltar < 0.2)
				iso20 += fEvt->track(iT).p.Pt();
		}

		mu.Set("PtconeMini5", miniiso5);
		mu.Set("PtconeMini10", miniiso10);
		mu.Set("PtconeMini15", miniiso15);
		mu.Set("PtconeMini20", miniiso20);
		mu.Set("PtconeMini25", miniiso25);
		mu.Set("PtconeMini30", miniiso30);
		mu.Set("PtconeMini35", miniiso35);
		mu.Set("PtconeMini40", miniiso40);
		mu.Set("PtconeMini45", miniiso45);
		mu.Set("PtconeMini60", miniiso60);
		mu.Set("PtconeMini80", miniiso80);
		mu.Set("PtconeMini100", miniiso100);
		mu.Set("PtconeMini45Max30", miniiso45max30);

		mu.Set("Ptcone40my", iso40);
		mu.Set("Ptcone30my", iso30);
		mu.Set("Ptcone20my", iso20);
	}
  
	if (fEvt->Debug()) cout << "EventBuilder_SMWZ::AddMuonMiniIsolation() Done" << endl;
	return true;
}

Bool_t EventBuilder_SMWZ::CopyMuonsTruth()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyMuonsTruth() Begin" << endl;
	fEvt->AddVec("truthsmuons");

	unsigned int nTruth = Get<Int_t>("muonTruth_n");
	for(unsigned int iT = 0; iT < nTruth; iT++) {
		Particle *truth = new Particle();
		truth->p.SetPtEtaPhiM(
			Get<vector<float> >("muonTruth_pt")[iT]*UNITCONVERSION,
			Get<vector<float> >("muonTruth_eta")[iT],
		  Get<vector<float> >("muonTruth_phi")[iT],
			Get<vector<float> >("muonTruth_m")[iT]*UNITCONVERSION);

		// Deduplicator
		/*
		int add=1;
		for(unsigned int iT2 = 0; iT2 < fEvt->Objs("truthsmuons"); iT2++) {
			if(fEvt->truth(iT2, "muons").p.DeltaR(truth->p) < 0.01) {
				add = 0;
				break;
			}
		}
		if(add == 0) {
			delete truth;
			break;
		}*/

		truth->Set("charge", Get<vector<float> >("muonTruth_charge")[iT]);
		truth->Set("origin", Get<vector<int> >("muonTruth_origin")[iT]);

		fEvt->Add("truthsmuons", truth);
	}

  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyMuonsTruth() Done" << endl;
	return true;
}
	

Bool_t EventBuilder_SMWZ::CopyTruth()
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyTruth() Begin" << endl;
	fEvt->AddVec("truths");
	map<int, Particle *> truths_by_ntuple_index;

	unsigned int nTruth = Get<Int_t>("mc_n");
	for(unsigned int iT = 0; iT < nTruth; iT++) {
		int status = Get<vector<int> >("mc_status")[iT];
		//if(status != 1 && status != 2 && status != 4) continue; //Recommendation from https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/PhysicsAnalysisWorkBookExMCRel15

		Particle *truth = new Particle();
		truth->p.SetPtEtaPhiM(
			Get<vector<float> >("mc_pt")[iT]*UNITCONVERSION,
			Get<vector<float> >("mc_eta")[iT],
		  Get<vector<float> >("mc_phi")[iT],
			Get<vector<float> >("mc_m")[iT]*UNITCONVERSION);
		truth->Set("pdgId", Get<vector<int> >("mc_pdgId")[iT]);
		truth->Set("barcode", Get<vector<int> >("mc_barcode")[iT]);
		truth->Set("status", status);

		truth->Set("_indexinntup", iT);
		truth->AddVec("children", true); // weak reference
		truth->AddVec("parents"); // weak reference
		fEvt->Add("truths", truth);
		truths_by_ntuple_index.insert( pair<int, Particle *>(iT, truth) );
	}

	// Build parent/children arrays
	for(int iT = 0; iT < fEvt->truths(); iT++) {
		Particle *truth = &fEvt->truth(iT);
		int index = truth->Int("_indexinntup");
		vector<int> index_parents = Get<vector<vector<int> > >("mc_parent_index")[index];

		for(int iT2=0; iT2 < index_parents.size(); iT2++) {
			int index_parent = index_parents[iT2];
			// Go look for the parent using the ntup map
			map<int, Particle *>::iterator it = truths_by_ntuple_index.find(index_parent);

			// Did we find a parent: if so do something
			if(it != truths_by_ntuple_index.end()) {
				Particle *truth_parent = (*it).second;
				truth_parent->Add("children", truth);
				truth->Add("parents", truth_parent);
			}
		}
	}

  if (fEvt->Debug()) cout << "EventBuilder_SMWZ::CopyTruth() Done" << endl;
	return true;
}


///=========================================
/// WriteAuxTrees
///=========================================
void EventBuilder_SMWZ::WriteAuxTrees(TDirectory* outfile)
{
  if (fEvt->Debug()) cout << "EventBuilder_SMWZ: DEBUG WriteAuxTrees() " << endl;
}
