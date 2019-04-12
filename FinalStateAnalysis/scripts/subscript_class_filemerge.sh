#!/bin/bash

class_name=${1,,}
final_state=${2,,}
info=${3,,}

shift 3
condor_job_IDs=$@

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

condor_output_directory="${DIR}/BIRD_output"

if [[ $info == "marlin" ]] ; then

	# Test if at least one of the jobs is still running = has status
	for condor_job_ID in ${condor_job_IDs[@]}; do
    job_log_path=$(ls ${condor_output_directory}/${condor_job_ID}*.log)
    wait_output=$(condor_wait ${job_log_path}) # Write into variable to suppress spammy output
	done
  sleep 20
	
	rm -rf ${DIR}/../xml/tmp_${class_name}_${final_state}_*

  echo ""
	echo "Combine root files for one process and final state."
	split_data_directory=/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/tmp_rootfiles
 	data_directory=/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/rootfiles   
   	#data_directory=/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/testFastJetOverlayRemoval_output/cone_parameter_1/rootfiles_force4jets    

	pol_profile=("lr" "rl" "ll" "rr")
	pol_length=${#pol_profile[@]}
	
	# First add all lctuples together, then add all others including lctuple
	for (( j=0; j<$(( $pol_length )); j++ ))
	do
		polarization=${pol_profile[j]}
		if [[ -a ${split_data_directory}/${class_name}_${final_state}_${polarization}_1.root ]] ; then
			hadd -k -f ${data_directory}/${class_name}_${final_state}_${polarization}.root ${split_data_directory}/${class_name}_${final_state}_${polarization}_*.root
			wait
			rm ${split_data_directory}/${class_name}_${final_state}_${polarization}_*.root
		fi
	done
fi
wait

exit

