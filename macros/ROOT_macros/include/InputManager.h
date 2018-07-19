#ifndef INPUTMANAGER_h 
#define INPUTMANAGER_h 1

#include <dirent.h>
#include <regex.h>



class InputManager {
  
  string m_directory {};
  string m_search_extension {"root"};
  vector<string> m_all_file_names {};
  vector<string> m_root_file_paths {};
  vector<TFile*> m_root_files {};

  public:

    void setInputDirectory( string directory );
    void setFilenameExtension( string search_extension );
    
    void openFiles();
    void getFiles( vector<TFile*> &file_vector );
    void closeFiles();

  private:

    void getDirectoryContent();
    void searchFileNamesForType();
    
};



#endif