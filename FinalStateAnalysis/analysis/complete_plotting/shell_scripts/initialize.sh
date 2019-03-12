#!/bin/bash

# Directory of this script
THIS_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Input settings
luminosity="1000"
e_beam_polarization="-0.8"
p_beam_polarization="0.3"

input_directory="/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/s5_output/rootfiles_after_selection"
output_directory="/afs/desy.de/group/flc/pool/beyerjac/VBS/nunu_hadron/v02-00-02_s5_o1_v02_output"

# input_directory="/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/l5_output/rootfiles_after_selection"
# output_directory="/afs/desy.de/group/flc/pool/beyerjac/VBS/nunu_hadron/v02-00-02_l5_o1_v02_output"

# input_directory="/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/backup_19_05_output/rootfiles_after_selection"
# input_directory="/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/backup_19_05_output/rootfiles_after_selection"
# output_directory="/afs/desy.de/group/flc/pool/beyerjac/backup/19_05_sample_results/output"

# input_directory="/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/rootfiles_after_selection"
# output_directory="/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons/analysis/complete_plotting/output"

tree_name="selected_datatrain"
db_path="/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons/scripts/event_file_dictionary.db"

# Get files from input directory
cd ${input_directory}
filenames=(*.root)
nfiles=${#filenames[@]}

fileinfo_array=()
comma_string=","

# First element in list is for whatever reason just an empty string -> in loop index+1
for (( i=0; i<$(( $nfiles )); i++ )) do
	filename=${filenames[i]}

	# Read in class, final state, polarization and xsection to file according to name correspondence with database
	db_output=$(sqlite3 ${db_path} 'select distinct class, final_state, pol, xsection from processes where ( "'${filename}'" like ("%" || class || "%" || final_state || "%" || pol || "%") )')

	# Append complete file info to fileinfo_array
	fileinfo_string="${filename}|${db_output}" 
	fileinfo_array+=$fileinfo_string
	# After all but last info put comma as delimiter
	if [ $i != `expr $nfiles - 1` ]; then
		fileinfo_array+=$comma_string
	fi
done

# Call the plotting framework and hand it the fileinfos
cd ${THIS_SCRIPT_DIR}/../src
root -l -b -q 'run_plotting_framework.cpp("'${luminosity}'","'${e_beam_polarization}'","'${p_beam_polarization}'","'$fileinfo_array'","'${input_directory}'","'${output_directory}'","'${tree_name}'")'
