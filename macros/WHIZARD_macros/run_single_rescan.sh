#!/bin/bash

if [[  $# != 1 ]]  ; then
        echo "usage: ./run_single_rescan.sh rescan_file_path"
        exit
fi

# Directory of this script
dir=$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )
echo $dir

# Standard HTCondor environment
standard_condor_directory=${dir}/../standard_HTCondor_submit
standard_condor_submit=${standard_condor_directory}/standard_htcondor_steering.submit

condor_output_directory=${dir}/../Condor_output
if [[ ! -d ${condor_output_directory} ]] ; then # Create if not existing
  mkdir ${condor_output_directory}
fi

# Get input and split into directory and filename
rescan_file_path=$1
rescan_file_dir=$( cd "$( dirname "${rescan_file_path}"  )" && pwd  )
rescan_file_name=$( basename "${rescan_file_path}"  )

# Submit job to condor 
cd ${standard_condor_directory}
condor_submit ${standard_condor_submit} arguments="cd ${rescan_file_dir} && whizard ${rescan_file_name}"