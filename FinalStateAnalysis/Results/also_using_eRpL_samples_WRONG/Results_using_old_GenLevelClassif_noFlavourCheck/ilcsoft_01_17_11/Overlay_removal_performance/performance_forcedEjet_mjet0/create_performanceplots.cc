/* We use FastJet to remove event overlay.
 To do this, FastJet is called in processor to cluster jets with the 
 kt_algorithm with a specific cone parameter and the Exclusive Njet
 setting given a specific number of jets to cluster. 
 From that clustering we retrieve the PFOs that ended up in the jets
 and assume them to be more overlay-free.
 Further Jet clusterig and analysis is then done on those PFOs.

 This script takes the results of Marlin runs using different cone
 parameters and number of clustered jets (for the overlay removal)
 and creates plots that allow a performance analysis.

 The plots that are created are of statistical nature and have to 
 be interpreted on a event-to-event basis.
 On the x-axis the generator level value of the observable is seen.
 The y-axis shows the ratio of the value on reconstructed level to
 the value on generator level. The plot is filled event per event 
 and the y-value represents the statistical mean and standard 
 deviation of all event whose generator level value land in this
 x-bin.

 Jakob Beyer, DESY-FLC 11'2017
 */

std::string cone_to_string(float cone) {
	if ( cone == 1 ) {
		int int_cone = 1;
		return to_string(int_cone);
	}
	std::string cone_str = std::to_string(cone);
	cone_str.erase( cone_str.find_last_not_of('0') + 1, std::string::npos );
	return cone_str;	
}

