/**************************************************************************
 **
 **   File:         ProofAna.cxx
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

#define ProofAna_cxx

#include "ProofAna.h"
#include "AnalysisChain.h"
#include "EventBuilderBase.h"
#include "AnaConfig.h"
#include "TDirCache.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <TROOT.h>
#include <TProofOutputFile.h>
#include <TFile.h>
#include <TKey.h>
#include <TSystem.h>
#include <TFileCollection.h>
#include <TFileInfo.h>
#include <THashList.h>

void ProofAna::Begin(TTree * /*tree*/)
{
	// The Begin() function is called at the start of the query.
	// When running with PROOF Begin() is only called on the client.
	// The tree argument is deprecated (on PROOF 0 is passed).

	fTimer->Start("ProofAna");
	const AnaKey aProofAna_Begin("ProofAna:Begin");
	fTimer->Start(aProofAna_Begin);
  	
	//Check for presence of input list. Only can be missing in local running
	if(!fInput) Abort("ProofAna: ERROR No input TList for local running (instantiate one and pass it using ProofAna->SetInputList(TList*)");
  	
	//Need to load input list objects from a file here
	//Add all objects from a file to input list
	TFile* input = new TFile("options.root","READ");
	if (!input->IsZombie()) {
   		AddDir(input,fInput);
   		input->Close();
   	}
   	else Abort("ProofAna: ERROR Cannot read input file (options.root)");

  	delete input;
  	
  	fTimer->Stop(aProofAna_Begin); 

}

