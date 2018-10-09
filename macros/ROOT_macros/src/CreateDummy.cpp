
void CreateDummy( string dummy_path, int N_branches, int N_entries ) {
  TFile *dummy_file = new TFile(dummy_path.c_str(), "recreate");
  TTree* dummy_weight_tree = new TTree("weightsTree", "weightsTree");
  float weight_ptrs [N_branches];

  // Create same branches as other file had
  for (int branch=0; branch<N_branches; branch++) {
    // string branch_name = branch_names[branch];
    string branch_name = "weight" + to_string(branch+1);
    dummy_weight_tree->Branch( branch_name.c_str(), &weight_ptrs[branch], (branch_name + "/F").c_str());
    weight_ptrs[branch] = 1.0;
  }

  for (int entry=0; entry<N_entries; entry++) {
    dummy_weight_tree->Fill();
  }

  dummy_file->cd();
  dummy_weight_tree->Write();
  dummy_file->Close();
  delete dummy_file;
}