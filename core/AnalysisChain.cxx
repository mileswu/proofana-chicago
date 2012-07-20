/**************************************************************************
 **
 **   File:         AnalysisChain.h
 **
 **   Description:  Analysis chain class
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      6-1-12
 **   Modified:
 **
 **************************************************************************/

#define AnalysisChain_cxx

#include "AnalysisChain.h"
#include "SelectorBase.h"
#include "EventBuilderBase.h"
#include "AnaConfig.h"

AnalysisChain::~AnalysisChain()
{
	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) delete *iter;
}

void AnalysisChain::SetConfig(Config* config)
{

	if(!config) Abort("AnalysisChain: ERROR No valid Config class");
	
	else AnalysisBase::SetConfig(config);
	SetOutput(false);

	const MomKey mANALYSIS("ANALYSIS");	

	if(Cfg()->Type(mANALYSIS)!=OBJ) Abort("AnalysisChain: ERROR ANALYSIS key not an object vector");
	if(!Cfg()->Objs(mANALYSIS)) Abort("AnalysisChain: ERROR ANALYSIS object vector is empty");

	for(int i = 0; i<Cfg()->Objs(mANALYSIS); ++i) {
		AnalysisBase* analysis(0);
		Config* cfg = (Config*)Cfg()->Obj(mANALYSIS,i);
		TString ana_name(cfg->String(mANALYSIS));
			
		TClass* myAnalysisClass = TClass::GetClass(TString::Format("Analysis_%s",ana_name.Data()).Data());
		if(myAnalysisClass) analysis = (AnalysisBase*)myAnalysisClass->New();
		else {
			TString msg("ProofAna: ERROR Analysis type ");
			msg.Append(ana_name);
			msg.Append(" not found or has no dictionary");
			Abort(msg);
		}
		
		analysis->SetConfig(cfg);
		fAnalyses.push_back(analysis);	
	}	
}

void AnalysisChain::SetInputList(TList* input)
{
	AnalysisBase::SetInputList(input);
	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) (*iter)->SetInputList(input);
}

void AnalysisChain::SetDatasetConfig(Config* config)
{
	AnalysisBase::SetDatasetConfig(config);
	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) (*iter)->SetDatasetConfig(config);
}

void AnalysisChain::SetEventBuilder(EventBuilderBase* eb)
{
	AnalysisBase::SetEventBuilder(eb);
	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) (*iter)->SetEventBuilder(eb);
}

void AnalysisChain::SetOutputDir(TDirectory* /*dir*/)
{
	Abort("AnalysisChain: ERROR AnalysisChain::SetOutputDir should NEVER be called!");
}

void AnalysisChain::SetSelector(SelectorBase* selector)
{
	AnalysisBase::SetSelector(selector);
	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) (*iter)->SetSelector(selector);
}

void AnalysisChain::FlushCache(TDirectory* file)
{
	AnalysisBase::FlushCache(file);
	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) (*iter)->FlushCache(file);
}

void AnalysisChain::SlaveBegin(TTree* /*tree*/)
{

	// When running with PROOF SlaveBegin() is called on each slave server.
	// The tree argument is deprecated (on PROOF 0 is passed).

	//TString option = GetOption();

	static const AnaKey a_SlaveBegin(":SlaveBegin");
	AnaKey aName_SlaveBegin(Name()+a_SlaveBegin);
	Time()->Start(aName_SlaveBegin);

	//Set up default output directory
	AnalysisBase::SetOutputDir(Selector()->GetFile("default"));
	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) {
		if((*iter)->Output()) (*iter)->SetOutputDir((TDirectory*)Selector()->GetFile("default")->Get((*iter)->Name().Data()));
		else (*iter)->SetOutputDir(Selector()->GetFile("default"));
	}

	//Set timer output directory, if requested
	if(SaveTimers()) {
		TDirectory* tDir = (TDirectory*)Selector()->GetFile("default")->Get("Timers");
		if(!tDir) tDir = Selector()->GetFile("default")->mkdir("Timers");
		Time()->SetHistDir(tDir);
		
		iter = fAnalyses.begin();
		for(; iter!=fAnalyses.end(); ++iter) {
			if((*iter)->SaveTimers()) {
				(*iter)->Time()->SetHistDir(tDir);
			}
		}
	}

	static const AnaKey a_SlaveBegin_WorkerBegin(":SlaveBegin:WorkerBegin");
	iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) {
		AnaKey aName_SlaveBegin_WorkerBegin((*iter)->Name()+a_SlaveBegin_WorkerBegin);

		Time()->Start(aName_SlaveBegin_WorkerBegin);
		if(Debug()) cout << "AnalysisChain: DEBUG Begin WorkerBegin() execution on analysis " << (*iter)->Name() << endl;	
		(*iter)->WorkerBegin();
		if(Debug()) cout << "AnalysisChain: DEBUG Finish WorkerBegin() execution on analysis " << (*iter)->Name() << endl;
		Time()->Stop(aName_SlaveBegin_WorkerBegin);		

		//Copy registered trees
		map<AnaKey, bool>::const_iterator tree = (*iter)->Trees().begin();
		for(; tree != (*iter)->Trees().end(); ++tree) RegisterTree(tree->first,tree->second);
	}
	
	Time()->Stop(aName_SlaveBegin);
}

