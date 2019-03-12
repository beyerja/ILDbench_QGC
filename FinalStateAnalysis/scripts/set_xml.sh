#!/bin/bash

# Always needs Higgs mass for directory
if [[  $# != 6 ]]  ; then
	echo "usage: ./set_xml.sh input_class_name (higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/e2/e1..) pol (lr/rl/ll/rr) COM_energy (in GeV) sw_version (01_19_05/...) detector_model (l5/...)"
	exit
fi

class_name=${1,,}
final_state=${2,,}
polarization=${3,,}

E_COM=${4,,}
SW_version=${5,,}
detector_model=${6,,}

DIR=${MA}/scripts
data_directory=/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/tmp_rootfiles

xml_directory=${DIR}/../xml

template_file=${xml_directory}/template.xml
compact_file="${lcgeo_DIR}/ILD/compact/ILD_${detector_model}/ILD_${detector_model}.xml"

pol_weight_aux=( ` eval " ${DIR}/get_database_entry.sh  ${class_name} ${final_state} ${polarization} file ${E_COM} ${SW_version} ${detector_model}" ` )

i=0
if [[ ! -z ${pol_weight_aux} ]] ; then
	files=($(${DIR}/get_database_entry.sh  ${class_name} ${final_state} ${polarization} file ${E_COM} ${SW_version} ${detector_model})) 
	# Do every slcio file seperately (harder to understand, but really need to speed up)
	for file in "${files[@]}"; do
		i=`expr ${i} + 1`
		tmp_xml_dir=${xml_directory}/tmp_${class_name}_${final_state}_${polarization}_${i}
		xml_file=${tmp_xml_dir}/My_${class_name}_${final_state}_${polarization}_${i}.xml

		if [[ ! -d ${tmp_xml_dir} ]] ; then
			mkdir ${tmp_xml_dir}
		fi
		cp ${template_file} ${xml_file}
		base_name=${data_directory}/${class_name}_${final_state}_${polarization}_${i}
		root_name=${base_name}.root 
		single_file=` echo -e ${file/'\n'} `
		# Set the input and output file names in the steering file
   	# CAREFUL: Line numbers should be decreasing!
		sed -i "131s\.*\ ${root_name} \  " ${xml_file}
		sed -i "30s\.*\ ${compact_file} \  " ${xml_file}
		sed -i "17s\.*\ ${single_file}\ " ${xml_file}
	done
	echo "true"
else
	for file in files; do 
		i=`expr ${i} + 1`
		tmp_xml_dir=${xml_directory}/tmp_${class_name}_${final_state}_${pol_in}_${i}
		if [[ -d ${tmp_xml_dir} ]] ; then
			rm -r ${tmp_xml_dir}
		fi
	done
	echo "false"
fi
