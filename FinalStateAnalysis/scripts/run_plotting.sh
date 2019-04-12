#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Code directory
PLOTTER_DIR="${DIR}/../analysis/complete_plotting/shell_scripts"

echo ""
echo "Staring complete plotting"
echo ""

cd ${PLOTTER_DIR}

./initialize.sh

echo ""
