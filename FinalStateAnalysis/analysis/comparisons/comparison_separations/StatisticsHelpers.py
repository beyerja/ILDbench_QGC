# ------------------------------------------------------------------------------

""" Functions and classes that help with statistics calculations.
"""

# ------------------------------------------------------------------------------

import numpy as np

# ------------------------------------------------------------------------------

def gaussian(x, norm, mu, sig):
  """ Return the value of a gaussian at the given position
  """
  return norm/(np.sqrt(2.*np.pi)*sig) * np.exp(-np.power((x - mu)/sig, 2.)/2.)


# ------------------------------------------------------------------------------