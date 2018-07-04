#include "parameter_point_iterator.h"
#include "tree_readout.h"
#include "result_class.h"


//template <class ResultType>
class ParameterPointCalculator {
	// Superclass for all calculations done on the input files
	// for this parameter point 
	
		ParameterPointIterator* parameter_point_iterator;
		TTreeReadout tree_readout;
		ResultStorage result_storage;
		ResultStorage* results_all_points;

		float current_weight;
	
		// Additional class saving results -> templates!!!

	public:

		void init( ParameterPointIterator &_parameter_point_iterator, ResultStorage &_results_all_points ){
			// Existence of init function means that it must be called before
			// doing anything else with the class object!			

			// Set the parameter point for which this calculation should happen
			parameter_point_iterator = &_parameter_point_iterator;

			results_all_points = &_results_all_points;
		}

		float get_parameter ( int parameter_index ) {
			return parameter_point_iterator->get_parameter_point()[parameter_index];
		}
		
		float get_weight() {
			return current_weight;
		}


		// These functions contain the actual calculation for the 
		// individual event of the file
		// -> Implemented in subclass
		virtual void do_event_calculation();

		void loop_all_events() {
			// Get current tree, set the addresses and do calculation for all events
			TTree* tree = parameter_point_iterator->get_tree();
			tree_readout.set_branchaddresses(tree);
			int number_entries = tree->GetEntries();
			for (int j = 0; j<number_entries; j++) {
				tree->GetEntry(j);
				do_event_calculation();
			}
		}

		virtual void create_parameter_point_plots();

		void loop_all_files() {
			// For all files do the individual calculation
			create_parameter_point_plots();
			while ( parameter_point_iterator->next_file() ) {
				current_weight = parameter_point_iterator->get_file_weight();
				loop_all_events();
			}
		}

		ResultStorage get_result_storage() {
			return result_storage;
		}


};