void ProofAna::SlaveBegin(TTree* tree)
{
	// The SlaveBegin() function is called after the Begin() function.
	// When running with PROOF SlaveBegin() is called on each slave server.
	// The tree argument is deprecated (on PROOF 0 is passed).

	const AnaKey aProofAna_SlaveBegin("ProofAna:SlaveBegin");
	fTimer->Start(aProofAna_SlaveBegin);

	const MomKey mANALYSIS("ANALYSIS");	
	const MomKey mEVENTBUILDER("EVENTBUILDER");	
	const MomKey mDEBUG("DEBUG");
	const MomKey mSAVETIMERS("SAVETIMERS");
	const MomKey mCOUNTER("COUNTER");

	// Find Config objects
	TObjLink* lnk = fInput->FirstLink();
	while (lnk) {
		if(lnk->GetObject()->InheritsFrom(Config::Class())) {
			// Find ProofAna config
			if(TString(((TNamed*)lnk->GetObject())->GetName()).CompareTo("ProofAna")==0)  {
				fConfig = (Config*)lnk->GetObject();
				
				TString eb_name(fConfig->String(mEVENTBUILDER));
				
				TClass* myEventBuilderClass = TClass::GetClass(TString::Format("EventBuilder_%s",eb_name.Data()).Data());
				if(myEventBuilderClass) fEventBuilder = (EventBuilderBase*)myEventBuilderClass->New();
				else {
					TString msg("ProofAna: ERROR EventBuilder type ");
					msg.Append(eb_name);
					msg.Append(" not found or has no dictionary");
					Abort(msg);
				}
				
				//EventBuilder initialize
				EventBuilderBase::SetSelector(this);
				EventBuilderBase::SetConfig(fConfig);
				EventBuilderBase::SetInputList(fInput);

				fEventBuilder->Initialize();
				
				fConfig->Get(mDEBUG,fDebug);
				fConfig->Get(mSAVETIMERS,fSaveTimers);
				fConfig->Get(mCOUNTER,fCounter);
			}
			else {
				Config* ana_config = (Config*)lnk->GetObject();
				
				AnalysisBase* analysis = 0;
				
				if(ana_config->Type(mANALYSIS)!=OBJ) { 
					TString ana_name(ana_config->String(mANALYSIS));
			
					TClass* myAnalysisClass = TClass::GetClass(TString::Format("Analysis_%s",ana_name.Data()).Data());
					if(myAnalysisClass) analysis = (AnalysisBase*)myAnalysisClass->New();
					else {
						TString msg("ProofAna: ERROR Analysis type ");
						msg.Append(ana_name);
						msg.Append(" not found or has no dictionary");
						Abort(msg);
					}
					
					analysis->SetConfig(ana_config);
				}
				else {
					analysis = new AnalysisChain();
					
					analysis->SetConfig(ana_config);
					
					//insert chain analysis names into name list to avoid conflicts
					vector<AnalysisBase*>::const_iterator iter = ((AnalysisChain*)analysis)->Analyses().begin();
					for(; iter!=((AnalysisChain*)analysis)->Analyses().end(); ++iter) {
						
						if(!(*iter)->Output()) continue;
						
						if(fAnalysesNames.find((*iter)->Name())!=fAnalysesNames.end()) {
							TString msg("ProofAna: ERROR More than one analysis with name ");
							msg.Append((*iter)->Name().Data());
							Abort(msg);
						}
						else fAnalysesNames.insert((*iter)->Name());
					}
				}

				analysis->SetSelector(this);
				analysis->SetInputList(fInput);

				if(fAnalyses.find(analysis->Name())!=fAnalyses.end()) {
					TString msg("ProofAna: ERROR More than one analysis with name ");
					msg.Append(analysis->Name().Data());
					Abort(msg);
				}
				fAnalyses[analysis->Name()] = analysis;
				
				if(analysis->Output()) {
					if(fAnalysesNames.find(analysis->Name())!=fAnalysesNames.end()) {
						TString msg("ProofAna: ERROR More than one analysis with name ");
						msg.Append(analysis->Name().Data());
						Abort(msg);
					}
					else fAnalysesNames.insert(analysis->Name());				
				}
			}
		}
		lnk = lnk->Next();
	}

	InitOutputFile("default");
	
	// timer histogram directory
	if(fSaveTimers) {
		TDirectory* tDir = fFiles["default"]->mkdir("Timers");
		fTimer->SetHistDir(tDir);	
	}
	
	const AnaKey aProofAna_SlaveBegin_("ProofAna:SlaveBegin:");
	const AnaKey a_SlaveBegin(":SlaveBegin");	

	// Propagate SlaveBegin
	map<AnaKey,AnalysisBase*>::iterator p = fAnalyses.begin();
	for(; p!=fAnalyses.end(); ++p) {
		//Need to set pointers to output file, EventBuilder, ProofAna config class
		p->second->SetEventBuilder(fEventBuilder);
		p->second->SetDatasetConfig(fConfig); //this includes dataset composition information for proper weighting

		const AnaKey aProofAna_SlaveBegin_Name_SlaveBegin(aProofAna_SlaveBegin_+p->second->Name()+a_SlaveBegin);
		fTimer->Start(aProofAna_SlaveBegin_Name_SlaveBegin);
		
		if(fDebug) cout << "ProofAna: DEBUG Begin SlaveBegin() execution on analysis " << p->second->Name() << endl;
		p->second->SlaveBegin(tree);
		if(fDebug) cout << "ProofAna: DEBUG Finish SlaveBegin() execution on analysis " << p->second->Name() << endl;
		
		fTimer->Stop(aProofAna_SlaveBegin_Name_SlaveBegin);
		
		//Get registered trees to set up in output files (must be done AFTER SlaveBegin is called)
		map<AnaKey,bool>::const_iterator t = p->second->Trees().begin();
		for(; t!=p->second->Trees().end(); ++t) {
			if(fAnalysesNames.find(t->first)!=fAnalysesNames.end()) {
				TString msg("ProofAna: ERROR Registered tree ");
				msg.Append(t->first.Data());
				msg.Append(" shares a name with an analysis");
				Abort(msg);						
			}

			if(t->second&&(fRawTrees.find(t->first)==fRawTrees.end())) fEventTrees[t->first].push_back(p->second->Name());
			else if((!t->second)&&(fEventTrees.find(t->first)==fEventTrees.end())) fRawTrees.insert(t->first);
			else {
				TString msg("ProofAna: ERROR Tree ");
				msg.Append(t->first.Data());
				msg.Append(" registered as both raw and Event-type in separate analyses");
				Abort(msg);
			}
		}
	}
	
	//Call again to make TDirectories for all analyses
	InitOutputFile("default");
	
	fTimer->Stop(aProofAna_SlaveBegin);
	
}

