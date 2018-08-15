#!/usr/bin/python

""" Create Sindarin files and according directory structure to given process.
"""

import os
import sys
import numpy as np
import ProcessMaps            as pMaps 
import GetSindarinParameters  as sinPars

#-------------------------------------------------------------------------------

def readFileContent( filepath ):
    """ Read content of file into string and return
    """
    with open(filepath, "r") as file:
        return file.read()

#-------------------------------------------------------------------------------

def createProcessSubdirectory( nunu, qqqq, beam_pol, model, output_dir ):
    """ Creates subdirectory for the specific process in the given output 
        directory.
    """
    subdir_name = "{}{}_{}_{}".format( nunu, qqqq, beam_pol, model )
    full_subdir_path = "{}/{}".format(output_dir, subdir_name) 
    
    # Create directory if not already existing
    newpath = r"{}".format( full_subdir_path )
    if not os.path.exists(newpath):
        os.makedirs(newpath)
        
    return full_subdir_path
        
#-------------------------------------------------------------------------------

def getFilenameBase ( nunu, qqqq, beam_pol, model, output_dir ):
    """ Returns standardized file name base. 
    """
    filename = "WHIZARD_E1000_{}{}_{}_{}".format( nunu, qqqq, beam_pol, model )  
    return filename
    
#-------------------------------------------------------------------------------
        
def setSingleProcessFile(   nunu, qqqq, beam_pol, ISR_file, model, luminosity, 
                            output_dir, output_format ):
                            
    # Create subdirectory specific to process
    process_dir = createProcessSubdirectory( nunu, qqqq, beam_pol, model, 
                                             output_dir )
                                             
    # Get process specific file name                                         
    filename_base       = getFilenameBase(nunu, qqqq, beam_pol, model, output_dir)
    filepath_base       = "{}/{}".format(process_dir, filename_base)
                            
    ## Collect all parameters needed for simulation
    context             = sinPars.getStandardSindarinContext( process_dir, filename_base )
    
    process_context = sinPars.getProcessSindarinContext( nunu, qqqq, beam_pol, 
                                                         ISR_file, model )
    parameter_context   = sinPars.getModelParameterSindarinContext( model )
    cut_context     = sinPars.getCutValueSindarinContext()
    sim_context         = sinPars.getSimulationSindarinContext( luminosity, output_format )
    
    # Bring all into one dictionary                    
    context.update(parameter_context)
    context.update(sim_context)
    context.update(process_context)
    context.update(cut_context)
                            
    setup_dict_path = "{}_setup_dictionary.py".format(filepath_base)

    # Safe these parameters so that rescanning can use them (for consistency)
    with open(setup_dict_path,'w') as myfile:
        print("creating process parameter dictionary {}".format(setup_dict_path))
        myfile.write("{}".format(context))
                                     
    ## Create the sindarin file for the simulation
    sim_filepath = "{}_sim.sin".format(filepath_base)
            
    # Simulation file = file with basic information + specific simulation stuff
    common_template = readFileContent("../../scripts/WHIZARD_scripts/CommonSetupTemplate.sin")
    sim_template    = readFileContent("../../scripts/WHIZARD_scripts/SimulationTemplate.sin")
    full_template   = "{}\n{}".format(common_template, sim_template)  
    
    # Replace template parameters and write to file
    with open(sim_filepath,'w') as myfile:
        print("creating {}".format(sim_filepath))
        myfile.write(full_template.format(**context))
    
#-------------------------------------------------------------------------------

def isSignalProcess( neutrino_string, beam_pol ):
    """ Checks if e+e- -> 2v4q is signal like:
        - neutrinos must be 1st generation
        - beam must be polarized as e-: left-handed; e+: right-handed 
          so that W+W- can be radiated
    """
    is_signal = False
    if ( neutrino_string == 'v1v1' and beam_pol == 'eLpR' ):
        is_signal = True
    return is_signal

#-------------------------------------------------------------------------------

def setAllSindarinFiles( model, ISR_file, luminosity, base_dir, output_format ):
    """ Set all singal and background sindarin files
            - for all signal processes:
                =>  all e+e- -> 2v4q processes where v are electron-neutrinos 
                    and polarization is eLpR
            - for all irreducible background processes:
                =>  e+e- -> 2v4q where VBS not possible (at first order)
                    Includes all neutrino families since they are inaccessible.
        Keep same model for all processes for consistency!
    """
    
    for nunu in pMaps.nunu_states:
        for qqqq in pMaps.qqqq_states:
            for beam_pol in pMaps.polarizations:
                output_dir = base_dir
                if isSignalProcess( nunu, beam_pol ):  
                    output_dir = '{}/signal'.format(base_dir)
                else:
                    output_dir = '{}/bkg'.format(base_dir)
                
                setSingleProcessFile(   nunu, qqqq, beam_pol, ISR_file, model, 
                                        luminosity, output_dir, output_format )
                
                                    
#-------------------------------------------------------------------------------

def main(arguments):
    """ Main function:
            Create Sindarin files for runs with the given parameters.
    """
    
    # TODO Properly document and rename: Everything in integration and simulation is Standard Model (just with SSC_2 => trivial CKM)
    
    model       = "SSC_2"
    
    luminosity  = "1000 / 1 fbarn"
    ISR_file    = "/afs/desy.de/user/b/beyerjac/flc/VBS/aQGC_analysis/scripts/WHIZARD_scripts/isr_spectrum_files/500_TDR_ws_ee021.circe"
    
    # base_dir must exist and contain two dirs called "signal" and "bkg"
    base_dir        = "/afs/desy.de/group/flc/pool/beyerjac/WHIZARD/vvqqqq"
    output_format   = "stdhep" 
    
    setAllSindarinFiles(model, ISR_file, luminosity, base_dir, output_format)
    
#-------------------------------------------------------------------------------

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

