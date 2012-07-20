#include <fstream>

int LoadLibraries()
{
	if(!gSystem->Getenv("ROOTCOREDIR")) {
		if(!gSystem->AccessPathName("RootCoreEnv.txt")) gSystem->Unlink("RootCoreEnv.txt");
		gSystem->Exec("source ../utils/RootCore/scripts/setup.sh; echo ROOTCOREDIR=$ROOTCOREDIR >> RootCoreEnv.txt; echo ROOTCOREBIN=$ROOTCOREBIN >> RootCoreEnv.txt; echo PATH=$PATH >> RootCoreEnv.txt; echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH >> RootCoreEnv.txt; echo DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH >> RootCoreEnv.txt; echo PYTHONPATH=$PYTHONPATH >> RootCoreEnv.txt;");
	
		char buffer[2000];
		ifstream fh("RootCoreEnv.txt");
    	if (!fh.is_open()) {
      		cout << "LoadLibraries ERROR opening temporary RootCore environment variable dump RootCoreEnv.txt" << endl;
      		return 1;
    	}

 	   fh.getline(buffer,1995);
		while (!fh.eof()) {
			TString line(buffer);
			if(line.IsNull()) {
				fh.getline(buffer,1995);
				continue;
			}
			
			TObjArray* arr = line.Tokenize("=");
			if(arr->GetEntries()!=2) {
				cout << "LoadLibraries ERROR parsed line into !=2 entries" << endl;
				return 1;
			}
	
			gSystem->Setenv(((TObjString*)arr->At(0))->String(),((TObjString*)arr->At(1))->String().Data());
		
			delete arr;
			fh.getline(buffer,1995);	
		}
    	fh.close();
    	
    	gSystem->Unlink("RootCoreEnv.txt");
    }

	gSystem->CompileMacro("../utils/RootCore/scripts/load_packages.C","k");
	load_packages();
	
	if (gSystem->Load("../lib/libTriggerMenuNtuple") == -1) return -1;
	if (gSystem->Load("../lib/libMctLib") == -1) return -1;
	if (gSystem->Load("../lib/libFastjet") == -1) return -1;
	if (gSystem->Load("../lib/libFastjetPlugins") == -1) return -1;
	if (gSystem->Load("../lib/libqgTag") == -1) return -1;
	if (gSystem->Load("../lib/libProofAna") == -1) return -1;
	return 0;
}

void WriteObject(TFile* options, const TString& filename, const TString& objname, TString name = "")
{
	TFile* myfile = new TFile(filename,"READ");
	TNamed* obj = (TNamed*)myfile->Get(objname);
	if(obj) {
		if(name.CompareTo("")) obj->SetName(name);
		options->cd();
		obj->Write();
	}
	else cout << "Object " << objname << " not found in file " << filename << endl;
	myfile->Close();
	delete myfile;
}

void WriteJERObject(TFile* options)
{
	JERProvider* myJER = new JERProvider("AntiKt4TopoJES","Truth","../utils/JERProvider/share/JERProviderPlots.root");  
	myJER->init();
	myJER->SetName("myJER");
	options->cd();
	myJER->Write();
}

void WriteJESUncertaintyObject(TFile* options)
{
	JESUncertaintyProvider* myJES = new JESUncertaintyProvider("AntiKt4TopoJetsEM","../utils/JetUncertainties/share/JESUncertainty.root"); // Class used for JES uncertainties
	myJES->init();
	myJES->SetName("myJES");
	options->cd();
	myJES->Write();
}

void WriteMultijetJESUncertaintyObject(TFile* options)
{
	MultijetJESUncertaintyProvider* myMultijetJES = new MultijetJESUncertaintyProvider("AntiKt4TopoJetsEM","../utils/JetUncertainties/share/JESUncertainty.root","../utils/JetUncertainties/share/JESUncertainty.root"); // Class used for JES uncertainties
	myMultijetJES->includeFlavorComposition(false); 
	myMultijetJES->init();
	myMultijetJES->SetName("myMultijetJES");
	options->cd();
	myMultijetJES->Write();
}

void WriteSUSYObjDefObject(TFile* options, bool isData, bool isAF2)
{
	SUSYObjDef* myObj = new SUSYObjDef(); // Class used to do SUSY object selection
	myObj->initialize(isData, isAF2); //"../utils/SUSYTools/macros"); 
	myObj->SetName("mySUSYObjDef");
	options->cd();
	myObj->Write();
}

