#include "../include/plotter.h"

// Show all plots for events in which at least one of the masses falls into one of the tails

class AllSignalsPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("all_signals_plots");	
	}

	void define_plots(){
		add_new_TH1D("true_evt_type", new TH1D("true_evt_type", "; evtType; Events", 3, 0, 3) );
		add_new_TH1D("true_pair1_mass", new TH1D("true_pair1_mass", "; m_{qq1,gen}; Events", 75, 0, 150) );	
		add_new_TH1D("true_pair2_mass", new TH1D("true_pair2_mass", "; m_{qq2,gen} ; Events", 75, 0, 150) );	
		add_new_TH1D("true_pair1_theta", new TH1D("true_pair1_theta", "; #theta_{qq1,gen} ; Events", 32, 0, 3.2) );
		add_new_TH1D("true_pair2_theta", new TH1D("true_pair2_theta", "; #theta_{qq2,gen} ; Events", 32, 0, 3.2) );
		add_new_TH1D("true_pair_theta", new TH1D("true_pair_theta", "; #theta_{qq,gen} ; (2 Bosons) * Events", 32, 0, 3.2) );
    add_new_TH1D("true_pair_E", new TH1D("true_pair_E", "; E_{qq,gen}; (2 Bosons) * Events", 300, 0, 600 ) );	
		add_new_TH1D("true_miss_mass", new TH1D("true_miss_mass", "; m_{miss,gen}; Events", 250, 0, 1000) );
		add_new_TH1D("true_sum_E_q", new TH1D("true_sum_E_q", "; #sum E_{q,gen}; Events", 250, 0, 1000) );
		add_new_TH1D("true_ISR_inJets_E", new TH1D("true_ISR_inJets_E", "; E_{ISRinJets,gen}; Events", 50, 0, 100) );;
		add_new_TH1D("true_ISR_inJets_p", new TH1D("true_ISR_inJets_p", "; p_{ISRinJets,gen}; Events", 25, 0, 50) );;
		add_new_TH1D("true_ISR_inJets_theta", new TH1D("true_ISR_inJets_theta", "; #theta_{ISRinJets,gen} ; Events", 32, 0, 3.2) );
		add_new_TH1D("NPFOs_inCorrectJets", new TH1D("NPFOs_inCorrectJets", "; NPFOs in CorrectJets ; Events", 80, 0, 160) );	
		add_new_TH1D("NPFOs_inClusteredJets", new TH1D("NPFOs_inClusteredJets", "; NPFOs in ClusteredJets ; Events", 80, 0, 160) );
		add_new_TH1D("NPFOs_inCorrectJets_notinClusteredJets", new TH1D("NPFOs_inCorrectJets_notinClusteredJets", "; NPFOs_inCorrectJets_notinClusteredJets; Events", 40, 0, 40) );
		add_new_TH1D("NPFOs_inClusteredJets_notinCorrectJets", new TH1D("NPFOs_inClusteredJets_notinCorrectJets", "; NPFOs_inClusteredJets_notinCorrectJets ; Events", 40, 0, 40) );
		add_new_TH1D("pair1_mass", new TH1D("pair1_mass", "; m_{jj1}; Events", 125, 0, 250) );	
		add_new_TH1D("pair2_mass", new TH1D("pair2_mass", "; m_{jj2} ; Events", 125, 0, 250) );	
		add_new_TH1D("pair1_theta", new TH1D("pair1_theta", "; #theta_{jj1} ; Events", 32, 0, 3.2) );
		add_new_TH1D("pair2_theta", new TH1D("pair2_theta", "; #theta_{jj2} ; Events", 32, 0, 3.2) );
		add_new_TH1D("pair_theta", new TH1D("pair_theta", "; #theta_{jj} ; (2 Bosons) * Events", 32, 0, 3.2) );
		add_new_TH1D("miss_mass", new TH1D("miss_mass", "; m_{miss}; Events", 250, 0, 1000) );
		add_new_TH1D("sum_E_q", new TH1D("sum_E_q", "; #sum E_{q}; Events", 250, 0, 1000) );
		add_new_TH1D("pair1_p", new TH1D("pair1_p", "; p_{jj1}; Events", 300, 0, 600 ) );	
		add_new_TH1D("pair2_p", new TH1D("pair2_p", "; p_{jj2}; Events", 300, 0, 600 ) );	
		add_new_TH1D("pair_E", new TH1D("pair_E", "; E_{jj}; (2 Bosons) * Events", 300, 0, 600 ) );	
		add_new_TH1D("E_T_vis", new TH1D("E_T_vis", "; E_{T,vis}; Events", 500, 0, 1000) );
		add_new_TH1D("p_T_vis", new TH1D("p_T_vis", "; ; Events", 250, 0, 500 ) );
		add_new_TH1D("E_10cone_EmaxTrack", new TH1D("E_10cone_EmaxTrack", "; E_{10cone,maxEtrack} ; Events", 400, 0, 800) );
		add_new_TH1D("cosTheta_miss", new TH1D("cosTheta_miss", "; cos(#theta_{miss}); Events", 20, -1, 1) );
		add_new_TH1D("cosTheta_EmaxTrack", new TH1D("cosTheta_EmaxTrack", "; cos(#theta_{maxEtrack} ; Events", 20, -1, 1) );
		add_new_TH1D("y_34", new TH1D("y_34", "; y_{34}; Events", 40, 0, 0.04) );
		add_new_TH1D("min_Ejet", new TH1D("min_Ejet", "; E_{jet,min}; Events", 75, 0, 150) );
		add_new_TH1D("min_num_particles_jet", new TH1D("min_num_particles_jet", "; minNumParticlesInAJet ; Events", 30, 0, 30) );
		add_new_TH1D("min_num_chargedparticles_jet", new TH1D("min_num_chargedparticles_jet", "; minNumChargedParticlesInAJet; Events", 15, 0, 15) );
		add_new_TH1D("num_isolated_leptons", new TH1D("num_isolated_leptons", "; numIsolatedLeptons ; Events", 4 , 0, 4) );

	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {

			if (true_evt_type == 0) { continue; }
			get_TH1D("true_evt_type")->Fill(true_evt_type, weight);
			get_TH1D("true_pair1_mass")->Fill(true_pair1_mass, weight);
			get_TH1D("true_pair2_mass")->Fill(true_pair2_mass, weight);
			get_TH1D("true_pair1_theta")->Fill(true_pair1_theta, weight);
			get_TH1D("true_pair2_theta")->Fill(true_pair2_theta, weight);
      get_TH1D("true_pair_theta")->Fill(true_pair1_theta, weight);
      get_TH1D("true_pair_theta")->Fill(true_pair2_theta, weight);
      get_TH1D("true_pair_E")->Fill( TMath::Sqrt( TMath::Sq(true_pair1_p) + TMath::Sq(true_pair1_mass) ), weight);
      get_TH1D("true_pair_E")->Fill( TMath::Sqrt( TMath::Sq(true_pair2_p) + TMath::Sq(true_pair2_mass) ), weight);
			get_TH1D("true_miss_mass")->Fill(true_miss_mass, weight);
			get_TH1D("true_sum_E_q")->Fill(true_sum_E_q, weight);
			get_TH1D("true_ISR_inJets_E")->Fill(true_ISR_inJets_E, weight);
			get_TH1D("true_ISR_inJets_p")->Fill(true_ISR_inJets_p, weight);
			get_TH1D("true_ISR_inJets_theta")->Fill(true_ISR_inJets_theta, weight);
			get_TH1D("NPFOs_inCorrectJets")->Fill(NPFOs_inCorrectJets, weight);
			get_TH1D("NPFOs_inClusteredJets")->Fill(NPFOs_inClusteredJets, weight);
			get_TH1D("NPFOs_inCorrectJets_notinClusteredJets")->Fill(NPFOs_inCorrectJets_notinClusteredJets, weight);
			get_TH1D("NPFOs_inClusteredJets_notinCorrectJets")->Fill(NPFOs_inClusteredJets_notinCorrectJets, weight);
			get_TH1D("pair1_mass")->Fill(pair1_mass, weight);
			get_TH1D("pair2_mass")->Fill(pair2_mass, weight);
			get_TH1D("pair1_theta")->Fill(pair1_theta, weight);
			get_TH1D("pair2_theta")->Fill(pair2_theta, weight);
			get_TH1D("pair_theta")->Fill(pair1_theta, weight);
			get_TH1D("pair_theta")->Fill(pair2_theta, weight);
			get_TH1D("miss_mass")->Fill(miss_mass, weight);
			get_TH1D("sum_E_q")->Fill(sum_E_q, weight);
			get_TH1D("pair1_p")->Fill(pair1_p, weight);
			get_TH1D("pair2_p")->Fill(pair2_p, weight);
			get_TH1D("pair_E")->Fill( TMath::Sqrt( TMath::Sq(pair1_p) + TMath::Sq(pair1_mass) ), weight);
			get_TH1D("pair_E")->Fill( TMath::Sqrt( TMath::Sq(pair2_p) + TMath::Sq(pair2_mass) ), weight);
			get_TH1D("E_T_vis")->Fill(E_T_vis, weight);
			get_TH1D("p_T_vis")->Fill(p_T_vis, weight);
			get_TH1D("E_10cone_EmaxTrack")->Fill(E_10cone_EmaxTrack, weight);
			get_TH1D("cosTheta_miss")->Fill(cosTheta_miss, weight);
			get_TH1D("cosTheta_EmaxTrack")->Fill(cosTheta_EmaxTrack, weight);
			get_TH1D("y_34")->Fill(y_34, weight);
			get_TH1D("min_Ejet")->Fill(min_Ejet, weight);
			get_TH1D("min_num_particles_jet")->Fill(min_num_particles_jet, weight);
			get_TH1D("min_num_chargedparticles_jet")->Fill(min_num_chargedparticles_jet, weight);
			get_TH1D("num_isolated_leptons")->Fill(num_isolated_leptons, weight);
		}
	}

	void draw_plots(){
		for (int i=0; i<get_number_TH1Ds(); i++) {
			TCanvas *canvas_h1 = new TCanvas("can_h1", "", 0, 0, 800, 800);
			TH1D* h1 = get_TH1D_i(i);
			h1->Draw("hist e");
			string plot_name_h1 = get_output_directory() + "/" + h1->GetName() + ".pdf";
			canvas_h1->Print(plot_name_h1.c_str());
			canvas_h1->Close();
		}
	}
};
