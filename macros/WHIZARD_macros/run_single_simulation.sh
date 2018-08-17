#!/bin/bash

if [[  $# != 1 ]]  ; then
        echo "usage: ./run_single_simulation.sh sim_file_path"
        exit
fi

# Directory of this script
dir=$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )
echo $dir

# Multithread HTCondor environment
standard_condor_directory=${dir}/../standard_HTCondor_submit
multithread_condor_submit=${standard_condor_directory}/multithread_htcondor_steering.submit

condor_output_directory=${dir}/../Condor_output
if [[ ! -d ${condor_output_directory} ]] ; then # Create if not existing
  mkdir ${condor_output_directory}
fi

# Get input and split into directory and filename
sim_file_path=$1
sim_file_dir=$( cd "$( dirname "${sim_file_path}"  )" && pwd  )
sim_file_name=$( basename "${sim_file_path}"  )

# Submit job to condor 
cd ${standard_condor_directory}
condor_submit ${multithread_condor_submit} arguments="cd ${sim_file_dir} && whizard ${sim_file_name}"