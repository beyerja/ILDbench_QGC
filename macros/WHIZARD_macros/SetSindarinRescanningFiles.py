#!/usr/bin/python

""" Create Sindarin files and according directory structure to given process.
"""

import sys
import ProcessMaps            as pMaps 
import GetSindarinParameters  as sinPars

#-------------------------------------------------------------------------------
# Global variables

target_dir = '/afs/desy.de/group/flc/pool/beyerjac/WHIZARD/vvqqqq'

#-------------------------------------------------------------------------------

def getRescanningTemplate():
    """ Return string of template file content.
    """
    with open("../../scripts/WHIZARD_scripts/RescanningTemplate.sin", "r") as template_file:
        return template_file.read()
        
#-------------------------------------------------------------------------------

def getWeightFileBase ( final_state, beam_pol, model, fkm ):
    """ Returns standardized file name. 
    """
        
    final_state_name = final_state.replace(',', '')
        
    filename = "WHIZARD_{}_{}_{}_fS0_{}_fS1_{}_{}".format(
                                                    final_state_name, beam_pol, 
                                                    model, fs_zero, fs_one,
                                                    fkm_strings[fkm])  
                                                
    filepath = target_dir + "/" + filename + '.sin'
    return filepath
    
#-------------------------------------------------------------------------------

def setSingleSindarinFile(  final_state, beam_pol, pol_file, 
                            model, fs_zero, fs_one, fkm  ):
    """ Create single Sindarin file from the template according to the given
        and global parameters.
    """
    
    process = 'electron, positron => {}'.format( final_state)
    
    filename  = "{}.sin".format( getFilenameBase(   final_state, beam_pol, 
                                                    model, fs_zero, fs_one, fkm
                                                ) )
                                            

    # Get dictionary with values that need to be replaced in template
    context         = sinPars.getStandardSindarinContext( filepath )
    process_context = sinPars.getProcessSindarinContext (   
                                process, beam_pol, pol_file, 
                                model, fs_zero, fs_one, fkm )
    cut_context     = sinPars.getCutValueSindarinContext()
    
    context.update( process_context )
    context.update( cut_context     )

    with  open(filepath,'w') as myfile:
        print("creating {}".format(filepath))
        myfile.write(getRescanningTemplate().format(**context))

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

def setAllSindarinFiles(  pol_file, model, fs_zero, fs_one, fkm ):
    """ Set all singal and background sindarin files
            - for all signal processes:
                =>  all e+e- -> 2v4q processes where v are electron-neutrinos 
                    and polarization is eLpR
            - for all irreducible background processes:
                =>  e+e- -> 2v4q where VBS not possible (at first order)
                    Includes all neutrino families since they are inaccessible.
                    To keep consistency with rest of bkg: use SM_CKM model
    """
    # TODO only signal files need to be reweighted 
    #TODO Find out how to reweight 
    
    
    for nunu in pMaps.di_neutrino_states:
        for qqqq in pMaps.quadro_q_processes:
            for beam_pol in pMaps.polarizations:
                final_state = '{},{}'.format( nunu, qqqq )
                
                if isSignalProcess( nunu, beam_pol ):  
                    setSingleSindarinFile(  final_state, beam_pol, pol_file, 
                    model, fs_zero, fs_one, fkm )
                    #TODO Need to scan, but ONLY for signal!
                
                                    
#-------------------------------------------------------------------------------

def main(arguments):
    """ Main function:
            Create Sindarin files for runs with the given parameters\
    """
    
    fkm = 1 # Switch on unitarization 
    
    fs_zero = 100
    fs_one  = 100
    
    model = "SSC_2"
    
    pol_file = "500_TDR_ws_ee021.circe"
    
    setAllSindarinFiles( pol_file, model, fs_zero, fs_one, fkm )
    
#-------------------------------------------------------------------------------

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

