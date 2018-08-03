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

#### Files:
- ```standard_bash_run.sh```: Generic scripts that takes any bash command as input, sources the .profile in the home directory, and then executes the command.
- ```standard_htcondor_steering.submit```: HTCondor submit file used in combination with standard_bash_run.sh to send Marlin runs to the BIRD cluster.

### WHIZARD_macros

#### Usage:

#### Files:
