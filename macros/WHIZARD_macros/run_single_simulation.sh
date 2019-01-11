#!/bin/bash

if [[  $# != 1 ]]  ; then
        echo "usage: ./run_single_simulation.sh sim_file_path"
        exit
fi

# Directory of this script
dir=$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )

# Multithread HTCondor environment
standard_condor_directory=${dir}/../standard_HTCondor_submit
multithread_condor_submit=${standard_condor_directory}/multithread_htcondor_steering.submit

condor_output_directory=${dir}/../Condor_output
if [[ ! -d ${condor_output_directory} ]] ; then # Create if not existing
  mkdir ${condor_output_directory}
fi

# TODO Set this centrally!
# Need newer compiler version (same as WHIZARD uses) -> WARNING May need to be updated in new WHIZARD version!
gcc_path="/nfs/dust/ilc/user/berggren/whiz2-tests/gcc6.3/x86_64-slc6/bin" 

# Get input and split into directory and filename
sim_file_path=$1
sim_file_dir=$( cd "$( dirname "${sim_file_path}"  )" && pwd  )
sim_file_name=$( basename "${sim_file_path}"  )

# Submit job to condor 
cd ${standard_condor_directory}
condor_submit ${multithread_condor_submit} arguments="export PATH=${gcc_path}:\${PATH} \&\& cd ${sim_file_dir} \&\& whizard ${sim_file_name}"