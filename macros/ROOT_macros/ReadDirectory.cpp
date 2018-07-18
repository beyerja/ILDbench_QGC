#include <dirent.h>
#include <regex.h>

// Copy-pasted from: https://www.linuxquestions.org/questions/programming-9/c-list-files-in-directory-379323/

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

vector<string> searchDirectoryForFileType( string directory, string file_extension )
{
    regex regular_expression {"(.*)(." + file_extension + " )"};
    vector<string> all_files {};
    vector<string> matched_files {};

    getdir( directory , all_files );

    for (unsigned int i = 0; i<all_files.size(); i++) {
      if ( regex_match( all_files[i], regular_expression ) ) {
        matched_files.push_back(all_files[i]);
      }
    }
    
    return matched_files;
}