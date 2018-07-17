#!/bin/bash

source /cvmfs/sft.cern.ch/lcg/contrib/gcc/7/x86_64-slc6/setup.sh

source /cvmfs/sft.cern.ch/lcg/releases/ROOT/6.14.00-39264/x86_64-slc6-gcc7-opt/bin/thisroot.sh

root -b -q test_macro.cpp