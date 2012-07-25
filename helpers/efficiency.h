void MakeEffPlot1D(TDirCache *output_dir, const char *reco_key, const char *truth_key, const char *eff_key)
{
	TH1* reco = dynamic_cast<TH1*>(output_dir->Get(reco_key));
	TH1* truth = dynamic_cast<TH1*>(output_dir->Get(truth_key));

	int xbins = reco->GetNbinsX();
	output_dir->cd();
	cout << xbins << endl;
	cout << reco->GetBinLowEdge(1) << endl;
	cout << reco->GetBinLowEdge(xbins) << endl;
	TH1F* eff = new TH1F(eff_key, eff_key, xbins, reco->GetBinLowEdge(1), reco->GetBinLowEdge(xbins));

	for(int i=0; i<xbins; i++) {
		float acceptance, err;
		if(truth->GetBinContent(i+1) == 0) {
			acceptance = 0;
			err = 0;
		}
		else {
			acceptance = reco->GetBinContent(i+1) / truth->GetBinContent(i+1);
			err = sqrt(acceptance * (1.0-acceptance) / truth->GetBinContent(i+1));
		}
		eff->SetBinContent(i+1, acceptance);
		eff->SetBinError(i+1, err);
	}
}
