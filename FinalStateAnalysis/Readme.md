## FinalStateAnalysis

Analysis of the 2neutrino+4quark final state for the purpose of a Vector Boson Scattering (VBS) analysis. 

### Installation

#### 1. Setting up the environment

##### 1.1 Initializing iLCSoft

The analysis requires an installation of `iLCSoft` which must be sourced before running anything:

```shell
source /path/to/ilcsoft/init_ilcsoft.sh
```

##### 1.2 Global variables

The scripts in this directory rely on specific global variables which need to be exported in the shell before running any scripts. These variables are:
- `CURRENT_ILCSOFT_VERSION`: The ilcsoft version to be used, e.g. "02-00-02". (Assumption: iLCSoft version == ILDConfig version)
- `CURRENT_ILD_MODEL`: ILD model which is used in the simulation, e.g. "l5_o1_v02".

Example for global variable export:
```shell
export CURRENT_ILCSOFT_VERSION="02-00-02"
export CURRENT_ILD_MODEL="l5_o1_v02"
```

#### 2. Compiling and setting up the processor
To compile the main processor used in the analysis run the compilation script in the `script` directory and add the processor library to the `MARLIN_DLL` path:
```shell
cd scripts
./run_make.sh
cd ../lib
MARLIN_DLL=$MARLIN_DLL:$(pwd)/libJakobsVBSProcessor.so
```
  
#### 3. Compiling and setting up the TJJetRecoParticleFinder processor
See the Readme in the `TJJetRecoParticleFinder` directory.

#### 4. Setting signal sample paths

The paths of the signal slcio files to be used in the analysis are set in a `sqlite` database in `./scripts/event_file_dictionary.db`. They are coupled to the software version, detector model, energy, etc used in the run. 

Manipulate the database e.g. using `sqlite3` (requires some minimal SQL skills):

```shell
sqlite3 event_file_dictionary.db
```

(This system should be switched at some point but it's what I started with so I'm stuck with it for now.)


### How to run the analysis

#### 1. Running the Marlin analysis

Use the `./run_class.sh` script in the `script` directory to run the full Marlin analysis all signal files: 

```shell
./run_class 6f_2l4q Marlin
```

(Each file will get a separate job sent to HTCondor, the output of files from the same final state will be automatically combined.)

This will create `.root` files in the output directory defined in the `./scripts/set_xml.sh` script.

#### 2. Running the event selection

To run the event selection on the output files from the Marlin analysis run:

```shell
./run_event_selection.sh
```
in the `scripts` directory.

#### 3. Running the ROOT analysis / plotting

Run the ROOT analysis on the files that went through the event selection using:

```shell
./run_plotting
```

in the `scripts` directory.

The plotters to be run can be manipulated in `analysis/complete_plotting/src/configuration.cpps`.

The input and output paths can be set in `analysis/complete_plotting/shell_scripts/initialize.sh`.

#### 4. Creating comparison plots

Using the histograms from the ROOT analysis some comparison plots can be created for different aspects of the analysis.
The scripts for that are located in `analysis/comparisons` and can be executed with simple `root` execution of the script.

Python is used for the `comparison_separations` code, so it can be simply executed with by running `python CompareGaussianSeparation.py`. This requires a modern python and pyROOT.