Bool_t AnalysisChain::Process()
{
	// The Process() function is called for each entry in the tree (or possibly
	// keyed object in the case of PROOF) to be processed. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	// It can be passed to either AnalysisChain::GetEntry() or TBranch::GetEntry()
	// to read either all or the required parts of the data. When processing
	// keyed objects with PROOF, the object is already loaded and is available
	// via the fObject pointer.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	//
	// Use fStatus to set the return value of TTree::Process().
	//
	// The return value is currently not used.

	static const AnaKey a_Process(":Process");
	AnaKey aName_Process(Name()+a_Process);
	Time()->Start(aName_Process);	
	
	//Reset default output directory
	AnalysisBase::SetOutputDir(Selector()->GetFile("default"));
	//Clear list of trees to be filled
	ClearFillTrees();
	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) {
		if((*iter)->Output()) (*iter)->SetOutputDir((TDirectory*)Selector()->GetFile("default")->Get((*iter)->Name().Data()));
		else (*iter)->SetOutputDir(Selector()->GetFile("default"));
		(*iter)->ClearFillTrees();
	}

	static const AnaKey a_Process_ClearEvent(":Process:ClearEvent");
	AnaKey aName_Process_ClearEvent(Name()+a_Process_ClearEvent);
	Time()->Start(aName_Process_ClearEvent);	
	
	if(Debug()) cout << "AnalysisChain: DEBUG Begin ClearEvent() execution on analysis " << Name() << endl;	
	ClearEvent(); //Clear previous event
	if(Debug()) cout << "AnalysisChain: DEBUG Finish ClearEvent() execution on analysis " << Name() << endl;
	
	Time()->Stop(aName_Process_ClearEvent);	

	static const AnaKey a_Process_CopyEvent(":Process:CopyEvent");
	AnaKey aName_Process_CopyEvent(Name()+a_Process_CopyEvent);
	Time()->Start(aName_Process_CopyEvent);	
	
	if(Debug()) cout << "AnalysisChain: DEBUG Begin CopyEvent() execution on analysis " << Name() << endl;
	bool reject = !Eb()->CopyEvent(this);
	if(Debug()) cout << "AnalysisChain: DEBUG Finish CopyEvent() execution on analysis " << Name() << endl;
	
	Time()->Stop(aName_Process_CopyEvent);	

	//Select appropriate output file if multi-output
	static const MomKey moutput_("output_");
	static const MomKey mChannelNumber("ChannelNumber");
	static const MomKey mRunNumber("RunNumber");
	MomKey moutput_key;
	
	TString key;
	if(Get(mChannelNumber,key)) moutput_key = moutput_ + MomKey(key); //R17
	else moutput_key = moutput_ + MomKey(String(mRunNumber)); //R16 and before, data

	static const MomKey mOutput("Output");
	if(Get(mOutput,key)) moutput_key = moutput_ + MomKey(key); //override RunNumber if desired
	if(DatasetCfg()->Exists(moutput_key)) {
		AnalysisBase::SetOutputDir(Selector()->GetFile(DatasetCfg()->String(moutput_key)));
		iter = fAnalyses.begin();
		for(; iter!=fAnalyses.end(); ++iter) {
			if((*iter)->Output()) (*iter)->SetOutputDir((TDirectory*)Selector()->GetFile(DatasetCfg()->String(moutput_key))->Get((*iter)->Name().Data()));
			else (*iter)->SetOutputDir(Selector()->GetFile(DatasetCfg()->String(moutput_key)));
		}
	}

	static const AnaKey a_Process_InitEvent(":Process:InitEvent");
	AnaKey aName_Process_InitEvent(Name()+a_Process_InitEvent);
	Time()->Start(aName_Process_InitEvent);	
	
	if(Debug()) cout << "AnalysisChain: DEBUG Begin InitEvent() execution on analysis " << Name() << endl;	
	InitEvent(); //Compute things like weights..
	if(Debug()) cout << "AnalysisChain: DEBUG Finish InitEvent() execution on analysis " << Name() << endl;
	
	Time()->Stop(aName_Process_InitEvent);	

	//Copy event to first analysis
	(*fAnalyses.begin())->MoveEvent(*static_cast<Event*>(this));

	static const AnaKey a_Process_ProcessEvent(":Process:ProcessEvent");

	vector<AnalysisBase*>::iterator olditer;
	iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ) {
		//Count this event as processed even if rejected at EventBuilder or previous analysis level
		if((*iter)->Output()) {
			(*iter)->SetPrefix();
			(*iter)->BookCutflow();
		}

		AnaKey aName_Process_ProcessEvent(Name()+a_Process_ProcessEvent);
		Time()->Start(aName_Process_ProcessEvent);			

		if(Debug()) cout << "AnalysisChain: DEBUG Begin ProcessEvent() execution on analysis " << (*iter)->Name() << endl;
		if(!reject) reject = !(*iter)->ProcessEvent();
		if(Debug()) cout << "AnalysisChain: DEBUG Finish ProcessEvent() execution on analysis " << (*iter)->Name() << endl;

		Time()->Stop(aName_Process_ProcessEvent);
		
		//Add filled trees to chain
		set<AnaKey>::const_iterator tree = (*iter)->FillTrees().begin();
		for(; tree != (*iter)->FillTrees().end(); ++tree) FillTree(*tree);
		
		//Reset weights to default
		(*iter)->Weight((*iter)->DefaultWeight());
		
		//Move event to next analysis
		olditer = iter++;
		if(iter!=fAnalyses.end()) (*iter)->MoveEvent(*static_cast<Event*>(*olditer));
	}
	
	//Move event from the last analysis to the chain (for tree filling, etc.)
	MoveEvent(*static_cast<Event*>(*fAnalyses.rbegin()));
	
	Time()->Stop(aName_Process);
	return !reject;
	
}

