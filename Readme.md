
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
```

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
export MARLIN_DLL=./lib/libILDbench_QGC.so
Marlin ./scripts/ExampleProcessor.xml
```

This creates a .root file in the scripts directory which can be open e.g. using

```shell
rootbrowse test.root
```

#### 1. Actually running the processor

<!-- TODO -->

#### 2. Analysing the results

<!-- TODO -->


<!-- If you want to provide a lot of details on your analysis, use the doc/Readme.md and point to it from this Readme.md file:

More documentation available here in [doc/Readme.md](doc/Readme.md) ! -->

### Issues and contact

For feedback, complaints, ... contact me:

- via the Github issue interface: https://github.com/ILDAnaSoft/ILDbench_QGC/issues



