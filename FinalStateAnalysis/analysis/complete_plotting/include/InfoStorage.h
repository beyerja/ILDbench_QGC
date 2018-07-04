#ifndef INFOSTORAGE_h
#define INFOSTORAGE_h 

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TF2.h"
#include "TF3.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLine.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include "TMath.h"

using namespace std;

class InfoStorage {
	/* Class to store all information about the files which 
 	are used for plotting */

	// Infos regarding computational environment
	string input_directory;	
	string output_directory;	
	string tree_name;

	// Infos regarding the setup
	float luminosity;
	float e_beam_polarization;
	float p_beam_polarization;

	// Infos regarding files
	vector<string> 	filenames;
	vector<string> 	class_names;
	vector<string> 	final_states;
	vector<string> 	polarizations;
	vector<float>	xsections;

	vector<float>	file_weights;

	void calculate_weight_i (int i) {
		/* Calculate and set the weight for file i*/
		TFile *file = TFile::Open((input_directory + "/" + filenames[i]).c_str());
		TTree *tree = (TTree*)file->Get(tree_name.c_str());

		int nEntries = tree->GetEntries();

		// Calculate weight for this input sample
		float pol_weight = 	( 1 + get_e_polarization_sign_i(i) * e_beam_polarization )/2 
							* ( 1 + get_p_polarization_sign_i(i) * p_beam_polarization )/2;
		file_weights[i] = ( xsections[i] * luminosity / nEntries ) * pol_weight; 
	}

	public:

		void set_environment(string _input_directory, string _output_directory, string _tree_name) {
			/* Set the computing environment */
			input_directory = _input_directory;
			output_directory = _output_directory;
			tree_name = _tree_name;
		}

		void set_setup (float _luminosity, float _e_beam_polarization, float _p_beam_polarization) {
			/* Set the experimental setup */
			luminosity = _luminosity;
			e_beam_polarization = _e_beam_polarization;
			p_beam_polarization = _p_beam_polarization;
		}

		void add_file_info(string filename, string class_name, string final_state, string polarization, float xsection) {
			/* Add a new file with its information */
			filenames.push_back(filename);
			class_names.push_back(class_name);
			final_states.push_back(final_state);
			polarizations.push_back(polarization);
			xsections.push_back(xsection);

			// Adjust length of weights vector and calculate new weight
			file_weights.resize(get_number_of_files());
			calculate_weight_i(get_number_of_files()-1);
		}

		// Get functions

		int get_number_of_files () {
			return filenames.size();
		}

		string get_file_name_i (int i) {
			return filenames[i];
		}

		string get_file_path_i (int i) {
			return input_directory + "/" + filenames[i];
		}

		string get_tree_name () {
			return tree_name;
		}

		string get_output_directory() {
			return output_directory;
		}

		string get_class_name_i (int i) {
			return class_names[i];
		}		

		string get_final_state_i (int i) {
			return final_states[i];
		}		

		string get_polarization_string_i (int i) {
			return polarizations[i];
		}		

		float get_xsection_i (int i) {
			return xsections[i];
		}		
		
		float get_file_weight_i (int i) {
			return file_weights[i];
		}

		int get_e_polarization_sign_i (int i) {
			// Extract the electron helicity in process i from the string version
			string polarization_string = polarizations[i];
			if ( (polarization_string == "rr") || (polarization_string == "rl") ) {
				return 1;
			} else if ( (polarization_string == "ll") || (polarization_string == "lr") ) {
				return -1;
			} else {
				return 0;
			}	
		}

		int get_p_polarization_sign_i (int i) {
			// Extract the positron helicity in process i from the string version
			string polarization_string = polarizations[i];
			if ( (polarization_string == "rr") || (polarization_string == "lr") ) {
				return 1;
			} else if ( (polarization_string == "ll") || (polarization_string == "rl") ) {
				return -1;
			} else {
				return 0;
			}	
		}
	



} ;

#endif
