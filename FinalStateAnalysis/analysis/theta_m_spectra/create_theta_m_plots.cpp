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
	TH2* h2_m_theta_genlevel = new TH2D("m_theta_genlevel", "generator level ; m_{jj} [GeV];  #theta_{V}", 60, 30, 150, 32, 0, 3.2);
	TH2* h2_m_theta_nocuts = new TH2D("m_theta_nocuts", "w/o cuts ; m_{jj} [GeV];  #theta_{V}", 60, 30, 150, 32, 0, 3.2);
	TH2* h2_m_theta = new TH2D("m_theta", "w/ cuts; m_{jj} [GeV];  #theta_{V}", 60, 30, 150, 32, 0, 3.2);
	TH2* h2_m_reco_theta_gen = new TH2D("m_reco_theta_gen", "w/ cuts; m_{jj,reco} [GeV];  #theta_{V,gen}", 60, 30, 150, 32, 0, 3.2);

	TH1* h1_theta_genlevel = new TH1D("theta_genlevel", "generator level (m not in [65,105]GeV); #theta_{V}; Events", 32, 0, 3.2);
	TH1* h1_theta_nocuts = new TH1D("theta_nocuts", "w/o cuts (m not in [65,105]GeV) ; #theta_{V}; Events", 32, 0, 3.2);
	TH1* h1_theta = new TH1D("theta", "w/ cuts (m not in [65,105]GeV); #theta_{V}; Events", 32, 0, 3.2);
	TH1* h1_theta_gen_mrecocut = new TH1D("theta_gen_mrecocut", "w/ cuts (m_{reco} not in [65,105]GeV); #theta_{V,gen}; Events", 32, 0, 3.2);


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
		float true_mjj1, true_mjj2, mjj1, mjj2, true_theta1, true_theta2, theta1, theta2, p1, p2;
		int passed_cuts;

		// Connect TBranch to addresses of the declared variables
		tree->SetBranchAddress("true_evt_type", &eventType);
		tree->SetBranchAddress("true_pair1_mass", &true_mjj1);
		tree->SetBranchAddress("true_pair2_mass", &true_mjj2);
		tree->SetBranchAddress("true_pair1_theta", &true_theta1);
		tree->SetBranchAddress("true_pair2_theta", &true_theta2);
		tree->SetBranchAddress("pair1_mass", &mjj1);
		tree->SetBranchAddress("pair2_mass", &mjj2);
		tree->SetBranchAddress("pair1_theta", &theta1);
		tree->SetBranchAddress("pair2_theta", &theta2);
		tree->SetBranchAddress("pair1_p", &p1);
		tree->SetBranchAddress("pair2_p", &p2);
		tree->SetBranchAddress("pass_selection", &passed_cuts);

		// Fill histograms with each entry
		for (int j = 0; j<nEntries; j++) {
			tree->GetEntry(j);
			if (eventType == 0) {continue;} // Here only interested in WW/ZZ as defined on generator level
			if ( ( (mjj1 < 65) || (mjj1 > 105) ) ) { // && ( p1 > 100) ) {
				h1_theta_genlevel->Fill(true_theta1, weight);			
				h1_theta_nocuts->Fill(theta1, weight);			
				h2_m_theta_genlevel->Fill(true_mjj1, true_theta1, weight);			
				h2_m_theta_nocuts->Fill(mjj1, theta1, weight);			
				if ( passed_cuts == 1 ) {
					h1_theta->Fill(theta1, weight);			
					h2_m_theta->Fill(mjj1, theta1, weight);			
					h1_theta_gen_mrecocut->Fill(true_theta1, weight);
					h2_m_reco_theta_gen->Fill(mjj1, true_theta1, weight);
				}
			}

			if ( ( (mjj2 < 65) || (mjj2 > 105) ) ) { // && ( p2 > 100) ) {
				h1_theta_genlevel->Fill(true_theta2, weight);			
				h1_theta_nocuts->Fill(theta2, weight);			
				h2_m_theta_genlevel->Fill(true_mjj2, true_theta2, weight);			
				h2_m_theta_nocuts->Fill(mjj2, theta2, weight);			
				if ( passed_cuts == 1 ) {
					h1_theta->Fill(theta2, weight);			
					h2_m_theta->Fill(mjj2, theta2, weight);			
					h1_theta_gen_mrecocut->Fill(true_theta2, weight);
					h2_m_reco_theta_gen->Fill(mjj2, true_theta2, weight);
				}
			}
		}

	}


	// Do drawing and saving of plots
	
	// put histograms into vectors for easier looping
	vector<TH2*> h2_vector;   
	h2_vector.push_back(h2_m_theta_genlevel);
	h2_vector.push_back(h2_m_theta_nocuts);
 	h2_vector.push_back(h2_m_theta);
 	h2_vector.push_back(h2_m_reco_theta_gen);
	vector<TH1*> h1_vector;
	h1_vector.push_back(h1_theta_genlevel);
 	h1_vector.push_back(h1_theta_nocuts);
 	h1_vector.push_back(h1_theta);
 	h1_vector.push_back(h1_theta_gen_mrecocut);

	vector<string> level_names = {"_genlevel", "_nocuts", "", "_theta_gen_m_reco"};
	vector<string> h2_plot_name = {"m_theta_genlevel", "m_theta_nocuts", "m_theta", "m_reco_theta_gen"};	
	vector<string> h1_plot_name = {"theta_genlevel", "theta_nocuts", "theta", "theta_gen_mrecocuts"};

	for (int l=0; l<level_names.size(); l++) {
		TCanvas *canvas_h2 = new TCanvas(("can_h2" + level_names[l]).c_str(), "", 0, 0, 800, 800);
		h2_vector[l]->Draw("colz");
		canvas_h2->SetRightMargin(0.22); // Adjust right margin so that color scheme is shown fully
		string plot_name_h2 = "output_plots/" + h2_plot_name[l] + ".pdf";
		canvas_h2->Print(plot_name_h2.c_str());
		canvas_h2->Close(); // Avoid memory leaks

		TCanvas *canvas_h1 = new TCanvas(("can_h1" + level_names[l]).c_str(), "", 0, 0, 800, 800);
		h1_vector[l]->Draw("hist e");
		string plot_name_h1 = "output_plots/" + h1_plot_name[l] + ".pdf";
		canvas_h1->Print(plot_name_h1.c_str());
		canvas_h1->Close();
	}

}
