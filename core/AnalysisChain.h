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

#ifndef AnalysisChain_h
#define AnalysisChain_h

#include "AnalysisBase.h"

class AnalysisChain : public AnalysisBase {
public :

	AnalysisChain() { }
	virtual 			~AnalysisChain();
	void    			SlaveBegin(TTree* tree);
	Bool_t  			Process();
	void    			SlaveTerminate();
	
	//Set inputs
	void    			SetInputList(TList* input);
	void    			SetConfig(Config* config);
	void    			SetDatasetConfig(Config* config);
	void    			SetEventBuilder(EventBuilderBase* eb);
	void    			SetOutputDir(TDirectory* dir);
    void				SetSelector(SelectorBase* selector);
    
	//Reset object cache
	void				FlushCache(TDirectory* file);
    
    const vector<AnalysisBase*>& Analyses() { return fAnalyses; }

	ClassDef(AnalysisChain,0);

private :

	vector<AnalysisBase*> fAnalyses;

};

#endif

