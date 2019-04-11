#ifndef SYSTEMHELPER_H
#define SYSTEMHELPER_H 1

namespace SysHelp {
  bool pathExists( string path );
  
  void haddROOTFiles( string output_path, string input_paths, string options="", bool silent=true );
  void makeNEventsTreeCopy( string input_path, string tree_name, int N_events, string output_path, string options="", bool silent=true );
  
  string extractDirectory( string full_path );
  string extractFilename( string full_path );
  
  void createDirectory( string path );
} 

#endif