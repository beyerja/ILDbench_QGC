## Marlin scripts directory

Directory for steering files for Marlin.

<!-- Put your Marlin steering files here and explain to which processor they refer (if the file name is not obvious) ! -->

#### Content

- ```ExampleProcessor.xml```: Simple example of running the processor that should work out-of-the-box. (Unless you're using new software, a different environment, quantum computers, magic, ...) 
- ```aQGC_processor_steering```: 

### ExampleProcessor.xml

#### Usage:

1. Change any parameter in the file that you feel like needs changing.
2. Run: 
```bash
Marlin ExampleProcessor.xml
```

### aQGC_processor_steering

#### Usage:

(Used automatically by *Marlin_macros*.)
The ```steering_template.xml``` is used as template for all Marlin runs. To start a run, the marked lines in the template must be replaced.

#### Files:
- ```steering_template.xml```: Template for Marlin runs of any process.
- (```tmp_...```: Dynamically generated and temporary directories used to start the Marlin runs. Should be remove automatically.)