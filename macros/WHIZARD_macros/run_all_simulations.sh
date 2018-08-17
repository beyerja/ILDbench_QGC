#!/bin/bash

if [[  $# != 0 ]]  ; then
        echo "usage: ./run_all_simulations.sh"
        exit
fi

# Directory of this script
dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

# Get WHIZARD output directory (python writes to stderr for some reason...)
whizard_directory=$(python ${dir}/GetOutputDirectory.py 2>&1)

# Search the signal and background directories for dirs that include simulation
# steering and collect those files
sim_files=()
for subdir in "signal" "bkg"; do
  # Loop over all directories
  for process_dir in "${whizard_directory}/${subdir}/"*/; do
    process_sim=$(ls ${process_dir}*_sim.sin)
    sim_files+="${process_sim} "
  done
done

# Start simulation for each sim file
for sim_file in ${sim_files[@]}; do
  ./run_single_simulation.sh ${sim_file}
done