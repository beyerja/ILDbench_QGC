#!/usr/bin/python

""" Create Sindarin files and according directory structure to given process.
"""

import sys
import numpy as np
import ProcessMaps            as pMaps 
import GetSindarinParameters  as sinPars

#-------------------------------------------------------------------------------
# Global variables

target_dir = '/afs/desy.de/group/flc/pool/beyerjac/WHIZARD/vvqqqq'

#-------------------------------------------------------------------------------

def getSimulationTemplate():
    """ Return string of template file content for simulation steering.
    """
    with open("../../scripts/WHIZARD_scripts/SimulationTemplate.sin", "r") as template_file:
        return template_file.read()
        
#-------------------------------------------------------------------------------

def getFilenameBase ( final_state, beam_pol, model, output_dir ):
    """ Returns standardized file name base. 
    """
        
    final_state_name = final_state.replace(',', '')
        
    filename = "WHIZARD_E1000_{}_{}_{}".format( 
                                            final_state_name, beam_pol, model )  
                                                
    filepath = output_dir + "/" + filename
    return filepath
    
#-------------------------------------------------------------------------------

def setSingleSindarinFile(  final_state, beam_pol, ISR_file, model, luminosity, 
                            output_dir, output_format ):
    """ Create single Sindarin file from the template, replace the template 
        parameters according to the input.
    """
    
    ## Collect dictonary with template parameters that need to be replaced
    
    # In Sindarin: e1 = electron, E1 = positron
    process = 'e1, E1 => {}'.format( final_state) 
    
    filepath_base  = getFilenameBase( final_state, beam_pol, model, output_dir )
    filepath       = "{}.sin".format(filepath_base)

    context         = sinPars.getStandardSindarinContext( filepath_base )
    process_context = sinPars.getProcessSindarinContext (   
                                process, beam_pol, ISR_file, model, 0, 0, 0 )
    cut_context     = sinPars.getCutValueSindarinContext()
    sim_context     = sinPars.getSimulationSindarinContext( luminosity, 
                                                            output_format )
    
    context.update( process_context )
    context.update( cut_context     )
    context.update( sim_context     )

    ## Create Sindarin file and replace template parameters 
    with  open(filepath,'w') as myfile:
        print("creating {}".format(filepath))
        myfile.write(getSimulationTemplate().format(**context))

#-------------------------------------------------------------------------------

def isSignalProcess( neutrino_string, beam_pol ):
    """ Checks if e+e- -> 2v4q is signal like:
        - neutrinos must be 1st generation
        - beam must be polarized as e-: left-handed; e+: right-handed 
          so that W+W- can be radiated
    """
    is_signal = False
    if ( neutrino_string == 'n1,N1' and beam_pol == 'eLpR' ):
        is_signal = True
    return is_signal

#-------------------------------------------------------------------------------

def setAllSindarinFiles(    model, ISR_file, luminosity, 
                            output_dir, output_format ):
    """ Set all singal and background sindarin files
            - for all signal processes:
                =>  all e+e- -> 2v4q processes where v are electron-neutrinos 
                    and polarization is eLpR
            - for all irreducible background processes:
                =>  e+e- -> 2v4q where VBS not possible (at first order)
                    Includes all neutrino families since they are inaccessible.
                    To keep consistency with rest of bkg: use SM_CKM model
    """
    
    for nunu in pMaps.di_neutrino_states:
        for qqqq in np.append(pMaps.ZZlike_4q_processes, pMaps.WWlike_4q_processes):
            for beam_pol in pMaps.polarizations:
                final_state = '{},{}'.format( nunu, qqqq )
                
                if isSignalProcess( nunu, beam_pol ):  
                    signal_dir = '{}/signal'.format(output_dir)
                    setSingleSindarinFile(  final_state, beam_pol, ISR_file, 
                                            model, luminosity, signal_dir, 
                                            output_format )
                
                else:
                    bkg_model = 'SM_CKM' # in case of bkg
                    bkg_dir = '{}/bkg'.format(output_dir)
                    setSingleSindarinFile(  final_state, beam_pol, ISR_file, 
                                            bkg_model, luminosity, bkg_dir, 
                                            output_format )
                
                
                                    
#-------------------------------------------------------------------------------

def main(arguments):
    """ Main function:
            Create Sindarin files for runs with the given parameters\
    """
    
    # TODO Properly document and rename: Everything in integration and simulation is Standard Model (just with SSC_2 => trivial CKM)
    
    model       = "SSC_2"
    
    luminosity  = "1000 / 1 fbarn"
    ISR_file    = "500_TDR_ws_ee021.circe" # TODO GLOBAL PATH -> put this somewhere here so that it can be stored on GitHub!
    
    output_dir      = "/afs/desy.de/group/flc/pool/beyerjac/WHIZARD/vvqqqq"
    output_format   = "stdhep" 
    
    setAllSindarinFiles(model, ISR_file, luminosity, output_dir, output_format )
    
#-------------------------------------------------------------------------------

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

