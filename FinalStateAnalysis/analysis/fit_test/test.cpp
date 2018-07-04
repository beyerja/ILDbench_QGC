Double_t fit(Double_t *coord, Double_t* par) {
	double x1 = coord[0];
	double x2 = coord[1];
	double E_prop_corr = ( par[1] + pow(par[3], 2) * x1 ) * x1;
	double N_prop_corr = ( par[2] + pow(par[4], 2) * x2 ) * x2 ;
	double E_N_corr = pow(par[5], 2) * x1 * x2;
	double offset = par[0];
	//cout << " " << " " <<   par[0] << " " <<  par[1] <<" " <<   par[2] << " " <<  par[3] << " " <<  x1 << " " << x2 << " " <<E_prop_corr<<" " << N_prop_corr<<" " << E_N_corr << " " <<offset << endl;
	return E_prop_corr + N_prop_corr + + E_N_corr + offset;
}


void test() 
{
	TFile* file = TFile::Open("Fit.root");

	TH3D* hist = (TH3D*)file->Get("Ejet_tos_VS_Ejet_s_N_chhad");
	//hist->GetFunction("f3")->SetBit(TF1::kNotDraw);

	const Int_t N_par = 6;

	Double_t pars[N_par] = {1, 1.2, -1, 0, 0, 0};
	TF3 *f3 = new TF3("f3",fit,0,400,-0.5,16.5,0,400, N_par);
	f3->SetParameters(pars);
	f3->SetParLimits(0, 1, 1.5);
	f3->SetParLimits(1, 1, 1.5);
	f3->SetParLimits(2, -1.1, -0.9);

	ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");

//	hist->Draw();

	//hist->Fit("f3", "WQN");
	hist->Fit("f3", "WV+");


/*f3->SetMarkerColor(2);
f3->SetLineColor(2);
hist->Draw("box");
f3->Draw("box same");*/

		TProfile2D* distr = hist->Project3DProfile("yx");
		distr->SetLineColor(1);

		TH2D* fit_2D = new TH2D("fit_proj", "", 150, 0, 600, 21, -0.5, 20.5);
		Double_t* result_pars = f3->GetParameters();

		int N_xbins = fit_2D->GetNbinsX();
		int N_ybins = fit_2D->GetNbinsY();

		Double_t x[2];
		for (int xbin=1; xbin<N_xbins; xbin++) {
			for (int ybin=1; ybin<N_ybins; ybin++) {
				x[0] = fit_2D->GetXaxis()->GetBinCenter(xbin);
				x[1] = fit_2D->GetYaxis()->GetBinCenter(ybin);

				//cout << " bin " << x[0] << " " << x[1] << " ";
				double _z = fit( x, result_pars );
				if ( distr->GetBinContent(xbin, ybin) !=0 ) {
					fit_2D->SetBinContent(xbin, ybin, _z);
				} else {
					fit_2D->SetBinContent(xbin, ybin, 0);
				}
			}
		}

		fit_2D->SetLineColor(2);
		fit_2D->Rebin2D(5,1);
		distr->Rebin2D(5,1);
		distr->Draw("box");
		fit_2D->Draw("box same");

}
