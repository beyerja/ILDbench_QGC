#include "../../include/plotter.h"

// Very simple example
//
//

class TJCorrectedJetsPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("TJ/TJ_corrected_jets");	
	}

	void define_plots(){

		add_new_TH1D( "Ejet_tos_correction_distribution",
					new TH1D( "Ejet_tos_correction_distribution",
					"E_{jet} correction factor distribution;\
					correction factor; N_{jets}", 80, 0.9, 1.3 ) );

		add_new_TProfile( "Ejet_before_tos_correction",
					new TProfile( "Ejet_before_tos_correction", 
					"detected E_{jet} VS true of detected E_{jet};\
					E_{true of detected}^{jet}; E_{detected}^{jet}",
					150, 0, 600) );


		add_new_TProfile( "Ejet_after_tos_correction",
					new TProfile( "Ejet_after_tos_correction", 
					"corrected detected E_{jet} VS true of detected E_{jet};\
					E_{true of detected}^{jet}; E_{detected, corr}^{jet}",
					150, 0, 600) );


		add_new_TH1D( "pjet_tos_correction_distribution",
					new TH1D( "pjet_tos_correction_distribution",
					"p_{jet} correction factor distribution;\
					correction factor; N_{jets}", 80, 0.9, 1.3 ) );


		add_new_TProfile( "pjet_before_tos_correction",
					new TProfile( "pjet_before_tos_correction", 
					"detected p_{jet} VS true of detected p_{jet};\
					p_{true of detected}^{jet}; p_{detected}^{jet}",
					150, 0, 600) );


		add_new_TProfile( "pjet_after_tos_correction",
					new TProfile( "pjet_after_tos_correction", 
					"corrected detected p_{jet} VS true of detected p_{jet};\
					p_{true of detected}^{jet}; p_{detected, corr}^{jet}",
					150, 0, 600) );



		add_new_TProfile( "mjet_before_tos_correction",
					new TProfile( "mjet_before_tos_correction", 
					"detected m_{jet} VS true of detected m_{jet};\
					m_{true of detected}^{jet}; m_{detected}^{jet}",
					50, 0, 200) );


		add_new_TProfile( "mjet_after_tos_correction",
					new TProfile( "mjet_after_tos_correction", 
					"corrected detected m_{jet} VS true of detected m_{jet};\
					m_{true of detected}^{jet}; m_{detected, corr}^{jet}",
					50, 0, 200) );




		get_TProfile( "Ejet_before_tos_correction" )->SetErrorOption( "s" );
		get_TProfile( "Ejet_after_tos_correction" )->SetErrorOption( "s" );
		get_TProfile( "pjet_before_tos_correction" )->SetErrorOption( "s" );
		get_TProfile( "pjet_after_tos_correction" )->SetErrorOption( "s" );
		get_TProfile( "mjet_before_tos_correction" )->SetErrorOption( "s" );
		get_TProfile( "mjet_after_tos_correction" )->SetErrorOption( "s" );


	}

	bool is_good_charged_hadron( int ijet, int ichhad ) {
		if (TJ_RecoChHad_exists[ijet][ichhad] == 0) { return false; }
		if (TJ_RecoChHad_E[ijet][ichhad] < 5) { return false; }
		else { return true; }

	}


	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
