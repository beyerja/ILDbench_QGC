#!/bin/bash

# -------------------------------------------------#
# Bash script for compiling the processor of the
# analysis.
# To be run simply by using: ./run_make.sh
# -------------------------------------------------#

dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
home_folder=${dir}/..

# -------------------------------------------------#
# Check if necessary directories are in place

if [ -d ${home_folder}/build  ] ; then
	echo "Already have  --build-- filter to compile"
else
	echo "no build filter"
	mkdir ${home_folder}/build
fi

# -------------------------------------------------#
# Move to build directory and to build framework
# (cmake + make)

cd ${home_folder}/build

if [ -f "make.output"  ] ; then
	rm make.output
fi

echo "begin to config"
echo
cmake -C $ILCSOFT/ILCSoft.cmake ..    >> make.output 2>&1
wait
echo "begin to make"
echo
make >> make.output  2>&1
wait
echo "begin to make install"
echo
make install >> make.output 2>&1
echo 
WARNINGMESSAGE=$(grep "warning" -irn ./make.output)
echo "check for warning message"
echo ${WARNINGMESSAGE}
echo 
ERRORMESSAGE=$(grep "error" -irn ./make.output)
echo "check for error message"
echo ${ERRORMESSAGE}
echo
echo "end"
