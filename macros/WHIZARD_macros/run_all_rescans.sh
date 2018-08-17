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
# Loop over all directories
for process_dir in "${whizard_directory}/signal/"*/; do
  process_rescan=$(ls ${process_dir}/rescan_*.sin)
  rescan_files+="${process_rescan} "
done

# Start simulation for each sim file
for rescan_file in ${rescan_files[@]}; do
  ./run_single_rescan.sh ${rescan_file}
done