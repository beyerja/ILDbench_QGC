#!/bin/bash

if [[  $# != 1 ]]  ; then
        echo "usage: ./run_single_aQGC_process.sh input_directory"
        exit
fi

# Directory of this bash script
dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

# Read input variables
input_directory=${1}

# Find all input files: Ignore basic_events files (needed for rescan only)
input_files="$(ls ${input_directory}/*events.*.slcio | grep -v 'basic_events')"

if [[ $input_files == "" ]]; then
  echo "Found no event files in directory ${input_directory} - no Marlin run started."
  exit
else 
  echo "Found event files in ${input_directory} - proceeding."
fi

# Output file name: remove file-extension and .0 and replace with .root
first_input_file=$(echo $input_files | awk '{print $1;}')
base_name="${first_input_file%.*}" # remove .slcio
base_name="${base_name%.*}" # remove .0
output_file="${base_name}.root"

# Get steering file path
source set_aQGC_environment.sh

steering_directory=${dir}/../../scripts/aQGC_processor_steering
process_steering_directory=${base_name}_Marlin_steering
process_steering_file=${process_steering_directory}/Marlin_steering.xml
    
if [[ ! -d ${process_steering_directory} ]] ; then # Create if not existing
  mkdir ${process_steering_directory}
fi

# Set up steering file    
cp ${steering_directory}/${XML_TEMPLATE_NAME} ${process_steering_file}
${dir}/set_aQGC_process_marlin_script.sh ${process_steering_file} ${output_file} ${input_files} 

# Run Marlin on BIRD cluster
standard_condor_directory=${dir}/../standard_HTCondor_submit
standard_condor_submit=${standard_condor_directory}/standard_htcondor_steering.submit
cd ${standard_condor_directory}
condor_submit ${standard_condor_submit} arguments="Marlin ${process_steering_file}"