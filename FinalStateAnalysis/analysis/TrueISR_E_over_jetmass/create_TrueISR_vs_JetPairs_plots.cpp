/* Create plots of theta distribution of bosons (=dijet-pairs)  
 * - 2D plots over mass of that boson 
 * - 1D plots with cut on that bosons mass
 * on different levels of event (generator, w/o cuts, w/ cuts).
 *
 *
 * Execute as: root -b -q create_theta_m_plots.cpp 
*/



{
	// Standard ROOT plot setting
	gROOT->Reset();
	gStyle->SetOptStat(0);
	TH2::SetDefaultSumw2();

	// Names of input files to be included
	std::vector<std::string> filenames;
	filenames.push_back("6f_2l4q_vvxyyx_lr.root");
	filenames.push_back("6f_2l4q_vvxyyx_rl.root");
	filenames.push_back("6f_2l4q_vvxxxx_lr.root");
	filenames.push_back("6f_2l4q_vvxxxx_rl.root");
	//filenames.push_back("6f_2l4q_vvyyyy_lr.root");
	//filenames.push_back("6f_2l4q_vvyyyy_rl.root");

	// Name of TTree in input files
	std::string input_tree_name = "selected_datatrain"; //"datatrain"; 

	// Details of the ILC Scenario 
	float luminosity = 1000.0;
	float e_pol_percentage = -0.8;
	float p_pol_percentage = 0.3;

	// Details about the physical samples
	std::vector<float> xsection = 	{50.5806,	0.269859,	1.63413,	0.0449741,	5.39289,	0.104988};
	std::vector<float> sample_e_pol =	{-1,		1,	 		-1,	    	1,			-1,			1}; 
	std::vector<float> sample_p_pol =	{1,			-1,	 		1,	    	-1,			1,			-1}; 

	// Input directory (possibly including prefix of filenames)
	//std::string file_dir = "/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/rootfiles/";
	std::string file_dir = "/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/rootfiles_after_selection/selected_";
	//std::string file_dir = "/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/testFastJetOverlayRemoval_output/output_forcedEjet_mjet0/cone_parameter_1.3/rootfiles_force4jets/";

	// Histograms, one for each step + one connecting generator level theta w/ reconstructed mass 
	TH2* h2_mjj_isrE = new TH2D("m_isrE", "w/ cuts; m_{jj} [GeV];  E_{ISR in Jets}", 45, 30, 120, 80, 0, 80);
	TH2* h2_mjj_isrP = new TH2D("m_isrP", "w/ cuts; m_{jj} [GeV];  P_{ISR in Jets}", 45, 30, 120, 80, 0, 80);
	TH2* h2_mjj_isrTheta = new TH2D("m_isrTheta", "w/ cuts; m_{jj} [GeV];  #Theta_{ISR in Jets}", 45, 30, 120, 32, 0, 3.2);

	// Fill histograms for each input file
	for (int i = 0; i<filenames.size(); i++) {
		// Read in input TTree
		TFile *file = TFile::Open((file_dir + filenames[i]).c_str());
		TTree *tree = (TTree*)file->Get(input_tree_name.c_str());

		int nEntries = tree->GetEntries();

		// Calculate weight for this input sample
		float pol_weight = ( 1 + sample_e_pol[i] * e_pol_percentage )/2 * ( 1 + sample_p_pol[i] * p_pol_percentage )/2;
		float weight = ( xsection[i] * luminosity / nEntries ) * pol_weight; 

		// Declare variables that are to be connected to TBranches of input file
		int eventType;
		float mjj1, mjj2, E_ISR, P_ISR, Theta_ISR;
		int passed_cuts;

		// Connect TBranch to addresses of the declared variables
		tree->SetBranchAddress("true_evt_type", &eventType);
		tree->SetBranchAddress("pair1_mass", &mjj1);
		tree->SetBranchAddress("pair2_mass", &mjj2);
		tree->SetBranchAddress("true_ISR_inJets_E", &E_ISR);
		tree->SetBranchAddress("true_ISR_inJets_p", &P_ISR);
		tree->SetBranchAddress("true_ISR_inJets_theta", &Theta_ISR);
		tree->SetBranchAddress("pass_selection", &passed_cuts);

		// Fill histograms with each entry
		for (int j = 0; j<nEntries; j++) {
			tree->GetEntry(j);
			if (eventType == 0) {continue;} // Here only interested in WW/ZZ as defined on generator level
			if ( passed_cuts != 1 ) {continue;}
			//if (mjj1 < 65) { 
				h2_mjj_isrE->Fill(mjj1, E_ISR, weight);			
				h2_mjj_isrP->Fill(mjj1, P_ISR, weight);			
				h2_mjj_isrTheta->Fill(mjj1, Theta_ISR, weight);			
			//}

			//if (mjj2 < 65) {
				h2_mjj_isrE->Fill(mjj2, E_ISR, weight);			
				h2_mjj_isrP->Fill(mjj2, P_ISR, weight);			
				h2_mjj_isrTheta->Fill(mjj2, Theta_ISR, weight);			
			//}
		}

	}


	// Do drawing and saving of plots
	
	// put histograms into vectors for easier looping
	vector<TH2*> h2_vector;   
	h2_vector.push_back(h2_mjj_isrE);
	h2_vector.push_back(h2_mjj_isrP);
	h2_vector.push_back(h2_mjj_isrTheta);

	vector<string> level_names = {"_E", "_P", "_Theta"};
	vector<string> h2_plot_name = {"mjj_isrE", "mjj_isrP", "mjj_isrTheta"};	

	for (int l=0; l<level_names.size(); l++) {
		TCanvas *canvas_h2 = new TCanvas(("can_h2" + level_names[l]).c_str(), "", 0, 0, 800, 800);
		h2_vector[l]->Draw("colz");
		canvas_h2->SetRightMargin(0.22); // Adjust right margin so that color scheme is shown fully
		string plot_name_h2 = "output_plots/" + h2_plot_name[l] + ".pdf";
		canvas_h2->Print(plot_name_h2.c_str());
		canvas_h2->Close(); // Avoid memory leaks
	}

}