void AnalysisChain::SlaveTerminate()
{
	// The SlaveTerminate() function is called after all entries or objects
	// have been processed. When running with PROOF SlaveTerminate() is called
	// on each slave server.
	static const AnaKey a_SlaveTerminate(":SlaveTerminate");
	AnaKey aName_SlaveTerminate(Name()+a_SlaveTerminate);
	Time()->Start(aName_SlaveTerminate);	

	static const AnaKey a_SlaveTerminate_WorkerTerminate(":SlaveTerminate:WorkerTerminate");

	vector<AnalysisBase*>::iterator iter = fAnalyses.begin();
	for(; iter!=fAnalyses.end(); ++iter) {
		AnaKey aName_SlaveTerminate_WorkerTerminate((*iter)->Name()+a_SlaveTerminate_WorkerTerminate);
		Time()->Start(aName_SlaveTerminate_WorkerTerminate);		
		
		if(Debug()) cout << "AnalysisChain: DEBUG Begin WorkerTerminate() execution on analysis " << (*iter)->Name() << endl;	
		(*iter)->WorkerTerminate();
		if(Debug()) cout << "AnalysisChain: DEBUG Finish WorkerTerminate() execution on analysis " << (*iter)->Name() << endl;
	
		Time()->Stop(aName_SlaveTerminate_WorkerTerminate);
		
		// worker timers
		cout << (*iter)->Name() << " timers" << endl;
		(*iter)->Time()->ShowStopped();	
	}

	Time()->Stop(aName_SlaveTerminate);

	//chain timers
	cout << Name() << " timers" << endl;
	Time()->ShowStopped();	
}

