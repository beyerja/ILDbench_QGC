#include "parameter_vector.h"
#include "parameter_point_calculator.h"


//template <class OutputType> 

class Calculation {

	// -> Belongs to ParameterPointCalculator
	//ResultStorage <ResultType> result_storage;

	ParameterSpaceVector parameter_space;
	vector<FileInfo> file_infos;

	ResultStorage results_all_points;
	vector<ResultStorage> results_per_point;

	virtual void create_plots_all_points();

	void do_parameter_point_calculation( ParameterPointIterator parameter_point_iterator ) {
		ParameterPointCalculator calculator;
		calculator.init(parameter_point_iterator, results_all_points);
		calculator.loop_all_files();
		results_per_point.push_back(calculator.get_result_storage());
	}

	public:

		void init(  ParameterSpaceVector _parameter_space, vector<FileInfo> _file_infos ) {
			parameter_space = _parameter_space;
			file_infos = _file_infos;
		}


		void do_calculation() {
			create_plots_all_points();
			for ( int i=0; i<parameter_space.get_N_parameter_points(); i++ ) {
				ParameterPointIterator parameter_point_iterator;
				parameter_point_iterator.init( parameter_space.get_parameter_point(i) , file_infos ); 
				do_parameter_point_calculation( parameter_point_iterator );
			} 
		}

		virtual void save_results();		

}; 
