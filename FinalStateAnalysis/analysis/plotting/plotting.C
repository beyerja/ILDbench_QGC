// This is just stuff that I accidentally already wrote in the analysis script

bool extract_polarisations(string filename, float *sample_e_pol, float *sample_p_pol) {
	/* Takes filename that includes usual polarisation marker and
 	sets the e and p polarisations accordingly */
	string eLpL = "eL.pL";
	string eLpR = "eL.pR";
	string eRpL = "eR.pL";
	string eRpR = "eR.pR";

	if (filename.find(eLpL) != string::npos) {
		*sample_e_pol = -1;
		*sample_p_pol = -1;
	} else if (filename.find(eLpR) != string::npos) {
		*sample_e_pol = -1;
		*sample_p_pol = 1;
	} else if (filename.find(eRpL) != string::npos) {
		*sample_e_pol = 1;
		*sample_p_pol = -1;
	} else if (filename.find(eRpR) != string::npos) {
		*sample_e_pol = 1;
		*sample_p_pol = 1;
	} else {
		return false;
	}

	return true;
}





	// This is fixed by the running scenario, change if necessary! 
	float luminosity=1000.0;
	float e_pol_percentage=-0.8;
	float p_pol_percentage=+0.3;

	// Get Polarisation of input sample
	float sample_e_pol, sample_p_pol;
	bool found_sample_pol = extract_polarisations(input_rootfile_name, &sample_e_pol, &sample_p_pol);
	if ( ! found_sample_pol ) {
		return;
	}

	// Calculate sample weight
	float pol_weight = ( 1 + sample_e_pol * e_pol_percentage )/2 * ( 1 + sample_p_pol * p_pol_percentage )/2;
	float weight = ( xsection * luminosity / nEntries ) * pol_weight;

