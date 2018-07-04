class FileInfo {
	// class holding basic information about the general input file
	// NOT parameter-point specific!


	private:

		// Point of this class is that it can be minimalistic
		string file_name;
		string in_directory;

		string tree_name;		

		float polarization_weight;
		float number_of_events;

	public:

		void init( string _file_name, string _in_directory, string _tree_name ) {
			file_name = _file_name;
			tree_name = _tree_name;
			in_directory = _in_directory;
		}

		string get_tree_name() { return tree_name; }

		// Return the path for this file in a parameter-specific directory
		virtual string get_file_path( vector<float> parameter_point );

		// Weight calculation should be universal 
		void calculate_polarization_weight ( float e_pol_sign, float e_pol_percentage, float p_pol_sign, float p_pol_percentage) {
			polarization_weight = ( 1 + e_pol_sign * e_pol_percentage )/2 * ( 1 + p_pol_sign * p_pol_percentage )/2;
		}

		void calculate_number_of_events ( float xsection, float luminosity ) {
			number_of_events = xsection * luminosity;
		}

		float get_file_weight( vector<float> parameter_point ) {
			// The weight is dependent on the number of events in the file -> extra function
 			TFile* file = TFile::Open( ( get_file_path( parameter_point ) ).c_str() );
            TTree* tree = (TTree*)file->Get( get_tree_name().c_str() );
			int number_of_entries = tree->GetEntries();
			file->Close();
			return ( number_of_events / number_of_entries ) * polarization_weight; 
		}




};
