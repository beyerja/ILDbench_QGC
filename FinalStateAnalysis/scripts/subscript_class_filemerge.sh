#!/bin/bash


class_name=${1,,}
final_state=${2,,}
info=${3,,}

shift 3
condor_job_IDs=$@

DIR=/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons/scripts

if [[ $info == "marlin" ]] ; then

	# Test if at least one of the jobs is still running = has status
	for condor_job_ID in ${condor_job_IDs[@]}; do

		# Does the output of all my running jobs contain the job number?
		# If so: wait a bit.
		# The -nobatch option is used so that a line is diplayed for each job!
		# Otherwise JobIDs will be shortened with "..." and I won't find them 
		while [[ $(condor_q -nobatch beyerjac) == *$condor_job_ID* ]]; do
			sleep 5
		done
	
	done
	
	rm -rf ${DIR}/../xml/tmp_${class_name}_${final_state}_*

	echo "combine root files for one process and final state"
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
			hadd -k -f ${split_data_directory}/${class_name}_${final_state}_${polarization}_lctuple.root ${split_data_directory}/${class_name}_${final_state}_${polarization}_*_lctuple.root
			wait
			rm ${split_data_directory}/${class_name}_${final_state}_${polarization}_*_lctuple.root

			wait

			hadd -k -f ${data_directory}/${class_name}_${final_state}_${polarization}.root ${split_data_directory}/${class_name}_${final_state}_${polarization}_*.root
			wait
			rm ${split_data_directory}/${class_name}_${final_state}_${polarization}_*.root
			
		fi
	done
fi
wait

exit
#qsub ./subscript_channel_analysis.sh ${class_name} ${final_state} ${mode} ${mass}

