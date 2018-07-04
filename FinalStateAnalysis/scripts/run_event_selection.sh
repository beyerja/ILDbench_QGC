#!/bin/bash

# Runs event selection over all files in the input directory

# Directory of the selection code
SELECTOR_DIR=/afs/desy.de/user/b/beyerjac/flc/VBS/nunu_hadrons/analysis/event_selection

IN_DIR=/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/rootfiles
OUT_DIR=/nfs/dust/ilc/group/ild/beyerjac/VBS/nunu_hadrons/output/rootfiles_after_selection

echo ""
echo "Running event selection"
echo "Output directory is: " ${OUT_DIR}
echo ""

cd ${SELECTOR_DIR}

for rootfile in ${IN_DIR}/*.root; do
	echo "Running selection on file: "${rootfile}
	 root -l -b -q 'eventSelector.C("'${rootfile}'", "'${OUT_DIR}'" )'
done

echo ""
