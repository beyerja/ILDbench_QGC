#!/bin/bash

if [[  $# < 1 ]]  ; then
        echo "usage: ./run_in_new_root.sh root_macro.cpp"
        exit
fi


source /cvmfs/sft.cern.ch/lcg/contrib/gcc/7/x86_64-slc6/setup.sh

source /afs/desy.de/project/ilcsoft/sw/x86_64_gcc49_sl6/root/6.14.00/bin/thisroot.sh

echo "Using ROOT 6.14 to process " $@ " ..."
root -l -b -q -t $@