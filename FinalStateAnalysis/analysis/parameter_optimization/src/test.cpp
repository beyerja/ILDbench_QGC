#include "specifications.cpp"


int test() {

	string in_dir = "/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/testFastJetOverlayRemoval_output/output_forcedEjet_mjet0";

 	std::vector<std::string> filenames;
	filenames.push_back("6f_vvww_h_eL.pR.root");
	filenames.push_back("6f_vvww_h_eR.pL.root");
	filenames.push_back("6f_xxxx_nunu_eL.pR.root");
	filenames.push_back("6f_xxxx_nunu_eR.pL.root");
	filenames.push_back("6f_yyyy_nunu_eL.pR.root");
	filenames.push_back("6f_yyyy_nunu_eR.pL.root"); 

	string tree_name = "datatrain";


	// Weighting of the samples
	// Important: put in correct run polarizations and sample cross-sections! 
	// Luminosity is in this case a global factor, not important
	float luminosity = 1.0;
	std::vector<float> xsections = 		{50.5806,	0.269859,	1.63413,	0.0449741,	5.39289,	0.104988};
	std::vector<float> e_pol_signs =	{-1,		1,	 		-1,	    	1,			-1,			1}; 
	std::vector<float> p_pol_signs =	{1,			-1,	 		1,	    	-1,			1,			-1}; 
	float e_polarization = -0.8;
	float p_polarization = 0.3;

	vector<FileInfo> file_infos;

	for (int i = 0; i<filenames.size(); i++) {
		FileInfo file_info;
		file_info.init( filenames[i], in_dir, tree_name );
		file_info.calculate_polarization_weight(e_pol_signs[i], e_polarization, p_pol_signs[i], p_polarization);
		file_info.calculate_number_of_events(xsections[i], luminosity);
		file_infos.push_back(file_info);
	}

	std::vector<float> cone_parameters = {0.1, 0.5, 1, 1.3, 1.5};
	std::vector<float> forced_jettopology = {2, 3, 4, 5, 6};

	ParameterSpaceVector parameter_space;
	parameter_space.add_parameter_vector(cone_parameters);
	parameter_space.add_parameter_vector(forced_jettopology);
	parameter_space.calculate_parameter_points();

	// Everything set - let's go!


	Calculation calculation;
	calculation.init(parameter_space, file_infos);
	calculation.do_calculation();
	calculation.save_results();

 return 1;
}
