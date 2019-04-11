#!/bin/bash

# Directory of this script
dir="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

#-------------------------------------------------------------------------------------------------
# Set environment variables to be used for running the Marlin jobs



export XML_TEMPLATE_NAME=aQGC_steering_template.xml
export WHIZARD_OUTPUT_DIRECTORY=$(python ${dir}/../WHIZARD_macros/GetOutputDirectory.py 2>&1)