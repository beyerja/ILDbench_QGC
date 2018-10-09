#include "../include/PythonHelper.h"

//-------------------------------------------------------------------------------------------------

vector<vector<float>> PyHelp::read2DMatrixFromFile ( string &py_file_path ) {
  // Reads a 2D python array from a Python file
  ifstream file(py_file_path);

  if ( !file.is_open() ) {
    cout << "ERROR in read2DMatrixFromFile: File not opened " << py_file_path << endl;
    exit(1);
  }

  vector<vector<float>> matrix{};
  while (!file.eof()){
    string line;
    getline(file,line);
    line.erase(remove(line.begin(), line.end(), '['), line.end());
    line.erase(remove(line.begin(), line.end(), ']'), line.end());
    istringstream buffer(line);
    vector<string> values { istream_iterator<string>(buffer),
                          istream_iterator<string>()};
    vector<float> line_values{};
    for ( auto &value: values ) {
      line_values.push_back(stof(value));
    }
    matrix.push_back(line_values);
  }
  file.close();

  return matrix;
}

//-------------------------------------------------------------------------------------------------