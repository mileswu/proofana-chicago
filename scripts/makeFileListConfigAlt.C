// Macro to make file list config files for those datasets without AMI information (in particular, data period containers). 
// Cross sections and k-factors need to be added manually for MC after this script is run

#include <TString.h>
#include <fstream>
#include <iostream>

void makeFileListConfigAlt(TString filelist, TString treename)
{
	TString configfile(filelist);
	configfile.ReplaceAll(".txt",".config");
	
	//Opening config file for writing
    char abuffer[400];
    ofstream afh(configfile);
    if (!afh.is_open()) {
      cout << "ERROR opening config file " << configfile << endl;
      return;
    }
    
    afh << "TREENAME=" << treename << endl;

    //Change this when we get to run/channel 350000 (lol)
    TH1D* runnumbers = new TH1D("runnumbers","runnumbers",250000,100000.5,350000.5);  
    TH1D* runnumbers_temp = new TH1D("runnumbers_temp","runnumbers_temp",250000,100000.5,350000.5);  
	
	char buffer[400];
	ifstream fh(filelist);
    if (!fh.is_open()) {
      	cout << "ERROR opening filelist " << filelist << endl;
      	return;
    }
    fh.getline(buffer,395);
    TString filename(buffer);
	int filecounter = 0;
	while (!fh.eof()) {
		++filecounter;
		TFile file(buffer);
		TTree* tree = (TTree*)file.Get(treename);
		if(!tree) {
			cout << "Tree " << treename << " not found in file " << buffer <<", skipping file." << endl;
			fh.getline(buffer,395);
			continue;
		}
		
		if(!(filecounter%100)) {
			cout << "File #" << filecounter << endl;
			cout << buffer << endl;
		}		

		//cout << buffer << endl;
		
		gROOT->cd();

		runnumbers_temp->Reset();

		if(tree->FindBranch("mc_channel_number")) {
			if((treename.CompareTo("susy")==0)&&(filename.Contains("McAtNlo"))) tree->Draw("mc_channel_number>>runnumbers_temp","mcevt_weight[0][0]"); //R17 McAtNlo
			else tree->Draw("mc_channel_number>>runnumbers_temp"); //R17 MC
		}
		else {
			if((treename.CompareTo("susy")==0)&&(filename.Contains("McAtNlo"))) tree->Draw("RunNumber>>runnumbers_temp","mcevt_weight[0][0]"); //R16 McAtNlo		
			else tree->Draw("RunNumber>>runnumbers_temp");	 //R17 data and R16 data & MC
		}
		
		runnumbers->Add(runnumbers_temp);
		file.Close();
		fh.getline(buffer,395);
	}
    fh.close();
    
	TRegexp group("group[0-1][0-9].*");
	TRegexp mc("mc[0-1][0-9]_[0-9]TeV.*");
	TRegexp data("data[0-1][0-9]_[0-9]TeV.*");
	TString dsetname("unknown");
	int* len = new int(); 
	int indexgroup = group.Index(filename,len);
	int indexmc = mc.Index(filename,len);
	int indexdata = data.Index(filename,len);
	delete len;	
	
	if(indexgroup!=-1) {
		dsetname = TString(filename(indexgroup,filename.Length()));
		dsetname.Remove(dsetname.Index("/"));
	}
	else if(indexmc!=-1) {
		dsetname = TString(filename(indexmc,filename.Length()));
		dsetname.Remove(dsetname.Index("/"));
		cout << "This dataset is MC, you will need to add cross sections and k factors manually!" << endl;
	}
	else if(indexdata!=-1) {
		dsetname = TString(filename(indexdata,filename.Length()));
		dsetname.Remove(dsetname.Index("/"));
	}
    
	for(unsigned int i=1; i<runnumbers->GetNbinsX(); i++) {
		if(runnumbers->GetBinContent(i)!=0.) {
			int runnumber = (int)runnumbers->GetBinCenter(i);
			int entries = (int)runnumbers->GetBinContent(i);
			afh << "name_" << runnumber << "=" << dsetname << "/" << endl;
			afh << "n_" << runnumber << "=" << entries << endl;
			std::cout << "n_" << runnumber << "=" << entries << endl;
		}
	}
    
    afh.close();
}
