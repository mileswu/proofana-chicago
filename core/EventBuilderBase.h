/**************************************************************************
 **
 **   File:         EventBuilderBase.h
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

#ifndef EventBuilderBase_h
#define EventBuilderBase_h

#include <TSelector.h>
#include <TChain.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TBranchElement.h>
//#include <TBranchObject.h>
#include "Error.h"
#include <map>
#include <set>
#include <iostream>
#include "Key.h"

class AnalysisBase;
class SelectorBase;
class Config;

using namespace std;

class BranchContainer {
public :

	BranchContainer() : Type(0), Obj(0), Branch(0), Delete(0), Loaded(false) { }
	virtual		~BranchContainer() { if(Delete) Delete(Obj); }
	
	const type_info* Type;
	void* Obj;
	TBranch* Branch;
	void (*Delete)(void* ptr);
	bool Loaded;
	
};

class EventBuilderBase {
public :

	EventBuilderBase(TTree * /*tree*/ =0) { }
	virtual 			~EventBuilderBase() { }

	//access
	static TTree*		Tree() { return fBaseChain; } 
	static const set<BranchKey>& BranchNames() { return fBranchNames; }
	static void			SetInputList(TList* input) { fInput = input; }

	static Long64_t		Entry() { return fEntry; }
    static void			SetSelector(SelectorBase* selector) { fSelector = selector; }
    static SelectorBase*	Selector() { return fSelector; }
	static void			SetConfig(Config* config);
	
	static void     	InitBase(TTree* tree) { if (!tree) return; fBaseChain = tree; }
	static Bool_t		NotifyBase();
	static Bool_t  		LoadTree(Long64_t entry);

	virtual void     	Init(TTree* /*tree*/) { /* to be overridden by derived class if needed */ }
	virtual Bool_t   	Notify() {/* to be overridden by derived class if needed */ return kTRUE;}
    virtual Bool_t  	CopyEvent(AnalysisBase* /*evt */) {/* to be overridden by derived class */ return kTRUE;}
    virtual void		WriteAuxTrees(TDirectory* /*file */) {/* to be overridden by derived class, if needed */}
	virtual void		Initialize() {/* to be overridden by derived class, if needed */}

	//Dynamic load methods
	
	//Get a reference
	template <class T>
	static const T& Get(const BranchKey& key) {
	
		map<BranchKey,BranchContainer>::iterator branch = LoadBranch<T>(key);
		
		return *static_cast<T*>(branch->second.Obj);
	}	

	//Get a pointer	
	template <class T>
	static const T* GetP(const BranchKey& key) { 
	
		map<BranchKey,BranchContainer>::iterator branch = LoadBranch<T>(key);
		
		return static_cast<T*>(branch->second.Obj);
	}

	//Get a pointer, you own it and must delete it	
	template <class T>
	static T* GetOwnedP(const BranchKey& key) { 
	
		map<BranchKey,BranchContainer>::iterator branch = LoadBranch<T>(key);
		
		static set<BranchKey> copy;

		//Copy if flagged for copy
		if(copy.find(key)!=copy.end()) return new T(*static_cast<T*>(branch->second.Obj));

		//Set flag for detecting if copying would be better
		if(fCalledBefore.find(key)!=fCalledBefore.end()) copy.insert(key);
		else fCalledBefore.insert(key);
		
		//Remove ownerskip from EventBuilder
		void* obj = branch->second.Obj;
		branch->second.Obj = 0;
		branch->second.Delete = 0;		
		branch->second.Loaded = false;
		
		return static_cast<T*>(obj);
	}

	//Load a branch, but do not set branch address
	//Intended for use with classes which do the SetBranchAddress themselves and expect it to be loaded
	static void Load(const BranchKey& key) { 
	
		static const type_info* type = &typeid(void);

		if(!fBaseChain) Abort("EventBuilderBase: ERROR LoadBranch() cannot be called without a loaded tree");
	
		map<BranchKey,BranchContainer>::iterator branch = fBranches.find(key);
		
		//if loading branch for the first time
		if(branch==fBranches.end()) {
			if(fBranchNames.find(key)==fBranchNames.end()) {
				TString msg("EventBuilderBase: ERROR no branch named \"");
				msg.Append(key.Data());
				msg.Append("\" found in tree. Problem file is: ");
				msg.Append(Tree()->GetTree()->GetCurrentFile()->GetName());
				Abort(msg);
			}

			branch = fBranches.insert(map<BranchKey,BranchContainer>::value_type(key,BranchContainer())).first;

			branch->second.Branch = Tree()->GetBranch(key.Data());
			branch->second.Type = type; //Make sure no Get functions are called on this branch without an error
		}
		
		//type check (should be fast...)
		if(branch->second.Type != type) {
			TString msg("EventBuilderBase: ERROR branch previously requested using type ");
			msg.Append(branch->second.Type->name());
			msg.Append(", requested type is ");
			msg.Append(type->name());
			Abort(msg);
		}
		
		//if branch not loaded yet, load it
		if(!branch->second.Loaded) {
			branch->second.Branch->GetEntry(fEntry);
			branch->second.Loaded=true;
		}
		
		return;
	}
	
	static TList* fInput;
	
	ClassDef(EventBuilderBase,0);
	
