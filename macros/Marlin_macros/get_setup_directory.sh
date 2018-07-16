#!/bin/bash

# List of base directories for different setups. -> Will be searched for all process files

if [[  $# != 0 ]]  ; then
	echo "usage: ./process_settings.sh"
	exit
fi

if [[ ${COLLIDER_ENERGY} == "500" ]] ; then
    echo "/pnfs/desy.de/ilc/prod/ilc/mc-opt-3/ild/dst-merged/500-TDR_ws"
fi 