void WriteMuonTriggerSFToolObject(TFile* options)
{
	MuonTriggerSFTool* myMuonSF = new MuonTriggerSFTool(); 
	myMuonSF->initialize("../utils/SUSYTools/data/trigger/SF_for_EPS.root","../utils/SUSYTools/data/trigger/SF_barrel_ptdep.root"); 
	myMuonSF->SetMu18OR40(true);
	myMuonSF->SetVerbose(false);
	myMuonSF->SetName("myMuonTriggerSFTool");
	options->cd();
	myMuonSF->Write();
}
        
void WriteMuonTrigReweighterObjects(TFile* options)
{
    TFile* reweightfile = new TFile("../utils/SUSYTools/data/muon_triggermaps.root");
	options->cd();

	THnSparse* mu18_numerator_hist = (THnSparse*)reweightfile->Get("ths_mu18_nom");
	THnSparse* mu18_denominator_hist = (THnSparse*)reweightfile->Get("ths_mu18_den");
    APReweightND* myMu18Reweighter = new APReweightND(mu18_denominator_hist,mu18_numerator_hist,true);
	myMu18Reweighter->SetName("myMu18Reweighter");
	myMu18Reweighter->Write();
	
	THnSparse* mu18med_numerator_hist = (THnSparse*)reweightfile->Get("ths_mu18_medium_nom");
	THnSparse* mu18med_denominator_hist = (THnSparse*)reweightfile->Get("ths_mu18_medium_den");
    APReweightND* myMu18MedReweighter = new APReweightND(mu18med_denominator_hist,mu18med_numerator_hist,true);
	myMu18MedReweighter->SetName("myMu18MedReweighter");
	myMu18MedReweighter->Write();
	
    reweightfile->Close();
}        

void WriteBTagCalibObject(TFile* options, TString tagger = "SV0", TString OP = "5_85")
{

	Analysis::CalibrationDataInterfaceROOT* myObj = new Analysis::CalibrationDataInterfaceROOT(tagger.Data(),"../utils/SUSYTools/data/BTagCalibration.env","../utils/SUSYTools/data/"); // Class used for b-tagging scale factors
	myObj->initialize("AntiKt4Topo",OP.Data(),Analysis::Total);
	//myObj->initialize("AntiKt4Topo",OP.Data(),Analysis::Systematic);
	
	TString name("myBTagCalib");
	name.Append(tagger);
	name.Append(OP);
	
	myObj->SetName(name.Data());
	options->cd();
	myObj->Write();
}

void WriteGRLObject(TString xmlfile)
{
	///++++++++++++++++++++++++++++++++++++++++++++++++
	/// Good Run List reader
	///++++++++++++++++++++++++++++++++++++++++++++++++
	Root::TGoodRunsListReader reader;   
	reader.SetXMLFile(TString("../config/grl/").Append(xmlfile));
	reader.Interpret();
	Root::TGoodRunsList* m_GRL = new Root::TGoodRunsList(reader.GetMergedGoodRunsList());  
	m_GRL->SetName("myGRL");
	m_GRL->Write();
}

void Write0LeptonPileupReweightingObject(TFile* options)
{
  Root::TPileupReweighting* myPile = new Root::TPileupReweighting();
  myPile->AddConfigFile("../utils/PileupReweighting/share/mc11b_defaults.prw.root");
  myPile->AddLumiCalcFile("../utils/SUSYTools/data/susy_0Lepton_avgintperbx.root");
  myPile->SetUnrepresentedDataAction(2); //Action needs investigation
  myPile->Initialize();
  myPile->SetName("myPRW0Lepton");
  options->cd();
  myPile->Write();
}

void WriteDefaultQGPRWO(TFile* options, TString file){
  Root::TPileupReweighting* myPile = new Root::TPileupReweighting();
//  myPile->AddConfigFile("../config/lumicalcs_R17/gj.prw.root");
  myPile->AddConfigFile("../utils/PileupReweighting/share/mc11b_defaults.prw.root");
  myPile->AddLumiCalcFile("../config/lumicalcs_R17/"+file+".root");
  myPile->SetUnrepresentedDataAction(2); //Action needs investigation
  myPile->DisableWarnings(true);
  myPile->Initialize();
  myPile->SetName(file);
  options->cd();
  myPile->Write();

}

void WriteGammaQGPRWO(TFile* options, TString file){
  Root::TPileupReweighting* myPile = new Root::TPileupReweighting();
  myPile->AddConfigFile("../config/lumicalcs_R17/gj.prw.root");
  //myPile->AddConfigFile("../utils/PileupReweighting/share/mc11b_defaults.prw.root");
  myPile->AddLumiCalcFile("../config/lumicalcs_R17/"+file+".root");
  myPile->SetUnrepresentedDataAction(2); //Action needs investigation
  myPile->DisableWarnings(true);
  myPile->Initialize();
  myPile->SetName(file);
  options->cd();
  myPile->Write();
}

