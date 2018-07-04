#!/bin/bash

# Script manipulates subscript_class_filemerge.sh and template.xml and
# calls ./run_class.sh so that overlay removal is done using varying FastJet
# parameters.

# All relevant direcories
LOCAL=/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons/scripts
XML_DIR=/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons/xml
OUT_DIR=/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/testFastJetOverlayRemoval_output/new_samples

# Relevant files
subscript_file=${LOCAL}/subscript_class_filemerge.sh
template_file=${XML_DIR}/template.xml

# Lines in the files that need to be changed
subscript_out_line=32
template_cone_line=82
template_Njet_line=84

# Retrieve original lines in files to set back later
original_subscript_out_line="`sed -n ${subscript_out_line}p ${subscript_file}`"
original_template_cone_line="`sed -n ${template_cone_line}p ${template_file}`"
original_template_Njet_line="`sed -n ${template_Njet_line}p ${template_file}`"

# All event classes relevant for vbs
vbs_class_names=("6f_2l4q")
class_name_length=${#vbs_class_names[@]}

# FastJet parameters for overlay removal
cone_parameters=("1.5") #("0.1" "0.5" "1" "1.3" "1.5")
forcedNjets=("6") #("2" "3" "4" "5" "6")

cone_parameters_length=${#cone_parameters[@]}
forcedNjets_length=${#forcedNjets[@]}

# Loop over all cone parameters and Njets in FastJet algorithm
for (( i=0; i<$(( $cone_parameters_length)); i++ )) do
  cone_parameter=${cone_parameters[i]}
  for (( j=0; j<$(( $forcedNjets_length)); j++ )) do
	Njet=${forcedNjets[j]}

	echo ""
	echo "---------------------------------------------------------"
	echo "Running for cone parameter "${cone_parameter}" and Njet "${Njet}
	echo "---------------------------------------------------------"
	echo ""

	# Adjust the steering file template
	cone_string='<parameter name="algorithm" type="StringVec"> kt_algorithm '${cone_parameter}' </parameter>'
	Njet_string='<parameter name="clusteringMode" type="StringVec"> ExclusiveNJets '${Njet}' </parameter>'
	sed -i "${template_cone_line}s\.*\ ${cone_string} \  " ${template_file}
	sed -i "${template_Njet_line}s\.*\ ${Njet_string} \  " ${template_file}

	# Adjust the filemerger script
	datadir_string='data_directory='
	THIS_OUT_DIR=${OUT_DIR}/cone_parameter_${cone_parameter}/rootfiles_force${Njet}jets
	sed -i "${subscript_out_line}s\.*\ ${datadir_string}${THIS_OUT_DIR} \  " ${subscript_file}
	
	# Do Marlin run for each relevant class
	for (( k=0; k<$(( $class_name_length)); k++ )) do
		${LOCAL}/run_class.sh ${vbs_class_names[k]} Marlin & 
	done

	wait
	sleep 5
	## Don't start new jobs while others are still running:
	username='beyerjac'
	not_running_jobs_string='Total for '${username}': 0 jobs; 0 completed, 0 removed, 0 idle, 0 running, 0 held, 0 suspended'
	is_condor_running=true
	while [ $is_condor_running = true ] ; do # [[ $(condor_q) != *${not_running_jobs_string}* ]]; do
		sleep 5
		if [[ $(condor_q) == *${not_running_jobs_string}* ]]; then
			is_condor_running=false
		fi
	done

	# Hopefully gives the subscript enough time to do its actions
	sleep 15
  done
done

# Set back lines in script to original ones
sed -i "${subscript_out_line}s\.*\ ${original_subscript_out_line} \  " ${subscript_file}
sed -i "${template_cone_line}s\.*\ ${original_template_cone_line} \  " ${template_file}
sed -i "${template_Njet_line}s\.*\ ${original_template_Njet_line} \  " ${template_file}
