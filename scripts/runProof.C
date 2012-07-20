// Macro to run analysis on PROOF.
#include <TEnv.h>
#include <TProof.h>
#include <TString.h>
#include <TDrawFeedback.h>
#include <TFileCollection.h>
#include "getProof.C"

TDrawFeedback *fb = 0;

void runProof(TString url, TString dataset, int nwrks = -1, TString treename = "")
{   
    TString sel("ProofAna"); // TSelector
    TString sharedlibrary("libProofAna"); // shared library name, par file must be name.par
    TString opt(""); // options string
  
    //Proof setup, performance histos/TTree
    //gEnv->SetValue("Proof.StatsHist",1);
    //gEnv->SetValue("Proof.StatsTrace",1);
    //gEnv->SetValue("Proof.SlaveStatsTrace",1);
    //gEnv->SetValue("Proof.DebugLevel",2);
    //gEnv->SetValue("Proof.DebugMask",8);

    // Temp dir
    TString tempdir = Form("%s/.proofanalysis", gSystem->TempDirectory());
    if (gSystem->AccessPathName(tempdir)) {
        Printf("runProof: creating the temporary directory"
            " (%s) ... ", tempdir.Data());
        if (gSystem->mkdir(tempdir, kTRUE) != 0) {
            Printf("runProof: could not assert / create the temporary directory"
                " (%s)", tempdir.Data());
            return;
        }
    }

    //Set PROOF version
    TProof::Mgr(url)->SetROOTVersion("default");
    //TProof::Mgr(url)->SetROOTVersion("v5-30-00");



    // Get the PROOF Session
    TProof *proof = getProof(url, nwrks, tempdir.Data(), "ask");
    if (!proof) {
        Printf("runProof: could not start/attach a PROOF session"); 
        return;
    }
    
    //Register dataset, if non-existent
    if(!proof->ExistsDataSet(dataset)) {
        TString dsetfile(dataset);
        dsetfile.Append(".txt");
        dsetfile.Prepend("../filelists/");
        TFileCollection *dset = new TFileCollection(dataset,"",dsetfile);
        cout << "Adding new dataset " << dataset << ", this may take a while for large datasets..." << endl;
        proof->RegisterDataSet(dataset,dset,"V"); //This seems to return true regardless of success or failure at the moment
        TFileCollection *fcs = proof->GetDataSet(dataset);
        if (fcs && fcs->GetStagedPercentage() < 10) proof->VerifyDataSet(dataset);
        fcs = proof->GetDataSet(dataset);
        if (fcs && fcs->GetStagedPercentage() < 10) proof->VerifyDataSet(dataset);
        fcs = proof->GetDataSet(dataset);
        if (fcs && fcs->GetStagedPercentage() < 10) cout << "!!! ERROR: There's a problem. Tried 3 times to verify the dataset and it's still not staged! f = " << fcs->GetStagedPercentage() << endl;
        if(treename.CompareTo("")!=0) proof->SetDataSetTreeName(dataset,treename);
    }
    TFileCollection *fcs = proof->GetDataSet(dataset);
    if (fcs && fcs->GetStagedPercentage() < 10) proof->VerifyDataSet(dataset);
    if (fcs && fcs->GetStagedPercentage() < 10) {
      cout << "!!! ERROR: There's still a problem. Tried 3 times to verify the dataset and it's still not staged! f = " << fcs->GetStagedPercentage() << endl;
      return;
    }
    if (treename.CompareTo("")!=0) proof->SetDataSetTreeName(dataset,treename);
    proof->ShowDataSets();
    
    TString proofsessions(Form("%s/sessions",tempdir.Data()));
    // Save tag of the used session
    FILE *fs = fopen(proofsessions.Data(), "a");
    if (!fs) {
        Printf("runProof: could not create files for sessions tags");
    } else {
        fprintf(fs,"session-%s\n", proof->GetSessionTag());
        fclose(fs);
    }
    if (!proof) {
        Printf("runProof: could not start/attach a PROOF session");
        return;
    }
  
    // Set the number of workers (may only reduce the number of active workers
    // in the session)
    if (nwrks > 0) proof->SetParallel(nwrks);
  
    // Where is the code to run
    char *rootbin = gSystem->Which(gSystem->Getenv("PATH"), "root", kExecutePermission);
    if (!rootbin) {
        Printf("runProof: root not found: please check the environment!");
        return;
    }
    TString rootsys(gSystem->DirName(rootbin));
    rootsys = gSystem->DirName(rootsys);
    delete[] rootbin;
  
    // Create feedback displayer
    if (!fb) {
        fb = new TDrawFeedback(proof);
    }
    if (!proof->GetFeedbackList() || !proof->GetFeedbackList()->FindObject("PROOF_EventsHist")) {
        // Number of events per worker
        proof->AddFeedback("PROOF_EventsHist");
    }
  
    // Have constant progress reporting based on estimated info
    proof->SetParameter("PROOF_RateEstimation", "average");
    
    //Enable sub-mergers (ROOT 5.30+)
    proof->SetParameter("PROOF_UseMergers",0);
    proof->SetParameter("PROOF_MergersByHost",1); //must be !=0 for enable
    //if(url.CompareTo("lite://")) proof->SetParameter("PROOF_ForceLocal", (Int_t)1); //Force local processing (stability?)
    
    //MC10b fix (maybe not necessary for us?)
    //proof->SetParameter( "PROOF_UseTreeCache", ( Int_t ) 0 );

    // Load the analysis shared library!
    TString par = Form("$PWD/../lib/%s.par",sharedlibrary.Data());
    gSystem->ExpandPathName(par);
    if (gSystem->AccessPathName(par.Data())) {
        Printf("runProof: par file not found (tried %s)", par.Data());
        return;
    }

    proof->UploadPackage(par.Data());
    proof->EnablePackage(sharedlibrary.Data());
    Printf("Enabled packages...\n");
    proof->ShowEnabledPackages(); 
  
    // Run it
    Printf("\nrunProof: running %s\n",sel.Data());
    proof->Process(dataset.Data(),sel.Data()); 
    
    //See what objects we are being sent
    //proof->GetOutputList()->ls();
    //gDirectory->ls();
    
    //gROOT->ProcessLine(".x $ROOTSYS/test/ProofBench/SavePerfInfo.C(\"perf.root\")");
}
