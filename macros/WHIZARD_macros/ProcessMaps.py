#!/usr/bin/python

"""
Collection of dictionaries and arrays used by createSindarin.py to create 
WHIZARD steering Sindarin files.
"""

import numpy as np

# Dictionary translating DBD naming convention for quark states to Sindarin
qqqq_states = {
    'xyyx' : 'all_uptype, all_downtype, all_downtype, all_uptype',
    'xxxx' : 'all_uptype, all_uptype, all_uptype, all_uptype',
    'yyyy' : 'all_downtype, all_downtype, all_downtype, all_downtype'
}

# Split into electron neutrino and non-electron neutrino bc only electron nu 
# is signal-like
nunu_states = {
    'v1v1' : 'all_nu_e, all_nu_e',
    'vv' : 'all_nu_l, all_nu_l',
}

polarizations = {
    'eLpL': '@(-1) , @(-1)',
    'eLpR': '@(-1) , @(+1)',
    'eRpL': '@(+1) , @(-1)',
    'eRpR': '@(+1) , @(+1)',
}




# TODO Rename file, e.g. ConventionToSindarinMaps