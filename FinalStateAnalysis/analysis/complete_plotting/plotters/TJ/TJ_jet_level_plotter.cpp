#include "../../include/plotter.h"

class TJJetLevelsPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_jet_levels");	
		set_1D_array_reading( true );
	}

	void define_plots(){
		vector<string> particle_names = {"light_quarks", "charm", "bottom", "photon", "gluon", "lepton"};


		for (int i=0; i<particle_names.size(); i++) {
			string particle_name = particle_names[i];


			add_new_TH1D(("tos_mass_" + particle_name).c_str(), 
					new TH1D(("trueofseen_mass_" + particle_name).c_str(), "true values of detected; m_{jet,detected particles true values} [GeV]; normalized", 25, 0, 150)); 
			add_new_TH1D(("tos_E_" + particle_name).c_str(), 
					new TH1D(("trueofseen_E_" + particle_name).c_str(), "true values of detected; E_{jet,detected particles true values} [GeV]; normalized", 40, 0, 400)); 
			add_new_TH1D(("tos_p_" + particle_name).c_str(), 
					new TH1D(("trueofseen_p_" + particle_name).c_str(), "true values of detected; p_{jet,detected particles true values} [GeV]; normalized", 40, 0, 400)); 


			add_new_TH2D(("s_tos_mass_" + particle_name).c_str(), 
					new TH2D(("detected_vs_trueofseen_mass_" + particle_name).c_str(), "x: true values of detected - y: reco values of detected; m_{detected particles true values}^{jet} [GeV]; m_{detected particles reco values}^{jet} [GeV]", 1200, 0, 60, 1200, 0, 60)); 


			add_new_TProfile(("profile_s_tos_mass_" + particle_name).c_str(), 
					new TProfile(("profile_detected_vs_trueofseen_mass_" + particle_name).c_str(), "x: true values of detected - y: reco values of detected; m_{detected particles true values}^{jet} [GeV]; m_{detected particles reco values}^{jet} [GeV]", 75, 0, 150)); 

			add_new_TProfile(("profile_s_tos_E_" + particle_name).c_str(), 
					new TProfile(("profile_detected_vs_trueofseen_E_" + particle_name).c_str(), "x: true values of detected - y: reco values of detected; E_{detected particles true values}^{jet} [GeV]; E_{detected particles reco values}^{jet} [GeV]", 75, 0, 150)); 

			add_new_TProfile(("profile_s_tos_p_" + particle_name).c_str(), 
					new TProfile(("profile_detected_vs_trueofseen_p_" + particle_name).c_str(), "x: true values of detected - y: reco values of detected; p_{detected particles true values}^{jet} [GeV]; p_{detected particles reco values}^{jet} [GeV]", 75, 0, 150)); 


			add_new_TProfile(("profile_s_t_mass_" + particle_name).c_str(), 
					new TProfile(("profile_detected_vs_true_mass_" + particle_name).c_str(), "x: true values - y: reco; m_{true}^{jet} [GeV]; m_{reco}^{jet} [GeV]", 75, 0, 150)); 

			add_new_TProfile(("profile_s_t_E_" + particle_name).c_str(), 
					new TProfile(("profile_detected_vs_true_E_" + particle_name).c_str(), "x: true - y: reco; E_{true}^{jet} [GeV]; E_{reco}^{jet} [GeV]", 75, 0, 150)); 

			add_new_TProfile(("profile_s_t_p_" + particle_name).c_str(), 
					new TProfile(("profile_detected_vs_true_p_" + particle_name).c_str(), "x: true - y: reco ; p_{true}^{jet} [GeV]; p_{reco}^{jet} [GeV]", 75, 0, 150)); 

		}
	}

	void fill_plots(){
		float weight = get_current_weight();

		// TODO For all samles: collect all masses of FEs -> draw lines in TH1D

		// This is the loop over all events
		while ( get_next_event() ) {

			//if ( pass_selection == 0 ) { continue; }

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

				get_TH1D( ("tos_mass_" + particle_name).c_str() )->Fill( TJ_jets_true_of_detectable_mass[i], weight); 
				get_TH1D( ("tos_E_" + particle_name).c_str() )->Fill( TJ_jets_true_of_detectable_E[i], weight); 
				get_TH1D( ("tos_p_" + particle_name).c_str() )->Fill( TJ_jets_true_of_detectable_p[i], weight); 
				get_TH2D( ("s_tos_mass_" + particle_name).c_str() )->Fill( TJ_jets_true_of_detectable_mass[i], TJ_jets_detected_mass[i], weight);
				get_TProfile( ("profile_s_tos_mass_" + particle_name).c_str() )->Fill( TJ_jets_true_of_detectable_mass[i], TJ_jets_detected_mass[i], weight);
				get_TProfile( ("profile_s_tos_E_" + particle_name).c_str() )->Fill( TJ_jets_true_of_detectable_E[i], TJ_jets_detected_E[i], weight);
				get_TProfile( ("profile_s_tos_p_" + particle_name).c_str() )->Fill( TJ_jets_true_of_detectable_p[i], TJ_jets_detected_p[i], weight);

				get_TProfile( ("profile_s_t_mass_" + particle_name).c_str() )->Fill( TJ_jets_true_mass[i], TJ_jets_detected_mass[i], weight);
				get_TProfile( ("profile_s_t_E_" + particle_name).c_str() )->Fill( TJ_jets_true_E[i], TJ_jets_detected_E[i], weight);
				get_TProfile( ("profile_s_t_p_" + particle_name).c_str() )->Fill( TJ_jets_true_p[i], TJ_jets_detected_p[i], weight);

			}  
		
		}
	}

	void draw_plots(){

		vector<string> particle_names = {"light_quarks", "charm", "bottom"};

		vector<string> variables = {"mass", "E", "p"};

		vector<array<float, 3>> colors {{27,158,119}, {217,95,2}, {117,112,179}, {231,41,138}};
//{27,158,119}, {217,95,2}, {117,112,179}, {231,41,138}, {102,166,30}, {230,171,2}};
		for (int j=0; j<colors.size(); j++) {
		    TColor *h2_color = new TColor(9100+j, colors[j][0]/256.0, colors[j][1]/256.0, colors[j][2]/256.0);
    	}                                                                                              

		vector<TCanvas*> tos_h1_canvases;
		vector<THStack*> tos_h1_stacks;
		vector<TLegend*> tos_legends;
		for (int j=0; j<variables.size(); j++) {
			tos_h1_canvases.push_back(new TCanvas( ("can_tos_" + variables[j]).c_str(), "", 0, 0, 600, 600) );
			tos_h1_stacks.push_back( new THStack( ( "hs_tos_" + variables[j]).c_str(), ("true values of detected; " + variables[j] + "^{jet}_{detected particles true values} [GeV]; normalized").c_str() ) );
			tos_legends.push_back(new TLegend(0.7, 0.6, 0.9, 0.9));
		}

		TCanvas *canvas_s_tos = new TCanvas( "can_s_tos", "", 0, 0, 600, 600);

		TLegend* legend_s_tos = new TLegend(0.7, 0.6, 0.9, 0.9);

		for (int i=0; i<particle_names.size(); i++) {
			string particle_name = particle_names[i];
			
			for (int j=0; j<variables.size(); j++) {
				tos_h1_canvases[j]->cd();
				TH1D* h1_tos = get_TH1D(("tos_" + variables[j] + "_" + particle_name).c_str());
				h1_tos->Scale(1. / h1_tos->Integral());
				h1_tos->SetLineColor(9100+i);
				tos_h1_stacks[j]->Add(h1_tos);
				tos_legends[j]->AddEntry(h1_tos, particle_name.c_str(), "l"); 
			}

			canvas_s_tos->cd();
			TH2D* h2_s_tos = get_TH2D(("s_tos_mass_" + particle_name).c_str());
			h2_s_tos->SetLineColor(9100+i);
			h2_s_tos->SetMarkerColor(9100+i);
			h2_s_tos->SetMarkerSize(0.1);
			legend_s_tos->AddEntry(h2_s_tos, particle_name.c_str(), "l"); 
			h2_s_tos->Draw("same");

			TCanvas *canvas_particle = new TCanvas( ("can_s_tos_" + particle_name).c_str(), "", 0, 0, 650, 600);
			TH2D* h2_s_tos_clone = (TH2D*)h2_s_tos->Clone();
			h2_s_tos_clone->Rebin2D(20, 20);
			h2_s_tos_clone->Draw("colz");
			TLine* m_m_line = new TLine(0, 0, 60, 60);
			m_m_line->SetLineWidth(1);
			m_m_line->Draw("same");
			canvas_particle->SetRightMargin((650-0.92*600.)/650.);
			string plot_name = get_output_directory() + "/" + particle_name +  ".pdf";
			canvas_particle->Print(plot_name.c_str());
			canvas_particle->Close();


		}

		for (int j=0; j<variables.size(); j++) {
			TCanvas *canvas_tos = tos_h1_canvases[j];
			canvas_tos->cd();
			tos_h1_stacks[j]->Draw("hist nostack");
			tos_legends[j]->Draw("same");
			string plot_name_tos = get_output_directory() + "/trueofseen_" + variables[j] +  ".pdf";
			canvas_tos->Print(plot_name_tos.c_str());
			canvas_tos->Close();
		}

		canvas_s_tos->cd();
		//canvas_s_tos->SetLogx();
		//canvas_s_tos->SetLogy();
		TLine* m_m_line = new TLine(0, 0, 60, 60);
		m_m_line->SetLineWidth(1);
		m_m_line->Draw("same");
		legend_s_tos->Draw();
		string plot_name_s_tos = get_output_directory() + "/detected_vs_trueofseen_mass.pdf";
		canvas_s_tos->Print(plot_name_s_tos.c_str());
		canvas_s_tos->Close();





		for (int j=0; j<variables.size(); j++) {

			TCanvas *canvas_profile_s_tos = new TCanvas(("can_profile_s_tos" + variables[j]).c_str(), "", 0, 0, 600, 600);
			TLegend* legend_profile_s_tos = new TLegend(0.25, 0.6, 0.45, 0.9);

			for (int i=0; i<particle_names.size(); i++) {
				string particle_name = particle_names[i];


				canvas_profile_s_tos->cd();
				TProfile* profile_s_tos = get_TProfile(("profile_s_tos_" + variables[j] + "_" + particle_name).c_str());
				profile_s_tos->SetLineColor(9100+i);
				profile_s_tos->SetMarkerColor(9100+i);
				// profile_s_tos->SetMarkerSize(0.1);
				legend_profile_s_tos->AddEntry(profile_s_tos, particle_name.c_str(), "lp"); 
				profile_s_tos->Draw("same");

			}

			canvas_profile_s_tos->cd();
			gPad->Update();
			TLine* profile_line = new TLine(0, 0, gPad->GetFrame()->GetY2(), gPad->GetFrame()->GetY2());
			profile_line->SetLineWidth(1);
			profile_line->Draw("same");
			legend_profile_s_tos->Draw();
			string plot_name_profile_s_tos = get_output_directory() + "/profile_detected_vs_trueofseen_" +  variables[j] + ".pdf";
			canvas_profile_s_tos->Print(plot_name_profile_s_tos.c_str());
			canvas_profile_s_tos->Close();
		
		}


		for (int j=0; j<variables.size(); j++) {

			TCanvas *canvas_profile_s_t = new TCanvas(("can_profile_s_t" + variables[j]).c_str(), "", 0, 0, 600, 600);
			TLegend* legend_profile_s_t = new TLegend(0.25, 0.6, 0.45, 0.9);

			for (int i=0; i<particle_names.size(); i++) {
				string particle_name = particle_names[i];


				canvas_profile_s_t->cd();
				TProfile* profile_s_t = get_TProfile(("profile_s_t_" + variables[j] + "_" + particle_name).c_str());
				profile_s_t->SetLineColor(9100+i);
				profile_s_t->SetMarkerColor(9100+i);
				// profile_s_t->SetMarkerSize(0.1);
				legend_profile_s_t->AddEntry(profile_s_t, particle_name.c_str(), "lp"); 
				profile_s_t->Draw("same");
				profile_s_t->GetYaxis()->SetTitleOffset(1.38);

			}

			canvas_profile_s_t->cd();
			canvas_profile_s_t->SetLeftMargin(0.19);
			canvas_profile_s_t->SetFrameBorderMode(0);
			gPad->Modified();
			gPad->Update();
			double y_max = gPad->GetFrame()->GetY2();
			if ( y_max > 150 ) { y_max = 150; }
			TLine* profile_line = new TLine(0, 0, y_max, y_max);
			profile_line->SetLineWidth(1);
			profile_line->Draw("same");
			legend_profile_s_t->Draw();
			string plot_name_profile_s_t = get_output_directory() + "/profile_detected_vs_true_" +  variables[j] + ".pdf";
			canvas_profile_s_t->Print(plot_name_profile_s_t.c_str());
			canvas_profile_s_t->Close();
		
		}

	};
};
