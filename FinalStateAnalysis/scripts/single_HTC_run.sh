#!/bin/bash

source /afs/desy.de/user/b/beyerjac/flc/load_current_software_versions.sh

VBSdir=/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons
PROJECTNAME=JakobsVBSProcessor
libname=$VBSdir"/lib/lib$PROJECTNAME"
export MARLIN_DLL=${MARLIN_DLL}:${libname}.so

cd ${VBSdir}/xml/$1
Marlin $2










