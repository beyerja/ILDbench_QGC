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


{
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
	std::vector<float> xsection = 	{50.5806,	0.269859,	1.63413,	0.0449741,	5.39289,	0.104988};
	std::vector<float> sample_e_pol =	{-1,		1,	 		-1,	    	1,			-1,			1}; 
	std::vector<float> sample_p_pol =	{1,			-1,	 		1,	    	-1,			1,			-1}; 
	float e_pol_percentage = -0.8;
	float p_pol_percentage = 0.3;


	// FastJet parameters relevant for overlay removal
	std::vector<int> cone_parameters = {1}; // {0.1, 0.5, 1, 1.3, 1.5}
	std::vector<int> forced_jettopology = {2, 3, 4, 5, 6};

	// List of histograms, one list per observable, holds histograms for each parameter pair
	TList* miss_mass_list = new TList();
	TList* sum_E_q_list = new TList();
	TList* V_mass_list = new TList();


	// Loop over all parameter possibilities 
	for (int j=0; j<forced_jettopology.size(); j++) {
		int forcedN = forced_jettopology[j];

		std::string file_dir = "/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/testFastJetOverlayRemoval_output/cone_parameter_1/rootfiles_force" + std::to_string(forcedN) + "jets/";
	

		// TProfile is 1D plot that plots statistics of values that are filled for each bin		
		TProfile* h1_miss_mass = new TProfile(	("miss_mass_" + std::to_string(forcedN)).c_str(), 
											"missing mass; m_{miss,GenLevel} [GeV]; value reco/gen", 50, 0, 1000);
		TProfile* h1_sum_E_q = new TProfile(	("sum_E_q_" + std::to_string(forcedN)).c_str(), 
										"sum quark/jet energies; Sum E_{q} [GeV]; value reco/gen", 50, 0, 1000);
		TProfile* h1_V_mass = new TProfile(	("V_mass_" + std::to_string(forcedN)).c_str(), 
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
			int partner_of_truthV1, partner_of_truthV2;
			float true_miss_mass, 	true_sum_E_q, 	true_pair1_mass,	true_pair2_mass; 
			float miss_mass, 		sum_E_q,		pair1_mass,			pair2_mass;	
	
			// Couple variables to the tree branch addresses
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
				h1_miss_mass->Fill(true_miss_mass, miss_mass/true_miss_mass, weight);			
				h1_sum_E_q->Fill(true_sum_E_q, sum_E_q/true_sum_E_q, weight);			
	
				// Compare the true and reconstructed V's
				if ( partner_of_truthV1 == 1 && partner_of_truthV2 == 2 ) {
					h1_V_mass->Fill(true_pair1_mass, pair1_mass/true_pair1_mass, weight);
					h1_V_mass->Fill(true_pair2_mass, pair2_mass/true_pair2_mass, weight);
				}
				else if ( partner_of_truthV1 == 2 && partner_of_truthV2 == 1 ) {
					h1_V_mass->Fill(true_pair1_mass, pair2_mass/true_pair1_mass, weight);
					h1_V_mass->Fill(true_pair2_mass, pair1_mass/true_pair2_mass, weight);
				}
				else { // This occurs if generator level V and reconstructed V were not paired correctly (shouldn't happen xP) 
					cout << "ERROR: Something wrong with V links" << endl 
						<< "Partners are: " << partner_of_truthV1 << partner_of_truthV1 << endl; 
					return; }
			}

		}
	
		// Add the plot for the parameter specific file to the corresponding TList
		miss_mass_list->Add(h1_miss_mass);
		sum_E_q_list->Add(h1_sum_E_q);
		V_mass_list->Add(h1_V_mass);

	}

	// Define a good looking RGB color palette
	std::vector<float[3]> colors = {{228,26,28}, {55,126,184}, {77,175,74}, {152,78,163}, {255,127,0}};
	for (int j=0; j<forced_jettopology.size(); j++) {
		TColor *h1_color = new TColor(9000+j, colors[j][0]/256.0, colors[j][1]/256.0, colors[j][2]/256.0);
	}

	// Following is pretty much copy-paste for each variable 
	// (not automated to keep myself from using too many variables and loosing overview)
	TCanvas *canvas_h1_miss_mass = new TCanvas("can_miss_mass", "", 0, 0, 600, 600);
	TLegend* leg_miss_mass = new TLegend(0.8, 0.6, .9, 0.9);
	for (int j=0; j<forced_jettopology.size(); j++) {
		TH1* h1_miss_mass = (TH1D*)miss_mass_list->At(j);
		leg_miss_mass->AddEntry(h1_miss_mass, (to_string(forced_jettopology[j])).c_str(), "l");
		h1_miss_mass->GetYaxis()->SetRangeUser(0,3);
		h1_miss_mass->SetMarkerStyle(0);
		h1_miss_mass->SetLineColor(9000+j);
		h1_miss_mass->Draw("e same");
	}
	leg_miss_mass->Draw();
	TLine *line_miss_mass = new TLine(0,1.0,1000,1.0);	
	line_miss_mass->Draw("same");
	string plot_name_h1_miss_mass = "output_plots/miss_mass.pdf";
	canvas_h1_miss_mass->Print(plot_name_h1_miss_mass.c_str());


	TCanvas *canvas_h1_sum_E_q = new TCanvas("can_sum_E_q", "", 0, 0, 600, 600);
	TLegend* leg_sum_E_q = new TLegend(0.8, 0.2, .9, 0.5);
	for (int j=0; j<forced_jettopology.size(); j++) {
		TH1* h1_sum_E_q = (TH1D*)sum_E_q_list->At(j);
		leg_sum_E_q->AddEntry(h1_sum_E_q, (to_string(forced_jettopology[j])).c_str(), "l");
		h1_sum_E_q->GetYaxis()->SetRangeUser(0,1.5);
		h1_sum_E_q->SetMarkerStyle(0);
		h1_sum_E_q->SetLineColor(9000+j);
		h1_sum_E_q->Draw("e same");
	}
	leg_sum_E_q->Draw();
	TLine *line_sum_E_q = new TLine(0,1.0,1000,1.0);	
	line_sum_E_q->Draw("same");
	string plot_name_h1_sum_E_q = "output_plots/sum_E_q.pdf";
	canvas_h1_sum_E_q->Print(plot_name_h1_sum_E_q.c_str());


	TCanvas *canvas_h1_V_mass = new TCanvas("can_V_mass", "", 0, 0, 600, 600);
	TLegend* leg_V_mass = new TLegend(0.8, 0.6, .9, 0.9);
	for (int j=0; j<forced_jettopology.size(); j++) {
		TH1* h1_V_mass = (TH1D*)V_mass_list->At(j);
		leg_V_mass->AddEntry(h1_V_mass, (to_string(forced_jettopology[j])).c_str(), "l");
		h1_V_mass->GetYaxis()->SetRangeUser(0,3);
		h1_V_mass->SetMarkerStyle(0);
		h1_V_mass->SetLineColor(9000+j);
		h1_V_mass->Draw("e same");
	}
	leg_V_mass->Draw();
	TLine *line_V_mass = new TLine(50,1.0,120,1.0);	
	line_V_mass->Draw("same");
	string plot_name_h1_V_mass = "output_plots/V_mass.pdf";
	canvas_h1_V_mass->Print(plot_name_h1_V_mass.c_str());

}
