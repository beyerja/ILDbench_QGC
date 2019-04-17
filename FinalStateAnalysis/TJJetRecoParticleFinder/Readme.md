## TJJetRecoParticleFinder
<!-- TODO  -->

### Installation
<!-- TODO  -->

```shell
source /path/to/ilcsoft/init_ilcsoft.sh
```
```shell
cd scripts
./run_make.sh
cd ../lib
MARLIN_DLL=$MARLIN_DLL:$(pwd)/libJakobsVBSProcessor.so
```


### How to run the processor
<!-- TODO  -->

```shell
cd test
Marlin test_TJJetRecoParticleFinder.xml
```