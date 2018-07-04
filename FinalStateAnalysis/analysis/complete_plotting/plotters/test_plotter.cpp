#include "../include/plotter.h"

// Very simple example

class TestPlotter : public Plotter {

	void set_plotter_settings() {
		set_output_folder_name("test");	
	}

	void define_plots(){
		add_new_TH1D("test", new TH1D("test", "test", 5, 0, 5) );
	}

	void fill_plots(){
		float weight = get_current_weight();

		// This is the loop over all events
		while ( get_next_event() ) {
			get_TH1D("test")->Fill(evt, weight);

		}
	}

	void draw_plots(){
		string output_dir = get_output_directory();

		TCanvas* c1 = new TCanvas("c1", "", 0, 0, 800, 800);
		get_TH1D("test")->Draw();
		c1->Print((output_dir + "/test.pdf").c_str());
	};
};
