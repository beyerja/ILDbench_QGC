#!/usr/bin/python

""" Create Sindarin files and according directory structure to given process.
""" # TODO Update!!!

import datetime
import ProcessMaps as pMaps 

#-------------------------------------------------------------------------------

def calculateAlphaConstant(dim8c):
    result = (0.246 ** 4) * dim8c / 16
    return result
        
#-------------------------------------------------------------------------------

def getaQGCSindarinContext(model, fs_zero,fs_one):
    
    if model == "SM_CKM":
        return " "
    
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

def getStandardSindarinContext( filepath_base ):
    """ Returns base dictonary with parameters common to all models. 
    """
    standard_context = {
        'date':                 datetime.datetime.today(),
        'out_file_base' :       filepath_base,
    }
    return standard_context
    
#-------------------------------------------------------------------------------

def getProcessSindarinContext(  nunu, qqqq, beam_pol, ISR_file, 
                                model, fs_zero, fs_one, fkm ):
    """ Return dictonary with parameters in template that configure the
        physics behind the interaction that is calculated by WHIZARD.
    """

    if model == 'SM_CKM':
        # -> Is background process
        eft_switch = ""
        unitarization_switch = ""
    else:
        # -> Is signal process
        eft_switch = "eft_h = 1"
        unitarization_switch = "fkm = {}".format(fkm)
        
    process = 'electron, positron => {}, {}'.format( pMaps.nunu_states[nunu],
                                                     pMaps.qqqq_states[qqqq] ) 
        
    process_name = "{}{}".format(nunu,qqqq) 
        
    context = {
        'process_name':         process_name,
        'process':              process,
        'beam_polarization' :   pMaps.polarizations[beam_pol],
        'ISR_file' :            ISR_file, 
        'model' :               model,
        'coupling' :            getaQGCSindarinContext(model, fs_zero,fs_one),
        'eft_switch':           eft_switch,
        'unitarization_switch': unitarization_switch,        
    }
    
    return context

#-------------------------------------------------------------------------------

def getCutValueSindarinContext():
    """ Returns cut value dictonary with cut parameters for WHIZARD. 
    """
    cut_context = {
        'y_34_min'          : 0.00001,
    }
    return cut_context

#-------------------------------------------------------------------------------

def getSimulationSindarinContext( luminosity, output_format ):
    """ Returns dictonary of parameters for simulation of events with WHIZARD.
    """ 
    
    context = {
        'luminosity':       luminosity,
        'sample_format':    output_format
    }
    
    return context