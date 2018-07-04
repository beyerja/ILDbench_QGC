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
	/* Helper function that takes a float and transforms it like:
 		1.900000 -> 1.9	
		1.000000 -> 1
	The outcome is returned as a string.
	*/
	if ( cone - (int)cone == 0 ) {
		int int_cone = (int)cone;
		return to_string(int_cone);
	}
	std::string cone_str = std::to_string(cone);
	cone_str.erase( cone_str.find_last_not_of('0') + 1, std::string::npos );
	return cone_str;	
}

int create_performanceplots() {
	// main function

	// standard ROOT plotting things
	gROOT->Reset();
	gStyle->SetOptStat(0);
	TH2::SetDefaultSumw2();

	// Input directory and files
	string in_dir = "/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/testFastJetOverlayRemoval_output/new_samples";//output_recoEjet";

	std::vector<std::string> filenames;
	filenames.push_back("6f_2l4q_vvxyyx_lr.root");
	filenames.push_back("6f_2l4q_vvxyyx_rl.root");
	filenames.push_back("6f_2l4q_vvxxxx_lr.root");
	filenames.push_back("6f_2l4q_vvxxxx_rl.root");
	//filenames.push_back("6f_2l4q_vvyyyy_lr.root");
	//filenames.push_back("6f_2l4q_vvyyyy_rl.root");


	// Weighting of the samples
	// Important: put in correct run polarizations and sample cross-sections! 
	// Luminosity is in this case a global factor, not important
	float luminosity = 1000.0;
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
	TProfile2D* h2_miss_mass 	= new TProfile2D("miss_mass_2D", "variable: missing mass; cone parameter; forced N_{jet}; <reco/MC - 1> [%]", 
											5, -0.15, 1.85, 4, 2.5, 6.5);
	TProfile2D* h2_sum_E_q 	= new TProfile2D("sum_E_q_2D", "variable: sum of q(jet) energies; cone parameter; forced N_{jet}; <reco/MC - 1> [%]", 
											5, -0.15, 1.85, 4, 2.5, 6.5);
	TProfile2D* h2_V_mass = new TProfile2D("V_mass_2D", "variable: dijet (V) mass; cone parameter; forced N_{jet}; <reco/MC - 1> [%]", 
											5, -0.15, 1.85, 4, 2.5, 6.5);



	TProfile2D* h2_eff 	= new TProfile2D("2D_eff", "efficiency; cone parameter; forced N_{jet}; efficiency [%]", 5, -0.15, 1.85, 4, 2.5, 6.5);
	TProfile2D* h2_pur 	= new TProfile2D("2D_pur", "purity; cone parameter; forced N_{jet}; purity [%]", 5, -0.15, 1.85, 4, 2.5, 6.5);

	TProfile2D* h2_dsigma 	= new TProfile2D("2D_dsigma", "1/sqrt(efficiency times purity); cone parameter; forced N_{jet}; 1/sqrt(eps*pi)", 5, -0.15, 1.85, 5, 1.5, 6.5);




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

			std::string file_dir = in_dir + "/cone_parameter_" + cone_to_string(cone) + "/rootfiles_force" + std::to_string(forcedN) + "jets/";
		
	
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
		
				int NPFOs_correct, NPFOs_clustered;
				int NPFOs_missing, NPFOs_overlayClustered;

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
	
				tree->SetBranchAddress("NPFOs_inCorrectJets", &NPFOs_correct);
				tree->SetBranchAddress("NPFOs_inClusteredJets", &NPFOs_clustered);

				tree->SetBranchAddress("NPFOs_inCorrectJets_notinClusteredJets", &NPFOs_missing);
				tree->SetBranchAddress("NPFOs_inClusteredJets_notinCorrectJets", &NPFOs_overlayClustered);

				// Fill plots for each event 
				for (int j = 0; j<nEntries; j++) {
					tree->GetEntry(j);

					if ( ! ( (eventType == 1) || (eventType == 2) ) ) {continue;}

					h2_miss_mass->Fill(cone, forcedN, (miss_mass/true_miss_mass-1), weight);			
					h2_sum_E_q->Fill(cone, forcedN, (sum_E_q/true_sum_E_q-1), weight);			

					h1_miss_mass->Fill(true_miss_mass, miss_mass/true_miss_mass, weight);			
					h1_sum_E_q->Fill(true_sum_E_q, sum_E_q/true_sum_E_q, weight);			


					// Compare the true and reconstructed V's
					if ( partner_of_truthV1 == 1 && partner_of_truthV2 == 2 ) {
						h2_V_mass->Fill(cone, forcedN, (pair1_mass/true_pair1_mass-1), weight);
						h2_V_mass->Fill(cone, forcedN, (pair2_mass/true_pair2_mass-1), weight);

						h1_V_mass->Fill(true_pair1_mass, pair1_mass/true_pair1_mass, weight);
						h1_V_mass->Fill(true_pair2_mass, pair2_mass/true_pair2_mass, weight);
					}
					else if ( partner_of_truthV1 == 2 && partner_of_truthV2 == 1 ) {
						h2_V_mass->Fill(cone, forcedN, (pair2_mass/true_pair1_mass-1), weight);
						h2_V_mass->Fill(cone, forcedN, (pair1_mass/true_pair2_mass-1), weight);

						h1_V_mass->Fill(true_pair1_mass, pair2_mass/true_pair1_mass, weight);
						h1_V_mass->Fill(true_pair2_mass, pair1_mass/true_pair2_mass, weight);
					}
					else { // This occurs if generator level V and reconstructed V were not paired correctly (shouldn't happen xP) 
						cout << "ERROR: Something wrong with V links" << endl 
							<< "Partners are: " << partner_of_truthV1 << partner_of_truthV1 << endl; 
						return 0; }
	
					// efficiency = "signal" in clustered / inital # of particles in correct jets
					float efficiency = ((float)NPFOs_correct - NPFOs_missing) / NPFOs_correct;
					// purity = "signal" in clustered / all in clustered
					float purity = ((float)NPFOs_clustered - NPFOs_overlayClustered) / NPFOs_clustered; 
	
					// estimator for statistical uncertainty: 1/sqrt(eps*pi)
					float dsigma = 1.0 / sqrt(efficiency * purity);

					h2_eff->Fill(cone, forcedN, efficiency, weight);
					h2_pur->Fill(cone, forcedN, purity, weight);
					h2_dsigma->Fill(cone, forcedN, dsigma, weight);

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

	vector<string> h1_plot_names = {"miss_mass.pdf", "sum_E_q.pdf", "V_mass.pdf"};
	vector<TList*> h1_lists = {miss_mass_list, sum_E_q_list, V_mass_list};
	vector<float> range_max = {3, 1.5, 3};

	int n_cones = cone_parameters.size();
	int n_Njets = forced_jettopology.size();

	for (int k=0; k<n_cones; k++) {
		float cone = cone_parameters[k];

		string output_dir = "output_cone_fixed/cone" + cone_to_string(cone) + "_";
	
		vector<TLegend*> h1_legends = {	new TLegend(0.8, 0.6, .9, 0.9), 
										new TLegend(0.8, 0.2, .9, 0.5), 
										new TLegend(0.8, 0.6, .9, 0.9)};
		vector<TLine*> h1_lines = {	new TLine(0,1.0,1000,1.0), 
									new TLine(0,1.0,1000,1.0), 
									new TLine(50,1.0,120,1.0)};
		vector<TCanvas*> h1_canvas_list = {	new TCanvas("can_miss_mass", "", 0, 0, 600, 600), 
											new TCanvas("can_sum_E_q", "", 0, 0, 600, 600), 
											new TCanvas("can_V_mass", "", 0, 0, 600, 600)};

		for (int h1_index = 0; h1_index<h1_plot_names.size(); h1_index++) {
			TList* h1_list 	= h1_lists[h1_index]; 
			TLegend* leg 	= h1_legends[h1_index];
			TLine* line 	= h1_lines[h1_index];
	
			TCanvas* canvas = h1_canvas_list[h1_index];
			canvas->cd();

			for (int j=0; j<n_Njets; j++) {
				int plot_index = k*n_Njets + j;

				TH1* h1 = (TH1D*)h1_list->At(plot_index); 
				leg->AddEntry(h1, (to_string(forced_jettopology[j])).c_str(), "l");

				h1->GetYaxis()->SetRangeUser(0, range_max[h1_index]);
				h1->SetMarkerStyle(0);
				h1->SetLineColor(9000+j);

				h1->Draw("e same");
			}

			leg->Draw();
			line->Draw("same");
			canvas->Print((output_dir + h1_plot_names[h1_index]).c_str()); 
			canvas->Close();
		}
	}



	for (int j=0; j<n_Njets; j++) {
		int Njet = forced_jettopology[j];

		string output_dir = "output_Njet_fixed/Njet" + to_string(Njet) + "_";
	
		vector<TLegend*> h1_legends = {	new TLegend(0.8, 0.6, .9, 0.9), 
										new TLegend(0.8, 0.2, .9, 0.5), 
										new TLegend(0.8, 0.6, .9, 0.9)};
		vector<TLine*> h1_lines = {	new TLine(0,1.0,1000,1.0), 
									new TLine(0,1.0,1000,1.0), 
									new TLine(50,1.0,120,1.0)};
		vector<TCanvas*> h1_canvas_list = {	new TCanvas("can_miss_mass", "", 0, 0, 600, 600), 
											new TCanvas("can_sum_E_q", "", 0, 0, 600, 600), 
											new TCanvas("can_V_mass", "", 0, 0, 600, 600)};

		for (int h1_index = 0; h1_index<h1_plot_names.size(); h1_index++) {
			TList* h1_list 	= h1_lists[h1_index]; 
			TLegend* leg 	= h1_legends[h1_index];
			TLine* line 	= h1_lines[h1_index];
	
			TCanvas* canvas = h1_canvas_list[h1_index];
			canvas->cd();
			for (int k=0; k<n_cones; k++) {
				int plot_index = k*n_Njets + j;
				float cone = cone_parameters[k];
				string cone_string = cone_to_string(cone);

				TH1* h1 = (TH1D*)h1_list->At(plot_index); 
				leg->AddEntry(h1, cone_string.c_str(), "l");

				h1->GetYaxis()->SetRangeUser(0, range_max[h1_index]);
				h1->SetMarkerStyle(0);
				h1->SetLineColor(9000+k);

				h1->Draw("e same");
			}

			leg->Draw();
			line->Draw("same");
			canvas->Print((output_dir + h1_plot_names[h1_index]).c_str()); 
			canvas->Close();
		}
	}

	// 2D plots

	// TODO Separate eff etc from deviations!!! -> different color schemes!!
	string output_dir_2D = "output_2Dparameterspace/";

	vector<string> h2_plot_names = {"miss_mass.pdf", "sum_E_q.pdf", "V_mass.pdf",
									"eff.pdf", "pur.pdf", "dsigma.pdf"}; 
	vector<string> h2_canvas_names= {"can_2D_miss_mass", "can_2D_sum_E_q", "can_2D_V_mass",
									"can_2D_eff", "can_2D_pur", "can_2D_dsigma"};
	vector<bool> in_percent = {true, true, true, true, true, false};

	vector<TProfile2D*> h2_list = {h2_miss_mass, h2_sum_E_q, h2_V_mass,
							h2_eff, h2_pur, h2_dsigma};

	float c_h2_xmin = 0;
	float c_h2_ymin = 0;
	float c_h2_xmax = 700;
	float c_h2_ymax = 600;
	float h2_ztitle_offset = 1.25;
	float c_h2_rightmargin = 0.22;
	string draw_opt = "colz text";


	gStyle->SetPalette(kBlackBody);
	for (int h2_index = 0; h2_index<h2_plot_names.size(); h2_index++) {
		TProfile2D* h2 = h2_list[h2_index];

		if ( in_percent[h2_index] ) {
			h2->Scale(100); //Scale to %
		}

		h2->GetXaxis()->SetTickSize(0);
		h2->GetYaxis()->SetTickSize(0);
		h2->GetZaxis()->SetTitleOffset(h2_ztitle_offset);

		for (int k=0; k<cone_parameters.size(); k++) {
    		float cone = cone_parameters[k];
    		h2->GetXaxis()->SetBinLabel(k+1, cone_to_string(cone).c_str());
		}	


		TCanvas *canvas_h2 = new TCanvas(h2_canvas_names[h2_index].c_str(), "", c_h2_xmin, c_h2_ymin, c_h2_xmax, c_h2_ymax);
		h2->Draw(draw_opt.c_str());
		canvas_h2->SetRightMargin(c_h2_rightmargin);
		canvas_h2->Print((output_dir_2D + h2_plot_names[h2_index]).c_str());

		canvas_h2->Close();
	}

	return 1;	
}
