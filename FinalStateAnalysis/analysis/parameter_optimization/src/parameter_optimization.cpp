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


#include "calculate_1D_ratio.cc"

// TODO Maybe this to other file
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

int parameter_optimization() {
	// main function

	// standard ROOT plotting things
	gROOT->Reset();
	gStyle->SetOptStat(0);
	TH2::SetDefaultSumw2();

	// Input directory and files
	string in_dir = "/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/testFastJetOverlayRemoval_output/output_forcedEjet_mjet0";

	std::vector<std::string> filenames;
	filenames.push_back("6f_vvww_h_eL.pR.root");
	filenames.push_back("6f_vvww_h_eR.pL.root");
	filenames.push_back("6f_xxxx_nunu_eL.pR.root");
	filenames.push_back("6f_xxxx_nunu_eR.pL.root");
	filenames.push_back("6f_yyyy_nunu_eL.pR.root");
	filenames.push_back("6f_yyyy_nunu_eR.pL.root"); 

	string tree_name = "datatrain";


	// Weighting of the samples
	// Important: put in correct run polarizations and sample cross-sections! 
	// Luminosity is in this case a global factor, not important
	float luminosity = 1.0;
	std::vector<float> xsections = 		{50.5806,	0.269859,	1.63413,	0.0449741,	5.39289,	0.104988};
	std::vector<float> e_pol_signs =	{-1,		1,	 		-1,	    	1,			-1,			1}; 
	std::vector<float> p_pol_signs =	{1,			-1,	 		1,	    	-1,			1,			-1}; 
	float e_polarization = -0.8;
	float p_polarization = 0.3;


	vector<FileInfo> file_infos;

	for (int i = 0; i<filenames.size(); i++) {
		FileInfo file_info;
		file_info.init( filenames[i], in_dir, tree_name );
		file_info.calculate_polarization_weight(e_pol_signs[i], e_polarization, p_pol_signs[i], p_polarization);
		file_info.calculate_number_of_events(xsections[i], luminosity);
		file_infos.push_back(file_info);
	}


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
			int exclusive_Njet = forced_jettopology[j];

			// Comparison works the following way:
			// 	For each variable: Look at event, take ratio between reco and truth of variable,
			// 	and fill into histogram that calculates the statistics of that ratio for each truth bin.
			// 	-> Info about how well each true value gets reconstructed 

			string cone_string = cone_to_string(cone);
			string exclusive_Njet_string = to_string(exclusive_Njet);

			ParameterPoint parameter_point;
			parameter_point.init( cone_string, exclusive_Njet_string, file_infos );

			Calculator1DRatio calculator_1D_ratio;
			calculator_1D_ratio.init(parameter_point);
			calculator_1D_ratio.loop_all_files();

		}
	}

	return 1;
}



			// TODO Thought: Maybe it would be better (more intuitive) if ParameterPoint has functions:
			//  AddFiles -> Takes filename vector and other file info
			//  SetParameters -> Takes vectors with physical information
			//  Together these two perform the loop below
			//
			//  No wait, better idea: separate "global" information (lumi, e_pol_perc, etc) into SetParameters function
			//  and let AddFiles take everything file-specific
			//  This way first SetParameters can be performed, then each file can be added without it getting messy in ParameterPoint
			// -> Then parameterpoint also stores global information. Generally seems unnecessary, but I might end up varying that too. From that perspective this is advantageous.

















/*


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

	// Scale to %
	h2_miss_mass->Scale(100);
	h2_sum_E_q->Scale(100);
	h2_V_mass->Scale(100);



	float c_h2_xmin = 0;
	float c_h2_ymin = 0;
	float c_h2_xmax = 700;
	float c_h2_ymax = 600;
	float h2_ztitle_offset = 1.25;
	float c_h2_rightmargin = 0.22;
	string draw_opt = "colz text";

	// No axis ticks, they are helpful here
	h2_miss_mass->GetXaxis()->SetTickSize(0);
	h2_sum_E_q->GetXaxis()->SetTickSize(0);
	h2_V_mass->GetXaxis()->SetTickSize(0);

	h2_miss_mass->GetYaxis()->SetTickSize(0);
	h2_sum_E_q->GetYaxis()->SetTickSize(0);
	h2_V_mass->GetYaxis()->SetTickSize(0);

	// Offset of color palette title such that it is readable
	h2_miss_mass->GetZaxis()->SetTitleOffset(h2_ztitle_offset);
	h2_sum_E_q->GetZaxis()->SetTitleOffset(h2_ztitle_offset);
	h2_V_mass->GetZaxis()->SetTitleOffset(h2_ztitle_offset);

	for (int k=0; k<cone_parameters.size(); k++) {
    	float cone = cone_parameters[k];
    	h2_miss_mass->GetXaxis()->SetBinLabel(k+1, cone_to_string(cone).c_str());
    	h2_sum_E_q->GetXaxis()->SetBinLabel(k+1, cone_to_string(cone).c_str());
    	h2_V_mass->GetXaxis()->SetBinLabel(k+1, cone_to_string(cone).c_str());
	}	

	gStyle->SetPalette(kBlackBody);
	TCanvas *canvas_h2_miss_mass = new TCanvas("can_2D_miss_mass", "", c_h2_xmin, c_h2_ymin, c_h2_xmax, c_h2_ymax);
	h2_miss_mass->Draw(draw_opt.c_str());
	canvas_h2_miss_mass->SetRightMargin(c_h2_rightmargin);
	canvas_h2_miss_mass->Print(plot_name_h2_miss_mass.c_str());

	TCanvas *canvas_h2_sum_E_q = new TCanvas("can_2D_sum_E_q", "", c_h2_xmin, c_h2_ymin, c_h2_xmax, c_h2_ymax);
	h2_sum_E_q->Draw(draw_opt.c_str());
	canvas_h2_sum_E_q->SetRightMargin(c_h2_rightmargin);
	canvas_h2_sum_E_q->Print(plot_name_h2_sum_E_q.c_str());

	TCanvas *canvas_h2_V_mass = new TCanvas("can_2D_V_mass", "", c_h2_xmin, c_h2_ymin, c_h2_xmax, c_h2_ymax);
	h2_V_mass->Draw(draw_opt.c_str());
	canvas_h2_V_mass->SetRightMargin(c_h2_rightmargin);
	canvas_h2_V_mass->Print(plot_name_h2_V_mass.c_str());


	return 1;	
}*/
