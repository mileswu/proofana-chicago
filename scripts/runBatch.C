#include <iostream>
#include <fstream>
using namespace std;

void runBatch(TString dataset, TString tree, TString system) {
	int numpernode = 1;

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

	ofstream batchoptions("options.batch");
	batchoptions << dataset.Data() << endl;
	batchoptions << tree.Data() << endl;
	batchoptions << numpernode << endl;

	if(system.CompareTo("pbs") ==0) {
		stringstream ss;
		ss << "qsub -j oe -o log -l walltime=7:59:59 -t 0-" << numnodes-1 << " ../scripts/runBatchRuntime.sh";
		gSystem->Exec(ss.str().c_str());
	}
	else if(system.CompareTo("lsf") == 0) {
		stringstream ss;
		ss << "bsub -o log/output.%J.%I -e log/error.%J.%I -J \"proofana[0-" << numnodes-1 << "\" ../scripts/runBatchRuntime.sh";
		gSystem->Exec(ss.str().c_str());
	}

}
