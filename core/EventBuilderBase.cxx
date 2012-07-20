/**************************************************************************
 **
 **   File:         EventBuilderBase.cxx
 **
 **   Description:  EventBuilder base class
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      2-23-11
 **   Modified:
 **
 **************************************************************************/

#define EventBuilderBase_cxx

#include "EventBuilderBase.h"
#include "AnaConfig.h"


//static initialization
set<BranchKey> EventBuilderBase::fBranchNames;
map<BranchKey,BranchContainer> EventBuilderBase::fBranches;
Config* EventBuilderBase::fConfig = 0;
TList* EventBuilderBase::fInput = 0;
Long64_t EventBuilderBase::fEntry = -1;
SelectorBase* EventBuilderBase::fSelector = 0;
Bool_t EventBuilderBase::fCache = false;
TTree* EventBuilderBase::fBaseChain = 0;
set<BranchKey> EventBuilderBase::fCalledBefore;

void EventBuilderBase::SetConfig(Config* config)
{
	fConfig = config;
	fConfig->Get("CACHE",fCache); //Also set cache flag
}

Bool_t EventBuilderBase::NotifyBase()
{	
	if (!Tree()->GetTree()) return kTRUE;

	//Save branch names in this ntuple
	fBranchNames.clear();	
	TObjArray* branchnames = Tree()->GetListOfBranches();
	for(int i = 0; i<branchnames->GetEntriesFast(); ++i) {
		TObjString* str = (TObjString*)branchnames->At(i);
		fBranchNames.insert(BranchKey(str->String()));
	}
	
	//Check if branches still exist, delete the ones which do not, set addresses for those that do
	map<BranchKey,BranchContainer>::iterator branch = fBranches.begin();
	for(; branch!=fBranches.end();) {
		if(fBranchNames.find(branch->first.Data())==fBranchNames.end()) fBranches.erase(branch++);
		else if(!branch->second.Delete) {
			Tree()->SetBranchAddress(branch->first.Data(), &branch->second.Obj, &branch->second.Branch);
			++branch;
		}
		else {
			Tree()->SetBranchAddress(branch->first.Data(), branch->second.Obj, &branch->second.Branch);
			++branch;		
		}
	}	

	return kTRUE;
}

Bool_t EventBuilderBase::LoadTree(Long64_t entry)
{
	fEntry = entry;
	fCalledBefore.clear();

	map<BranchKey,BranchContainer>::iterator branch = fBranches.begin();
	if(fCache) {
		for(; branch!=fBranches.end(); ++branch) {
			branch->second.Branch->GetEntry(fEntry);			
			branch->second.Loaded=true;		
		}
	}
	else {
		for(; branch!=fBranches.end(); ++branch) branch->second.Loaded=false;	
	}
	
	return kTRUE;
}
