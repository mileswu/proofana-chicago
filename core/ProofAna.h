/**************************************************************************
 **
 **   File:         ProofAna.h
 **
 **   Description:  Top-level TSelector class to run requested analyses
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      2-24-11
 **   Modified:
 **
 **************************************************************************/

#ifndef ProofAna_h
#define ProofAna_h

#include <TChain.h>
#include <TSelector.h>
#include "Timer.h"
#include "SelectorBase.h"
#include "Key.h"
#include <set>

class Config;
class EventBuilderBase;
class AnalysisBase;
class TFile;
class TDirectory;
class TList;
class TProofOutputFile;

using namespace std;

class ProofAna : public TSelector, public SelectorBase {
public :
	TTree          		*fChain;   //!pointer to the analyzed TTree or TChain

	//Standard TSelector methods
	ProofAna(TTree* /*tree*/ =0) : fConfig(0), fEventBuilder(0), fTimer(new Timer()), fCurrentTree(0), fEntry(-1), fProcessed(0), fCounter(10000),
		fDebug(false), fSaveTimers(false), fDefaultFileInit(false), fNFiles(0) { } 
	virtual 			~ProofAna() { delete fTimer;}
	virtual Int_t   	Version() const { return 2; }
	virtual void    	Begin(TTree* tree);
	virtual void    	SlaveBegin(TTree* tree);
	virtual void    	Init(TTree* tree);
	virtual Bool_t  	Notify();
	virtual Bool_t  	Process(Long64_t entry);
	virtual Int_t   	GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
	virtual void    	SetOption(const char* option) { fOption = option; }
	virtual void    	SetObject(TObject* obj) { fObject = obj; }
	virtual void    	SetInputList(TList* input) { fInput = input; }
	virtual TList*  	GetOutputList() const { return fOutput; }
	virtual void    	SlaveTerminate();
	virtual void    	Terminate();

	ClassDef(ProofAna,0);
	
	//ProofAna methods
	Bool_t 				AddDir(TDirectory* source, TList* list); 
	Bool_t 				Debug() {return fDebug;}
	void				FillEventTree(AnaKey treename);
	void				FillRawTree(AnaKey treename);
	void				InitOutputFile(AnaKey name);
	void				ReplaceOutputFile(AnaKey name);
	TFile*				GetFile(AnaKey name);

private :

	map<AnaKey,AnalysisBase*>		fAnalyses; //!
	set<AnaKey>						fAnalysesNames; //!
	
	Config* 						fConfig;
	EventBuilderBase*				fEventBuilder;
	map<AnaKey,TProofOutputFile*>	fProofFiles; //!
	map<AnaKey,TFile*>				fFiles; //!
	Timer* 							fTimer;
	
	map<AnaKey,vector<AnaKey> >		fEventTrees; //! name of tree, list of analyses "participating"
	set<AnaKey>						fRawTrees; //!
	set<AnaKey>						fFillTrees; //!
	TTree*							fCurrentTree;
	
	Long64_t   						fEntry;
	Long64_t						fProcessed;
	Long64_t						fCounter;
	Bool_t							fDebug;
	Bool_t 							fSaveTimers;
	Bool_t							fDefaultFileInit;
	Long64_t						fNFiles;
};

#endif
