#!/bin/bash

# Get environment
source set_aQGC_environment.sh

for subdir in "signal" "bkg"; do
  # Loop over all directories
  for process_dir in "${WHIZARD_OUTPUT_DIRECTORY}/${subdir}/"*; do
    ./run_single_aQGC_process.sh ${process_dir}
    sleep 0.5
  done
done