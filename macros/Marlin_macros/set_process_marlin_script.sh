#!/bin/bash

if [[  $# != 3 ]]  ; then
  echo "usage: ./set_process_marlin_script.sh steering_file input_file output_file"
  exit
fi

# Get input parameters
steering_file=${1}
input_file=${2}
output_file=${3}

# Line numbers of lines in template that need to be replaced
input_file_line=13
output_file_line=80

isolep_ee_weights_line=32 
isolep_mm_weights_line=35

# Replace the relevant lines (in decreasing line order to avoid problems caused be added line breaks)
sed -i "${output_file_line}s\.*\ ${output_file} \  " ${steering_file}
sed -i "${isolep_mm_weights_line}s\.*\ ${ISOLEPFINDER_WEIGHTS_mm} \  " ${steering_file}
sed -i "${isolep_ee_weights_line}s\.*\ ${ISOLEPFINDER_WEIGHTS_ee} \  " ${steering_file}
sed -i "${input_file_line}s\.*\ ${input_file} \  " ${steering_file}
