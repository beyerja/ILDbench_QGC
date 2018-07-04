#include "fileinfo.h"


// TODO Maybe call this "...Iterator" since its main purpose is to iterate other the files
class ParameterPointIterator {
	// class provides interface that allows you to 
	// access the files specific to this parameter point

	private:
		vector<float> parameter_point;

		vector<FileInfo> file_infos;	
		int current_file_index = -1;
		float current_file;

	public:

		void init ( vector<float> _parameter_point, vector<FileInfo> _file_infos ) {
			parameter_point = _parameter_point;
			file_infos = _file_infos;
		}


		vector<float> get_parameter_point() {
			return parameter_point;
		}

		bool next_file() {
			// Function that sets index to next file if possible
			// and return false if not

			// +2 because: +1 because index starts w/ 0, +1 for next file
			if ( current_file_index + 2 > file_infos.size() ) {
				return false;
			}
			current_file_index++;
			return true;
		}

		TTree* get_tree() {
			FileInfo current_file_info = file_infos[current_file];
			TFile *file = TFile::Open( ( current_file_info.get_file_path(parameter_point) ).c_str() );
			TTree *tree = (TTree*)file->Get( ( current_file_info.get_tree_name() ).c_str() );
			return tree;
		}

		float get_file_weight() {
			return file_infos[current_file].get_file_weight(parameter_point);
		} 

};

