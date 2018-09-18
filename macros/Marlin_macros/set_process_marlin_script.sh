#!/bin/bash

if [[  $# != 3 ]]  ; then
  echo "usage: ./set_process_marlin_script.sh steering_file input_file output_file"
  exit
fi

# Get input parameters
steering_file=${1}
input_file=${2}
output_file=${3}

# Detemine detector model from input file and set according compact file
# -> Get detector parameter line from dumpevent and extract model name
detector_model=$( dumpevent ${input_file} 1 | grep " detector :" | rev | cut -d' ' -f1 | rev )
compact_file="${lcgeo_DIR}/ILD/compact/${detector_model}/${detector_model}.xml"

# Line numbers of lines in template that need to be replaced
input_file_line=14
output_file_line=90

isolep_ee_weights_line=42
isolep_mm_weights_line=45

compact_file_line=31

# Replace the relevant lines (in decreasing line order to avoid problems caused by added line breaks)
sed -i "${output_file_line}s\.*\ ${output_file} \  " ${steering_file}
sed -i "${isolep_mm_weights_line}s\.*\ ${ISOLEPFINDER_WEIGHTS_mm} \  " ${steering_file}
sed -i "${isolep_ee_weights_line}s\.*\ ${ISOLEPFINDER_WEIGHTS_ee} \  " ${steering_file}
sed -i "${compact_file_line}s\.*\ ${compact_file} \  " ${steering_file}
sed -i "${input_file_line}s\.*\ ${input_file} \  " ${steering_file}
