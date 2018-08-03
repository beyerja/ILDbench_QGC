## Marlin scripts directory

Directory for steering files for Marlin and WHIZARD.

<!-- Put your Marlin steering files here and explain to which processor they refer (if the file name is not obvious) ! -->

#### Content


- ```aQGC_processor_steering```: Steering files which use the Marlin processor from the *source* directory.
- ```WHIZARD_scripts```: Sindarin steering files used with WHIZARD to produce EFT signal samples and normal background samples.

### aQGC_processor_steering

#### Usage:

-  **ExampleProcessor**

  0. Make sure processor was compile and library is in MARLIN_DD.
  1. Change any parameter in the file that you feel like needs changing.
  2. Run: 
  ```bash
  Marlin ExampleProcessor.xml
  ```

- **Template/...**

  (Used automatically by *Marlin_macros*.)
  The ```steering_template.xml``` is used as template for all Marlin runs. To start a run, the marked lines in the template must be replaced.

#### Files:
- ```ExampleProcessor.xml```: Simple example of running the processor that should work out-of-the-box. (Unless you're using new software, a different environment, quantum computers, magic, ...) 
- ```steering_template.xml```: Template for Marlin runs of any process.
- (```tmp_...```: Dynamically generated and temporary directories used to start the Marlin runs. Should be remove automatically.)


### WHIZARD_steering

#### Usage:
(Used automatically by *WHIZARD_macros*.)
The ```sindarinTemplate.sin``` is a template file in which all variable enclosed in curly brackets will be replaced by the python scripts in *WHIZARD_macros* to produce runnable Sindarin files.

#### Files:
- ```sindarinTemplate.sin```: Template sindarin file in python template style.