Bool_t ProofAna::Process(Long64_t entry)
{
	// The Process() function is called for each entry in the tree (or possibly
	// keyed object in the case of PROOF) to be processed. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	// It can be passed to either ProofAna:GetEntry() or TBranch::GetEntry()
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
	
	static const AnaKey aProofAna_Process("ProofAna:Process");
	fTimer->Start(aProofAna_Process);
	
	//Clear list of trees to be filled
	fFillTrees.clear();
	
	fEntry = entry;
	
	++fProcessed;
	if(fProcessed%fCounter==0 || fDebug) cout << "Processing Event #" << fProcessed << endl;

	static const AnaKey aProofAna_Process_LoadTree("ProofAna:Process:LoadTree");
	fTimer->Start(aProofAna_Process_LoadTree);
	
	if(fDebug) cout << "ProofAna: DEBUG Begin loading tree" << endl;	
	EventBuilderBase::LoadTree(entry);
	if(fDebug) cout << "ProofAna: DEBUG Finish loading tree" << endl;
	
	
	fTimer->Stop(aProofAna_Process_LoadTree);


	static const AnaKey aProofAna_Process_("ProofAna:Process:");
	static const AnaKey a_Process(":Process");

	bool result = true;
	map<AnaKey,AnalysisBase*>::iterator p = fAnalyses.begin();
	for(; p!=fAnalyses.end(); ++p) {
		const AnaKey aProofAna_Process_Name_Process(aProofAna_Process_+p->second->Name()+a_Process);
		fTimer->Start(aProofAna_Process_Name_Process);
		
		if(fDebug) cout << "ProofAna: DEBUG Begin Process() execution on analysis " << p->second->Name() << endl;
		if(!p->second->Process()) result = false;
		set<AnaKey>::iterator t = p->second->FillTrees().begin();
		for(; t!=p->second->FillTrees().end(); ++t) {
			if(fFillTrees.find(*t)==fFillTrees.end()) fFillTrees.insert(*t);
		}
		if(fDebug) cout << "ProofAna: DEBUG Finish Process() execution on analysis " << p->second->Name() << endl;
		
		fTimer->Stop(aProofAna_Process_Name_Process);
	}
	
	static const AnaKey aProofAna_Process_FillTrees("ProofAna:Process:FillTrees");	
	fTimer->Start(aProofAna_Process_FillTrees);
	
	set<AnaKey>::iterator t = fFillTrees.begin();
	for(; t!=fFillTrees.end(); ++t) {
		if(fEventTrees.find(*t)!=fEventTrees.end()) FillEventTree(*t);
		else if(fRawTrees.find(*t)!=fRawTrees.end()) FillRawTree(*t);
		else {
			TString msg("ProofAna: ERROR Fill request for unregistered tree ");
			msg.Append(t->Data());
			Abort(msg);		
		}
	}	
	
	fTimer->Stop(aProofAna_Process_FillTrees);	

	fTimer->Stop(aProofAna_Process);
	
	return result;
}

void ProofAna::SlaveTerminate()
{
	// The SlaveTerminate() function is called after all Analyses
	// have been processed. When running with PROOF SlaveTerminate() is called
	// on each slave server.

	const AnaKey aProofAna_SlaveTerminate("ProofAna:SlaveTerminate");		
	fTimer->Start(aProofAna_SlaveTerminate);

	const AnaKey aProofAna_SlaveTerminate_("ProofAna:SlaveTerminate:");
	const AnaKey a_SlaveTerminate(":SlaveTerminate");	

	//Propagate SlaveTerminate, delete analyses
	map<AnaKey,AnalysisBase*>::iterator p = fAnalyses.begin();
	for(; p!=fAnalyses.end(); ++p) {
		const AnaKey aProofAna_SlaveTerminate_Name_SlaveTerminate(aProofAna_SlaveTerminate_+p->second->Name()+a_SlaveTerminate);
		fTimer->Start(aProofAna_SlaveTerminate_Name_SlaveTerminate);
		
		if(fDebug) cout << "ProofAna: DEBUG Begin SlaveTerminate() execution on analysis " << p->second->Name() << endl;
		p->second->SlaveTerminate();
		if(fDebug) cout << "ProofAna: DEBUG Finish SlaveTerminate() execution on analysis " << p->second->Name() << endl;
		
		fTimer->Stop(aProofAna_SlaveTerminate_Name_SlaveTerminate);
		delete p->second;
	}	

	if(fDebug) cout << "ProofAna: DEBUG Begin writing output file(s)" << endl;
	// Write the output file(s)
	
	const AnaKey aProofAna_SlaveTerminate_aWriteOutputFiles("ProofAna:SlaveTerminate:WriteOutputFiles");
	const AnaKey aProofAna_SlaveTerminate_aCloseOutputFiles("ProofAna:SlaveTerminate:CloseOutputFiles");	

	fTimer->Start(aProofAna_SlaveTerminate_aWriteOutputFiles);	
	map<AnaKey,TFile*>::iterator file = fFiles.begin();
	for(;file!=fFiles.end();++file) {
		if (file->second) {
			file->second->cd();
			fProofFiles[file->first]->Print();
			fOutput->Add(fProofFiles[file->first]);
			file->second->Write();
			gROOT->GetListOfFiles()->Remove(file->second); //Assume no shared pointers between files/directories, safe bet, increases speed IMMENSELY
		}
	}
	fTimer->Stop(aProofAna_SlaveTerminate_aWriteOutputFiles);
	
	fTimer->Start(aProofAna_SlaveTerminate_aCloseOutputFiles);	
	for(file = fFiles.begin();file!=fFiles.end();++file) {
		if (file->second) delete file->second;
	}
	fTimer->Stop(aProofAna_SlaveTerminate_aCloseOutputFiles);
	
	fTimer->SetHistDir(0); //in case local running
	if(fDebug) cout << "ProofAna: DEBUG Finish writing output file(s)" << endl;
	
	delete fEventBuilder;
	
	fTimer->Stop(aProofAna_SlaveTerminate);

	// worker timers
	cout << "ProofAna worker timers" << endl;
	fTimer->ShowStopped();	

}