private :

	static Config*	fConfig;
	static TTree* fBaseChain;
	static Long64_t fEntry;
	static SelectorBase* fSelector;
	
	static Bool_t fCache;
	
	static set<BranchKey> fBranchNames;
	static map<BranchKey,BranchContainer> fBranches;
	
	static set<BranchKey> fCalledBefore;
	
	template <class T>
	static map<BranchKey,BranchContainer>::iterator LoadBranch(const BranchKey& key) {
	
		static const type_info* type = &typeid(T*);

		if(!fBaseChain) Abort("EventBuilderBase: ERROR LoadBranch() cannot be called without a loaded tree");
	
		map<BranchKey,BranchContainer>::iterator branch = fBranches.find(key);
		
		//if loading branch for the first time
		if(branch==fBranches.end()) {
			if(fBranchNames.find(key)==fBranchNames.end()) {
				TString msg("EventBuilderBase: ERROR no branch named \"");
				msg.Append(key.Data());
				msg.Append("\" found in tree. Problem file is: ");
				msg.Append(Tree()->GetTree()->GetCurrentFile()->GetName());
				Abort(msg);
			}

			branch = fBranches.insert(map<BranchKey,BranchContainer>::value_type(key,BranchContainer())).first;

			Tree()->SetBranchAddress(key.Data(), &branch->second.Obj, &branch->second.Branch);

			if(branch->second.Branch->IsA() == TBranchElement::Class()) {
				cout << "hi in this tbranchelement" << endl;
				TClass* tclass = TClass::GetClass(((TBranchElement*)branch->second.Branch)->GetTypeName());
				if(!tclass) {
					TString msg("EventBuilderBase: ERROR no dictionary found for class ");
					msg.Append(tclass->GetName());
					Abort(msg);
				}
				branch->second.Type = type; //This doesn't necessarily mean the type is correct, but means that the type will be consistent
			}
			else {
				branch->second.Type = type; //This doesn't necessarily mean the type is correct, but means that the type will be consistent
				branch->second.Delete = &DeletePointer<T*>;
				int size = Tree()->GetBranch(key.Data())->GetLeaf(key.Data())->GetNdata();
				if(size == 1)
					branch->second.Obj = new T();
				else { // It is an array
					branch->second.Obj = new T[size]();
				}
				fBaseChain->SetBranchAddress(key.Data(), branch->second.Obj, &branch->second.Branch);
			}
		}
		
		//type check (should be fast...)
		if(branch->second.Type != type) {
			TString msg("EventBuilderBase: ERROR branch previously requested using type ");
			msg.Append(branch->second.Type->name());
			msg.Append(", requested type is ");
			msg.Append(type->name());
			Abort(msg);
		}
		
		//if branch not loaded yet, load it
		if(!branch->second.Loaded) {
			branch->second.Branch->GetEntry(fEntry);
			branch->second.Loaded=true;
		}
		
		return branch;
	}
	
	template <class T>
	static void DeletePointer(void* ptr) { delete static_cast<T*>(ptr); } 

};

#endif
