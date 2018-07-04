

class TTreeReadout {
	// class allows you to skip setting each branch address in every
	// script that uses the variables. Rather gives easy unified way
	// of doing so and then accessing the current variable with
	// ->get_int( var_name ) / ->get_float( var_name ) /...

	private:
		// To add new variable: Add to declaration and type-specific map
		
		// Link generator to reco level
		int partner_of_truthV1, partner_of_truthV2;

		// Generator level
		int eventType;

		float 	true_miss_mass,
				true_sum_E_q,   
				true_pair1_mass,
			    true_pair2_mass;

		// Reconstructed - clustered level
		float 	miss_mass,        
				sum_E_q,        
				pair1_mass,         
				pair2_mass; 


		// Maps that connect the variable to its name in the Branch
		// ->	Allow both loop over SetBranchAddress and a more 
		// 		unified way of getting each variable (w/o directly
		// 		accessing the tree)
		// CAREFUL: Map connects string with POINTER

		map<string, int*> branchname_to_int_map = {
				{ "true_evt_type" , &eventType },
				{ "partner_of_truthV1" , &partner_of_truthV1 },
				{ "partner_of_truthV2" , &partner_of_truthV2 },
		};

		map<string, float*> branchname_to_float_map = {
				{ "true_miss_mass", &true_miss_mass },
				{ "true_sum_E_q", &true_sum_E_q },
				{ "true_pair2_mass", &true_pair2_mass },
				{ "true_pair1_mass", &true_pair1_mass },

				{ "miss_mass", &miss_mass },
				{ "pair1_mass", &pair1_mass },
				{ "sum_E_q", &sum_E_q },
				{ "pair2_mass", &pair2_mass },
		};


	public: 

		void set_branchaddresses( TTree* tree ) {
			// Couple variables to the tree branch addresses
			
			// Iterate of integer variables
			for ( map<string,int*>::iterator it=branchname_to_int_map.begin(); it!=branchname_to_int_map.end(); ++it) {
				tree->SetBranchAddress( ( it->first ).c_str() , it->second ); 
			}
			
			// Iterate of float variables
			for ( map<string,float*>::iterator it=branchname_to_float_map.begin(); it!=branchname_to_float_map.end(); ++it) {
				tree->SetBranchAddress( ( it->first ).c_str() , it->second ); 
			}

		}

		int get_int( string variable_name ) {
			// return the current value of the Branch variable variable_name
			return *branchname_to_int_map[variable_name.c_str()];
		}

		int get_float( string variable_name ) {
			// return the current value of the Branch variable variable_name
			return *branchname_to_float_map[variable_name.c_str()];
		}

};