void ProofAna::Terminate()
{
	// The Terminate() function is the last function to be called during
	// a query. It always runs on the client, it can be used to present
	// the results graphically or save the results to file.

	const AnaKey aProofAna_Terminate("ProofAna:Terminate");	
	fTimer->Start(aProofAna_Terminate);

	//Write out TFileCollections
	TObjLink* lnk = fOutput->FirstLink();
	while (lnk) {
		if(lnk->GetObject()->InheritsFrom(TFileCollection::Class())) {
		
			TFileCollection* fileCol = (TFileCollection*)lnk->GetObject();

			fileCol->Print();
			
			//Opening output file list for writing
			TString filelist("../filelists/");
			filelist.Append(fileCol->GetName());
			filelist.Append(".txt");
			
    		ofstream afh(filelist.Data());
    		if (!afh.is_open()) {
      			cout << "ERROR opening output file list " << filelist << " for dataset " << fileCol->GetName() << endl;
      			continue;
    		}
    		else {
    			cout << "Writing file list " << filelist << " for dataset " << fileCol->GetName() << endl;
  				cout << "A config file, if needed, must be created manually." << endl;
  			}
			
			TObjLink* sublnk = fileCol->GetList()->FirstLink();
			while (sublnk) {
				TFileInfo* info = (TFileInfo*)sublnk->GetObject();
				afh << info->GetFirstUrl()->GetUrl() << endl;
				sublnk = sublnk->Next();
			}
			
			afh.close();

		}
		lnk = lnk->Next();
	}

	fTimer->Stop(aProofAna_Terminate);

	fTimer->Stop("ProofAna");
	
	// client timers
	cout << "ProofAna client timers" << endl;
	fTimer->ShowStopped();

}

void ProofAna::Init(TTree *tree)
{
	static const AnaKey aProofAna_Init("ProofAna:Init");
	fTimer->Start(aProofAna_Init);
	
	if(fDebug) cout << "ProofAna: DEBUG Start Init()" << endl;
	EventBuilderBase::InitBase(tree);	
	fEventBuilder->Init(tree);
	fCurrentTree = tree;
	
	if(!fDefaultFileInit) {
		//Call again to make output trees
		InitOutputFile("default");
		fDefaultFileInit = true;
	}
	else {
		bool doMerge = true;
		static const MomKey mMERGE("MERGE");
		fConfig->Get(mMERGE,doMerge);		

		Long64_t nfiles = 1;
		static const MomKey mNFILES("NFILES");
		if(!doMerge) fConfig->Get(mNFILES,nfiles);
	
		map<AnaKey,TFile*>::iterator f = fFiles.begin();
		for(; f!=fFiles.end(); ++f) {
			//If in dataset mode, replace files which are too big
			if(!doMerge) {
				if(fNFiles>=nfiles) {
					ReplaceOutputFile(f->first);
					fNFiles=0;
				}
			}
		}
	}
	
	++fNFiles;
	
	if(fDebug) cout << "ProofAna: DEBUG End Init()" << endl;
	fTimer->Stop(aProofAna_Init);
}

