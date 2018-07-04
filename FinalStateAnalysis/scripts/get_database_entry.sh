#!/bin/bash


if [[  $# != 7 ]]  ; then
	echo "usage: ./get_database_entry.sh input_class_name (4f_WW/4f_ZZ..) final_state (l/sl/e2/e1..) pol (lr/rl/ll/rr) content (file/weight) COM_energy (in GeV) sw_version (01_19_05/...) detector_model (l5/...)"
	exit
fi

if [[  $4 != "file" && $4 != "weight" ]]  ; then
	echo "usage: ./get_database_entry.sh input_class_name (4f_WW/4f_ZZ..) final_state (l/sl/e2/e1..) pol (lr/rl/ll/rr) content (file/weight) COM_energy (in GeV) sw_version (01_19_05/...) detector_model (l5/...)"
	exit
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

# Specifications about physical process in the files
input_class_name=${1,,}
final_state=${2,,}
pol=${3,,}

# What it should give back
content=${4,,}

# The setup that I'm working with
E_COM=${5,,}
SW_version=${6,,}
detector_model=${7,,}


# Translate readable input into common names
if [[ ${input_class_name} == "6f_2l4q" ]] ; then
	class_name="6f_2l4q" 
fi


direction=${DIR}
if [[ -a ${direction}/event_file_dictionary.db ]] ; then
	file_name=${direction}/event_file_dictionary.db
else
	echo "no event_file_dictionary.dat in the folder."${direction}
fi



if [[  ${content} == "file" ]]  ; then
	sqlite3 ${file_name} 'select distinct files.file_path from files, processes, setups where files.process_ID=processes.process_ID and processes.setup_ID = setups.setup_ID and class="'${class_name}'" and final_state="'${final_state}'" and pol="'${pol}'" and setups.com_energy='${E_COM}' and setups.sw_version="'${SW_version}'" and setups.detector_model="'${detector_model}'" ;'  
else
	echo "wrong for the 4th argument."
fi
