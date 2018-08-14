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
    'veve' : 'nu_e, nu_e',
    'vlvl' : 'nu_l, nu_l',
}

polarizations = {
    'eLpL': '@(-1) , @(-1)',
    'eLpR': '@(-1) , @(+1)',
    'eRpL': '@(+1) , @(-1)',
    'eRpR': '@(+1) , @(+1)',
}

# TODO Move this to where it belongs
# Is K-matrix unitarization turned on(1) or off(0)? -> file name part
fkm_strings = {
    1 : "kmon",
    0 : "kmoff",
}


# TODO Rename file, e.g. ConventionToSindarinMaps