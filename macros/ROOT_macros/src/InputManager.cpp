#include "../include/InputManager.h"

//-------------------------------------------------------------------------------------------------

void InputManager::setInputDirectory( string directory ) {
  m_directory = directory;
}

//-------------------------------------------------------------------------------------------------

void InputManager::setFilenameExtension( string search_extension ) {
  m_search_extension = search_extension;
}

//-------------------------------------------------------------------------------------------------

void InputManager::getFilePaths( vector<string> &filename_vector ) {
  filename_vector = m_root_file_paths;
}

//-------------------------------------------------------------------------------------------------

void InputManager::cleanDirectoryContent(){
  m_all_file_names = {};
}

//-------------------------------------------------------------------------------------------------

void InputManager::getDirectoryContent()
// Copy-pasted from: https://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/
{
  DIR *dp;
  struct dirent *dirp;
  if((dp  = opendir(m_directory.c_str())) == NULL) {
    cout << "Error(" << errno << ") opening " << m_directory << endl;
    return errno;
  }

  while ((dirp = readdir(dp)) != NULL) {
    m_all_file_names.push_back(string(dirp->d_name));
  }
  closedir(dp);
}

//-------------------------------------------------------------------------------------------------

void InputManager::searchFileNamesForType()
{
  regex regular_expression {".*" + m_search_extension + ""};

  for ( auto const& file_name : m_all_file_names ) {
    if ( regex_match( file_name, regular_expression ) ) {
      m_root_file_paths.push_back( m_directory + "/" + file_name );
    }
  }
}

//-------------------------------------------------------------------------------------------------

void InputManager::findFiles() {
    this->cleanDirectoryContent();
    this->getDirectoryContent();
    this->searchFileNamesForType();
}

//-------------------------------------------------------------------------------------------------


