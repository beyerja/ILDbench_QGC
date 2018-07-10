
## ILDbench_QGC

Anomalous quartic gauge coupling measurement

### Installation

Explain here:

- what are the package dependencies (iLCSoft, others ?)
- how to compile your package. Should normally be something like:

```shell
source /path/to/ilcsoft/init_ilcsoft.sh
cd macros
chmod u+x run_make.sh
./run_make
```

### How to run the analysis

Explain here:

- where to find data needed for your analysis or how to produce them
- how to run you analysis: 
   - Marlin processors to run ?
   - ROOT macros to run ?
   - Shell scripts ?
   - Run the analysis on grid if you provide scripts for that

Example for how to run processor:

```shell
export MARLIN_DLL=./lib/libILDbench_QGC.so
Marlin ./scripts/ExampleProcessor.xml
```

This creates a .root file in the scripts directory which can be open e.g. using

```shell
rootbrowse test.root
```


If you want to provide a lot of details on your analysis, use the doc/Readme.md and point to it from this Readme.md file:

More documentation available here in [doc/Readme.md](doc/Readme.md) !

### Issues and contact

Explain here how can people reach you:

- via the Github issue interface. For the skeleton package: https://github.com/ILDAnaSoft/ILDbench_QGC/issues
- **not mandatory**:
    - email address
    - working institute


