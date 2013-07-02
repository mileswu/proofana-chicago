#include <iostream>
#include <fstream>
using namespace std;

void runCondor(TString dataset, TString identifier) {
	int numpernode = 25;

	TString dsetfilename(dataset);
	dsetfilename.Append(".txt");
	dsetfilename.Prepend("../filelists/");

	ifstream dsetfile(dsetfilename.Data());
	if(!dsetfile.is_open()) {
		cout << "Can't find file at: " << dsetfilename.Data() << endl;
		return;
	}

	int n_files = 0;
	int lastlinesize = 0;
	string line;
	while (getline(dsetfile, line)) {
		lastlinesize = line.size();
		n_files++;
	}
	if(lastlinesize < 2) n_files--; //hack to stop single new line at end of file
	dsetfile.close();

	int numnodes = n_files/numpernode;
	if(n_files%numpernode !=0) numnodes++;

	cout << "Number of files: " << n_files << endl;
	cout << "Number of nodes needed: " << numnodes << endl;

	//numnodes = 1;

	stringstream env_numpernode;
	env_numpernode << numpernode;
	stringstream env_numnodes;
	env_numnodes << numnodes;

	gSystem->Setenv("NUMPERNODE", env_numpernode.str().c_str());
	gSystem->Setenv("NUMNODES", env_numnodes.str().c_str());
	gSystem->Setenv("DATASET", dataset);
	gSystem->Setenv("IDENTIFIER", identifier);

	stringstream exec_tar, env_jobtar;
	stringstream uuid; uuid << TUUID().AsString();
	exec_tar << "tar czf ../condor/joboptions-" << uuid.str();
	exec_tar << ".tar.gz options.root ../filelists/" << dataset << ".txt";
	env_jobtar << "../condor/joboptions-" << uuid.str() << ".tar.gz";
	gSystem->Setenv("JOBTAR", env_jobtar.str().c_str());

	gSystem->Exec(exec_tar.str().c_str());
	gSystem->Exec("LD_LIBRARY_PATH=\"\" condor_submit ../scripts/proofAna.condor");
	//gSystem->Exec("LD_LIBRARY_PATH=\"\" condor_submit -addr \"<128.135.158.245:34130>\" -spool ../scripts/proofAna.condor");
	//gSystem->Exec("rm ../joboptions.tar.gz");

	gSystem->Unsetenv("NUMPERNODE");
	gSystem->Unsetenv("NUMNODES");
	gSystem->Unsetenv("DATASET");
	gSystem->Unsetenv("IDENTIFIER");
	gSystem->Unsetenv("JOBTAR");
}
