## Macros directory

Directory for any code which is needed to perform an analysis using the *aQGCObservablesProcessor* and its output.

### Content

- ```run_make.sh```: Bash script for compiling the processor. Simply execute w/o arguments.
- ```Marlin_macros```: Directory for bash macros needed to run the aQGCObservablesProcessor over all files of a process, and to send the according jobs to the HTCondor scheduler.
- ```standard_HTCondor_submit```: Files needed to submit some command to HTCondor (in this case the Marlin jobs).
- ( ```Condor_output```: Output directory for all logging from HTCondor. Dynamically generated.) 
- ```ROOT_macros```: Directory for ROOT macros for my analysis.
- ```WHIZARD_macros```: Python scripts for setting up the steering files for WHIZARD. 

 
### Marlin_macros

#### Usage:

<!-- TODO MENTION THAT IT IS DONE WITH HTCONDOR -->

1. Set the experimental and computational environment variables in the scripts ```set_simulation_environment.sh``` and ```set_output_environment.sh```.
2. Check if the directories in which we search for the input files is correct in ```get_setup_directory.sh```.
3. Run the *aQGCObservablesProcessor* over any final state, e.g.:
```bash
./run_single_process.sh vvxyyx
```
or run over all signal and background signatures using:
```bash
./run_all_signal_files.sh
./run_all_background_files.sh
```
4. Find your output in the directory you set in ```set_output_environment.sh```!


#### Files:
- ```get_setup_directory.sh```: Provides the directory in which to search for process (DST) files depending on the setup. 
- ```manage_process_files.sh```: Responsible for setting all files where they need to be in order to start sending Marlin jobs to HTCondor, work with them afterwards, and not make a huge mess of temporary files.
- ```run_all_background_files.sh```: Run the *run_single_process.sh* macro for all background final states.
- ```run_all_signal_files.sh```: Run the *run_single_process.sh* macro for all signal final states (includes irreducible background).
- ```run_single_process.sh```: Runs over all polarizations of a given final state using HTCondor.
- ```set_output_environment.sh```: Defines the output directories for the Marlin runs.
- ```set_process_marlin_script.sh```: Changes the respective line in a given Marlin steering file template to the correct variables.
- ```set_simulation_environment.sh```: Defines parameters of the experimental and simulation setup, e.g. detector models, BDT weights, software versions...


### ROOT_macros

The macros are written to use the ```RDataFrame``` class of ROOT to effectively analzse the root files. A basic setup for a analyzer class is provided, specific analyzers can then be written in the *analyzers* directory. Since this is all executed as ROOT macros not additional compiling is necessary. 

#### Content

- ```analyzers```: Code for the specific analyzers.
- ```include``` and ```src```: Basic setup code for the analyzer class and standard cut definitions.
- ```TestMacro.cpp```: Simple test for construction.
- ```run_in_new_root.sh```: Used to run scripts with newer ROOT version.


#### Usage:

For now:
1. Modify the ```TestMacro.cpp``` so that the analyzer of your choice is executed.
2. Run: 
  ```bash
  ./run_in_new_root.sh TestAnalyzer.cpp
  ```


#### Analyzers:

- ```TestAnalyzer```:
- ```aQGCAnalyzer```:
- ```HighestSignificanceAnalyzer```:

### standard_HTCondor_submit 

#### Usage:

(Is automatically used by scripts in *Marlin_scripts*.)
Run any command on HTCondor using:
```bash
condor_submit standard_htcondor_steering.submit arguments="insert-your-command-here"
```
or same with ```multithread_htcondor_steering.submit``` for more cpus.

#### Files:
- ```multithread_htcondor_steering.submit```: HTCondor submit file used in combination with standard_bash_run.sh to send Marlin runs to the BIRD cluster, has increased demands (more cpus, time, ...).
- ```standard_bash_run.sh```: Generic scripts that takes any bash command as input, sources the .profile in the home directory, and then executes the command.
- ```standard_htcondor_steering.submit```: HTCondor submit file used in combination with standard_bash_run.sh to send Marlin runs to the BIRD cluster.

### WHIZARD_macros

#### Usage:

<!-- set output dir in GetOutputDirectory, create sim files with SetSimFiles, create rescan with SetRescanFiles, run with respective bash scripts -->

1. **Set input parameters**:
  - The output/working directory for whizard is set in *GetOutputDirectory.py* and can be changed there.
  - Parameters which are intended to be changeable for the integration and simulation are set in the main function of *SetSindarinSimulationFiles.py*.
  - Parameters which are intended to be changeable for the rescanning for weights at different parameter points are set in the main function of *SetSindarinRescanningFiles.py*.
2. **Creating Sindarin steering files**:
  - This is split into creating scripts for integration + event simulation and scripts for rescanning the created events.
  - To create the steering scripts run:
    ```shell
      python3 SetSindarinSimulationFiles.py
      python3 SetSindarinRescanningFiles.py
    ```
    (The simulation files must(!) be created first.)
3. **Running the simulation and rescanning**:
  - Once the steering is set it can be run using the bash scripts. The simulation must be run first and execute successfully in order for the rescanning to do anything useful.
  - Individual simulation scripts can be run using:
  ```shell
    ./run_single_simulation.sh simulation_file_path
  ```
  or one can run all simulation files in the output directory using:
  ```shell
    ./run_all_simulations.sh
  ```
  - Individual rescanning scripts can be run using:
  ```shell
    ./run_single_rescan.sh rescan_file_path
  ```
  or one can run all rescan files in the signal directory using:
  ```shell
    ./run_all_rescans.sh
  ```
  - Finally, convert the weight files into usable ROOT format:
  ```shell
    python ConvertWeightsToTree.py
  ```
  
#### Files:
- ```GetOutputDirectory.py```: Central script that holds/returns the output directory for all WHIZARD runs.
- ```GetSindarinParameters.py```: Functions that return python dictionaries with the parameters for the sindarin templates. Includes some settings that are not intended to be changed such as cut values.
- ```ProcessMaps.py```: Python dictionaries which map the filename convention to the respective Sindarin syntax.
- ```run_all_rescans.sh```  : Searches for all rescanning sindarin steerings in the signal directory, checks if there is an event file to reweight and if so runs them.
- ```run_all_simulations.sh```: Searches for all simulation sindarin steering files in the output directory and starts a whizard run for each of them.
- ```run_single_rescan.sh```: Takes a single sindarin rescan file and sends a job to HTCondor to run it.
- ```run_single_simulation.sh```: Takes a single sindarin simulation file and sends a job to HTCondor to run it.
- ```SetSindarinRescanningFiles.py```: Builds full sindarin template out of CommonSetupTemplate and RescanningTemplate from *scripts* directory, fills the template with the given parameters and writes the output to the respective signal directories. Uses dictionaries from *SetSindarinSimulationFiles.py* for consistency. In the main function the scanned parameter grid is determined.
- ```SetSindarinSimulationFiles.py```: Builds full sindarin template out of CommonSetupTemplate and SimulationTemplate from *scripts* directory, fills the template with the given parameters and writes the output to an according structure in the output directory. Some simulation-specific variables can be set in the main function (e.g. luminosity).
- ```SystemHelperFunctions.py```:  Some helper functions for dealing with file reading and directory structure.