Bool_t ProofAna::Notify()
{
	//Protection against PROOF double-calling Notify, is this a bug or intended?
	static const AnaKey aProofAna_Notify("ProofAna:Notify");	
	if(fTimer->Running(aProofAna_Notify)) {
		while(fTimer->Running(aProofAna_Notify)) sleep(1);
		return true;
	}
	
	fTimer->Start(aProofAna_Notify);
	
	if(fDebug) cout << "ProofAna: DEBUG Start Notify()" << endl;
	bool result = EventBuilderBase::NotifyBase();
	result = result && fEventBuilder->Notify();
	
	fTimer->Stop(aProofAna_Notify);
	if(fDebug) cout << "ProofAna: DEBUG End Notify()" << endl;
	
	return result;
}

bool ProofAna::AddDir(TDirectory* source, TList* list) { 
	if(!source) return false;
	TDirectory *savdir = gROOT;
	//loop on all entries of this directory
	TKey *key;
	TIter nextkey(source->GetListOfKeys());
	while ((key = (TKey*)nextkey())) {
		const char *classname = key->GetClassName();
		TClass *cl = gROOT->GetClass(classname);
		if (!cl) continue;
		if (cl->InheritsFrom(TDirectory::Class())) {
			source->cd(key->GetName());
			TDirectory *subdir = gDirectory;
			savdir->cd();
			AddDir(subdir,list);
 			savdir->cd();
		} else if (cl->InheritsFrom(TTree::Class())) {
			//This is untested for it's purpose a la PROOF
			TTree *T = (TTree*)source->Get(key->GetName());
			savdir->cd();
			TTree *newT = T->CloneTree(-1,"fast");
			list->Add(newT);
		} else {
			source->cd();
			TObject *fileobj = key->ReadObj();
			TObject* memobj = savdir->CloneObject(fileobj);
			delete fileobj;
			list->Add(memobj);
		}
	}
	savdir->cd();
	return true;
}

void ProofAna::FillEventTree(AnaKey treename)
{
	vector<AnaKey>::iterator a = fEventTrees[treename].begin(); //should have unique values by construction

	//Get appropriate output file from the first analysis
	TDirectory* dir = fAnalyses[*a]->OutputDir()->GetFile();
	
	TTree* tree = (TTree*)dir->Get(treename.Data());

	map<AnaKey,Event*> evts; //remember, need to make pointers to pointers unique, as SetAddress takes the POINTER address
	if(!tree) {
		//Set up Event tree in output file
		dir->cd();
		tree = new TTree(treename.Data(),treename.Data());
	
		for(; a!=fEventTrees[treename].end(); ++a) {
			evts[*a] = new Event();
			evts[*a]->MoveEvent(*static_cast<Event*>(fAnalyses[*a])); //no deep copy, temporarily move data objects
			tree->Branch(a->Data(), &evts[*a]);
		}
	}
	else {
		for(; a!=fEventTrees[treename].end(); ++a) {
			evts[*a] = new Event();
			evts[*a]->MoveEvent(*static_cast<Event*>(fAnalyses[*a])); //no deep copy, temporarily move data objects
			tree->GetBranch(a->Data())->SetAddress(&evts[*a]);
		}
	}
	
	tree->Fill();
	
	//Move data back to analysis, a little awkward, but should be fast
	//We move and move back instead of copy, as copying requires an interface to the internal
	//structure of MomentObj to clear the internal map, and that is dangerous
	map<AnaKey,Event*>::iterator e = evts.begin();
	for(; e!=evts.end(); ++e) {
		fAnalyses[e->first]->MoveEvent(*e->second);
		delete e->second;
	}
}

void ProofAna::FillRawTree(AnaKey treename)
{
	//Get appropriate output file from the first analysis
	TDirectory* dir = fAnalyses.begin()->second->OutputDir()->GetFile();
	
	TTree* tree = (TTree*)dir->Get(treename.Data());
	if(!tree) {
		//Set up raw tree in output file	
		dir->cd();
		tree = fCurrentTree->CloneTree(0);
		tree->SetName(treename.Data());			
	}
	fEventBuilder->WriteAuxTrees(dir); //for trigger trees, metadata trees, etc. Leave to user to implement this in their EventBuilder class
	
	fCurrentTree->GetTree()->GetEntry(fEntry); //Have to reload every event, as unclear which branches have been loaded or not
	fCurrentTree->GetTree()->CopyAddresses(tree); //Have to copy addresses it every event, not sure which SetBranchAddress calls have been made since last fill
	
	tree->Fill();
}

TFile* ProofAna::GetFile(AnaKey name)
{
	if(fFiles.find(name)!=fFiles.end()) return fFiles[name];
	
	InitOutputFile(name);
	
	return fFiles[name];
}

