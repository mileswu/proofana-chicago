//Check file lists for entries with the same event number

#include <TString.h>
#include <fstream>
#include <iostream>
#include <set>
#include <map>

void checkDuplicateEvents(TString filelist, TString treename = "susy")
{
    vector<TString> files;
    //Run number/dataset number, event number, file index
    map<int, map<int, int> > evts;	
	
    char buffer[400];
    ifstream fh(filelist);
    if (!fh.is_open()) {
      	cout << "ERROR opening filelist " << filelist << endl;
      	return;
    }
    cout << "Begin check..." << endl;

    fh.getline(buffer,395);
    TString filename(buffer);
	int filecounter = 0;
	while (!fh.eof()) {
		++filecounter;
		TFile file(buffer);
		files.push_back(buffer);

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
		
		int runnumber;
		TBranch* runbranch = tree->GetBranch("mc_channel_number");
		if(!runbranch) runbranch = tree->GetBranch("RunNumber");
		if(!runbranch) {
			cout << "Neither mc_channel_number or RunNumber branches found...are you using the right tree?" << endl;
			exit(1);
		}
		
		runbranch->SetAddress(&runnumber);		

		int evtnumber;
		TBranch* evtbranch = tree->GetBranch("EventNumber");
		if(!evtbranch) {
			cout << "EventNumber branch not found...are you using the right tree?" << endl;
			exit(1);
		}
		
		evtbranch->SetAddress(&evtnumber);		

		for(Long64_t entry = 0; entry<tree->GetEntries(); entry++) {
			runbranch->GetEntry(entry);
			evtbranch->GetEntry(entry);

			map<int, int>* ptr = &evts[runnumber];
			map<int, int>::iterator iter = ptr->lower_bound(evtnumber);
			if(iter != ptr->end() && (evtnumber==iter->first)) {
				cout << "Duplicate event number " << evtnumber << " in run/dataset number " << runnumber << endl;
				cout << "First copy found in file " << files.at(iter->second) << endl;
				cout << "Second copy found in file " << buffer << endl;
			}
			
			else ptr->insert(iter,map<int,int>::value_type(evtnumber,filecounter-1));
		}

		file.Close();
		fh.getline(buffer,395);
	}
	fh.close();

	cout << "done" << endl;
}
