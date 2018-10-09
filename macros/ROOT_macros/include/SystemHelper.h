#ifndef SYSTEMHELPER_H
#define SYSTEMHELPER_H 1

namespace SysHelp {
  bool file_exists( string file_path );
  
  void hadd_rootfiles( string output_path, string input_paths, string options="", bool silent=true );
  void make_Nevents_treecopy( string input_path, string tree_name, int N_events, string output_path, string options="", bool silent=true );
  
  string extract_directory( string full_path );
  string extract_filename( string full_path );
} 

#endif