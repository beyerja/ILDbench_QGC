/* Script to plot m_jj : m_jj for WW and ZZ events (+nunu) each normalized in same plot
run in ROOT Terminal as: .x scriptname.cc

Goal: recreate plots from 
	- ILC TDR Volume 4, chapter 6.2.3 (Strong EWSB)
	- ILD Letter of intent, chapter 3.3.6


If it doesn't work assess:
	- is input directory correct
	- are filenames correct
	- is name of TTree in input file correct
	- are names of TBranches correct
	?
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
	filenames.push_back("6f_2l4q_vvyyyy_lr.root");
	filenames.push_back("6f_2l4q_vvyyyy_rl.root");

	// old notation
	/*filenames.push_back("6f_vvww_h_eL.pR.root");
	filenames.push_back("6f_vvww_h_eR.pL.root");
	filenames.push_back("6f_xxxx_h_eL.pR.root");
	filenames.push_back("6f_xxxx_h_eR.pL.root");
	filenames.push_back("6f_yyyy_h_eL.pR.root");
	filenames.push_back("6f_yyyy_h_eR.pL.root");*/
	

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

	// Histograms for WW/ZZ events, for each step of the generation/analysis
	TH1* h1_WW_genlevel = new TH1D("m_WW_genlevel", "generator level; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120);
	TH1* h1_ZZ_genlevel = new TH1D("m_ZZ_genlevel", "generator level; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120);
	TH2* h2_WW_genlevel = new TH2D("m_m_WW_genlevel", "generator level; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120);
	TH2* h2_ZZ_genlevel = new TH2D("m_m_ZZ_genlevel", "generator level; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120);

	TH1* h1_WW_nocuts = new TH1D("m_WW_nocuts", "w/o cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120);
	TH1* h1_ZZ_nocuts = new TH1D("m_ZZ_nocuts", "w/o cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120);
	TH2* h2_WW_nocuts = new TH2D("m_m_WW_nocuts", "w/o cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120);
	TH2* h2_ZZ_nocuts = new TH2D("m_m_ZZ_nocuts", "w/o cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120);

	TH1* h1_WW = new TH1D("m_WW", "w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120);
	TH1* h1_ZZ = new TH1D("m_ZZ", "w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120);
	TH2* h2_WW = new TH2D("m_m_WW", "w/ cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120);
	TH2* h2_ZZ = new TH2D("m_m_ZZ", "w/ cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120);


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
		float true_mjj1, true_mjj2, mjj1, mjj2;
		int passed_cuts;

		// Connect TBranch to addresses of the declared variables
		tree->SetBranchAddress("true_evt_type", &eventType);
		tree->SetBranchAddress("true_pair1_mass", &true_mjj1);
		tree->SetBranchAddress("true_pair2_mass", &true_mjj2);
		tree->SetBranchAddress("pair1_mass", &mjj1);
		tree->SetBranchAddress("pair2_mass", &mjj2);
		tree->SetBranchAddress("pass_selection", &passed_cuts);

		// Fill histograms with each entry
		for (int j = 0; j<nEntries; j++) {
			tree->GetEntry(j);
			if ( eventType == 1 ) {
				h1_WW_genlevel->Fill((true_mjj1+true_mjj2)/2., weight);			
				h2_WW_genlevel->Fill(true_mjj1, true_mjj2, weight);			
				h1_WW_nocuts->Fill((mjj1+mjj2)/2., weight);			
				h2_WW_nocuts->Fill(mjj1, mjj2, weight);			
				if ( passed_cuts == 1 ) {
					h1_WW->Fill((mjj1+mjj2)/2., weight);			
					h2_WW->Fill(mjj1, mjj2, weight);			
				}
			}
			else if ( eventType == 2 ) {
				h1_ZZ_genlevel->Fill((true_mjj1+true_mjj2)/2., weight);			
				h2_ZZ_genlevel->Fill(true_mjj1, true_mjj2, weight);			
				h1_ZZ_nocuts->Fill((mjj1+mjj2)/2., weight);			
				h2_ZZ_nocuts->Fill(mjj1, mjj2, weight);			
				if ( passed_cuts == 1 ) {
					h1_ZZ->Fill((mjj1+mjj2)/2., weight);			
					h2_ZZ->Fill(mjj1, mjj2, weight);			
				}
			}
		}

	}

	// Normalization the 2D mjj:mjj distributions so that ZZ and WW are comparable

	h2_WW_genlevel->Scale(1./ h2_WW_genlevel->Integral());
	h2_ZZ_genlevel->Scale(1./ h2_ZZ_genlevel->Integral());

	h2_WW_nocuts->Scale(1./ h2_WW_nocuts->Integral());
	h2_ZZ_nocuts->Scale(1./ h2_ZZ_nocuts->Integral());
	
	h2_WW->Scale(1./ h2_WW->Integral());
	h2_ZZ->Scale(1./ h2_ZZ->Integral());

	// WW = blue, ZZ = red
	h1_WW_genlevel->SetLineColor(4);	
	h1_ZZ_genlevel->SetLineColor(2);	
	h2_WW_genlevel->SetLineColor(4);	
	h2_ZZ_genlevel->SetLineColor(2);	

	h1_WW_nocuts->SetLineColor(4);	
	h1_ZZ_nocuts->SetLineColor(2);	
	h2_WW_nocuts->SetLineColor(4);	
	h2_ZZ_nocuts->SetLineColor(2);	

	h1_WW->SetLineColor(4);	
	h1_ZZ->SetLineColor(2);	
	h2_WW->SetLineColor(4);	
	h2_ZZ->SetLineColor(2);	


	// Do drawing and saving of plots
	TCanvas *canvas_h1_genlevel = new TCanvas("can_h1_genlevel", "", 0, 0, 600, 600);
	h1_WW_genlevel->Draw("hist");
	h1_ZZ_genlevel->Draw("hist same");
	
	string plot_name_h1_genlevel = "output_plots/m_genlevel.pdf";
	canvas_h1_genlevel->Print(plot_name_h1_genlevel.c_str());

	TCanvas *canvas_h2_genlevel = new TCanvas("can_h2_genlevel", "", 0, 0, 800, 800);
	h2_WW_genlevel->Draw("box");
	h2_ZZ_genlevel->Draw("box same");
	
	string plot_name_h2_genlevel = "output_plots/m_m_genlevel.pdf";
	canvas_h2_genlevel->Print(plot_name_h2_genlevel.c_str());


	TCanvas *canvas_h1_nocuts = new TCanvas("can_h1_nocuts", "", 0, 0, 600, 600);
	h1_WW_nocuts->Draw("hist");
	h1_ZZ_nocuts->Draw("hist same");
	
	string plot_name_h1_nocuts = "output_plots/m_nocuts.pdf";
	canvas_h1_nocuts->Print(plot_name_h1_nocuts.c_str());

	TCanvas *canvas_h2_nocuts = new TCanvas("can_h2_nocuts", "", 0, 0, 800, 800);
	h2_WW_nocuts->Draw("box");
	h2_ZZ_nocuts->Draw("box same");
	
	string plot_name_h2_nocuts = "output_plots/m_m_nocuts.pdf";
	canvas_h2_nocuts->Print(plot_name_h2_nocuts.c_str());


	TCanvas *canvas_h1 = new TCanvas("can_h1", "", 0, 0, 600, 600);
	h1_WW->Draw("hist");
	h1_ZZ->Draw("hist same");

	string plot_name_h1 = "output_plots/m.pdf";
	canvas_h1->Print(plot_name_h1.c_str());

	TCanvas *canvas_h2 = new TCanvas("can_h2", "", 0, 0, 800, 800);
	h2_WW->Draw("box");
	h2_ZZ->Draw("box same");

	string plot_name_h2 = "output_plots/m_m.pdf";
	canvas_h2->Print(plot_name_h2.c_str());


	cout << h1_WW_nocuts->GetEffectiveEntries() << " " << h1_ZZ_nocuts->GetEffectiveEntries() << "  ";
	cout << h1_WW->GetEffectiveEntries() << " " << h1_ZZ->GetEffectiveEntries() << "  ";
}
