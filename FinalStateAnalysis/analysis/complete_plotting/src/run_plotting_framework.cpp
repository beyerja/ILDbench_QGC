#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split

// Own header and source files
#include "../include/plotter.h"

#include "configuration.cpp"


vector<vector<string>> fileinfo_input_splitting (string fileinfo_input) {
	/* Takes the long input string from the sqlite readout and separates it into 
 	the individual elements -> vector for each file containing all infos as strings
	 */

	// delimiters between two files and between the infos for one file
	string inter_file_delimiter=",";
	string intra_file_delimiter="|";

	// Split up the infos so that there is one long string for each file
	vector<string> split_file_infos;
	split(split_file_infos, fileinfo_input, boost::is_any_of(inter_file_delimiter.c_str()), boost::token_compress_on);

	// For each file split up the long string into the individual elements
	// and push the results into a vector per file
	vector<vector<string>> result;
	for (int i=0; i<split_file_infos.size(); i++) {
		vector<string> single_file_info;
		split(single_file_info, split_file_infos[i], boost::is_any_of(intra_file_delimiter.c_str()), boost::token_compress_on);
		
		result.push_back(single_file_info);
	}

	return result;
}

void fill_info_storage (string _luminosity, string _e_beam_polarization, string _p_beam_polarization, 
						string fileinfo_input, string input_directory, string output_directory, string tree_name,
						InfoStorage &info_storage) {
	/* Fill the InfoStorage with the file information encoded in the long
 	string fileinfo_input */

	// Transfer the computing environment to the info_storage
	info_storage.set_environment(input_directory, output_directory, tree_name);

	// Transfer the setup to the info_storage
	float luminosity = stof(_luminosity);
	float e_beam_polarization = stof(_e_beam_polarization);
	float p_beam_polarization = stof(_p_beam_polarization);

	info_storage.set_setup(luminosity, e_beam_polarization, p_beam_polarization);

	// Get the splitted input
	vector<vector<string>> split_fileinfo_input = fileinfo_input_splitting(fileinfo_input);

	// For each file information: 
	// Convert the string information into the appropriate format and add
	// the entry in the info_storage
	for (int i=0; i<split_fileinfo_input.size(); i++) {
		vector<string> single_file_info = split_fileinfo_input[i];
		
		string filename = single_file_info[0];
		string class_name = single_file_info[1]; 
		string final_state = single_file_info[2];
		string polarization = single_file_info[3];
		float xsection = stof(single_file_info[4]);

		info_storage.add_file_info(filename, class_name, final_state, polarization, xsection);
	}

}


void run_plotting_framework(string luminosity, string e_beam_polarization, string p_beam_polarization, 
							string fileinfo_input, string input_directory, string output_directory,	string tree_name) {
	/* Creates central info_storage and passes it to the individual plotters to run */

	// Create central info_storage object storing infos for all plotters
	InfoStorage info_storage;
	fill_info_storage(	luminosity, e_beam_polarization, p_beam_polarization, 
						fileinfo_input, input_directory, output_directory, tree_name, 
						info_storage);

	// Store all plotters in this vector
	// -> Possible with pointers despite being different subclasses
	vector<Plotter*> plotters;

	set_plotters(plotters);

	// Execute common functionality of all Plotters
	for (int i=0; i<plotters.size(); i++) {
		plotters[i]->init(info_storage);
		plotters[i]->run();
	}

}


int main() {
	/* This function gets called when using the executable in the build directory.*/
	system("../shell_scripts/initialize.sh");
	return 1;
}
