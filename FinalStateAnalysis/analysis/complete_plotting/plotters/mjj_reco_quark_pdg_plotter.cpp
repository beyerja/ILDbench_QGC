#include "../include/plotter.h"

class RecoMjjVSQuarkPDGPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("mjj_reco_quark_pdg");	
	}

	void define_plots(){

		for (int j=0; j<5; j++) {
			string j_string = to_string(j);

			add_new_TH1D(("m_WW_" + j_string + "heavyq").c_str(), new TH1D(("m_WW_" + j_string + "heavyq").c_str(), "w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
			add_new_TH1D(("m_ZZ_" + j_string + "heavyq").c_str(), new TH1D(("m_ZZ_" + j_string + "heavyq").c_str(), "w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));

			add_new_TH1D(("thetaV_WW_" + j_string + "heavyq").c_str(), new TH1D(("thetaV_WW_" + j_string + "heavyq").c_str(), "w/ cuts; #theta_{V}; Events", 32, 0, 3.2));
			add_new_TH1D(("thetaV_ZZ_" + j_string + "heavyq").c_str(), new TH1D(("thetaV_ZZ_" + j_string + "heavyq").c_str(), "w/ cuts; #theta_{V}; Events", 32, 0, 3.2));
		}



		add_new_TH2D("mjj_number_heavyq_WW", new TH2D("mjj_number_heavyq_WW", "w/ cuts; Number of cb quarks; (m_{jj,1} + m_{jj,2})/2 [GeV]", 5, -0.5, 4.5, 35, 50, 120));
		add_new_TH2D("mjj_number_heavyq_ZZ", new TH2D("mjj_number_heavyq_ZZ", "w/ cuts; Number of cb quarks; (m_{jj,1} + m_{jj,2})/2 [GeV]", 5, -0.5, 4.5, 35, 50, 120));

		add_new_TH2D("mjj_number_c_WW", new TH2D("mjj_number_c_WW", "w/ cuts; Number of c quarks; (m_{jj,1} + m_{jj,2})/2 [GeV]", 5, -0.5, 4.5, 35, 50, 120));
		add_new_TH2D("mjj_number_c_ZZ", new TH2D("mjj_number_c_ZZ", "w/ cuts; Number of c quarks; (m_{jj,1} + m_{jj,2})/2 [GeV]", 5, -0.5, 4.5, 35, 50, 120));

		add_new_TH2D("mjj_number_b_WW", new TH2D("mjj_number_b_WW", "w/ cuts; Number of b quarks; (m_{jj,1} + m_{jj,2})/2 [GeV]", 5, -0.5, 4.5, 35, 50, 120));
		add_new_TH2D("mjj_number_b_ZZ", new TH2D("mjj_number_b_ZZ", "w/ cuts; Number of b quarks; (m_{jj,1} + m_{jj,2})/2 [GeV]", 5, -0.5, 4.5, 35, 50, 120));

	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {

			if ( pass_selection == 0 ) { continue; }

			int number_heavy_quarks = 0;
			int number_c_quarks = 0;
			int number_b_quarks = 0;

			for ( int i=0; i<4; i++ ) {
				if ( fabs(true_quarks_pdg_id[i]) > 3 )  { number_heavy_quarks++; }
				if ( fabs(true_quarks_pdg_id[i]) == 4 )  { number_c_quarks++; }
				if ( fabs(true_quarks_pdg_id[i]) == 5 )  { number_b_quarks++; }
			}  
		
			string number_heavy_quarks_string = to_string(number_heavy_quarks);

			if ( true_evt_type == 1 ) {
				get_TH1D( ("m_WW_" + number_heavy_quarks_string + "heavyq").c_str() )->Fill((pair1_mass+pair2_mass)/2., weight);
				get_TH1D( ("thetaV_WW_" + number_heavy_quarks_string + "heavyq").c_str() )->Fill(pair1_theta , weight);
				get_TH1D( ("thetaV_WW_" + number_heavy_quarks_string + "heavyq").c_str() )->Fill(pair2_theta , weight);
				get_TH2D("mjj_number_heavyq_WW")->Fill(number_heavy_quarks, (pair1_mass+pair2_mass)/2., weight);			
				get_TH2D("mjj_number_c_WW")->Fill(number_c_quarks, (pair1_mass+pair2_mass)/2., weight);	
				get_TH2D("mjj_number_b_WW")->Fill(number_b_quarks, (pair1_mass+pair2_mass)/2., weight);	
			}
			else if ( true_evt_type == 2 ) {
				get_TH1D( ("m_ZZ_" + number_heavy_quarks_string + "heavyq").c_str() )->Fill((pair1_mass+pair2_mass)/2., weight);
				get_TH1D( ("thetaV_ZZ_" + number_heavy_quarks_string + "heavyq").c_str() )->Fill(pair1_theta , weight);
				get_TH1D( ("thetaV_ZZ_" + number_heavy_quarks_string + "heavyq").c_str() )->Fill(pair2_theta , weight);
				get_TH2D("mjj_number_heavyq_ZZ")->Fill(number_heavy_quarks, (pair1_mass+pair2_mass)/2., weight);			
				get_TH2D("mjj_number_c_ZZ")->Fill(number_c_quarks, (pair1_mass+pair2_mass)/2., weight);	
				get_TH2D("mjj_number_b_ZZ")->Fill(number_b_quarks, (pair1_mass+pair2_mass)/2., weight);	
			}
		}
	}

	void draw_plots(){

		vector<string> reco_variables = {"m", "thetaV"};

		vector<int> line_styles = {1, 9, 10, 2, 4};
		vector<int> WW_colors = {4, 9, 38, 39, 40};
		vector<int> ZZ_colors = {2, 46, 45, 44, 42};

		for (int i=0; i<reco_variables.size(); i++) {

			TCanvas *canvas_h1 = new TCanvas( ("can_h1_" + reco_variables[i]).c_str(), "", 0, 0, 600, 600);
			TLegend* legend = new TLegend(0.8, 0.6, 0.9, 0.9);

			THStack* h1_histos = new THStack( ( reco_variables[i] + "_stack" ).c_str(), "w/ cuts" ); 

			for (int j=0; j<5; j++) {

				string j_string = to_string(j);

				TH1D* h1_WW_jheavyq = get_TH1D( ( reco_variables[i] + "_WW_" + j_string + "heavyq" ).c_str() ); 
				TH1D* h1_ZZ_jheavyq = get_TH1D( ( reco_variables[i] + "_ZZ_" + j_string + "heavyq" ).c_str() ); 

				h1_WW_jheavyq->SetLineColor(WW_colors[j]);
				h1_ZZ_jheavyq->SetLineColor(ZZ_colors[j]);

				//h1_WW_jheavyq->SetLineStyle(line_styles[j]);
				//h1_ZZ_jheavyq->SetLineStyle(line_styles[j]);

				legend->AddEntry(h1_WW_jheavyq, ("WW " + j_string + "cb").c_str(), "l"); 
				legend->AddEntry(h1_ZZ_jheavyq, ("ZZ " + j_string + "cb").c_str(), "l"); 

				//h1_WW_jheavyq->Draw("hist same");
				//h1_ZZ_jheavyq->Draw("hist same");
				h1_histos->Add(h1_WW_jheavyq);
				h1_histos->Add(h1_ZZ_jheavyq);

			}

			h1_histos->Draw("NOSTACK hist");
			legend->Draw();

			draw_mass_line("W", "DBD", "y");
			draw_mass_line("Z", "DBD", "y");

			string plot_name = get_output_directory() + "/" + reco_variables[i] + ".pdf";
			canvas_h1->Print(plot_name.c_str());
			canvas_h1->Close();
		}


		vector<string> counted_q_types = {"heavyq", "c", "b"};

		for (int i=0; i<counted_q_types.size(); i++) {
			string counted_q_type = counted_q_types[i];

			TH2D* h2_WW = get_TH2D(("mjj_number_" + counted_q_type + "_WW").c_str());	
			TH2D* h2_ZZ = get_TH2D(("mjj_number_" + counted_q_type + "_ZZ").c_str());
			h2_WW->Scale(1./ h2_WW->Integral());
			h2_ZZ->Scale(1./ h2_ZZ->Integral());
		
			h2_WW->SetLineColor(4);	
			h2_ZZ->SetLineColor(2);	
		
			TCanvas *canvas_h2 = new TCanvas("can_h2", "", 0, 0, 800, 800);
			h2_WW->Draw("box");
			h2_ZZ->Draw("box same");
		
			draw_mass_line("W", "DBD", "x");
			draw_mass_line("Z", "DBD", "x");
		
			string plot_name_h2 = get_output_directory() + "/N" + counted_q_type + "_mjj.pdf";
			canvas_h2->Print(plot_name_h2.c_str());
			canvas_h2->Close();
		}
	};
};
