#!/bin/bash

if [[ $# > 1 ]] ; then 
	echo "usage: ./run_make.sh"
	exit
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
home_folder=${DIR}/.. 



# -------------------------------------------------#
# Get the current setup

# ILCSoft version and detector model as defined in /flc/load_current_software_versions.sh
version=$CURRENT_ILCSOFT_VERSION

# Choose cmake file depending on global ilcsoft version
if [[ $version == "01-17-11" ]]; then
	cmakelist_path=${home_folder}/CMakeLists/17_11_CMakeLists.txt
elif [[ $version == "01-19-05" ]]; then
	cmakelist_path=${home_folder}/CMakeLists/19_05_CMakeLists.txt
elif [[ $version == "02-00-02" ]]; then
	cmakelist_path=${home_folder}/CMakeLists/02_00_02_CMakeLists.txt
else 
	echo "ERROR: no CMakeLists found for this ilcsoft version! Exiting now..."
	exit
fi


# -------------------------------------------------#
# Set the appropriate files in the xml directory
# according to the setup 

templates_dir=${home_folder}/xml/steering_files/template_files
ILD_gearoutputs_dir=${home_folder}/xml/steering_files/ILD_GearOutputs

# TODO Make this better in combination with the above

rm ${home_folder}/xml/template.xml

# Set the template with the correct processor versions
if [[ $version == "01-17-11" ]]; then
	template=${templates_dir}/01_17_11_template.xml
elif [[ $version == "01-19-05" ]]; then
	template=${templates_dir}/01_19_05_template.xml
elif [[ $version == "02-00-02" ]]; then
	template=${templates_dir}/02_00_02_template.xml
else 
	echo "ERROR: not valid ilcsoft version! Exiting now..."
	exit
fi

echo
echo "Setting template to ilcsoft version " ${version}
cp ${template} ${home_folder}/xml/template.xml
echo

ILD_model=$CURRENT_ILD_MODEL

ILD_gearoutput=""
# Set the correct steering file for the detector
if [[ $ILD_model == "l5_v02" ]]; then
	ILD_gearoutput=${ILD_gearoutputs_dir}/gear_ILD_l5_v02.xml
elif [[ $ILD_model == "s5_v02" ]]; then
	ILD_gearoutput=${ILD_gearoutputs_dir}/gear_ILD_s5_v02.xml
elif [[ $ILD_model == "o1_v05" ]]; then
	ILD_gearoutput=${ILD_gearoutputs_dir}/GearOutput_ILD_o1_v05.xml
elif [[ $ILD_model == "o2_v05" ]]; then
	ILD_gearoutput=${ILD_gearoutputs_dir}/GearOutput_ILD_o2_v05.xml
elif [[ $ILD_model == "l5_o1_v02" ]]; then
  echo "" >/dev/null # do nothing
else
	echo "ERROR: not valid ILD model version! Exiting now..."
	exit
fi

rm ${home_folder}/xml/GearOutput.xml
if [[ ${ILD_gearoutput} != "" ]]; then
  echo "Setting GearOutput to ILD version " ${ILD_model}
  cp ${ILD_gearoutput} ${home_folder}/xml/GearOutput.xml
  echo
fi

# -------------------------------------------------#
# Check if necessary directories are in place

if [ -d ${home_folder}/build  ] ; then
	echo "Already have  --build-- filter to compile"
else
	echo "no build filter"
	mkdir ${home_folder}/build 
fi

echo 

# -------------------------------------------------#
# Move to build directory and to build framework 
# (cmake + make)

cd ${home_folder}/build

if [ -a "make.output"  ] ; then
	rm make.output
fi

# Copy correct cmake file to main directory for making
cp ${cmakelist_path} ${home_folder}/CMakeLists.txt

echo "begin to config"
echo "WARNING: Currently  using '-w' in CMakefile to suppress Warnings of TrueJet_Parser!!!"
echo 
cmake -C $ILCSOFT/ILCSoft.cmake ..    &> make.output                                                                 
wait
echo "begin to make"
echo 
make &> make.output
wait
echo "begin to make install"
echo 
make install &> make.output
ERRORMESSAGE=$(grep "error" -irn ./make.output)
echo "check the error message"
echo 
echo ${ERRORMESSAGE} 
echo "end"

# Clean up, don't need dublicate files
rm ${home_folder}/CMakeLists.txt

# Create tags for my Processor
cd ${home_folder}
ctags -R .
