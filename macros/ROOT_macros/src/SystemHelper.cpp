#include "../include/SystemHelper.h"

//-------------------------------------------------------------------------------------------------


bool SysHelp::file_exists( string file_path ){
  return access( file_path.c_str(), F_OK ) != -1;
}

//-------------------------------------------------------------------------------------------------

void SysHelp::hadd_rootfiles( string output_path, string input_paths, string options, bool silent){
  string hadd_command = " hadd " + options + " " + output_path + " " + input_paths; // Create run_system_silent
  if ( silent ) { hadd_command += " >/dev/null"; }
  system(hadd_command.c_str());
}

//-------------------------------------------------------------------------------------------------

void SysHelp::make_Nevents_treecopy( string input_path, string tree_name, int N_events, string output_path, string options, bool silent){
  string copy_command = "rooteventselector " + options + " -f 1 -l " + to_string(N_events) + " " + input_path + ":" + tree_name + " " + output_path;
  if ( silent ) { copy_command += " >/dev/null"; }
  system(copy_command.c_str());
}

//-------------------------------------------------------------------------------------------------

string SysHelp::extract_directory( string full_path ){
  return full_path.substr(0, full_path.find_last_of('/'));
}

//-------------------------------------------------------------------------------------------------

string SysHelp::extract_filename( string full_path ){
  string directory = SysHelp::extract_directory(full_path);
  return full_path.substr(directory.size() + 1);
}

//-------------------------------------------------------------------------------------------------