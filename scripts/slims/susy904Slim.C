#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <stdio>
#include <string>
#include <vector>


void susy904Slim(bool isMC = false){

  // Say Hello
  cout << "susy904Slim() has been called" << endl;

  TString argStr;
  cin >> argStr;
  cout << " Data File Retrieved = " << argStr << endl;
  vector<TString> fileList;

  while (argStr.Contains(",")){
    fileList.push_back(argStr(0,argStr.Index(",") ) );
    argStr = argStr(argStr.Index(",")+1,argStr.Sizeof());
  };
  if(argStr.Sizeof()!=0) fileList.push_back(argStr);
  
  //==================================
  // INFO
  //==================================
  
  // Open input files
  TChain* tree = new TChain("susy");

  for (int iFile=0; iFile<fileList.size(); ++iFile) {
    cout << "open " << fileList[iFile] << endl;
    tree->Add(fileList[iFile]);
  }

  //==================================
  // Branch activation
  //==================================
  cout << "Chain has " << tree->GetEntries() << " entries." << endl;
  tree->SetBranchStatus("*",0);
  
  //--------------------
  // Event info
  //--------------------
  tree->SetBranchStatus("RunNumber",1);
  tree->SetBranchStatus("EventNumber",1);
  tree->SetBranchStatus("lbn",1);
  tree->SetBranchStatus("bcid",1);
  tree->SetBranchStatus("timestamp",1);
  //tree->SetBranchStatus("Weight",1);
  

  if(isMC) {
     tree->SetBranchStatus("mc_*",1);
     tree->SetBranchStatus("mcevt_*",1);
  }



  //--------------------
  // Trigger info
  //--------------------

  //tree->SetBranchStatus("collcand_passTrigger",1);

  //Jet triggers
  tree->SetBranchStatus("EF_j*",1);
  tree->SetBranchStatus("EF_2j*",1);
  //tree->SetBranchStatus("EF_3j*",1);
  //tree->SetBranchStatus("EF_4j*",1);
  tree->SetBranchStatus("L1_J*",1);
  tree->SetBranchStatus("L1_2J*",1);
  tree->SetBranchStatus("L1_3J*",1);
  //tree->SetBranchStatus("L1_4J*",1);
  tree->SetBranchStatus("L2_j*",1);
  tree->SetBranchStatus("L2_2j*",1);
  //tree->SetBranchStatus("L2_3j*",1);
  //tree->SetBranchStatus("L2_4j*",1);
  //ETmiss triggers
  tree->SetBranchStatus("EF_xe*",1);
  tree->SetBranchStatus("L1_XE*",1);
  tree->SetBranchStatus("L2_xe*",1);
  //muon triggers
  tree->SetBranchStatus("L1_MU*",1);
  tree->SetBranchStatus("L2_mu*",1);
  tree->SetBranchStatus("EF_mu*",1);
  //electron triggers
  tree->SetBranchStatus("EF_e*",1);
  tree->SetBranchStatus("L1_EM*",1);
  tree->SetBranchStatus("L2_e*",1);
  


  //Minbias triggers
  //tree->SetBranchStatus("L1_MBTS_1",1);
  //tree->SetBranchStatus("L1_MBTS_1_1",1);



  //L1 Jet ROI
  tree->SetBranchStatus("trig_L1_jet_n",1);
  //tree->SetBranchStatus("trig_L1_jet_RoIWord",1);
  tree->SetBranchStatus("trig_L1_jet_thrNames",1);
  tree->SetBranchStatus("trig_L1_jet_thrValues",1);
  tree->SetBranchStatus("trig_L1_jet_eta",1);
  tree->SetBranchStatus("trig_L1_jet_phi",1);
  //tree->SetBranchStatus("trig_L1_jet_et4x4",1);
  //tree->SetBranchStatus("trig_L1_jet_et6x6",1);
  //tree->SetBranchStatus("trig_L1_jet_et8x8",1);


  tree->SetBranchStatus("trig_L2_jet_E",1);
  //tree->SetBranchStatus("trig_L2_jet_pt",1);
  tree->SetBranchStatus("trig_L2_jet_eta",1);
  tree->SetBranchStatus("trig_L2_jet_phi",1);
  //tree->SetBranchStatus("trig_L2_jet_ehad0",1);
  //tree->SetBranchStatus("trig_L2_jet_eem0",1);
  tree->SetBranchStatus("trig_L2_jet_RoIWord",1);


  tree->SetBranchStatus("trig_EF_jet_E",1);
  tree->SetBranchStatus("trig_EF_jet_phi",1);
  tree->SetBranchStatus("trig_EF_jet_eta",1);
  tree->SetBranchStatus("trig_EF_jet_pt",1);

  //tree->SetBranchStatus("trig_L1_esum_energyX",1);  
  //tree->SetBranchStatus("trig_L1_esum_energyY",1);
  //tree->SetBranchStatus("trig_L1_esum_energyT",1);
  //tree->SetBranchStatus("trig_L1_esum_RoIWord0",1);
  tree->SetBranchStatus("trig_L1_esum_thrNames",1);
 
  tree->SetBranchStatus("trig_L2_met_MEx",1);
  tree->SetBranchStatus("trig_L2_met_MEy",1);
  tree->SetBranchStatus("trig_L2_met_nameOfComponent",1);
  tree->SetBranchStatus("trig_L2_met_MExComponent",1);
  tree->SetBranchStatus("trig_L2_met_MEyComponent",1);

  tree->SetBranchStatus("trig_EF_met_MEx",1);
  tree->SetBranchStatus("trig_EF_met_MEy",1);
  tree->SetBranchStatus("trig_EF_met_sumEt",1);
  tree->SetBranchStatus("trig_EF_met_sumE",1);
  tree->SetBranchStatus("trig_EF_met_flag",1);
  tree->SetBranchStatus("trig_EF_met_nameOfComponent",1);
  tree->SetBranchStatus("trig_EF_met_MExComponent",1);
  tree->SetBranchStatus("trig_EF_met_MEyComponent",1);



  
  //--------------------
  // Vertex
  //--------------------
  tree->SetBranchStatus("vx_n",1);
  //tree->SetBranchStatus("vx_chi2",1);
  //tree->SetBranchStatus("vx_ndof",1);
  tree->SetBranchStatus("vx_type",1);
  tree->SetBranchStatus("vx_x",1);
  tree->SetBranchStatus("vx_y",1);
  tree->SetBranchStatus("vx_z",1);
  tree->SetBranchStatus("vx_nTracks",1);
  
  //--------------------
  // MET
  //--------------------

  //Keep all branches of MET with EMJES corrections
  //tree->SetBranchStatus("MET_EMJES*",1);
  //For now, all branches of MET Simplified
  tree->SetBranchStatus("MET_Simplified20_*",1);
  //tree->SetBranchStatus("MET_Simplified30*",1);
  /*
  tree->SetBranchStatus("MET_Base_etx",1);
  tree->SetBranchStatus("MET_Base_ety",1);
  tree->SetBranchStatus("MET_Base_et",1);
  tree->SetBranchStatus("MET_Base_sumet",1);
  tree->SetBranchStatus("MET_Base_phi",1);
  tree->SetBranchStatus("MET_Topo_etx",1);
  tree->SetBranchStatus("MET_Topo_ety",1);
  tree->SetBranchStatus("MET_Topo_et",1);
  tree->SetBranchStatus("MET_Topo_sumet",1);
  tree->SetBranchStatus("MET_Topo_phi",1);
  tree->SetBranchStatus("MET_RefFinal_etx",1);
  tree->SetBranchStatus("MET_RefFinal_ety",1);
  tree->SetBranchStatus("MET_RefFinal_et",1);
  tree->SetBranchStatus("MET_RefFinal_sumet",1);
  tree->SetBranchStatus("MET_RefFinal_phi",1);
  tree->SetBranchStatus("MET_LocHadTopo_etx",1);
  tree->SetBranchStatus("MET_LocHadTopo_ety",1);
  tree->SetBranchStatus("MET_LocHadTopo_et",1);
  tree->SetBranchStatus("MET_LocHadTopo_sumet",1);
  tree->SetBranchStatus("MET_LocHadTopo_phi",1);
  */

  //--------------------
  // Electrons
  //--------------------

  tree->SetBranchStatus("el_pt",1);
  tree->SetBranchStatus("el_eta",1);
  tree->SetBranchStatus("el_phi",1);
  tree->SetBranchStatus("el_cl_phi",1);
  tree->SetBranchStatus("el_cl_eta",1);
  tree->SetBranchStatus("el_cl_E",1);
  tree->SetBranchStatus("el_trackphi",1);
  tree->SetBranchStatus("el_tracketa",1);
  tree->SetBranchStatus("el_author",1);
  tree->SetBranchStatus("el_etas2",1);
  tree->SetBranchStatus("el_reta",1);
  tree->SetBranchStatus("el_weta2",1);
  tree->SetBranchStatus("el_trackpt",1);
  tree->SetBranchStatus("el_expectHitInBLayer",1);
  tree->SetBranchStatus("el_nBLHits",1);
  tree->SetBranchStatus("el_nPixHits",1);
  tree->SetBranchStatus("el_nSCTHits",1);
  tree->SetBranchStatus("el_isEM",1);
  tree->SetBranchStatus("el_medium",1);
  tree->SetBranchStatus("el_charge",1);
  tree->SetBranchStatus("el_Etcone20",1);
  //tree->SetBranchStatus("el_L1*",1);
  //tree->SetBranchStatus("el_L2*",1);
  //tree->SetBranchStatus("el_EF*",1);





  //--------------------
  // Muons
  //--------------------

  tree->SetBranchStatus("mu_staco_E",1);
  tree->SetBranchStatus("mu_staco_pt",1);
  tree->SetBranchStatus("mu_staco_eta",1);
  tree->SetBranchStatus("mu_staco_phi",1);
  tree->SetBranchStatus("mu_staco_charge",1);
  //tree->SetBranchStatus("mu_staco_trackfitchi2",1);
  tree->SetBranchStatus("mu_staco_matchchi2",1);
  tree->SetBranchStatus("mu_staco_etcone20",1);
  tree->SetBranchStatus("mu_staco_ptcone20",1);
  tree->SetBranchStatus("mu_staco_z0_exPV",1);
  tree->SetBranchStatus("mu_staco_isCombinedMuon",1);
  //tree->SetBranchStatus("mu_staco_tight",1);

  //tree->SetBranchStatus("mu_staco_L1*",1);
  //tree->SetBranchStatus("mu_staco_L2*",1);
  //tree->SetBranchStatus("mu_staco_EF*",1);

  tree->SetBranchStatus("mu_staco_me_qoverp_exPV",1);
  tree->SetBranchStatus("mu_staco_me_theta_exPV",1);
  tree->SetBranchStatus("mu_staco_id_qoverp_exPV",1);
  tree->SetBranchStatus("mu_staco_id_theta_exPV",1);

  tree->SetBranchStatus("mu_staco_isLowPtReconstructedMuon",1);
  tree->SetBranchStatus("mu_staco_expectBLayerHit",1);
  tree->SetBranchStatus("mu_staco_nBLHits",1);
  tree->SetBranchStatus("mu_staco_nPixHits",1);
  tree->SetBranchStatus("mu_staco_nSCTHits",1);
  tree->SetBranchStatus("mu_staco_nPixHoles",1);
  tree->SetBranchStatus("mu_staco_nSCTHoles",1);
  tree->SetBranchStatus("mu_staco_nPixelDeadSensors",1);
  tree->SetBranchStatus("mu_staco_nSCTDeadSensors",1);
  tree->SetBranchStatus("mu_staco_nTRTHits",1);
  tree->SetBranchStatus("mu_staco_nTRTOutliers",1);
  tree->SetBranchStatus("mu_staco_id_theta",1);

  //--------------------
  // Jets
  //--------------------
  Int_t nJetTypes = 1;
  TString jetTypes[] = {
    //"jet_AntiKt4H1Tower",
    "jet_AntiKt4TopoNewEM",
    //"jet_AntiKt4TopoEMJES"
  };
  /// LOOP
  for (Int_t i = 0; i < nJetTypes; i++) {
  
    /// Kinematics
    tree->SetBranchStatus(TString(jetTypes[i] + "_E"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_pt"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_eta"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_phi"),1);

    /// EM-Scale Kinematics
    tree->SetBranchStatus(TString(jetTypes[i] + "_emscale_E"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_emscale_pt"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_emscale_eta"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_emscale_phi"),1);
    
    /// Physics Kinematics
    tree->SetBranchStatus(TString(jetTypes[i] + "_Eta*"), 1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_Phi*"), 1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_M*"), 1);
    

    /// Jet Properties
    tree->SetBranchStatus(TString(jetTypes[i] + "_WIDTH"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_GCWJES"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_EMJES"),1);

    /// JVF
    //tree->SetBranchStatus(TString(jetTypes[i] + "_PRIM_VTX_F"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_jvtx*"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_sumPtTrk"),1);
    
    
       
    /// Jet Quality Information
    tree->SetBranchStatus(TString(jetTypes[i] + "_LArQuality"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_HECQuality"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_NegativeE"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_Offset"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_n90"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_emfrac"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_Timing"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_hecf"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_fracSamplingMax"),1);
    //tree->SetBranchStatus(TString(jetTypes[i] + "_isGood"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_isBadLoose"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_isBadTight"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_isUgly"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_N_BAD_CELLS"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_SamplingMax"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_BAD_CELLS_CORR_E"),1);
    

    /// b-tag info
    tree->SetBranchStatus(TString(jetTypes[i] + "_flavor_truth*"),1);

    tree->SetBranchStatus(TString(jetTypes[i] + "_flavor_weight_IP*"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_flavor_weight_JetProb*"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_flavor_weight_JetFitter*"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_flavor_weight_SV*"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_flavor_weight_SoftMuon*"),1);
    tree->SetBranchStatus(TString(jetTypes[i] + "_flavor_weight_SoftEle*"),1);
     

    
    
  }






   
  TFile *newfile = new TFile("Slim.root", "recreate");
  
  TTree *newtree = tree->CloneTree(); 
  newtree->Print(); 
  newtree->Write();
   
}


