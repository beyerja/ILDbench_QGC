#!/bin/bash

if [[  $# != 1 ]]  ; then
        echo "usage: ./run_single_process.sh final_state (vvxyyx..)"
        exit
fi

# Read input variables
final_state=${1}

# Get some global variables set up which describe the sample setup 
source ./set_simulation_environment.sh

# Arrays with possible polarizations for looping
e_polarizations=("eL" "eR")
p_polarizations=("pL" "pR")

for e_pol in "${e_polarizations[@]}"; do
  for p_pol in "${p_polarizations[@]}"; do
    { # Use this scope for parallization of loop (maybe better with function)
      
    # Array to collect condor job IDs so that I can keep track of if any are still running
    condor_job_IDs=()
    
    # Get all files for process, then loop to create job for each file
    process_files=$( ./manage_process_files.sh --get-paths ${final_state} ${e_pol} ${p_pol} )
      
    ### TODO Maybe better: let manage_process_files do the setting of marlin scripts -> Then it has control over all paths
    } &
  done
done




condor_job_output=$(condor_submit executable=${}) # -> Setting executable here allows me to have one central submit file
# Split output up by spaces and only read last part (which is cluster ID).
# Details at: https://stackoverflow.com/questions/3162385/how-to-split-a-string-in-shell-and-get-the-last-field
condor_job_IDs+=${condor_job_output##* }
condor_job_IDs+=${empty}
###FOR LOOP END


for job_ID in "${condor_job_IDs[@]}"; do
  condor_wait ${condor_output_dir}/${job_ID}*.log
done

./manage_process_files.sh --combine-output ${process_details}