
## ILDbench_QGC

Anomalous quartic gauge coupling measurement

### Installation

<!-- Explain here:

- what are the package dependencies (iLCSoft, others ?)
- how to compile your package. Should normally be something like: -->

The processor can be compiled by loading ilcsoft and executing the compile-macro: 

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

### How to run the analysis

<!-- Explain here:

- where to find data needed for your analysis or how to produce them
- how to run you analysis: 
   - Marlin processors to run ?
   - ROOT macros to run ?
   - Shell scripts ?
   - Run the analysis on grid if you provide scripts for that -->

#### 0. Example how to run Marlin

Example for how to run processor:

```shell
Marlin ./scripts/ExampleProcessor.xml
```

This creates a .root file in the scripts directory which can be open e.g. using

```shell
rootbrowse test.root
```

#### 1. Actually running the processor

<!-- What needs to be here: (Not to detailed!) -->
<!-- Refer to local README -->
<!-- How do I run over all files? -->
<!-- What is the output? -->

<!-- TODO -->

#### 2. Analysing the results

<!-- TODO -->


<!-- If you want to provide a lot of details on your analysis, use the doc/Readme.md and point to it from this Readme.md file:

More documentation available here in [doc/Readme.md](doc/Readme.md) ! -->

### Acknowledgements

- **WHIZARD macros and steering:** I received a lot of help from the WHIZARD team in setting up my file production and creating useful samples. Special thanks go to Marco Sekulla who provided me with a python template setup which my macros and template are based on.


### Issues and contact

For feedback, complaints, ... contact me:

- via the Github issue interface: https://github.com/ILDAnaSoft/ILDbench_QGC/issues



