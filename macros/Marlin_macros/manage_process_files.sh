#!/bin/bash

if [[  $# != 4 ]]  ; then
        echo "usage: ./manage_process_files.sh [--get-paths/--combine-output] final_state (vvxyyx..) e-pol (eL/pL) p-pol (pL/pR)"
        exit
fi

dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

# Requested action
action=${1}

# Specifications about physical process in the files
final_state=${2}
e_pol=${3}
p_pol=${4}

if  [[ ${action} == "--get-paths" ]]; then
  files_dir=$( ${dir}/get_setup_directory.sh )
  cd ${files_dir}

  # Do case-insensitive (`-iname`) search in directory (and subdirectories) for relevant DST files 
  # according to convention of DST file naming and print complete paths (->`pwd`).
  find `pwd` -iname "*rv${RECONSTRUCTION_SOFTWARE_VERSION}*sv${SIMULATION_SOFTWARE_VERSION}*mILD_${DETECTOR_MODEL_VERSION}*E${COLLIDER_ENERGY}*${final_state}*${e_pol}.${p_pol}*DST*.slcio" -print
  
else if [[ ${action} == "--combine-output" ]]; then
  
fi