#!/bin/bash

if [[  $# != 4 ]]  ; then
        echo "usage: ./manage_process_files.sh [--set-steeringfiles/--combine-output/--clean-up] final_state (vvxyyx..) e-pol (eL/pL) p-pol (pL/pR)"
        exit
fi

dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

# Requested action
action=${1}

# Specifications about physical process in the files
final_state=${2}
e_pol=${3}
p_pol=${4}

# Get output paths
source set_output_environment.sh

steering_directory=${dir}/../../scripts/aQGC_processor_steering

# Base name and steering directory for specific to the input variables
base_name=rv${RECONSTRUCTION_SOFTWARE_VERSION}_sv${SIMULATION_SOFTWARE_VERSION}_mILD_${DETECTOR_MODEL_VERSION}_E${COLLIDER_ENERGY}_${final_state}_${e_pol}_${p_pol}
tmp_base_name=tmp_${base_name}
tmp_steering_directory=${steering_directory}/${tmp_base_name}

if  [[ ${action} == "--set-steeringfiles" ]]; then
  files_dir=$( ${dir}/get_setup_directory.sh )
  cd ${files_dir}

  # Do case-insensitive (`-iname`) search in directory (and subdirectories) for relevant DST files 
  # according to convention of DST file naming and print complete paths (->`pwd`).
  # # Additional problem addressed by if statement: New high-Q^2 6f samples are in same main directory as normal files
  # # => If they are supposed to be used (main directory set as high-Q^2) use them, else ignore that specific directory
  if [[ ${files_dir} == *"6f_WWS"* ]]; then
    files=$(find `pwd` -iname "*rv${RECONSTRUCTION_SOFTWARE_VERSION}*sv${SIMULATION_SOFTWARE_VERSION}*mILD_${DETECTOR_MODEL_VERSION}*E${COLLIDER_ENERGY}*${final_state}*${e_pol}.${p_pol}*DST*.slcio" -print)
  else 
    ignore_dir="/pnfs/desy.de/ilc/prod/ilc/mc-opt-3/ild/dst-merged/1000-B1b_ws/6f_WWS"
    files=$(find `pwd` -path "${ignore_dir}" -prune -o -iname "*rv${RECONSTRUCTION_SOFTWARE_VERSION}*sv${SIMULATION_SOFTWARE_VERSION}*mILD_${DETECTOR_MODEL_VERSION}*E${COLLIDER_ENERGY}*${final_state}*${e_pol}.${p_pol}*DST*.slcio" -print)
  fi
  
  file_steering_paths=() # Collect paths of steering files for job starting
  
  file_index=0
  for file in ${files[@]}; do
    if [[ $file == "" ]]; then
      exit # If empty nofile found -> exit subprocess
    fi
    
    file_index=$((file_index + 1))
    
    tmp_steering_file=${tmp_steering_directory}/${tmp_base_name}_${file_index}_steering.xml
    tmp_output_file=${TMP_OUTPUT_DIRECTORY}/${tmp_base_name}_${file_index}.root
    
    if [[ ! -d ${tmp_steering_directory} ]] ; then # Create if not existing
      mkdir ${tmp_steering_directory}
    fi
    
    cp ${steering_directory}/${XML_TEMPLATE_NAME} ${tmp_steering_file}
    
    ${dir}/set_process_marlin_script.sh ${tmp_steering_file} ${file} ${tmp_output_file}
    
    file_steering_paths+="${tmp_steering_file} " # Keep track of steering file for job starting
  done
  
  echo ${file_steering_paths} # Echo steering files that need to be run
  
elif [[ ${action} == "--combine-output" ]]; then
  hadd -k -f ${OUTPUT_DIRECTORY}/${base_name}.root ${TMP_OUTPUT_DIRECTORY}/${tmp_base_name}_*.root
  
elif [[ ${action} == "--clean-up" ]]; then
  rm -r ${tmp_steering_directory}
  rm ${TMP_OUTPUT_DIRECTORY}/${tmp_base_name}_*.root
  
else   
  echo "In manage_process_files.sh: Unknown action '"${action}"' requested."
  echo "Options are: --set-steeringfiles --combine-output --clean-up"
  echo "Exiting."
  exit
fi