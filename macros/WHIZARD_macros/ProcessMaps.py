#!/usr/bin/python

"""
Collection of dictionaries and arrays used by createSindarin.py to create 
WHIZARD steering Sindarin files.
"""

import numpy as np

# Notation: nu anti-nu q anti-q anti-q q
# Careful! Avoid duplicate processes (e.g. vvudus and vvusud) 
ZZlike_4q_processes = np.array([
    'u,U,U,u',             
    'd,D,U,u', 'd,D,D,d', 
    's,S,U,u', 's,S,D,d', 's,S,S,s', 
    'c,C,U,u', 'c,C,D,d', 'c,C,S,s', 
    'b,B,U,u', 'b,B,D,d', 'b,B,S,s', 'b,B,C,c', 'b,B,B,b'
])

WWlike_4q_processes = np.array([
  # -> along x: first pair constant (W+), second pair changing (W-)
  # -> along y: first pair changing (W+), second pair constant (W-)
    'u,D,U,d', 'u,D,U,s', 'u,D,U,b', 'u,D,C,d', 'u,D,C,s', 'u,D,C,b', 
    'u,S,U,d', 'u,S,U,s', 'u,S,U,b', 'u,S,C,d', 'u,S,C,s', 'u,S,C,b',
    'u,B,U,d', 'u,B,U,s', 'u,B,U,b', 'u,B,C,d', 'u,B,C,s', 'u,B,C,b',
    'c,D,U,d', 'c,D,U,s', 'c,D,U,b', 'c,D,C,d', 'c,D,C,s', 'c,D,C,b',
    'c,S,U,d', 'c,S,U,s', 'c,S,U,b', 'c,S,C,d', 'c,S,C,s', 'c,S,C,b',
    'c,B,U,d', 'c,B,U,s', 'c,B,U,b', 'c,B,C,d', 'c,B,C,s', 'c,B,C,b'
])

di_neutrino_states = np.array([
    'n1,N1', # Signal
    'n2,N2', # Background
    'n3,N3'
])

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