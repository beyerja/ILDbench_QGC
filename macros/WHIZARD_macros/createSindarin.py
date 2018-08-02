#!/usr/bin/python

""" Create Sindarin files and according directory structure to given process.
"""


import numpy as np

import datetime
import sys

from processMaps    import *

#-------------------------------------------------------------------------------
# Global variables

target_dir = '/afs/desy.de/group/flc/pool/beyerjac/WHIZARD/vvqqqq'

#-------------------------------------------------------------------------------

def getTemplate():
    """ Return string of template file content.
    """
    with open("../../scripts/WHIZARD_scripts/sindarinTemplate.sin", "r") as template_file:
        return template_file.read()

#-------------------------------------------------------------------------------

def getFilePath ( final_state, beam_pol, model, fs_zero, fs_one, fkm ):
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

def calculateAlphaConstant(dim8c):
    result = (0.246 ** 4) * dim8c / 16
    return result
        
#-------------------------------------------------------------------------------

def getaQGCSindarinContext(model, fs_zero,fs_one):
    
    if model == "SM":
        fs_zero = fs_one = 0
    
    aqgccontext = {
        'fs0' : fs_zero,
        'fs1' : fs_one,
        'a4'  : calculateAlphaConstant(fs_zero),
        'a5'  : calculateAlphaConstant(fs_one),
    }

    result = """
fs0 = {fs0}
fs1 = {fs1}
real a4 = {a4}
real a5 = {a5}
""".format(**aqgccontext)
    return result

#-------------------------------------------------------------------------------

def getStandardSindarinContext( filepath ):
    """ Returns base dictonary with parameters common to all models. 
    """
    standard_context = {
        'date':                 datetime.datetime.today(),
        'out_file' :            filepath + '.dat',
    }
    return standard_context
    
#-------------------------------------------------------------------------------

def getProcessSindarinContext(  process, beam_pol, pol_file, 
                                model, fs_zero, fs_one, fkm ):
                    
    # TODO This isn't nice, separate it so that its clear what is happening
    if model == 'SM':
        eft_h       = 0
    else:
        eft_h = 1
    
    context = {
        'process':              process,
        'beam_polarization' :   polarizations[beam_pol],
        'polarization_file' :   pol_file, 
        'model' :               model,
        'coupling' :            getaQGCSindarinContext(model, fs_zero,fs_one),
        'eft_h' :               eft_h,
        'fkm' :                 fkm,
        'sat_amplitudes' :      ''
    }
    return context

#-------------------------------------------------------------------------------

def getCutValueSindarinContext():
    """ Returns cut value dictonary with cut parameters for WHIZARD. 
    """
    cut_context = {
        'kT_34_min'         : 0.00005,
        'cosTheta_max_nus'  : 0.999,
        'cosTheta_max_qs'   : 0.999
    }
    return cut_context

#-------------------------------------------------------------------------------

def setSingleSindarinFile(  final_state, beam_pol, pol_file, 
                            model, fs_zero, fs_one, fkm  ):
    """ Create single Sindarin file from the template according to the given
        and global parameters.
    """
    
    process = 'electron, positron => {}'.format( final_state)
    
    filepath  = getFilePath(    final_state, beam_pol, 
                                model, fs_zero, fs_one, fkm )

    # Get dictionary with values that need to be replaced in template
    context         = getStandardSindarinContext(   filepath )
    process_context = getProcessSindarinContext (   process, beam_pol, pol_file, 
                                                    model, fs_zero, fs_one, 
                                                    fkm )
    cut_context     = getCutValueSindarinContext()
    
    context.update( process_context )
    context.update( cut_context     )

    with  open(filepath,'w') as myfile:
        print("creating {}".format(filepath))
        myfile.write(getTemplate().format(**context))

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

# def setAllIrreduciableBackgroundFiles( process ):
#     """ Set sindarin files for all irreducible background processes:
#         => e+e- -> 2v4q where VBS not possible (at first order)
#         Includes different neutrino families since they are inaccessible
#     """
#     # All neutrino families, all polarizations


#-------------------------------------------------------------------------------

def setAllSignalFiles(  pol_file, model, fs_zero, fs_one, fkm ):
    """ Set sindarin files for all signal processes:
        =>  all e+e- -> 2v4q processes where v are electron-neutrinos and
            polarization is eLpR
    """
    # TODO only signal files need to be reweighted 
    #TODO Find out how to reweight 
    
    nunu = 'n1,N1'
    
    for qqqq in quadro_q_processes:
        for beam_pol in polarizations:
            if not isSignalProcess( nunu, beam_pol ): continue 
            
            final_state = '{},{}'.format( nunu, qqqq )
            
            setSingleSindarinFile(  final_state, beam_pol, pol_file, 
                                    model, fs_zero, fs_one, fkm )
                                    




## TODO Same Loop for signal and background, and build isSignalProcess function


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
    
    setAllSignalFiles( pol_file, model, fs_zero, fs_one, fkm )
    
    
    # Use instead: 
    # F_S0,1 directly 
    
    # Loop over M_VV bin centers
    
    
    
    # for diboson_mass in range( M_VV_first_center, M_VV_last_center, bin_width): #range(200, 2200, 50):
    #     for process in ['wwds']:
    #         for model in ['SM']:
    #             setSingleSindarinFile(  process, model, fkm, diboson_mass, 
    #                                     bin_width, res_mass, res_ratio )
    #             # writeSindarin_ppul(process, bsm, fkm, mass, ratio, energy, 100)
    #             # writeSindarin_ppul(process, bsm, fkm, diboson_mass, bin_width)

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