int create_performanceplots() {
	gROOT->Reset();
	gStyle->SetOptStat(0);
	TH2::SetDefaultSumw2();


	// LATER: Loot over all forcedNjet and compare
	std::vector<std::string> filenames;
	filenames.push_back("6f_vvww_h_eL.pR.root");
	filenames.push_back("6f_vvww_h_eR.pL.root");
	filenames.push_back("6f_xxxx_nunu_eL.pR.root");
	filenames.push_back("6f_xxxx_nunu_eR.pL.root");
	filenames.push_back("6f_yyyy_nunu_eL.pR.root");
	filenames.push_back("6f_yyyy_nunu_eR.pL.root"); 


	// Weighting of the samples
	// Important: put in correct run polarizations and sample cross-sections! 
	// Luminosity is in this case a global factor, not important
	float luminosity = 1.0;
	std::vector<float> xsection = 		{50.5806,	0.269859,	1.63413,	0.0449741,	5.39289,	0.104988};
	std::vector<float> sample_e_pol =	{-1,		1,	 		-1,	    	1,			-1,			1}; 
	std::vector<float> sample_p_pol =	{1,			-1,	 		1,	    	-1,			1,			-1}; 
	float e_pol_percentage = -0.8;
	float p_pol_percentage = 0.3;


	// FastJet parameters relevant for overlay removal
	std::vector<float> cone_parameters = {0.1, 0.5, 1, 1.3, 1.5};
	std::vector<int> forced_jettopology = {2, 3, 4, 5, 6};


	// 2D Profile plots to somehow illustrate the performance in the 2D parameter space. 
	// They again show the (mean) event-wise results.
	TProfile2D* h2_miss_mass 	= new TProfile2D("miss_mass_2D", "variable: missing mass; cone parameter; forced N_{jet}; -|1-reco/MC|", 
											5, -0.15, 1.85, 4, 2.5, 6.5);
	TProfile2D* h2_sum_E_q 	= new TProfile2D("sum_E_q_2D", "variable: sum of q(jet) energies; cone parameter; forced N_{jet}; -|1-reco/MC|", 
											5, -0.15, 1.85, 4, 2.5, 6.5);
	TProfile2D* h2_V_mass = new TProfile2D("V_mass_2D", "variable: dijet (V) mass; cone parameter; forced N_{jet}; -|1-reco/MC|", 
											5, -0.15, 1.85, 4, 2.5, 6.5);


	// List of histograms, one list per observable, holds histograms for each parameter pair
	// List is filled like: fixed_cone_1(Njet_1 Njet_2 ... Njet_n) fixed_cone_2(Njet_1 ....
	// -> histogram of (cone_i,Njet_j) is at index i*n + j   
	TList* miss_mass_list = new TList();
	TList* sum_E_q_list = new TList();
	TList* V_mass_list = new TList();



	// Loop over all parameter possibilities 
	for (int k=0; k<cone_parameters.size(); k++) {
		float cone = cone_parameters[k];

		for (int j=0; j<forced_jettopology.size(); j++) {
			int forcedN = forced_jettopology[j];

			std::string file_dir = "/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/testFastJetOverlayRemoval_output/cone_parameter_" + cone_to_string(cone) + "/rootfiles_force" + std::to_string(forcedN) + "jets/";
		
	
			// TProfile is 1D plot that plots statistics of values that are filled for each bin		
			TProfile* h1_miss_mass = new TProfile(("miss_mass_" + cone_to_string(cone) + "_" + std::to_string(forcedN)).c_str(),
												"missing mass; m_{miss,GenLevel} [GeV]; value reco/gen", 50, 0, 1000);
			TProfile* h1_sum_E_q = new TProfile(("sum_E_q_" + cone_to_string(cone) + "_" + std::to_string(forcedN)).c_str(), 
											"sum quark/jet energies; Sum E_{q} [GeV]; value reco/gen", 50, 0, 1000);
			TProfile* h1_V_mass = new TProfile(	("V_mass_" + cone_to_string(cone) + "_" + std::to_string(forcedN)).c_str(), 
											"V boson mass; m_{V,GenLevel} [GeV]; value reco/gen", 70, 50, 120);
		
		
			// Comparison works the following way:
			// 	For each variable: Look at event, take ratio between reco and truth of variable,
			// 	and fill into histogram that calculates the statistics of that ratio for each truth bin.
			// 	-> Info about how well each true value gets reconstructed 
	
			// Loop over all files of the parameter setting	
			for (int i = 0; i<filenames.size(); i++) {
				// Read TTree
				TFile *file = TFile::Open((file_dir + filenames[i]).c_str());
				TTree *tree = (TTree*)file->Get("datatrain");
	
				int nEntries = tree->GetEntries();
	
				// Calculate weight for sample file 
				float pol_weight = ( 1 + sample_e_pol[i] * e_pol_percentage )/2 * ( 1 + sample_p_pol[i] * p_pol_percentage )/2;
				float weight = ( xsection[i] * luminosity / nEntries ) * pol_weight; 
		
				// Declare variables that are read from the tree
				int eventType;
				int partner_of_truthV1, partner_of_truthV2;
				float true_miss_mass, 	true_sum_E_q, 	true_pair1_mass,	true_pair2_mass; 
				float miss_mass, 		sum_E_q,		pair1_mass,			pair2_mass;	
		
				// Couple variables to the tree branch addresses
				tree->SetBranchAddress("true_evt_type", &eventType);

				tree->SetBranchAddress("partner_of_truthV1", &partner_of_truthV1);
				tree->SetBranchAddress("partner_of_truthV2", &partner_of_truthV2);
		
				tree->SetBranchAddress("true_miss_mass", &true_miss_mass);
				tree->SetBranchAddress("true_sum_E_q", &true_sum_E_q);
				tree->SetBranchAddress("true_pair1_mass", &true_pair1_mass);
				tree->SetBranchAddress("true_pair2_mass", &true_pair2_mass);
		
				tree->SetBranchAddress("miss_mass", &miss_mass);
				tree->SetBranchAddress("sum_E_q", &sum_E_q);
				tree->SetBranchAddress("pair1_mass", &pair1_mass);
				tree->SetBranchAddress("pair2_mass", &pair2_mass);
	

				// Fill plots for each event 
				for (int j = 0; j<nEntries; j++) {
					tree->GetEntry(j);

					if ( ! ( (eventType == 1) || (eventType == 2) ) ) {continue;}

					h2_miss_mass->Fill(cone, forcedN, -1.0*fabs(1-miss_mass/true_miss_mass), weight);			
					h2_sum_E_q->Fill(cone, forcedN, -1.0*fabs(1-sum_E_q/true_sum_E_q), weight);			

					h1_miss_mass->Fill(true_miss_mass, miss_mass/true_miss_mass, weight);			
					h1_sum_E_q->Fill(true_sum_E_q, sum_E_q/true_sum_E_q, weight);			
		
					// Compare the true and reconstructed V's
					if ( partner_of_truthV1 == 1 && partner_of_truthV2 == 2 ) {
						h2_V_mass->Fill(cone, forcedN, -1.0*fabs(1-pair1_mass/true_pair1_mass), weight);
						h2_V_mass->Fill(cone, forcedN, -1.0*fabs(1-pair2_mass/true_pair2_mass), weight);

						h1_V_mass->Fill(true_pair1_mass, pair1_mass/true_pair1_mass, weight);
						h1_V_mass->Fill(true_pair2_mass, pair2_mass/true_pair2_mass, weight);
					}
					else if ( partner_of_truthV1 == 2 && partner_of_truthV2 == 1 ) {
						h2_V_mass->Fill(cone, forcedN, -1.0*fabs(1-pair2_mass/true_pair1_mass), weight);
						h2_V_mass->Fill(cone, forcedN, -1.0*fabs(1-pair1_mass/true_pair2_mass), weight);

						h1_V_mass->Fill(true_pair1_mass, pair2_mass/true_pair1_mass, weight);
						h1_V_mass->Fill(true_pair2_mass, pair1_mass/true_pair2_mass, weight);
					}
					else { // This occurs if generator level V and reconstructed V were not paired correctly (shouldn't happen xP) 
						cout << "ERROR: Something wrong with V links" << endl 
							<< "Partners are: " << partner_of_truthV1 << partner_of_truthV1 << endl; 
						return 0; }
				}
	
			}
		
			// Add the plot for the parameter specific file to the corresponding TList
			miss_mass_list->Add(h1_miss_mass);
			sum_E_q_list->Add(h1_sum_E_q);
			V_mass_list->Add(h1_V_mass);

		}
	}



	// Define a good looking RGB color palette
	std::vector<float[3]> colors = {{228,26,28}, {55,126,184}, {77,175,74}, {152,78,163}, {255,127,0}};
	for (int j=0; j<colors.size(); j++) {
		TColor *h1_color = new TColor(9000+j, colors[j][0]/256.0, colors[j][1]/256.0, colors[j][2]/256.0);
	}


	// -> histogram of (cone_i,Njet_j) is at index i*n + j   
	

	// Following is pretty much copy-paste for each variable 
	// (not automated to keep myself from using too many variables and loosing overview)

	int n_cones = cone_parameters.size();
	int n_Njets = forced_jettopology.size();

	for (int k=0; k<n_cones; k++) {
		float cone = cone_parameters[k];

		string output_dir = "output_cone_fixed/cone" + cone_to_string(cone) + "_";
	
		string plot_name_h1_miss_mass = output_dir + "miss_mass.pdf";
		string plot_name_h1_sum_E_q = output_dir + "sum_E_q.pdf";
		string plot_name_h1_V_mass = output_dir + "V_mass.pdf";
	
		TLegend* leg_miss_mass = new TLegend(0.8, 0.6, .9, 0.9);
		TLegend* leg_sum_E_q = new TLegend(0.8, 0.2, .9, 0.5);
		TLegend* leg_V_mass = new TLegend(0.8, 0.6, .9, 0.9);
	
		TLine *line_miss_mass = new TLine(0,1.0,1000,1.0);	
		TLine *line_sum_E_q = new TLine(0,1.0,1000,1.0);	
		TLine *line_V_mass = new TLine(50,1.0,120,1.0);	
	
		TCanvas *canvas_h1_miss_mass = new TCanvas("can_miss_mass", "", 0, 0, 600, 600);
		TCanvas *canvas_h1_sum_E_q = new TCanvas("can_sum_E_q", "", 0, 0, 600, 600);
		TCanvas *canvas_h1_V_mass = new TCanvas("can_V_mass", "", 0, 0, 600, 600);

		for (int j=0; j<n_Njets; j++) {
			int plot_index = k*n_Njets + j;

			TH1* h1_miss_mass = (TH1D*)miss_mass_list->At(plot_index);
			TH1* h1_sum_E_q = (TH1D*)sum_E_q_list->At(plot_index);
			TH1* h1_V_mass = (TH1D*)V_mass_list->At(plot_index);
	
			leg_miss_mass->AddEntry(h1_miss_mass, (to_string(forced_jettopology[j])).c_str(), "l");
			leg_sum_E_q->AddEntry(h1_sum_E_q, (to_string(forced_jettopology[j])).c_str(), "l");
			leg_V_mass->AddEntry(h1_V_mass, (to_string(forced_jettopology[j])).c_str(), "l");
	
			h1_miss_mass->GetYaxis()->SetRangeUser(0,3);
			h1_sum_E_q->GetYaxis()->SetRangeUser(0,1.5);
			h1_V_mass->GetYaxis()->SetRangeUser(0,3);
	
			h1_miss_mass->SetMarkerStyle(0);
			h1_sum_E_q->SetMarkerStyle(0);
			h1_V_mass->SetMarkerStyle(0);
	
			h1_miss_mass->SetLineColor(9000+j);
			h1_sum_E_q->SetLineColor(9000+j);
			h1_V_mass->SetLineColor(9000+j);

			canvas_h1_miss_mass->cd();
			h1_miss_mass->Draw("e same");
			canvas_h1_sum_E_q->cd();
			h1_sum_E_q->Draw("e same");
			canvas_h1_V_mass->cd();
			h1_V_mass->Draw("e same");

		}
	
        canvas_h1_miss_mass->cd();
		leg_miss_mass->Draw();
		line_miss_mass->Draw("same");
		canvas_h1_miss_mass->Print(plot_name_h1_miss_mass.c_str());
	
		canvas_h1_sum_E_q->cd();
		leg_sum_E_q->Draw();
		line_sum_E_q->Draw("same");
		canvas_h1_sum_E_q->Print(plot_name_h1_sum_E_q.c_str());
	
		canvas_h1_V_mass->cd();
		leg_V_mass->Draw();
		line_V_mass->Draw("same");
		canvas_h1_V_mass->Print(plot_name_h1_V_mass.c_str());


		canvas_h1_miss_mass->Close();
		canvas_h1_sum_E_q->Close();
		canvas_h1_V_mass->Close();
	}


	for (int j=0; j<n_Njets; j++) {
		int Njet = forced_jettopology[j];

		string output_dir = "output_Njet_fixed/Njet" + to_string(Njet) + "_";
	
		string plot_name_h1_miss_mass = output_dir + "miss_mass.pdf";
		string plot_name_h1_sum_E_q = output_dir + "sum_E_q.pdf";
		string plot_name_h1_V_mass = output_dir + "V_mass.pdf";
	
		TLegend* leg_miss_mass = new TLegend(0.8, 0.6, .9, 0.9);
		TLegend* leg_sum_E_q = new TLegend(0.8, 0.2, .9, 0.5);
		TLegend* leg_V_mass = new TLegend(0.8, 0.6, .9, 0.9);
	
		TLine *line_miss_mass = new TLine(0,1.0,1000,1.0);	
		TLine *line_sum_E_q = new TLine(0,1.0,1000,1.0);	
		TLine *line_V_mass = new TLine(50,1.0,120,1.0);	
	
		TCanvas *canvas_h1_miss_mass = new TCanvas("can_miss_mass", "", 0, 0, 600, 600);
		TCanvas *canvas_h1_sum_E_q = new TCanvas("can_sum_E_q", "", 0, 0, 600, 600);
		TCanvas *canvas_h1_V_mass = new TCanvas("can_V_mass", "", 0, 0, 600, 600);

		for (int k=0; k<n_cones; k++) {
			int plot_index = k*n_Njets + j;
			float cone = cone_parameters[k];
			string cone_string = cone_to_string(cone);

			TH1* h1_miss_mass = (TH1D*)miss_mass_list->At(plot_index);
			TH1* h1_sum_E_q = (TH1D*)sum_E_q_list->At(plot_index);
			TH1* h1_V_mass = (TH1D*)V_mass_list->At(plot_index);
	
			leg_miss_mass->AddEntry(h1_miss_mass, cone_string.c_str(), "l");
			leg_sum_E_q->AddEntry(h1_sum_E_q, cone_string.c_str(), "l");
			leg_V_mass->AddEntry(h1_V_mass, cone_string.c_str(), "l");
	
			h1_miss_mass->GetYaxis()->SetRangeUser(0,3);
			h1_sum_E_q->GetYaxis()->SetRangeUser(0,1.5);
			h1_V_mass->GetYaxis()->SetRangeUser(0,3);
	
			h1_miss_mass->SetMarkerStyle(0);
			h1_sum_E_q->SetMarkerStyle(0);
			h1_V_mass->SetMarkerStyle(0);
	
			h1_miss_mass->SetLineColor(9000+k);
			h1_sum_E_q->SetLineColor(9000+k);
			h1_V_mass->SetLineColor(9000+k);

			canvas_h1_miss_mass->cd();
			h1_miss_mass->Draw("e same");
			canvas_h1_sum_E_q->cd();
			h1_sum_E_q->Draw("e same");
			canvas_h1_V_mass->cd();
			h1_V_mass->Draw("e same");

		}
	
        canvas_h1_miss_mass->cd();
		leg_miss_mass->Draw();
		line_miss_mass->Draw("same");
		canvas_h1_miss_mass->Print(plot_name_h1_miss_mass.c_str());
	
		canvas_h1_sum_E_q->cd();
		leg_sum_E_q->Draw();
		line_sum_E_q->Draw("same");
		canvas_h1_sum_E_q->Print(plot_name_h1_sum_E_q.c_str());
	
		canvas_h1_V_mass->cd();
		leg_V_mass->Draw();
		line_V_mass->Draw("same");
		canvas_h1_V_mass->Print(plot_name_h1_V_mass.c_str());


		canvas_h1_miss_mass->Close();
		canvas_h1_sum_E_q->Close();
		canvas_h1_V_mass->Close();

	}

	string output_dir_2D = "output_2Dparameterspace/";
	
	string plot_name_h2_miss_mass 	= output_dir_2D + "miss_mass.pdf";
	string plot_name_h2_sum_E_q 	= output_dir_2D + "sum_E_q.pdf";
	string plot_name_h2_V_mass	 	= output_dir_2D + "V_mass.pdf";

	
	TCanvas *canvas_h2_miss_mass = new TCanvas("can_2D_miss_mass", "", 0, 0, 700, 600);
	h2_miss_mass->GetZaxis()->SetTitleOffset(1.5);
	h2_miss_mass->Draw("colz text");
	canvas_h2_miss_mass->SetRightMargin(0.25);
	canvas_h2_miss_mass->Print(plot_name_h2_miss_mass.c_str());

	TCanvas *canvas_h2_sum_E_q = new TCanvas("can_2D_sum_E_q", "", 0, 0, 700, 600);
	h2_sum_E_q->GetZaxis()->SetTitleOffset(1.5);
	h2_sum_E_q->Draw("colz text");
	canvas_h2_sum_E_q->SetRightMargin(0.25);
	canvas_h2_sum_E_q->Print(plot_name_h2_sum_E_q.c_str());

	TCanvas *canvas_h2_V_mass = new TCanvas("can_2D_V_mass", "", 0, 0, 700, 600);
	h2_V_mass->GetZaxis()->SetTitleOffset(1.5);
	h2_V_mass->Draw("colz text");
	canvas_h2_V_mass->SetRightMargin(0.25);
	canvas_h2_V_mass->Print(plot_name_h2_V_mass.c_str());

	return 1;	
}