//			if ( pass_selection == 0 ) { continue; } 
			if ( ! (pass_isolep_cut == 1 &&
					pass_min_chargedparticles_jet_cut == 1 &&
					pass_min_particles_jet_cut == 1 &&
					pass_y_34_cut == 1 &&
					pass_miss_mass_cut == 1 &&
					pass_cosTheta_miss_cut == 1 &&
					pass_cosTheta_EmaxTrack_cut == 1 &&
					pass_E_10cone_EmaxTrack_cut == 1 ) ) { continue; } 

			for (int ijet=0; ijet<max_Njets; ijet++) {

				if ( ! TJ_jets_exists[ijet] ) { continue; } 

				if ( fabs(TJ_jets_final_elementon_pdgID[ijet]) > 5 ) { continue; }

				int N_highE_reco_chhad = 0;

				for (int ichhad=0; ichhad<max_Nparticles; ichhad++) {
					if ( ! is_good_charged_hadron(ijet, ichhad) ) { continue; }
					else { N_highE_reco_chhad++; }
				}

				get_TProfile( "pjet_before_tos_correction" ) 
				->Fill( TJ_jets_true_of_detectable_p[ijet], TJ_jets_detected_p[ijet],  weight );

				double corrected_p = s_tos_p_correction(TJ_jets_detected_p[ijet], N_highE_reco_chhad);
			
				get_TProfile( "pjet_after_tos_correction" ) 
				->Fill( TJ_jets_true_of_detectable_p[ijet], corrected_p,  weight );

				if ( TJ_jets_detected_p[ijet] != 0 ) {
					double p_corr_factor = corrected_p/TJ_jets_detected_p[ijet];
					get_TH1D("pjet_tos_correction_distribution")
					->Fill( p_corr_factor, weight );
				}


				get_TProfile( "Ejet_before_tos_correction" ) 
				->Fill( TJ_jets_true_of_detectable_E[ijet], TJ_jets_detected_E[ijet],  weight );

				double corrected_E = s_tos_E_correction(TJ_jets_detected_E[ijet], 
														N_highE_reco_chhad,
														corrected_p);
			
				get_TProfile( "Ejet_after_tos_correction" ) 
				->Fill( TJ_jets_true_of_detectable_E[ijet], corrected_E,  weight );

				if ( TJ_jets_detected_E[ijet] != 0 ) {
					double E_corr_factor = corrected_E/TJ_jets_detected_E[ijet];
					get_TH1D("Ejet_tos_correction_distribution")
					->Fill( E_corr_factor, weight );
				}


				double m_tos = mass( 	TJ_jets_true_of_detectable_E[ijet],
										TJ_jets_true_of_detectable_p[ijet] );

				double m_s_before_corr = mass( 	TJ_jets_detected_E[ijet],
												TJ_jets_detected_p[ijet] ); 				

				double m_s_after_corr = mass( corrected_E, corrected_p );

				get_TProfile( "mjet_before_tos_correction" )
				->Fill( m_tos, m_s_before_corr, weight );

				get_TProfile( "mjet_after_tos_correction" )
				->Fill( m_tos, m_s_after_corr, weight );

			}

		}
	}


	void get_resolution_plot( TH1D* plot, TH1D* resolution_plot ) {
	
		int N_bins = plot->GetNcells();
		
		for ( int bin=0; bin<N_bins+2; bin++ ) {
			double error = plot->GetBinError(bin);
			double content = plot->GetBinContent(bin);
			double resolution;
			if ( content == 0) {
				resolution = 0;
			} else {
				resolution = error/content;
			} 
			resolution_plot->SetBinContent( bin, resolution );		
			resolution_plot->SetBinError( bin, 0.0 );		
		}
	} 

	void get_ratio_plot( TH1D* plot, TH1D* ratio_plot ) {
		int N_bins = plot->GetNcells();

		for( int bin=0; bin<N_bins+2; bin++ ) {
			double x = plot->GetBinCenter(bin); 
			double y = plot->GetBinContent(bin);
			
			double ratio;
			if ( x==0 ) { 
				ratio = 0;
			} else {
				ratio = y/x;
			}
			
			ratio_plot->SetBinContent( bin, ratio );	
		} 

	}

	void draw_plots(){




	{
	TCanvas* can = new TCanvas("can", "" , 1000, 1000);
	TLegend* leg = new TLegend(0.25, 0.6, 0.45, 0.9);

	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.5, 1, 1.0);
	pad1->SetBottomMargin(0); // Upper and lower plot are joined
	pad1->SetTopMargin(0.1); 
	pad1->Draw();             // Draw the upper pad: pad1
	pad1->cd();               // pad1 becomes the current pad

	TProfile* wo_corr = get_TProfile( "Ejet_before_tos_correction" );
	wo_corr->SetLineColor(2);
	wo_corr->SetMarkerSize(0);
	wo_corr->Draw("p");
	leg->AddEntry(wo_corr, "no corr", "l");

	TProfile* w_corr = get_TProfile( "Ejet_after_tos_correction" );
	w_corr->SetLineColor(4);
	w_corr->SetMarkerSize(0);
	w_corr->Draw("p same");
	leg->AddEntry(w_corr, "with corr", "l");

	gPad->Update();
	float axis_max = gPad->GetFrame()->GetY2();
	if ( axis_max > 600 ) { axis_max = 600; }
	float axis_min = gPad->GetFrame()->GetY1();
	if ( axis_min < 0 ) { axis_min = 0; }
	TLine* line = new TLine(axis_min, axis_min, axis_max, axis_max);
	line->Draw("same");

	leg->Draw();

	// lower plot will be in pad
	double y2 = 0.5/(2.+0.4); 
	can->cd();          // Go back to the main canvas before defining pad2
	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.5-y2, 1, 0.5);
	pad2->SetTopMargin(0);
	pad2->SetBottomMargin(0);
	pad2->Draw();
	pad2->cd();       // pad2 becomes the current pad

	TH1D* wo_corr_resolution = new TH1D("wo_corr_E_res" , ";; std dev/value",	150, 0, 600) ;

	get_resolution_plot( wo_corr, wo_corr_resolution );
	wo_corr_resolution->SetLineColor(2);
	wo_corr_resolution->Draw("hist");

	// Y axis ratio plot settings
	wo_corr_resolution->GetYaxis()->SetTitleSize(30);
	wo_corr_resolution->GetYaxis()->SetTitleFont(43);
	wo_corr_resolution->GetYaxis()->SetTitleOffset(1.55);
	wo_corr_resolution->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_resolution->GetYaxis()->SetLabelSize(30);
	
	wo_corr_resolution->SetMinimum(0.001);
	wo_corr_resolution->SetMaximum(1.9);

	TH1D* w_corr_resolution = new TH1D("w_corr_E_res" , ";; std dev/value", 150, 0, 600) ;

	get_resolution_plot( w_corr, w_corr_resolution );
	w_corr_resolution->SetLineColor(4);
	w_corr_resolution->Draw("hist same");


	can->cd();          // Go back to the main canvas before defining pad2
	TPad *pad3 = new TPad("pad3", "pad3", 0, 0, 1, 0.5-y2);
	pad3->SetTopMargin(0);
	pad3->SetBottomMargin(0.4);
	pad3->Draw();
	pad3->cd();       // pad2 becomes the current pad

	TH1D* wo_corr_ratio = new TH1D("wo_corr_E_ratio" , ";E_{true of detected}^{jet}; ratio y/x", 150, 0, 600) ;

	get_ratio_plot( wo_corr, wo_corr_ratio );
	wo_corr_ratio->SetLineColor(2);
	wo_corr_ratio->Draw("hist");

	wo_corr_ratio->SetMinimum(0.6);
	wo_corr_ratio->SetMaximum(1.39);

	// Y axis ratio plot settings
	wo_corr_ratio->GetYaxis()->SetTitleSize(30);
	wo_corr_ratio->GetYaxis()->SetTitleFont(43);
	wo_corr_ratio->GetYaxis()->SetTitleOffset(1.55);
	wo_corr_ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_ratio->GetYaxis()->SetLabelSize(30);

	// X axis ratio plot settings
	wo_corr_ratio->GetXaxis()->SetTitleSize(30);
	wo_corr_ratio->GetXaxis()->SetTitleFont(43);
	wo_corr_ratio->GetXaxis()->SetTitleOffset(3.5);
	wo_corr_ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_ratio->GetXaxis()->SetLabelSize(30);


	TH1D* w_corr_ratio = new TH1D("w_corr_E_ratio" , ";E_{true of detected}^{jet}; ratio y/x", 150, 0, 600) ;

	get_ratio_plot( w_corr, w_corr_ratio );
	w_corr_ratio->SetLineColor(4);
	w_corr_ratio->Draw("hist same");

	TLine* v_line = new TLine(0, 1, 600, 1);
	v_line->SetLineStyle(2);
	v_line->Draw("same");

	can->Print( ( get_output_directory() + "/Ejet_tos_corr.pdf" ).c_str() );
	}



	{
	TCanvas* can = new TCanvas("can", "" , 1000, 1000);
	TLegend* leg = new TLegend(0.25, 0.6, 0.45, 0.9);

	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.5, 1, 1.0);
	pad1->SetBottomMargin(0); // Upper and lower plot are joined
	pad1->SetTopMargin(0.1); 
	pad1->Draw();             // Draw the upper pad: pad1
	pad1->cd();               // pad1 becomes the current pad

	TProfile* wo_corr = get_TProfile( "pjet_before_tos_correction" );
	wo_corr->SetLineColor(2);
	wo_corr->SetMarkerSize(0);
	wo_corr->Draw("p");
	leg->AddEntry(wo_corr, "no corr", "l");

	TProfile* w_corr = get_TProfile( "pjet_after_tos_correction" );
	w_corr->SetLineColor(4);
	w_corr->SetMarkerSize(0);
	w_corr->Draw("p same");
	leg->AddEntry(w_corr, "with corr", "l");

	gPad->Update();
	float axis_max = gPad->GetFrame()->GetY2();
	if ( axis_max > 600 ) { axis_max = 600; }
	float axis_min = gPad->GetFrame()->GetY1();
	if ( axis_min < 0 ) { axis_min = 0; }
	TLine* line = new TLine(axis_min, axis_min, axis_max, axis_max);
	line->Draw("same");

	leg->Draw();

	// lower plot will be in pad
	double y2 = 0.5/(2.+0.4); 
	can->cd();          // Go back to the main canvas before defining pad2
	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.5-y2, 1, 0.5);
	pad2->SetTopMargin(0);
	pad2->SetBottomMargin(0);
	pad2->Draw();
	pad2->cd();       // pad2 becomes the current pad

	TH1D* wo_corr_resolution = new TH1D("wo_corr_p_res" , ";; std dev/value",	150, 0, 600) ;

	get_resolution_plot( wo_corr, wo_corr_resolution );
	wo_corr_resolution->SetLineColor(2);
	wo_corr_resolution->Draw("hist");

	// Y axis ratio plot settings
	wo_corr_resolution->GetYaxis()->SetTitleSize(30);
	wo_corr_resolution->GetYaxis()->SetTitleFont(43);
	wo_corr_resolution->GetYaxis()->SetTitleOffset(1.55);
	wo_corr_resolution->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_resolution->GetYaxis()->SetLabelSize(30);
	
	wo_corr_resolution->SetMinimum(0.001);
	wo_corr_resolution->SetMaximum(1.9);

	TH1D* w_corr_resolution = new TH1D("w_corr_p_res" , ";; std dev/value", 150, 0, 600) ;

	get_resolution_plot( w_corr, w_corr_resolution );
	w_corr_resolution->SetLineColor(4);
	w_corr_resolution->Draw("hist same");


	can->cd();          // Go back to the main canvas before defining pad2
	TPad *pad3 = new TPad("pad3", "pad3", 0, 0, 1, 0.5-y2);
	pad3->SetTopMargin(0);
	pad3->SetBottomMargin(0.4);
	pad3->Draw();
	pad3->cd();       // pad2 becomes the current pad

	TH1D* wo_corr_ratio = new TH1D("wo_corr_p_ratio" , ";p_{true of detected}^{jet}; ratio y/x", 150, 0, 600) ;

	get_ratio_plot( wo_corr, wo_corr_ratio );
	wo_corr_ratio->SetLineColor(2);
	wo_corr_ratio->Draw("hist");

	wo_corr_ratio->SetMinimum(0.6);
	wo_corr_ratio->SetMaximum(1.39);

	// Y axis ratio plot settings
	wo_corr_ratio->GetYaxis()->SetTitleSize(30);
	wo_corr_ratio->GetYaxis()->SetTitleFont(43);
	wo_corr_ratio->GetYaxis()->SetTitleOffset(1.55);
	wo_corr_ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_ratio->GetYaxis()->SetLabelSize(30);

	// X axis ratio plot settings
	wo_corr_ratio->GetXaxis()->SetTitleSize(30);
	wo_corr_ratio->GetXaxis()->SetTitleFont(43);
	wo_corr_ratio->GetXaxis()->SetTitleOffset(3.5);
	wo_corr_ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_ratio->GetXaxis()->SetLabelSize(30);


	TH1D* w_corr_ratio = new TH1D("w_corr_p_ratio" , ";p_{true of detected}^{jet}; ratio y/x", 150, 0, 600) ;

	get_ratio_plot( w_corr, w_corr_ratio );
	w_corr_ratio->SetLineColor(4);
	w_corr_ratio->Draw("hist same");

	TLine* v_line = new TLine(0, 1, 600, 1);
	v_line->SetLineStyle(2);
	v_line->Draw("same");

	can->Print( ( get_output_directory() + "/pjet_tos_corr.pdf" ).c_str() );
	}






	{
	TCanvas* can = new TCanvas("can", "" , 1000, 1000);
	TLegend* leg = new TLegend(0.25, 0.6, 0.45, 0.9);

	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.5, 1, 1.0);
	pad1->SetBottomMargin(0); // Upper and lower plot are joined
	pad1->SetTopMargin(0.1); 
	pad1->Draw();             // Draw the upper pad: pad1
	pad1->cd();               // pad1 becomes the current pad

	TProfile* wo_corr = get_TProfile( "mjet_before_tos_correction" );
	wo_corr->SetLineColor(2);
	wo_corr->SetMarkerSize(0);
	wo_corr->Draw("p");
	leg->AddEntry(wo_corr, "no corr", "l");

	TProfile* w_corr = get_TProfile( "mjet_after_tos_correction" );
	w_corr->SetLineColor(4);
	w_corr->SetMarkerSize(0);
	w_corr->Draw("p same");
	leg->AddEntry(w_corr, "with corr", "l");

	gPad->Update();
	float axis_max = gPad->GetFrame()->GetY2();
	if ( axis_max > 600 ) { axis_max = 600; }
	float axis_min = gPad->GetFrame()->GetY1();
	if ( axis_min < 0 ) { axis_min = 0; }
	TLine* line = new TLine(axis_min, axis_min, axis_max, axis_max);
	line->Draw("same");

	leg->Draw();

	// lower plot will be in pad
	double y2 = 0.5/(2.+0.4); 
	can->cd();          // Go back to the main canvas before defining pad2
	TPad *pad2 = new TPad("pad2", "pad2", 0, 0.5-y2, 1, 0.5);
	pad2->SetTopMargin(0);
	pad2->SetBottomMargin(0);
	pad2->Draw();
	pad2->cd();       // pad2 becomes the current pad

	TH1D* wo_corr_resolution = new TH1D("wo_corr_m_res" , ";; std dev/value",	150, 0, 600) ;

	get_resolution_plot( wo_corr, wo_corr_resolution );
	wo_corr_resolution->SetLineColor(2);
	wo_corr_resolution->Draw("hist");

	// Y axis ratio plot settings
	wo_corr_resolution->GetYaxis()->SetTitleSize(30);
	wo_corr_resolution->GetYaxis()->SetTitleFont(43);
	wo_corr_resolution->GetYaxis()->SetTitleOffset(1.55);
	wo_corr_resolution->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_resolution->GetYaxis()->SetLabelSize(30);
	
	wo_corr_resolution->SetMinimum(0.001);
	wo_corr_resolution->SetMaximum(1.9);

	TH1D* w_corr_resolution = new TH1D("w_corr_m_res" , ";; std dev/value", 150, 0, 600) ;

	get_resolution_plot( w_corr, w_corr_resolution );
	w_corr_resolution->SetLineColor(4);
	w_corr_resolution->Draw("hist same");


	can->cd();          // Go back to the main canvas before defining pad2
	TPad *pad3 = new TPad("pad3", "pad3", 0, 0, 1, 0.5-y2);
	pad3->SetTopMargin(0);
	pad3->SetBottomMargin(0.4);
	pad3->Draw();
	pad3->cd();       // pad2 becomes the current pad

	TH1D* wo_corr_ratio = new TH1D("wo_corr_m_ratio" , ";m_{true of detected}^{jet}; ratio y/x", 150, 0, 600) ;

	get_ratio_plot( wo_corr, wo_corr_ratio );
	wo_corr_ratio->SetLineColor(2);
	wo_corr_ratio->Draw("hist");

	wo_corr_ratio->SetMinimum(0.6);
	wo_corr_ratio->SetMaximum(1.39);

	// Y axis ratio plot settings
	wo_corr_ratio->GetYaxis()->SetTitleSize(30);
	wo_corr_ratio->GetYaxis()->SetTitleFont(43);
	wo_corr_ratio->GetYaxis()->SetTitleOffset(1.55);
	wo_corr_ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_ratio->GetYaxis()->SetLabelSize(30);

	// X axis ratio plot settings
	wo_corr_ratio->GetXaxis()->SetTitleSize(30);
	wo_corr_ratio->GetXaxis()->SetTitleFont(43);
	wo_corr_ratio->GetXaxis()->SetTitleOffset(3.5);
	wo_corr_ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	wo_corr_ratio->GetXaxis()->SetLabelSize(30);


	TH1D* w_corr_ratio = new TH1D("w_corr_m_ratio" , ";m_{true of detected}^{jet}; ratio y/x", 150, 0, 600) ;

	get_ratio_plot( w_corr, w_corr_ratio );
	w_corr_ratio->SetLineColor(4);
	w_corr_ratio->Draw("hist same");

	TLine* v_line = new TLine(0, 1, 600, 1);
	v_line->SetLineStyle(2);
	v_line->Draw("same");

	can->Print( ( get_output_directory() + "/mjet_tos_corr.pdf" ).c_str() );
	}



	{

	TCanvas* can = new TCanvas( "can_Efactors", "", 0, 0, 600, 600);				
				
	get_TH1D("Ejet_tos_correction_distribution")->Draw("hist");

	string plot_name = get_output_directory() + "/E_corr_factors.pdf";
	can->Print(plot_name.c_str());
	can->Close();

	}

	{

	TCanvas* can = new TCanvas( "can_pfactors", "", 0, 0, 600, 600);				
				
	get_TH1D("pjet_tos_correction_distribution")->Draw("hist");

	string plot_name = get_output_directory() + "/p_corr_factors.pdf";
	can->Print(plot_name.c_str());
	can->Close();

	}





	}
			
};
