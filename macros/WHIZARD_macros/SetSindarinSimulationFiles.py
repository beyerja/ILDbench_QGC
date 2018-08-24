#!/usr/bin/python

""" Create Sindarin files for simulation of vvqqqq final state without anomalous
    couplings and according directory structure.
"""

import sys
import ProcessMaps            as pMaps 
import GetSindarinParameters  as sinPars
import SystemHelperFunctions  as sysHelp
import GetOutputDirectory     as outDir


#-------------------------------------------------------------------------------

def createProcessSubdirectory( nunu, qqqq, beam_pol, model, output_dir ):
    """ Creates subdirectory for the specific process in the given output 
        directory.
    """
    subdir_name = "{}{}_{}_{}".format( nunu, qqqq, beam_pol, model )
    full_subdir_path = "{}/{}".format(output_dir, subdir_name) 
    
    sysHelp.createSubdirectory( full_subdir_path )
        
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
    """ Function that sets up everything needed for the simulation of the
        process.
        - Creates subdirectory
        - Gets all the information that needs to be filled into the template
        - Gets the template, fills and saves it
        Also saves all the filled information so that it can be consistently 
        used when rescanning the events.
    """
    
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
    cut_context         = sinPars.getCutValueSindarinContext()
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
    common_template = sysHelp.readFileContent("../../scripts/WHIZARD_scripts/CommonSetupTemplate.sin")
    sim_template    = sysHelp.readFileContent("../../scripts/WHIZARD_scripts/SimulationTemplate.sin")
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
            Create Sindarin files for simulation runs with the given parameters.
    """
    
    # TODO Properly document everything -> How are steerings created, how can one change settings, .......
    # TODO Properly document and rename: Everything in integration and simulation is Standard Model (just with SSC_2 => trivial CKM)
    
    # Model used by WHIZARD to calculate the matrix element
    model       = 'SSC_2'
    
    # Settings for the simulation
    luminosity  = '1000 / 1 fbarn'
    ISR_file    = '/afs/desy.de/user/b/beyerjac/flc/VBS/aQGC_analysis/scripts/WHIZARD_scripts/isr_spectrum_files/ee_Waisty_opt_Jan2012_1000GeV_B1b.circe'
    
    # base_dir must exist and contain two dirs called "signal" and "bkg"
    base_dir        = outDir.getOutputDirectory()
    output_format   = 'lcio' 
    
    setAllSindarinFiles(model, ISR_file, luminosity, base_dir, output_format)
    
#-------------------------------------------------------------------------------

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

#-------------------------------------------------------------------------------
