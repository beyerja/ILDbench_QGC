#!/usr/bin/python

""" Create Sindarin files for calculation of weights for different model 
    parameters.
"""

import os   # for scanning directory structure etc
import sys  # for system status and exit 
import glob # For file search with wildcards
import datetime
import numpy                  as np
import ProcessMaps            as pMaps 
import GetSindarinParameters  as sinPars
import SystemHelperFunctions  as sysHelp
import GetOutputDirectory     as outDir

#-------------------------------------------------------------------------------

def getWeightFileBase(fs0_max, fs1_max, n_fs0_steps, n_fs1_steps, fkm, scan2D):
    """ Returns standardized file name for weight files. 
    """
    
    fkm_strings     = { 1 : "kmon", 0 : "kmoff"}
    scan2D_strings  = { True : "2Don", False : "2Doff"}
        
    filename = "rescan_FS0max{}steps{}_FS1max{}steps{}_{}_{}".format(
                                                    fs0_max, n_fs0_steps, 
                                                    fs1_max, n_fs1_steps, 
                                                    fkm_strings[fkm],
                                                    scan2D_strings[scan2D])  
                                                
    return filename
    

#-------------------------------------------------------------------------------

def readSimulationDictionary( process_path ):
    """ Searches for parameter dictionary from simulation and returns its 
        content.
    """

    # Get all dictionaries in process path
    dictionary_ending  = '_setup_dictionary.py'
    all_dictionaries   = glob.glob( '{}/*{}'.format(process_path, 
                                                    dictionary_ending) )
    
    # Make sure there's only one
    if len(all_dictionaries) > 1: 
        print("ERROR: More than one dict in dir: {}".format(process_dir))
        sys.exit(1)
    elif len(all_dictionaries) == 0:
        print("No dict in dir: {}".format(process_dir))
        sys.exit(1)
    else:
        # Read file, evaluate as python dict and return
        dict_path       = all_dictionaries[0]
        dict_string     = sysHelp.readFileContent(dict_path)    
        process_dict    = eval(dict_string)    
        return process_dict

#-------------------------------------------------------------------------------

def getCouplingPoints( fs_max, n_fs_steps ):
    """ Create array with fs (anomalous coupling) points, including the
        SM point 0.
        Array will
            - go from -fs_max to fs_max 
            - include 0
            - contain n_fs_steps(+1 if even) points 
    """

    # Create arrays with fs0 and fs1 points -> 0 Should always be included
    one_side_fs_steps  = int(n_fs_steps/2.) 
    fs_neg_points      = np.linspace( -fs_max, 0, one_side_fs_steps, 
                                       endpoint=False )
    fs_pos_points      = np.linspace(  0, fs_max, one_side_fs_steps+1 )
    fs_points = np.concatenate((fs_neg_points, fs_pos_points))

    return fs_points 

#-------------------------------------------------------------------------------

def saveParameterGrid( fs0_points, fs1_points, scan2D, path_base ):
    """ Get 2D grid and save it to a file for later combination with found 
        weights.
    """
    grid = sinPars.get2DParameterGrid( fs0_points, fs1_points, scan2D )
    
    grid_file_path = "{}_grid.py".format(path_base)
    with open(grid_file_path,'w') as grid_file:
        print("creating {}".format(grid_file_path))
        # Make sure array is not summarized!
        grid_file.write(np.array2string(grid, threshold=99999999999))

#-------------------------------------------------------------------------------

def setRescanSindarinFile( process_path, fs0_max, fs1_max, 
                           n_fs0_steps, n_fs1_steps, fkm, scan2D ):
    """ - searches in process_path for template parameter dictionary from 
          simulation
        - then gets all parameters for the rescan, including all parameter 
          points for anomalous couplings
        - writes record of all used parameter points
        - gets template for rescan
        - replaces parameters in template and writes Sindarin file for rescan
    """
    
    # TODO Bring all directory names to top to avoid confusion!
    
    context = readSimulationDictionary( process_path )
    
    # Rescanning always done with SSC_2 model, base point still SM
    parameter_context = sinPars.getModelParameterSindarinContext( "SSC_2", 0, 0,
                                                                  fkm )
    
    # Input sample from old file name base 
    input_sample = context['out_file_base']
    
    weight_file_base = getWeightFileBase( fs0_max, fs1_max, n_fs0_steps, 
                                          n_fs1_steps, fkm, scan2D )
    
    # Create directory for all reweighting output 
    weight_output_dir = "rescan_output"
    sysHelp.createSubdirectory("{}/{}".format(process_path, weight_output_dir))
    weight_output_base = "{}/{}".format(weight_output_dir, weight_file_base)
    
    fs0_points = getCouplingPoints( fs0_max, n_fs0_steps )
    fs1_points = getCouplingPoints( fs1_max, n_fs1_steps )
    
    # Save the parameter grid for later combination with weights
    weight_output_path_base = "{}/{}".format(process_path, weight_output_base)
    saveParameterGrid( fs0_points, fs1_points, scan2D, weight_output_path_base )
    
    # Get alternative setups
    alt_setup_context = sinPars.getAlternativeParametersSindarinContext( 
                                                fs0_points, fs1_points, scan2D )
    
    # New complete context for reweighting
    context.update(parameter_context)
    context.update(alt_setup_context)
    context['input_sample'] = input_sample
    context['out_file_base'] = weight_output_base
    
    common_template = sysHelp.readFileContent("../../scripts/WHIZARD_scripts/CommonSetupTemplate.sin")
    rescan_template = sysHelp.readFileContent("../../scripts/WHIZARD_scripts/RescanningTemplate.sin")

    full_template   = "{}\n{}".format(common_template, rescan_template)  
    
    # Replace template parameters and write to file
    weight_file_path = "{}/{}.sin".format(process_path, weight_file_base)
    with open(weight_file_path,'w') as myfile:
        print("creating {}".format(weight_file_path))
        myfile.write(full_template.format(**context))



#-------------------------------------------------------------------------------

def setAllRescanFiles( signal_dir, fs0_max, fs1_max, 
                       n_fs0_steps, n_fs1_steps, fkm, scan2D ):
    """ For each process in the signal directory set a rescan file. 
    """
    
    # Get all subdirectories in signal_dir
    process_dirs = next(os.walk(signal_dir))[1]
    
    for process_dir in process_dirs:
        if process_dir[0] == ".": continue
        process_path = "{}/{}".format( signal_dir, process_dir )
        setRescanSindarinFile( process_path, fs0_max, fs1_max, 
                               n_fs0_steps, n_fs1_steps, fkm, scan2D )
    
#-------------------------------------------------------------------------------

def main(arguments):
    # Directory with all signal runs
    signal_dir = "{}/signal".format(outDir.getOutputDirectory())
    
    # Parameters determining the scanned parameter points
    fs0_max = 200
    fs1_max = 50
    
    n_fs0_steps = 5
    n_fs1_steps = 3 
    
    scan2D = True
    
    # Renomalization switch (K-matrix)
    fkm = 1
    
    setAllRescanFiles( signal_dir, fs0_max, fs1_max, n_fs0_steps, n_fs1_steps, 
                       fkm, scan2D )
    
#-------------------------------------------------------------------------------

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

#-------------------------------------------------------------------------------