void WriteDijetQGPRWO(TFile* options, TString file){
  Root::TPileupReweighting* myPile = new Root::TPileupReweighting();
  //myPile->EnableDebugging(true);
  //cout << "Setting up prw object for " << file << ".root" << endl;
  myPile->AddConfigFile("../config/lumicalcs_R17/dj.prw.root");
  //myPile->AddConfigFile("../utils/PileupReweighting/share/mc11b_defaults.prw.root");
  myPile->AddLumiCalcFile("../config/lumicalcs_R17/"+file+".root");
  myPile->SetUnrepresentedDataAction(2); //Action needs investigation
  myPile->DisableWarnings(true);
  myPile->Initialize();
  myPile->SetName(file);
  options->cd();
  myPile->Write();
}

void WriteGroomedPRWO(TFile* options, TString file){
  Root::TPileupReweighting* myPile = new Root::TPileupReweighting();
  //myPile->EnableDebugging(true);
  //cout << "Setting up prw object for " << file << ".root" << endl;
  myPile->AddConfigFile("../config/groomedPRW/groomed.prw.root");
  myPile->AddLumiCalcFile("../config/groomedPRW/"+file+".root");
  myPile->SetUnrepresentedDataAction(2); //Action needs investigation
  myPile->DisableWarnings(true);
  myPile->Initialize();
  myPile->SetName(file);
  options->cd();
  myPile->Write();
}

void Write1ElectronPileupReweightingObject(TFile* options)
{
  Root::TPileupReweighting* myPile = new Root::TPileupReweighting();
  myPile->AddConfigFile("../utils/PileupReweighting/share/mc11b_defaults.prw.root");
  myPile->AddLumiCalcFile("../utils/SUSYTools/data/susy_1Electron_avgintperbx.root");
  myPile->SetUnrepresentedDataAction(2); //Action needs investigation
  myPile->Initialize();
  myPile->SetName("myPRW1Electron");
  options->cd();
  myPile->Write();
}

void Write1MuonPileupReweightingObject(TFile* options)
{
  Root::TPileupReweighting* myPile = new Root::TPileupReweighting();
  myPile->AddConfigFile("../utils/PileupReweighting/share/mc11b_defaults.prw.root");
  myPile->AddLumiCalcFile("../utils/SUSYTools/data/susy_1Muon_avgintperbx.root");
  myPile->SetUnrepresentedDataAction(2); //Action needs investigation
  myPile->Initialize();
  myPile->SetName("myPRW1Muon");
  options->cd();
  myPile->Write();
}

void WriteqgTagObject(TFile* options)
{
  qgTag* myqg = new qgTag();
  myqg->setInFile("../utils/qgTag/share/qgOut.root");
  options->cd();
  myqg->SetName("myqg");
  myqg->Write();
}

void WriteFakeMetEstimator(TFile* options)
{
  FakeMetEstimator* fakemet = new FakeMetEstimator();
  fakemet->initialize("../utils/SUSYTools/data/fest_periodF_v1.root");
  options->cd();
  fakemet->SetName("myFakeMetEstimator");
  fakemet->Write();
}

void WriteSmearingReweightHist(TFile* options)
{
   TFile file("../config/smearing/Reweight_pT.root","READ");
   TH1D* hist = (TH1D*)file.Get("hist1_Clone");
   hist->SetName("myReweightHist");
   options->cd();
   hist->Write();
   file.Close();
}

void WriteElectronMMObject(TFile* options)
{
	ElectronMatrixMethod* myElectronMM = new ElectronMatrixMethod();
	myElectronMM->initialize("../utils/FakeLeptBkg/Root/MM_electron.root");
	options->cd();
	myElectronMM->SetName("myElectronMM");
	myElectronMM->Write();
}

void ReadDatasetInfo(TString dataset, TObject* conf)
{
	TString filelist(dataset);
	filelist.Append(".txt").Prepend("../filelists/");

    char buffer[256];
    ifstream fi(filelist);
    if (!fi.is_open()) {
      cout << "helperFunc: ERROR opening dataset file list " << filelist << endl;
      return;
    }
    fi.getline(buffer,250);    
	TString firstfile(buffer);
	fi.close();
	
	TString filelistconfig("../filelists/");
	filelistconfig.Append(dataset).Append(".config");
	if(((Config*)conf)->ReadFile(filelistconfig)) return;
	
	cout << "If this sample is Monte Carlo, please create " << filelistconfig << " manually or use the makeFileListConfig.sh script to generate it automatically using AMI for proper event weighting." << endl;
}

