#!/bin/bash

if [[ $# == 2 ]] ; then
	echo "begin to cal events!"
	class_name=${1,,}
	info=${2,,}
else
	echo "usage: ./run_class.sh class_name run_info(Marlin,other)"
	exit
fi


E_COM=1000
SW_version=$CURRENT_ILCSOFT_VERSION
detector_model=$CURRENT_ILD_MODEL

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [[ ${class_name} = "6f_2l4q" ]] ; then
	class_name_array=("6f_2l4q")
	final_state=("vvxyyx" "vvxxxx" "vvyyyy")
else
	echo ""
	echo "ERROR in run_class.sh: Unknown event class name!"
	echo ""
	exit
fi


pol_profile=("lr") #  "rl" "ll" "rr")
pol_length=${#pol_profile[@]}

final_state_length=${#final_state[@]}
class_name_length=${#class_name_array[@]}

echo ""
echo "has set all variables!"
echo ""


echo "begin to looping over initial and final states!"
echo ""

# Loop over all inital states belonging to this event class
for (( k=0; k<$(( $class_name_length)); k++ ))
do
	current_class_name=${class_name_array[k]}

	# Loop over all final states
	for (( i=0; i<$(( $final_state_length )); i++ ))
	do
		{
		current_final_state=${final_state[i]}

		# Array to collect condor job IDs so that I can keep track of if any are still running
		condor_job_IDs=()

		# Run Marlin jobs only if asked to do so
		if [[ $info == "marlin" ]] ; then

			# Loop over all initial polarization states
			for (( j=0; j<$(( $pol_length )); j++ ))
			do
				pol=${pol_profile[j]}
				${DIR}/set_xml.sh ${current_class_name} ${current_final_state} ${pol} ${E_COM} ${SW_version} ${detector_model}

				# Loop over subfiles of the process
				# (Process events may be split up over several files)
				files=($(${DIR}/get_database_entry.sh  ${current_class_name} ${current_final_state} ${pol} file ${E_COM} ${SW_version} ${detector_model}))
				nfiles=${#files[@]}
				echo "generate samples for:" ${current_class_name} ${current_final_state} ${pol}
				for n in `seq 1 ${nfiles}`; do

					tmp_xml_dir=tmp_${current_class_name}_${current_final_state}_${pol}_${n}
					xml_file=My_${current_class_name}_${current_final_state}_${pol}_${n}.xml
					empty=" "

					if [[ -a ${DIR}/../xml/${tmp_xml_dir}/${xml_file} ]] ; then
						# Submit Marlin run to HTCondor
						condor_job_output=$(condor_submit ${DIR}/single_HTC_job.submit arguments="${tmp_xml_dir} ${xml_file}")
						# Split output up by spaces and only read last part (which is cluster ID)
						# For details on splitting look at:
						# https://stackoverflow.com/questions/3162385/how-to-split-a-string-in-shell-and-get-the-last-field
						condor_job_IDs+=${condor_job_output##* }
						condor_job_IDs+=${empty}
					fi
				done
			done
		fi

		sleep 1
		# Call subscript that will do file-merging (using hadd) for this IS- FS combination
		# Needs the IDs of the condor jobs to check if any are still running (only if Marlin jobs were started)
		./subscript_class_filemerge.sh ${current_class_name} ${current_final_state} ${info} ${condor_job_IDs}
		} &
	done
done

wait
echo " "
