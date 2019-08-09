## TJJetRecoParticleFinder

Processor which forms a new ReconstructedParticle collection containing only those particles which originated from a hadronic jet.
These particles are identified using the `TrueJet` processor and its `TrueJet_Parser` helper tool.
The identification is therefore based on generator level information.

### Installation

The installation and use of the processor requires an installed and sourced `iLCSoft` version:
```shell
source /path/to/ilcsoft/init_ilcsoft.sh
```

A script for compiling the processor is in the `scripts` directory:
```shell
cd scripts
./run_make.sh
```

To use the processor in `Marlin` on must add the path of the compiled library to the `MARLIN_DLL` variable:
```shell
cd ../lib
MARLIN_DLL=$MARLIN_DLL:$(pwd)/libTJJetRecoParticleFinder.so
```

### How to run the processor

A short script for a test run of the processor is provided in the `test` directory:

```shell
cd test
Marlin test_TJJetRecoParticleFinder.xml
```

Else the processor can be added to your usual Marlin scripts (once you attached it to the `MARLIN_DLL` path).