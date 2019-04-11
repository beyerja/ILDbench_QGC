#!/bin/bash

if [[  $# != 0 ]]  ; then
        echo "usage: ./run_all_simulations.sh"
        exit
fi

# Directory of this script
dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

# Get WHIZARD output directory (python writes to stderr for some reason...)
whizard_directory=$(python ${dir}/GetOutputDirectory.py 2>&1)

# Search the signal directory for rescan files
rescan_files=()
# Loop over all directories, only take rescan files when the directory contains
# a non-empty slcio file (ASSUMES OUTPUT FORMAT SLCIO).
for process_dir in "${whizard_directory}/signal/"*/; do
  contains_simulated_events=false
  event_files=$(ls ${process_dir}/*.slcio)
  for event_file in ${event_files[@]}; do
    n_events=$(lcio_event_counter ${event_file})
    if [[  ${n_events} > 0 ]]; then
      contains_simulated_events=true
    fi
  done
  
  if [ "${contains_simulated_events}" = true ] ; then
    process_rescan=$(ls ${process_dir}/rescan_*.sin)
    rescan_files+="${process_rescan} "
  fi
done

# Start simulation for each sim file
for rescan_file in ${rescan_files[@]}; do
  ./run_single_rescan.sh ${rescan_file}
  sleep 1
done