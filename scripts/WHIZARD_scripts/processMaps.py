#!/usr/bin/python

"""
Collection of dictionaries and arrays used by createSindarin.py to create 
WHIZARD steering Sindarin files.
"""

import numpy as np

# Notation: nu anti-nu q anti-q anti-q q
# Careful! Avoid duplicate processes (e.g. vvudus and vvusud) 
quadro_q_processes = np.array([
  ### To be produced with anomalous couplings: (only in LR)
  # Only ZZ: 
    'u,U,U,u',             
    'd,D,U,u', 'd,D,D,d', 
    's,S,U,u', 's,S,D,d', 's,S,S,s', 
    'c,C,U,u', 'c,C,D,d', 'c,C,S,s', 
    'b,B,U,u', 'b,B,D,d', 'b,B,S,s', 'b,B,C,c', 'b,B,B,b',

  # Only WW:
    'u,D,U,d', 'u,D,U,s', 'u,D,U,b', 'u,D,C,d', 'u,D,C,s', 'u,D,C,b', 
               'u,S,U,s', 'u,S,U,b', 'u,S,C,d', 'u,S,C,s', 'u,S,C,b',
                          'u,B,U,b', 'u,B,C,d', 'u,B,C,s', 'u,B,C,b',
                                     'c,D,C,d', 'c,D,C,s', 'c,D,C,b',
                                                'c,S,C,s', 'c,S,C,b',
                                                           'c,B,C,b'
])

polarizations = {
    'eLpL': '@(-1) , @(-1)',
    'eLpR': '@(-1) , @(+1)',
    'eRpL': '@(+1) , @(-1)',
    'eRpR': '@(+1) , @(+1)',
}

di_neutrino_states = [ # => This should be in main file
    'n1,N1', # Signal
    'n2,N2', # Background
    'n3,N3'
]

# Is K-matrix unitarization turned on(1) or off(0)? -> file name part
fkm_strings = {
    1 : "kmon",
    0 : "kmoff",
}