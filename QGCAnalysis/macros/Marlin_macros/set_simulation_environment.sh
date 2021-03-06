#!/bin/bash

#-------------------------------------------------------------------------------------------------
# Set environment variables to be used for running the Marlin jobs

export RECONSTRUCTION_SOFTWARE_VERSION=${CURRENT_ILCSOFT_VERSION}
export SIMULATION_SOFTWARE_VERSION=${CURRENT_ILCSOFT_VERSION}

export DETECTOR_MODEL_VERSION=${CURRENT_ILD_MODEL}
export COLLIDER_ENERGY=1000

# export XML_TEMPLATE_NAME=steering_template.xml
export XML_TEMPLATE_NAME=steering_template_with_vertexing_rerun.xml #TODO REMOVE/CLEANUP ASAP!!!!!!!!
export ISOLEPFINDER_WEIGHTS_ee=/cvmfs/ilc.desy.de/sw/x86_64_gcc49_sl6/v02-00-02/MarlinReco/v01-25/Analysis/IsolatedLeptonTagging/weights/yyxyev_yyxyyx_500.mILD_l5_o1_v02
export ISOLEPFINDER_WEIGHTS_mm=/cvmfs/ilc.desy.de/sw/x86_64_gcc49_sl6/v02-00-02/MarlinReco/v01-25/Analysis/IsolatedLeptonTagging/weights/yyxylv_yyxyyx_woYoke_500.mILD_l5_o1_v02