void ProofAna::ReplaceOutputFile(AnaKey name)
{
	static const AnaKey adefault("default");
	static const MomKey mDATASET("DATASET");
	TString output(name.Data());
	if(name==adefault) output = fConfig->String(mDATASET);
	output.Append(filenameSuffix);

	//Close old file, add to output list, save name
	map<AnaKey,TFile*>::iterator tfile = fFiles.find(name);
	map<AnaKey,TProofOutputFile*>::iterator pfile = fProofFiles.find(name);
	if(tfile==fFiles.end()) Abort("ProofAna: ReplaceOutputFile called for nonexistent output file");

	if (tfile->second) {
		//remove object pointer caches for removed file
		map<AnaKey,AnalysisBase*>::iterator iter = fAnalyses.begin();
		for(; iter!=fAnalyses.end(); ++iter) iter->second->FlushCache(tfile->second);
		
		tfile->second->cd();
		pfile->second->Print();
		fOutput->Add(pfile->second);
		tfile->second->Write();
		gROOT->GetListOfFiles()->Remove(tfile->second); //Assume no shared pointers between files/directories, safe bet, increases speed IMMENSELY
		delete tfile->second;
	}
	
	TString prevfilename(pfile->second->GetFileName());
	
	static const MomKey mIDENTIFIER("IDENTIFIER");
	static const MomKey mOUTPUTPATH("OUTPUTPATH");
	
	//Make output file
	TString filename(fConfig->String(mIDENTIFIER)+"."+output+".root");
	TString outfilename(filename);

	//Make filename unique
	prevfilename.ReplaceAll(filename,"");
	prevfilename.ReplaceAll("-","");	
	if(prevfilename.IsNull()) filename.Prepend("0-");
	else filename.Prepend(TString::Format("%i-",prevfilename.Atoi()+1));
	
	TString dsname(fConfig->String(mIDENTIFIER)+"."+output);
	TProofOutputFile* proofFile = new TProofOutputFile(filename,TProofOutputFile::kDataset,TProofOutputFile::kCreate,dsname); 
	proofFile->SetOutputFileName(fConfig->String(mOUTPUTPATH)+outfilename);
	TFile* file = proofFile->OpenFile("RECREATE");
	if (file->IsZombie()) {
		TString msg("ProofAna: ERROR Problems opening output file: ");
		msg.Append(proofFile->GetDir());
		msg.Append("/");
		msg.Append(proofFile->GetFileName());
		Abort(msg);
	}
	pfile->second=proofFile;
	tfile->second=file;

	InitOutputFile(name);
}

void ProofAna::InitOutputFile(AnaKey name)
{
	static const AnaKey adefault("default");
	static const MomKey mDATASET("DATASET");
	TString output(name.Data());
	if(name==adefault) output = fConfig->String(mDATASET);
	output.Append(filenameSuffix);

	bool doMerge = true;
	static const MomKey mMERGE("MERGE");
	fConfig->Get(mMERGE,doMerge);
	
	TProofOutputFile::ERunType type = TProofOutputFile::kMerge;
	unsigned int opt = TProofOutputFile::kLocal;
	if(!doMerge) {
		type = TProofOutputFile::kDataset;
		opt =  TProofOutputFile::kCreate; //TProofOutputFile::kRegister | TProofOutputFile::kOverwrite | TProofOutputFile::kVerify;
	}

	static const MomKey mIDENTIFIER("IDENTIFIER");
	static const MomKey mOUTPUTPATH("OUTPUTPATH");

	if(fFiles.find(name)==fFiles.end()) {
		//Make output file
		TString filename(fConfig->String(mIDENTIFIER)+"."+output+".root");
		TString dsname(fConfig->String(mIDENTIFIER)+"."+output);
		TProofOutputFile* proofFile = new TProofOutputFile(filename,type,opt,dsname); 
		proofFile->SetOutputFileName(fConfig->String(mOUTPUTPATH)+filename);
		TFile* file = proofFile->OpenFile("RECREATE");
		if (file->IsZombie()) {
			TString msg("ProofAna: ERROR Problems opening output file: ");
			msg.Append(proofFile->GetDir());
			msg.Append("/");
			msg.Append(proofFile->GetFileName());
			Abort(msg);
		}
		fProofFiles[name]=proofFile;
		fFiles[name]=file;
	}

	set<AnaKey>::iterator p = fAnalysesNames.begin();
	for(; p!=fAnalysesNames.end(); ++p) {
		if(!fFiles[name]->Get((*p).Data())) fFiles[name]->mkdir((*p).Data());
	}

}
