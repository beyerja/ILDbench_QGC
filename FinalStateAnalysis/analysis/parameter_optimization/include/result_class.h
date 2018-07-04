


class ResultStorage {
	// Template class for storing the result of the 
	// calculation for a parameter point.
	// Result can be stored either as ResultType
	// or vector of ResultType.
	// Instantiate as "ResultStorage <exampleResultType> example_name"



	TH1D* TH1D_result;
	vector<TH1D*> TH1D_result_vector;

	TH2D* TH2D_result;
	vector<TH2D*> TH2D_result_vector;

	TProfile* TProfile_result;
	vector<TProfile*> TProfile_result_vector;

	TProfile2D* TProfile2D_result;
	vector<TProfile2D*> TProfile2D_result_vector;

	public:


		void set_TH1D_result ( TH1D* _TH1D_result ) {
			TH1D_result = _TH1D_result;
		}
		
		TH1D* get_TH1D_result() {
			return TH1D_result;
		}

		void add_TH1D_result ( TH1D* _TH1D_result ) {
			TH1D_result_vector.push_back(_TH1D_result);
		}

		vector<TH1D*> get_TH1D_vector() {
			return TH1D_result_vector;
		}

		TH1D* get_TH1D_result_by_index( int index ) {
			return TH1D_result_vector[index];
		}


		void set_TH2D_result ( TH2D* _TH2D_result ) {
			TH2D_result = _TH2D_result;
		}
		
		TH2D* get_TH2D_result() {
			return TH2D_result;
		}

		void add_TH2D_result ( TH2D* _TH2D_result ) {
			TH2D_result_vector.push_back(_TH2D_result);
		}

		vector<TH2D*> get_TH2D_vector() {
			return TH2D_result_vector;
		}

		TH2D* get_TH2D_result_by_index( int index ) {
			return TH2D_result_vector[index];
		}


		void set_TProfile_result ( TProfile* _TProfile_result ) {
			TProfile_result = _TProfile_result;
		}
		
		TProfile* get_TProfile_result() {
			return TProfile_result;
		}

		void add_TProfile_result ( TProfile* _TProfile_result ) {
			TProfile_result_vector.push_back(_TProfile_result);
		}

		vector<TProfile*> get_TProfile_vector() {
			return TProfile_result_vector;
		}

		TProfile* get_TProfile_result_by_index( int index ) {
			return TProfile_result_vector[index];
		}


		void set_TProfile2D_result ( TProfile2D* _TProfile2D_result ) {
			TProfile2D_result = _TProfile2D_result;
		}
		
		TProfile2D* get_TProfile2D_result() {
			return TProfile2D_result;
		}

		void add_TProfile2D_result ( TProfile2D* _TProfile2D_result ) {
			TProfile2D_result_vector.push_back(_TProfile2D_result);
		}

		vector<TProfile2D*> get_TProfile2D_vector() {
			return TProfile2D_result_vector;
		}

		TProfile2D* get_TProfile2D_result_by_index( int index ) {
			return TProfile2D_result_vector[index];
		}








































/*

	float float_result;
	vector<float> float_result_vector;

	int int_result;
	vector<int> int_result_vector;



		void set_float_result ( float _float_result ) {
			float_result = _float_result;
		}
		
		float get_float_result() {
			return float_result;
		}

		void add_float_result ( float _float_result ) {
			float_result_vector.push_back(_float_result);
		}

		vector<float> get_float_vector() {
			return float_result_vector;
		}



		void set_int_result ( int _int_result ) {
			int_result = _int_result;
		}
		
		int get_int_result() {
			return int_result;
		}

		void add_int_result ( int _int_result ) {
			int_result_vector.push_back(_int_result);
		}

		vector<int> get_int_vector() {
			return int_result_vector;
		}

*/


};

