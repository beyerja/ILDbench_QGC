#ifndef FILE_INFORMATION_H
#define FILE_INFORMATION_H 1

#include <string>

class FileInfo {
	/* Class to hold information relevant for the analysis
 	and common for all events / the whole file */

	// N_events not! stored here bc it would lead to problems when merging files
	public:
		float xsection;

		float e_polarization_sign;
		float p_polarization_sign;

		void init() {
			xsection = -10.0;
		
			e_polarization_sign = 0.0;
			p_polarization_sign = 0.0;
		}
		
		//TODO Build this; probably needs string to come as input to processor since info not in event header...
		//void set_polarization( std::string pol_string ) {
		//	/* Take the usual string format and convert to numbers*/
		//	if ( pol_string.c_str() == "ll" ) {
		//		
		//	}
		//
		//}		


};




#endif
