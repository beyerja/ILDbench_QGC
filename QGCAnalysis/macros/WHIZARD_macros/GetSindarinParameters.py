#!/usr/bin/python

""" Create Sindarin files and according directory structure to given process.
""" # TODO Update!!!

import datetime
import numpy as np
import ProcessMaps            as pMaps 

#-------------------------------------------------------------------------------

def calculateAlphaConstant(dim8c):
    result = (0.246 ** 4) * dim8c / 16
    return result
        
#-------------------------------------------------------------------------------

def getaQGCSindarinContext(model, fs0, fs1):
    
    if model == "SM_CKM":
        return " "
    
    a4 = calculateAlphaConstant(fs0)
    a5 = calculateAlphaConstant(fs1)

    result = ' fs0 = {}\n fs1 = {}\n real a4 = {}\n real a5 = {}'.format( 
                                                            fs0, fs1, a4, a5)
    return result

#-------------------------------------------------------------------------------

def getStandardSindarinContext( output_dir, filepath_base ):
    """ Returns base dictonary with parameters common to all models. 
    """
    standard_context = {
        'date':                 datetime.datetime.today(),
        'out_dir' :             filepath_base,
        'out_file_base' :       filepath_base,
    }
    return standard_context
    
#-------------------------------------------------------------------------------

def getProcessSindarinContext(  nunu, qqqq, beam_pol, ISR_file, model ):
    """ Return dictonary with parameters in template that configure the
        physics behind the interaction that is calculated by WHIZARD.
    """
        
    process = 'electron, positron => {}, {}'.format( pMaps.nunu_states[nunu],
                                                     pMaps.qqqq_states[qqqq] ) 
        
    process_name = "{}{}".format(nunu,qqqq) 
        
    context = {
        'process_name':         process_name,
        'process':              process,
        'beam_polarization' :   pMaps.polarizations[beam_pol],
        'ISR_file' :            ISR_file, 
        'model' :               model,
    }
    
    return context

#-------------------------------------------------------------------------------

def getModelParameterSindarinContext( model, fs_zero=0, fs_one=0, fkm=0 ):
    """ Return dictonary with model parameters for template.
    """    
    
    if model == 'SM_CKM':
        # -> Is background process
        eft_switch = ""
        unitarization_switch = ""
    else:
        # -> Is signal process
        eft_switch = "eft_h = 1"
        unitarization_switch = "fkm = {}".format(fkm)

    context = {
        'coupling' :            getaQGCSindarinContext(model, fs_zero, fs_one),
        'eft_switch':           eft_switch,
        'unitarization_switch': unitarization_switch
    }
    
    return context
    
#-------------------------------------------------------------------------------

def getCutValueSindarinContext():
    """ Returns cut value dictonary with cut parameters for WHIZARD. 
    """
    cut_context = {
        'M_qq_min'  : '10 GeV',
        'E_q_min'   : '1  GeV'
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
    
#-------------------------------------------------------------------------------

def get2DParameterGrid( fs0_points, fs1_points, scan2D ):
    """ Get 2D grid of (FS0, FS1) parameter space.
        Can choose whether to include points where both are non-zero.
        Do not include SM point if it is in arrays.
    """
    grid = []
    
    
    if scan2D:
        N_2D_points = len(fs0_points)*len(fs1_points)
        if ( (0 in fs0_points) and (0 in fs1_points)):
            # exclude SM point when in array
            N_2D_points -= 1
            
        # Empty array of right shape 
        grid = np.zeros( shape=(N_2D_points, 2) )
        index = 0
        for fs0_point in fs0_points:
            for fs1_point in fs1_points:
                if ( fs0_point == 0 and fs1_point == 0 ): continue
                grid[index] = [fs0_point, fs1_point]
                index += 1
    else:
        N_2D_points = len(fs0_points)+len(fs1_points)
        # exclude SM point when in array
        if 0 in fs0_points: N_2D_points -= 1
        if 0 in fs1_points: N_2D_points -= 1
            
        # Empty array of right shape (exclude SM points!)
        grid = np.zeros( shape=(N_2D_points, 2) )
        index = 0
        for fs0_point in fs0_points:
            if ( fs0_point == 0 ): continue
            grid[index] = [fs0_point, 0]
            index += 1
        for fs1_point in fs1_points:
            if ( fs1_point == 0 ): continue
            grid[index] = [0, fs1_point]
            index += 1
            
    return grid

#-------------------------------------------------------------------------------

def getAlternativeParametersSindarinContext( fs0_points, fs1_points, scan2D ):
    """ Takes arrays of EFT coupling points, creates Sindarin language 
        command for reweighting with alternative setups for all parameter point
        combinations and return dictionary for placing this into the template.
    """
    
    alt_setups = "alt_setup ="
    
    parameter_grid = get2DParameterGrid( fs0_points, fs1_points, scan2D )
    
    for fs0_point, fs1_point in parameter_grid:
        couplings_string = getaQGCSindarinContext("SSC_2", fs0_point, fs1_point)
        alt_setups = "{} {{\n{}\n}},".format( alt_setups, couplings_string )

    # Remove last comma
    alt_setups = alt_setups[:-1]
    
    context = {
        'alternative_setups': alt_setups
    }
    
    return context

#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
