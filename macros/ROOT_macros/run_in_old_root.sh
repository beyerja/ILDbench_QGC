#!/bin/bash

if [[  $# < 1 ]]  ; then
        echo "usage: ./run_in_new_root.sh root_macro.cpp"
        exit
fi

source /cvmfs/ilc.desy.de/sw/x86_64_gcc49_sl6/v02-00-02/root/6.08.06/bin/thisroot.sh

echo "Using ROOT 6.08 to process $@ ..."
# root -l -b -q -t $@
$@