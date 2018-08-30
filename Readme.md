
## ILDbench_QGC

Anomalous quartic gauge coupling measurement

### Installation

<!-- Explain here:

- what are the package dependencies (iLCSoft, others ?)
- how to compile your package. Should normally be something like: -->

For creating 2neutrino+4quark generator level samples this code requires WHIZARD2.
It must be installed and its binary and library directory added to the respective paths:

```shell
export WHIZARD_DIR=/path/to/WHIZARD2installation
export PATH=${PATH}:${WHIZARD_DIR}/install/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${WHIZARD_DIR}/install/lib
```

The analysis processor can be compiled by loading ilcsoft and executing the compile-macro: 

```shell
source /path/to/ilcsoft/init_ilcsoft.sh
cd macros
chmod u+x run_make.sh
./run_make
cd ..
```

Afterwards the processor library must be added to the list of paths in which Marlin searches for processors:

```shell
export MARLIN_DLL=./lib/libILDbench_QGC.so
```

It is recommended to do this in your ```~/.profile``` (or which-ever local shell equivalent).

#### Example how to run Marlin

Example for how to run the processor:

```shell
Marlin ./scripts/ExampleProcessor.xml
```

This creates a .root file in the scripts directory which can be open e.g. using

```shell
rootbrowse test.root
```

### How to run the analysis

<!-- Explain here:

- where to find data needed for your analysis or how to produce them
- how to run you analysis: 
   - Marlin processors to run ?
   - ROOT macros to run ?
   - Shell scripts ?
   - Run the analysis on grid if you provide scripts for that -->

#### 1. Creating samples for signal (& irred. bkg)

Macros for the creation of 2neutrino+4quark samples sits in the ```macros/WHIZARD_macros``` directory.
Two steps are required to create the samples:

1. Setting the Sindarin steering files.
```shell
python3 SetSindarinSimulationFiles.py
```
2. Running the simulation (will send jobs to the BIRD cluster):
  - Either all at once:
  ```shell
  ./run_all_simulations.sh
  ```
  - Or individual:
  ```shell
  ./run_single_simulation.sh [complete_path_of_simulation_sindarin]
  ```

Additional two steps are required to get event weights for aQGC points:
3. Setting the Sindarin steering files.
```shell
python3 SetSindarinRescanningFiles.py
```
4. Running the rescan (will send jobs to the BIRD cluster):
  - Either all at once:
  ```shell
  ./run_all_rescans.sh
  ```
  - Or individual:
  ```shell
  ./run_single_rescan.sh [complete_path_of_rescan_sindarin]
  ```
  
**Output**:
  - The simulation will create *.slcio* generator level event files in the directory of its respective steering file.
  - The rescan will produce weight files in *rescan_output* subdirectory of the respective steering file.

#### 2. Running the processor

<!-- What needs to be here: (Not to detailed!) -->
<!-- Refer to local README -->
<!-- How do I run over all files? -->
<!-- What is the output? -->

<!-- TODO -->

#### 3. Analysing the results

<!-- TODO -->


<!-- If you want to provide a lot of details on your analysis, use the doc/Readme.md and point to it from this Readme.md file:

More documentation available here in [doc/Readme.md](doc/Readme.md) ! -->

### Acknowledgements

- **WHIZARD macros and steering:** I received a lot of help from the WHIZARD team in setting up my file production and creating useful samples. Special thanks go to Marco Sekulla who provided me with a python template setup which my macros and template are based on.


### Issues and contact

For feedback, complaints, ... contact me:

- via the Github issue interface: https://github.com/ILDAnaSoft/ILDbench_QGC/issues



