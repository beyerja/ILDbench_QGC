#include "../../include/plotter.h"

class TJJetMassInvestigationPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_seen_mass_investigation");	
		set_1D_array_reading( true );
	}

	void define_plots(){

		add_new_TH2D("jet_vs_miss_mass_light_quarks", 
					new TH2D("jet_vs_miss_mass_light_quarks", "x: missing mass - y: reco jet mass (normed by x distribution); m_{miss} [GeV]; m_{jet,reco} [GeV]", 25, 0, 1000, 30, 0, 60)); 
		add_new_TH2D("jet_vs_miss_mass_charm", 
					new TH2D("jet_vs_miss_mass_charm", "x: missing mass - y: reco jet mass (normed by x distribution); m_{miss} [GeV]; m_{jet,reco} [GeV]", 25, 0, 1000, 30, 0, 60)); 
		add_new_TH2D("jet_vs_miss_mass_bottom", 
					new TH2D("jet_vs_miss_mass_bottom", "x: missing mass - y: reco jet mass (normed by x distribution); m_{miss} [GeV]; m_{jet,reco} [GeV]", 25, 0, 1000, 30, 0, 60)); 
		add_new_TH2D("jet_vs_miss_mass_photon", 
					new TH2D("jet_vs_miss_mass_photon", "x: missing mass - y: reco jet mass (normed by x distribution); m_{miss} [GeV]; m_{jet,reco} [GeV]", 25, 0, 1000, 30, 0, 60)); 
		add_new_TH2D("jet_vs_miss_mass_gluon", 
					new TH2D("jet_vs_miss_mass_gluon", "x: missing mass - y: reco jet mass (normed by x distribution); m_{miss} [GeV]; m_{jet,reco} [GeV]", 25, 0, 1000, 30, 0, 60)); 
		add_new_TH2D("jet_vs_miss_mass_lepton", 
					new TH2D("jet_vs_miss_mass_lepton", "x: missing mass - y: reco jet mass (normed by x distribution); m_{miss} [GeV]; m_{jet,reco} [GeV]", 25, 0, 1000, 30, 0, 60)); 


		add_new_TH2D("jet_mass_vs_miss_cosTheta_light_quarks", 
					new TH2D("jet_mass_vs_miss_theta_light_quarks", "x: missing theta - y: reco jet mass (normed by x distribution); #theta_{miss} [GeV]; m_{jet,reco} [GeV]", 10, -1, 1, 30, 0, 60)); 
		add_new_TH2D("jet_mass_vs_miss_cosTheta_charm", 
					new TH2D("jet_mass_vs_miss_theta_charm", "x: missing theta - y: reco jet mass (normed by x distribution); #theta_{miss} [GeV]; m_{jet,reco} [GeV]", 10, -1, 1, 30, 0, 60)); 
		add_new_TH2D("jet_mass_vs_miss_cosTheta_bottom", 
					new TH2D("jet_mass_vs_miss_theta_bottom", "x: missing theta - y: reco jet mass (normed by x distribution); #theta_{miss} [GeV]; m_{jet,reco} [GeV]", 10, -1, 1, 30, 0, 60)); 
		add_new_TH2D("jet_mass_vs_miss_cosTheta_photon", 
					new TH2D("jet_mass_vs_miss_theta_photon", "x: missing theta - y: reco jet mass (normed by x distribution); #theta_{miss} [GeV]; m_{jet,reco} [GeV]", 10, -1, 1, 30, 0, 60)); 
		add_new_TH2D("jet_mass_vs_miss_cosTheta_gluon", 
					new TH2D("jet_mass_vs_miss_theta_gluon", "x: missing theta - y: reco jet mass (normed by x distribution); #theta_{miss} [GeV]; m_{jet,reco} [GeV]", 10, -1, 1, 30, 0, 60)); 
		add_new_TH2D("jet_mass_vs_miss_cosTheta_lepton", 
					new TH2D("jet_mass_vs_miss_theta_lepton", "x: missing theta - y: reco jet mass (normed by x distribution); #theta_{miss} [GeV]; m_{jet,reco} [GeV]", 10, -1, 1, 30, 0, 60)); 


		

	}

	void fill_plots(){

		//for normalization over x distribution:
		TH1D* miss_mass_h1 = new TH1D("miss_mass_h1", "", 25, 0, 1000);
		TTree* tree = get_current_tree();
		tree->Draw("miss_mass>>miss_mass_h1", "");
		TAxis *xaxis_mass = miss_mass_h1->GetXaxis();

		TH1D* miss_cosTheta_h1 = new TH1D("miss_cosTheta_h1", "", 10, -1, 1);
		tree->Draw("cosTheta_miss>>miss_cosTheta_h1", "");
		TAxis *xaxis_cosTheta = miss_cosTheta_h1->GetXaxis();


		float weight = get_current_weight();

		// TODO For all samles: collect all masses of FEs -> draw lines in TH1D

		// This is the loop over all events
		while ( get_next_event() ) {

			if ( pass_selection == 0 ) { continue; }

			//for normalization over x distribution:
			float N_in_miss_mass_bin = miss_mass_h1->GetBinContent(xaxis_mass->FindBin(miss_mass));
			float N_in_miss_mass_total = miss_mass_h1->GetEntries();
			float x_distribution_weight_mass = 0;
			if ( N_in_miss_mass_bin > 0 ) {
		 		x_distribution_weight_mass = float(N_in_miss_mass_total)/float(N_in_miss_mass_bin);
			}
			float N_in_miss_cosTheta_bin = miss_cosTheta_h1->GetBinContent(xaxis_cosTheta->FindBin(cosTheta_miss));
			float N_in_miss_cosTheta_total = miss_cosTheta_h1->GetEntries();
			float x_distribution_weight_cosTheta = 0;
			if ( N_in_miss_cosTheta_bin > 0 ) {
		 		x_distribution_weight_cosTheta = float(N_in_miss_cosTheta_total)/float(N_in_miss_cosTheta_bin);
			}


			int number_heavy_quarks = 0;
			for ( int i=0; i<max_Njets; i++ ) {
				if (TJ_jets_exists[i] == 0) { break; }

				float abs_final_elementon_pdgID = fabs( TJ_jets_final_elementon_pdgID[i] );

				string particle_name;
				if ( (abs_final_elementon_pdgID == 1) ||  (abs_final_elementon_pdgID == 2) || (abs_final_elementon_pdgID == 3) ) {
					particle_name = "light_quarks";
				}
				else if ( abs_final_elementon_pdgID == 4 ) { particle_name = "charm"; } 
				else if ( abs_final_elementon_pdgID == 5 ) { particle_name = "bottom"; } 
				else if ( abs_final_elementon_pdgID == 22 ) { particle_name = "photon"; } 
				else if ( abs_final_elementon_pdgID == 21 ) { particle_name = "gluon"; } 
				else if ( (abs_final_elementon_pdgID > 10) && (abs_final_elementon_pdgID < 19) ) {
					particle_name = "lepton";
				}
				else { particle_name = "other"; continue; } 

				get_TH2D( ("jet_vs_miss_mass_" + particle_name).c_str() )->Fill( miss_mass, TJ_jets_detected_mass[i], weight * x_distribution_weight_mass);
				get_TH2D( ("jet_mass_vs_miss_cosTheta_" + particle_name).c_str() )->Fill( cosTheta_miss, TJ_jets_detected_mass[i], weight * x_distribution_weight_cosTheta);
			}  
		
		}
	}

	void draw_plots(){

		vector<string> particle_names = {"light_quarks", "charm", "bottom", "gluon"};

		vector<array<float, 3>> colors {{27,158,119}, {217,95,2}, {117,112,179}, {231,41,138}};
//{27,158,119}, {217,95,2}, {117,112,179}, {231,41,138}, {102,166,30}, {230,171,2}};
		for (int j=0; j<colors.size(); j++) {
		    TColor *h2_color = new TColor(9100+j, colors[j][0]/256.0, colors[j][1]/256.0, colors[j][2]/256.0);
    	}                                                                                              

		for (int i=0; i<particle_names.size(); i++) {
			string particle_name = particle_names[i];
			
			TCanvas *canvas_m_particle = new TCanvas( ("can_miss_mass_" + particle_name).c_str(), "", 0, 0, 650, 600);
			TH2D* h2_m = get_TH2D(("jet_vs_miss_mass_" + particle_name).c_str());
			h2_m->SetLineColor(9100+i);
			h2_m->Draw("same");
			h2_m->Draw("colz");
			canvas_m_particle->SetRightMargin((650-0.92*600.)/650.);
			string plot_name_m = get_output_directory() + "/jet_mass_miss_mass_" + particle_name +  ".pdf";
			canvas_m_particle->Print(plot_name_m.c_str());
			canvas_m_particle->Close();



			TCanvas *canvas_theta_particle = new TCanvas( ("can_miss_theta_" + particle_name).c_str(), "", 0, 0, 650, 600);
			TH2D* h2_theta = get_TH2D(("jet_mass_vs_miss_cosTheta_" + particle_name).c_str());
			h2_theta->SetLineColor(9100+i);
			h2_theta->Draw("same");
			h2_theta->Draw("colz");
			canvas_theta_particle->SetRightMargin((650-0.92*600.)/650.);
			string plot_name_theta = get_output_directory() + "/jet_mass_miss_cosTheta_" + particle_name +  ".pdf";
			canvas_theta_particle->Print(plot_name_theta.c_str());
			canvas_theta_particle->Close();

		}


	};
};
