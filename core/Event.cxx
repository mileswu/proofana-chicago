/**************************************************************************
 **
 **   File:         Event.cxx
 **
 **   Description:  Event class
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      3-7-11
 **   Modified:
 **
 **************************************************************************/

#define Event_cxx

#include "Event.h"
#include "AnaConfig.h"

Event::~Event()
{

	// Destructor
	
}

void Event::ResetDefaults() 
{
	static const MomKey leptons("leptons"), electrons("electrons"), muons("muons"), jets("jets"),
		vtxs("vtxs"), tracks("tracks"), photons("photons"), clusters("clusters"), 
		truths("truths"), mx("Metx"), my("Mety");
		
	fDefaultLeptons = MomKey(leptons);
	fDefaultElectrons = MomKey(electrons);
	fDefaultMuons = MomKey(muons);
	fDefaultJets = MomKey(jets);
	fDefaultVtxs = MomKey(vtxs);
	fDefaultTracks = MomKey(tracks);
	fDefaultPhotons = MomKey(photons);
	fDefaultClusters = MomKey(clusters);
	fDefaultTruths = MomKey(truths);
	fDefaultMetx = MomKey(mx); 
	fDefaultMety = MomKey(my); 
	
}

void Event::SetTruthDefaults() 
{
	static const MomKey leptons("leptonstruth"), electrons("electronstruth"), muons("muonstruth"), jets("jetstruth"),
		vtxs("vtxstruth"), tracks("trackstruth"), photons("photonstruth"), clusters("clusterstruth"), 
		truths("truthstruth"), mx("Metxtruth"), my("Metytruth");
		
	fDefaultLeptons = MomKey(leptons);
	fDefaultElectrons = MomKey(electrons);
	fDefaultMuons = MomKey(muons);
	fDefaultJets = MomKey(jets);
	fDefaultVtxs = MomKey(vtxs);
	fDefaultTracks = MomKey(tracks);
	fDefaultPhotons = MomKey(photons);
	fDefaultClusters = MomKey(clusters);
	fDefaultTruths = MomKey(truths);
	fDefaultMetx = MomKey(mx); 
	fDefaultMety = MomKey(my); 

}

void Event::ClearEvent()
{
	Reset();
}

void Event::InitEvent()
{
	bool MCweights = true;
	static const MomKey mcws("MCWEIGHTS");
	Cfg()->Get(mcws,MCweights);
	
	bool evtweights = true;
	static const MomKey ews("EVTWEIGHTS");	
	Cfg()->Get(ews,evtweights);
	
	static const MomKey ew("EventWeight"), lum("LUMI"), eb("EVENTBUILDER");
	
	static const bool isEvent = !(DatasetCfg()->String(eb).CompareTo("event"));
	if(!isEvent) {
		//Compute/reset weight
		if((fPrevChannelNumber!=ChannelNumber())||fFirstEvent) {

			fPrevChannelNumber=ChannelNumber();

			if(isMC()&&MCweights) ComputeCrossSectionWeight();
			else fCrossSectionWeight = 1.;
		}
		fDefaultWeight = fCrossSectionWeight;

		//do Mc@Nlo/overlap/trigger prescale weights (weights you want applied regardless of analysis)	
		if(Exists(ew)&&evtweights) fDefaultWeight*=Float(ew);
	}
	else { //Reweight luminosity, cross-section if info available

		if(isMC()) {
			TString xs_str = TString::Format("xs_%i",ChannelNumber());
			double xs = 0.;
			if(MCweights&&DatasetCfg()->Get(xs_str,xs)) ComputeCrossSectionWeight(); //Only recompute if info available (skim has config file)
			else if(MCweights&&(fLumi>0.)) fCrossSectionWeight*=(Cfg()->Double(lum)/fLumi); //Rescale luminosity, leaving cross section calculation unchanged
			else if(MCweights) Abort("Event: ERROR Attempted luminosity reweighting on MC missing cross section weights. Supply a config file with cross section information");
			else if(!MCweights) fCrossSectionWeight = 1.; //Remove cross-section weights if desired
		}

		fDefaultWeight = fCrossSectionWeight;

		//do Mc@Nlo/overlap/trigger prescale weights (weights you want applied regardless of analysis)
		if(Exists(ew)&&evtweights) fDefaultWeight*=Float(ew);	
	}
	
	fLumi = Cfg()->Double(lum);
	if(isMC()&&(!MCweights)) fLumi = 0.;
	
	fWeight = fDefaultWeight;
	
	if(fFirstEvent) fFirstEvent = false;
	
}

void Event::ComputeCrossSectionWeight()
{

	double xs;
	double k;
	Long64_t n;
	static const MomKey lum("LUMI");

	if((!DatasetCfg()->Get(TString::Format("xs_%i",ChannelNumber()),xs))||(!DatasetCfg()->Get(TString::Format("n_%i",ChannelNumber()),n))||(!DatasetCfg()->Get(TString::Format("k_%i",ChannelNumber()),k))) {
		TString msg("Event: WARNING Missing cross-section, K-factor, or number of entries information for MC process number ");
		msg.Append(String("ChannelNumber"));
		msg.Append(", set MCWEIGHTS to false if you do not want cross-section/luminosity weighting");
		Abort(msg);
	}

	fCrossSectionWeight = (Cfg()->Double(lum)*xs*k)/n;
	
}

bool Event::isMC()
{
	bool isMC;
	static const MomKey ismc("isMC"), chn("ChannelNumber"), rn("RunNumber");
	
	if(Get(ismc,isMC)) return isMC; 
	int channelnum;
	if(Get(chn,channelnum)) { //R17
		if(channelnum!=Int(rn)) return true;
		else return false;
	}
	else return (RunNumber()<152166); //backwards compatibility, release 16 and before
}

