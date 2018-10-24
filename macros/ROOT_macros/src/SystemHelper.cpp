#include "../include/SystemHelper.h"

//-------------------------------------------------------------------------------------------------


bool SysHelp::pathExists( string path ){
  return access( path.c_str(), F_OK ) != -1;
}

//-------------------------------------------------------------------------------------------------

void SysHelp::haddROOTFiles( string output_path, string input_paths, string options, bool silent){
  string hadd_command = " hadd " + options + " " + output_path + " " + input_paths; // Create run_system_silent
  if ( silent ) { hadd_command += " >/dev/null"; }
  system(hadd_command.c_str());
}

//-------------------------------------------------------------------------------------------------

void SysHelp::makeNEventsTreeCopy( string input_path, string tree_name, int N_events, string output_path, string options, bool silent){
  string copy_command = "rooteventselector " + options + " -f 1 -l " + to_string(N_events) + " " + input_path + ":" + tree_name + " " + output_path;
  if ( silent ) { copy_command += " >/dev/null"; }
  system(copy_command.c_str());
}

//-------------------------------------------------------------------------------------------------

string SysHelp::extractDirectory( string full_path ){
  return full_path.substr(0, full_path.find_last_of('/'));
}

//-------------------------------------------------------------------------------------------------

string SysHelp::extractFilename( string full_path ){
  string directory = SysHelp::extractDirectory(full_path);
  return full_path.substr(directory.size() + 1);
}

//-------------------------------------------------------------------------------------------------

void SysHelp::createDirectory( string path ) {
  // By default also creates necessary parent directories if possible
  string mkdir_command = "mkdir -p " + path;
  if ( ! SysHelp::pathExists(path) ) {
    system(mkdir_command.c_str());
  }
}

//-------------------------------------------------------------------------------------------------