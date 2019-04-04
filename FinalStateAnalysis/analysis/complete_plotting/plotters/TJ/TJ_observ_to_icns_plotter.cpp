#include "../../include/plotter.h"

// Very simple example

class TJObservToICNsPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_observ_to_icns");
	}

	void define_plots(){
		add_new_TH1D("m_WW", new TH1D("m_WW", "using reco of inital cns, w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ", new TH1D("m_ZZ", "using reco of inital cns, w/ cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW", new TH2D("m_m_WW", "using reco of inital cns, w/ cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ", new TH2D("m_m_ZZ", "using reco of inital cns, w/ cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));

		add_new_TH1D("m_WW_nocuts", new TH1D("m_WW_nocuts", "using reco of inital cns, no cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_nocuts", new TH1D("m_ZZ_nocuts", "using reco of inital cns, no cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_nocuts", new TH2D("m_m_WW_nocuts", "using reco of inital cns, no cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_nocuts", new TH2D("m_m_ZZ_nocuts", "using reco of inital cns, no cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));

		add_new_TH1D("m_WW_custom_pairing_nocuts", new TH1D("m_WW_custom_pairing_nocuts", "using reco of inital cns, custom pairing, no cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_custom_pairing_nocuts", new TH1D("m_ZZ_custom_pairing_nocuts", "using reco of inital cns, custom pairing, no cuts; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_custom_pairing_nocuts", new TH2D("m_m_WW_custom_pairing_nocuts", "using reco of inital cn custom pairing, no cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_custom_pairing_nocuts", new TH2D("m_m_ZZ_custom_pairing_nocuts", "using reco of inital cn custom pairing, no cuts; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));

		add_new_TH1D("m_WW_cheated_overlay_nocuts", new TH1D("m_WW_cheated_overlay_nocuts", "using TJ to cheat overlay removal; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH1D("m_ZZ_cheated_overlay_nocuts", new TH1D("m_ZZ_cheated_overlay_nocuts", "using TJ to cheat overlay removal; (m_{jj,1} + m_{jj,2})/2 [GeV]; Events", 70, 50, 120));
		add_new_TH2D("m_m_WW_cheated_overlay_nocuts", new TH2D("m_m_WW_cheated_overlay_nocuts", "using TJ to cheat overlay removal; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));
		add_new_TH2D("m_m_ZZ_cheated_overlay_nocuts", new TH2D("m_m_ZZ_cheated_overlay_nocuts", "using TJ to cheat overlay removal; m_{jj,1} [GeV];  m_{jj,2} [GeV]", 35, 50, 120, 35, 50, 120));

	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
			if ( true_evt_type == 1 ) {
				get_TH1D("m_WW_nocuts")->Fill((TJ_pair1_mass_from_icn+TJ_pair2_mass_from_icn)/2., weight);
				get_TH2D("m_m_WW_nocuts")->Fill(TJ_pair1_mass_from_icn, TJ_pair2_mass_from_icn, weight);
        get_TH1D("m_WW_custom_pairing_nocuts")->Fill((TJ_pair1_mass_from_icn_custom_pairing+TJ_pair2_mass_from_icn_custom_pairing)/2., weight);
				get_TH2D("m_m_WW_custom_pairing_nocuts")->Fill(TJ_pair1_mass_from_icn_custom_pairing, TJ_pair2_mass_from_icn_custom_pairing, weight);
        get_TH1D("m_WW_cheated_overlay_nocuts")->Fill((TJ_pair1_mass_cheated_overlay+TJ_pair2_mass_cheated_overlay)/2., weight);
				get_TH2D("m_m_WW_cheated_overlay_nocuts")->Fill(TJ_pair1_mass_cheated_overlay, TJ_pair2_mass_cheated_overlay, weight);
				if ( pass_selection == 1 ) {
					get_TH1D("m_WW")->Fill((TJ_pair1_mass_from_icn+TJ_pair2_mass_from_icn)/2., weight);
					get_TH2D("m_m_WW")->Fill(TJ_pair1_mass_from_icn, TJ_pair2_mass_from_icn, weight);
				}
			}
			else if ( true_evt_type == 2 ) {
				get_TH1D("m_ZZ_nocuts")->Fill((TJ_pair1_mass_from_icn+TJ_pair2_mass_from_icn)/2., weight);
				get_TH2D("m_m_ZZ_nocuts")->Fill(TJ_pair1_mass_from_icn, TJ_pair2_mass_from_icn, weight);
        get_TH1D("m_ZZ_custom_pairing_nocuts")->Fill((TJ_pair1_mass_from_icn_custom_pairing+TJ_pair2_mass_from_icn_custom_pairing)/2., weight);
        get_TH2D("m_m_ZZ_custom_pairing_nocuts")->Fill(TJ_pair1_mass_from_icn_custom_pairing, TJ_pair2_mass_from_icn_custom_pairing, weight);
        get_TH1D("m_ZZ_cheated_overlay_nocuts")->Fill((TJ_pair1_mass_cheated_overlay+TJ_pair2_mass_cheated_overlay)/2., weight);
        get_TH2D("m_m_ZZ_cheated_overlay_nocuts")->Fill(TJ_pair1_mass_cheated_overlay, TJ_pair2_mass_cheated_overlay, weight);
				if ( pass_selection == 1 ) {
					get_TH1D("m_ZZ")->Fill((TJ_pair1_mass_from_icn+TJ_pair2_mass_from_icn)/2., weight);
					get_TH2D("m_m_ZZ")->Fill(TJ_pair1_mass_from_icn, TJ_pair2_mass_from_icn, weight);
				}
			}
		}
	}

	void draw_plots(){

		TH1D* h1_WW = get_TH1D("m_WW");
		TH1D* h1_ZZ = get_TH1D("m_ZZ");
		h1_WW->SetLineColor(4);
		h1_ZZ->SetLineColor(2);


		TCanvas *canvas_h1 = new TCanvas("can_h1", "", 0, 0, 600, 600);
		h1_WW->Draw("hist");
		h1_ZZ->Draw("hist same");

		draw_mass_line("W", "DBD", "y");
		draw_mass_line("Z", "DBD", "y");

		string plot_name_h1 = get_output_directory() + "/m.pdf";
		canvas_h1->Print(plot_name_h1.c_str());
		canvas_h1->Close();


		TH2D* h2_WW = get_TH2D("m_m_WW");
		TH2D* h2_ZZ = get_TH2D("m_m_ZZ");
		h2_WW->Scale(1./ h2_WW->Integral());
		h2_ZZ->Scale(1./ h2_ZZ->Integral());

		h2_WW->SetLineColor(4);
		h2_ZZ->SetLineColor(2);

		TCanvas *canvas_h2 = new TCanvas("can_h2", "", 0, 0, 800, 800);
		h2_WW->Draw("box");
		h2_ZZ->Draw("box same");

		draw_mass_line("W", "DBD", "x");
		draw_mass_line("Z", "DBD", "x");
		draw_mass_line("W", "DBD", "y");
		draw_mass_line("Z", "DBD", "y");

		string plot_name_h2 = get_output_directory() + "/m_m.pdf";
		canvas_h2->Print(plot_name_h2.c_str());
		canvas_h2->Close();
	};
};
