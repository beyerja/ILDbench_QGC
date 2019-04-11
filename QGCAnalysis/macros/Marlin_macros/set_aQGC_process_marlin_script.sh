#!/bin/bash

if [[  $# < 3 ]]  ; then
  echo "usage: ./set_process_marlin_script.sh steering_file output_file input_files"
  exit
fi

# Get input parameters
steering_file=${1}
output_file=${2}

shift 2
input_files=$@

# Detemine detector model from input file and set according compact file
# -> Get detector parameter line from dumpevent and extract model name
first_input_file=$(echo $input_files | awk '{print $1;}')
detector_model=$( dumpevent ${input_files[0]} 1 | grep " detector :" | rev | cut -d' ' -f1 | rev )
if [[ ${detector_model} == "unknown" ]]; then
  compact_file="${lcgeo_DIR}/ILD/compact/${detector_model}/${detector_model}.xml"
else
  compact_file="${lcgeo_DIR}/ILD/compact/${detector_model}/${detector_model}.xml"
fi

# Line numbers of lines in template that need to be replaced
input_files_line=11
output_file_line=27

# Replace the relevant lines (in decreasing line order to avoid problems caused by added line breaks)
sed -i "${output_file_line}s\.*\ ${output_file} \  " ${steering_file}
sed -i "${input_files_line}s\.*\ ${input_files} \  " ${steering_file}
