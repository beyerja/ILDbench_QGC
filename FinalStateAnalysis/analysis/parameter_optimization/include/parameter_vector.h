
class ParameterSpaceVector {

	vector<vector<float>> parameter_space_vector;
	
	vector<vector<float>> parameter_points;


	void combinations(vector<vector<float> > array, int i, vector<float> accum, vector<vector<float>> &comb)
	{ /* from https://stackoverflow.com/questions/8620030/generate-all-combination-of-elements-in-2d-vector */
	    if (i == array.size()) // done, no more rows
	    {
	        comb.push_back(accum); // assuming comb is global
	    }
	    else
	    {
	        vector<float> row = array[i];
   	    for(int j = 0; j < row.size(); ++j)
   	    {
        	   	vector<float> tmp(accum);
   	        	tmp.push_back(row[j]);
   	        	combinations(array,i+1,tmp, comb);
   	    	}
   	 	}
	}

	public:

		void add_parameter_vector( vector<float> parameter_vector ){
			parameter_space_vector.push_back(parameter_vector);
		}

		void calculate_parameter_points() {
			int counter=0;
			vector<float> accum;
			combinations(parameter_space_vector, counter, accum, parameter_points);
		}
		
		int get_N_parameter_points() {
			return parameter_points.size();
		}
		
		vector<float> get_parameter_point( int point_index ) {
			return parameter_points[point_index];
		}

};
