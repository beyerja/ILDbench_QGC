#!/usr/bin/python
import ROOT

import os   # for scanning directory structure etc
import sys  # for system status and exit 
import glob # For file search with wildcards
from array import array
import GetOutputDirectory       as outDir
import SystemHelperFunctions    as sysHelp


#-------------------------------------------------------------------------------

def readWeightFile( process_dir ):
    """ Searches for weight file and return its content.
    """

    # Get all dictionaries in process path
    weight_file_pattern = ".fake_weights.dat"
    all_weight_files    = glob.glob( '{}/rescan_output/*{}'.
                                    format(process_dir, weight_file_pattern) )
    
    # Make sure there's only one
    if len(all_weight_files) > 1: 
        print("ERROR: More than one dict in dir: {}".format(process_dir))
        return False
    elif len(all_weight_files) == 0:
        print("No dict in dir: {}".format(process_dir))
        return False
    else:
        # Read file, evaluate as python dict and return
        weights_path       = all_weight_files[0]
        weights_string     = sysHelp.readFileContent(weights_path)    
        return weights_string

#-------------------------------------------------------------------------------

def extractWeights( weights_string ):
    
    lines = weights_string.split("\n")
    
    weights_dict = {}
    
    for line in lines:
        split_line = line.split("  ")
        setting = split_line[0]
        if setting == '0': continue # SM line
        if setting == '': continue # last line
        
        weight = split_line[1]
        if setting in weights_dict:
            weights_dict[setting].append(float(weight))
        else:
            weights_dict[setting] = [weight]
        
    return weights_dict

#-------------------------------------------------------------------------------

def writeWeightsToTree( weights_dict, root_path ):
    file = ROOT.TFile( root_path, 'recreate' )
    tree = ROOT.TTree( 'weightsTree', 'weights for anomalous couplings' )

    # Writing trees in Python complicated because it works with references..
    weight_ptrs = {}
    
    # First create all branches and the according reference pointers
    for key in weights_dict:
        weight_ptr = array( 'f', [ 0. ] )
        weight_ptrs[key] = weight_ptr
        tree.Branch( "weight{}".format(key), weight_ptrs[key], "weight{}/F".format(key) )
    
    # Then fill (all parameter sets should have number of weights)
    n_events = len(weights_dict[weights_dict.keys()[0]])
    for i_event in range(n_events):
        for key in weights_dict:
            weight = float(weights_dict[key][i_event])
            weight_ptrs[key][0] = weight
        tree.Fill()
    
    file.Write()
    file.Close()

#-------------------------------------------------------------------------------

def convertSingleWeightFile( process_dir ):
    weights_string  = readWeightFile(process_dir)
    if not weights_string:
        print("Did not find single weights file in {}, moving on...".format(process_dir))
        return
    weights_dict    = extractWeights(weights_string)
    root_path = "{}/rescan_output/weights.root".format(process_dir)
    writeWeightsToTree(weights_dict, root_path)


#-------------------------------------------------------------------------------

def main(arguments):
    
    # Get all subdirectories in signal_dir
    signal_dir = "{}/signal".format(outDir.getOutputDirectory())
    process_dirs = next(os.walk(signal_dir))[1]
    
    for process_dir in process_dirs:
        path = "{}/{}".format(signal_dir, process_dir)
        convertSingleWeightFile(path)
    
#-------------------------------------------------------------------------------

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

#-------------------------------------------------------------------------------
