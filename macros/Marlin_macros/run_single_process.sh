#!/bin/bash

if [[  $# != 1 ]]  ; then
        echo "usage: ./run_single_process.sh final_state (vvxyyx/..)"
        exit
fi

dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

# Read input variables
final_state=${1}

# Get some global variables set up which describe the sample setup 
source ./set_simulation_environment.sh

# Arrays with possible polarizations for looping
e_polarizations=("eL" "eR")
p_polarizations=("pL" "pR")

# Standard HTCondor environment
standard_condor_directory=${dir}/../standard_HTCondor_submit
standard_condor_submit=${standard_condor_directory}/standard_htcondor_steering.submit

condor_output_directory=${dir}/../Condor_output
if [[ ! -d ${condor_output_directory} ]] ; then # Create if not existing
  mkdir ${condor_output_directory}
fi


echo "Start initalizing process ${final_state} running.\n"

for e_pol in "${e_polarizations[@]}"; do
  for p_pol in "${p_polarizations[@]}"; do
    echo "Initializing polarization ${e_pol} ${p_pol}."
    { # Use this scope for parallization of loop (maybe better with function)
      
    # Array to collect condor job IDs so that I can keep track of if any are still running
    condor_job_IDs=()
    
    # Get all steering files for process, then loop to create job for each file
    steering_files=$( ./manage_process_files.sh --set-steeringfiles ${final_state} ${e_pol} ${p_pol} )
    
    if [[ $steering_files == "" ]]; then
      echo "No files found for process ${final_state} ${e_pol} ${p_pol}."
      exit # If empty no steering file found -> exit subprocess
    else 
      echo "Found files for ${final_state} ${e_pol} ${p_pol}, send Marlin jobs to BIRD cluster."
    fi
    
    # Start jobs for each steering file
    cd ${standard_condor_directory}
    
    for steering_file in ${steering_files[@]}; do
      # Submit job to HTCondor using standard submitting setup
      # -> Start Marlin job and keep track of job ID to know when it's done
      condor_job_output=$(condor_submit ${standard_condor_submit} arguments="Marlin ${steering_file}")
      
      # Split output up by spaces and only read last part (which is cluster ID).
      # Details at: https://stackoverflow.com/questions/3162385/how-to-split-a-string-in-shell-and-get-the-last-field
      condor_job_IDs+="${condor_job_output##* } "
    done
    cd ${dir}
    
    echo "Waiting for jobs of ${final_state} ${e_pol} ${p_pol} to finish."
    for job_ID in ${condor_job_IDs[@]}; do
      job_log_path=$(ls ${condor_output_directory}/${job_ID}*.log)
      wait_output=$(condor_wait ${job_log_path}) # Write into variable to suppress spammy output
    done
    echo "Jobs of ${final_state} ${e_pol} ${p_pol} finished!"
    
    echo "Combine individual files of ${final_state} ${e_pol} ${p_pol} and clean up temporary stuff."
    ./manage_process_files.sh --combine-output ${final_state} ${e_pol} ${p_pol}
    ./manage_process_files.sh --clean-up ${final_state} ${e_pol} ${p_pol}
    
    echo "Done with ${final_state} ${e_pol} ${p_pol}!"
    } &
  done
done
wait
echo "Done with process ${final_state}!"



