#include "../include/calculation.h"


string float_to_string(float number) {
	// Helper function that takes a float and transforms it like:
	//	1.900000 -> 1.9	
	//	1.000000 -> 1
	//The outcome is returned as a string.
	//
	if ( number - (int)number == 0 ) {
		int int_number = (int)number;
		return to_string(int_number);
	}
	std::string number_str = std::to_string(number);
	number_str.erase( number_str.find_last_not_of('0') + 1, std::string::npos );
	return number_str;	
}



/* --------------------------------------------------------------------------------------*/
/* These must be defined for each new calculation */
// TODO Maybe write into some kind of "steering file" -> behaviour of calculation separate from input
string FileInfo::get_file_path( vector<float> parameter_point ) {
	return in_directory + "/cone_parameter_" + float_to_string(parameter_point[0]) 
			+ "/rootfiles_force" + float_to_string(parameter_point[1]) + "jets/" + file_name;
}


void ParameterPointCalculator::do_event_calculation() {
	int true_evt_type 		=tree_readout.get_int("true_evt_type");
	int partner_of_truthV1	=tree_readout.get_int("partner_of_truthV1");
	int partner_of_truthV2	=tree_readout.get_int("partner_of_truthV2");
    //float true_miss_mass	=tree_readout.get_float("true_miss_mass");   
	//float true_sum_E_q		=tree_readout.get_float("true_sum_E_q");   
	float true_pair1_mass	=tree_readout.get_float("true_pair1_mass");    
	float true_pair2_mass	=tree_readout.get_float("true_pair2_mass");
    //float miss_mass			=tree_readout.get_float("miss_mass");       
	//float sum_E_q			=tree_readout.get_float("sum_E_q");        
	float pair1_mass		=tree_readout.get_float("pair1_mass");         
	float pair2_mass		=tree_readout.get_float("pair2_mass");

	TProfile2D* h2_V_mass = (TProfile2D*)results_all_points->get_TProfile2D_result_by_index(0);
	float cone				= get_parameter(0);
	float exclusive_Njet	= get_parameter(1);
	float weight = get_weight(); 

	if ( ! ( (true_evt_type == 1) || (true_evt_type == 2) ) ) {return;}

	//h2_miss_mass->Fill(cone, forcedN, (miss_mass/true_miss_mass-1), weight);			
	//h2_sum_E_q->Fill(cone, forcedN, (sum_E_q/true_sum_E_q-1), weight);			

	//h1_miss_mass->Fill(true_miss_mass, miss_mass/true_miss_mass, weight);			
	//h1_sum_E_q->Fill(true_sum_E_q, sum_E_q/true_sum_E_q, weight);			

	// Compare the true and reconstructed V's
	if ( partner_of_truthV1 == 1 && partner_of_truthV2 == 2 ) {

		h2_V_mass->Fill(cone, exclusive_Njet, (pair1_mass/true_pair1_mass-1), weight);
		//current_file_result_storage.add_float_result(pair1_mass/true_pair1_mass);
		//current_file_result_storage.add_float_result(pair2_mass/true_pair2_mass);
	}
	else if ( partner_of_truthV1 == 2 && partner_of_truthV2 == 1 ) {
		//current_file_result_storage.add_float_result(pair1_mass/true_pair2_mass-1);
		//current_file_result_storage.add_float_result(pair2_mass/true_pair1_mass-1);
	}
	else { // This occurs if generator level V and reconstructed V were not paired correctly (shouldn't happen xP) 
		cout << "ERROR: Something wrong with V links" << endl 
			<< "Partners are: " << partner_of_truthV1 << partner_of_truthV1 << endl; 
		return;
	}

}


void ParameterPointCalculator::create_parameter_point_plots() {
}


void  Calculation::create_plots_all_points() {
	TProfile2D* h2_V_mass = new TProfile2D("V_mass_2D", "variable: dijet (V) mass; cone parameter; forced N_{jet}; <reco/MC - 1> [%]",
                                              5, -0.15, 1.85, 4, 2.5, 6.5);
	results_all_points.add_TProfile2D_result( h2_V_mass );

}

void Calculation::save_results() { 

	TCanvas *canvas = new TCanvas("can_2D", "", 0, 0, 700, 600);
	results_all_points.get_TProfile2D_result_by_index(0)->Draw("colz text");	
	canvas->Print("./test.pdf");


/*	TProfile2D* h2_V_mass = new TProfile2D("V_mass_2D", "variable: dijet (V) mass; cone parameter; forced N_{jet}; <reco/MC - 1> [%]",
                                              5, -0.15, 1.85, 4, 2.5, 6.5);

	for (int i=0; i<parameter_space.get_N_parameter_points(); i++) {
		// TODO Make this more easily accessible -> Make this standard in Calculation
		vector<float> parameter_point = parameter_space.get_parameter_point(i);
		vector<ResultStorage> = result_vectorall_points_result_vectors[i];
		for (int file_i=0; file_i<result_vectorall_points_result_vectors.size(); file_i++) {
			float cone =  
			float exclusive_Njet = 
			float file_weight =
			h2_V_mass->Fill(cone, forcedN, (pair1_mass/true_pair1_mass-1), weight);
		}
	}
*/	
}



