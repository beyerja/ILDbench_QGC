#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <vector>

#include <algorithm> // for std::find

#include "selectionCriteria.C"

string filename_from_path(string path) {
	/* Extracts filename-string form path by taking everything after last "/" */
 	int filename_begin_idx 	= path.rfind('/') + 1;
	string filename 	= path.substr(filename_begin_idx);
	return filename;
}


void eventSelector(string input_rootfile_path, string output_dir) {
	/* Function takes input .root file that contains a TTree.
 	TTree is read into a new TTree and a pass_selection TBranch is added.
	Then runs selection script over all events, selection script sets
	pass_selection flag to either 0 (not passed) or 1 (passed).
	New TTree is then written to output directory in file with same
	name as input file put with "selected_" added to beginning.
	*/	
		
	// Extracts name of input file and set path of output file
	string input_rootfile_name = filename_from_path(input_rootfile_path);
	string output_rootfile_name = output_dir + "/selected_" + input_rootfile_name; 

	// Read in the TTree
	TFile *file = TFile::Open(input_rootfile_path.c_str());
	TTree *tree = (TTree*)file->Get("datatrain");
	int nEntries = tree->GetEntries();

	// Open new root file
	TFile *outfile = new TFile(output_rootfile_name.c_str(),"RECREATE");

	// Copy the LCTuple 
	TTree* LCTuple = (TTree*)file->Get("MyLCTuple");
	//outfile->cd();
	TTree* LCTuple_copy = LCTuple->CloneTree();
	LCTuple_copy->SetName("LCTuple");
	LCTuple_copy->Write();	

	// Create copy of TTree in new .root-file
	TTree *new_tree = tree->CloneTree(); 
	file->Close();
	new_tree->SetName("selected_datatrain");

	// Structure: class that holds parameters and sets adresses of TBranches
	// using a function that takes pointer to new_tree.
	// Instance is created and event variables in class update automatically 
	// when new GetEntry is called.
	// Just need to pass class instance to Selection and it can do its job 
	// with the parameters provided from the instance 

	TreeEvent event;
	event.set_addresses(new_tree);

	for (int evt=0; evt<nEntries; evt++){
		new_tree->GetEntry(evt);
		eventSelection(event);
		event.fill_new_branches();
	}

	// Write resulting tree into output ROOT file
	new_tree->Write();

	// Don't also copy old trees
	gDirectory->Delete("datatrain;*"); 
	gDirectory->Delete("MyLCTuple;*"); 

	outfile->Close();

}	


