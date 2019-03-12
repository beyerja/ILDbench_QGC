#!/bin/bash

source /afs/desy.de/user/b/beyerjac/flc/load_current_software_versions.sh

VBSdir=/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons
PROJECTNAME=JakobsVBSProcessor
libname=$VBSdir"/lib/lib$PROJECTNAME"
export MARLIN_DLL=${MARLIN_DLL}:${libname}.so

# For now use own TJ for better reco to jets
for possible_lib in $( ls /cvmfs/ilc.desy.de/sw/x*_*_gcc*_sl*/v*/MarlinReco/v*/lib/libMarlinReco.so ); do
  export MARLIN_DLL=$(echo $MARLIN_DLL | perl -pe "s[$possible_lib][]g;")
done
export MARLIN_DLL=${MARLIN_DLL}:/afs/desy.de/user/b/beyerjac/flc/VBS/LocalMarlinReco/lib/libMarlinReco.so

export MARLIN_DLL=${MARLIN_DLL}:/afs/desy.de/user/b/beyerjac/flc/VBS/Yasser_JERJES/lib/libILDbench_JERandJES.so

cd ${VBSdir}/xml/$1
Marlin $2










