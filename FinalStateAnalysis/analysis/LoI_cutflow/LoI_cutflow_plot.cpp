

{


		TH1D* cutflow 		= new TH1D("cutflow", "cutflow; ;Events", 12, -0.5, 11.5) ;
		TH1D* cutflow_WW 	= new TH1D("cutflow_WW", "cutflow; ;Events", 12, -0.5, 11.5) ;
		TH1D* cutflow_ZZ    = new TH1D("cutflow_ZZ", "cutflow; ;Events", 12, -0.5, 11.5) ;


		double WW_bins[12] = { 14646, 9843, 9843, 9843, 9843, 9835, 9580, 6624, 6402, 6342, 6342, 6308 };
		double ZZ_bins[12] = { 6816, 2073, 2073, 2073, 2073, 2073, 2073, 2073, 2073, 2073, 2073 , 2073};

		for (int i=0; i<12; i++) {
			int bin = i+1;
			cutflow_WW->SetBinContent(bin, WW_bins[i]);
			cutflow_ZZ->SetBinContent(bin, ZZ_bins[i]);
			cutflow->SetBinContent(bin, WW_bins[i] + ZZ_bins[i]);
		}

	
		vector<string> cut_labels = {	"no cuts", "isolep", "min_chargedparticles_jet", "min_particles_jet", "min_Ejet", 
										"y_34", "miss_mass", "E_T_vis", "p_T_vis", "cosTheta_miss", "cosTheta_EmaxTrack", 
										"E_10cone_EmaxTrack" };

		TCanvas* c1 = new TCanvas("c_cutflow", "", 0, 0, 900, 950);

		cutflow->SetMinimum(0);

		TAxis* x_axis = cutflow->GetXaxis();
		for (int i = 0; i<cut_labels.size(); i++) {
			x_axis->SetBinLabel(x_axis->FindBin(i), cut_labels[i].c_str());
			x_axis->ChangeLabel(x_axis->FindBin(i), 0.8);
		}
		x_axis->SetLabelOffset(0.004);
		x_axis->SetLabelSize(0.03);

		cutflow->Draw("hist");

		cutflow_WW->SetLineColor(4);	
		cutflow_ZZ->SetLineColor(2);	


		TLegend* legend = new TLegend(0.7, 0.6, 0.9, 0.9);
		legend->AddEntry(cutflow, "total", "l"); 
		legend->AddEntry(cutflow_WW, "WW", "l"); 
		legend->AddEntry(cutflow_ZZ, "ZZ", "l"); 
		legend->Draw();

		cutflow_WW->Draw("hist same");
		cutflow_ZZ->Draw("hist same");

		c1->SetBottomMargin(0.111);
		c1->SetRightMargin(0.158);

		c1->Print("./LoI_cutflow.pdf");
};
