#ifndef INPUTMANAGER_h 
#define INPUTMANAGER_h 1

#include <dirent.h>
#include <regex.h>



class InputManager {
  
  string m_directory {};
  string m_search_extension {"root"};
  vector<string> m_all_file_names {};
  vector<string> m_root_file_paths {};

  public:

    void setInputDirectory( string directory );
    void setFilenameExtension( string search_extension );
    
    void findFiles();
    void getFilePaths( vector<string> &filename_vector );

  private:

    void getDirectoryContent();
    void searchFileNamesForType();
    
};



#endif