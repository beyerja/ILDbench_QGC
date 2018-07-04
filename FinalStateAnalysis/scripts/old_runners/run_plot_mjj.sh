#!/bin/bash

# Runs plotting of (mjj+mjj)/2 and mjj VS mjj plots
# output is in ../analysis/mjjmjj_spectra

# Code directory
PLOTTER_DIR=/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons/analysis/mjjmjj_spectra

echo ""
echo "Creating mjj plots"
echo ""

cd ${PLOTTER_DIR}

root -l -b -q create_mjjmjjplots.cc

echo ""
