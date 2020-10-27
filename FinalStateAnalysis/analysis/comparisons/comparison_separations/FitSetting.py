import SeparationCalculation as SC

class FitSetting:
  """ Class that stores the fit settings for a given reconstruction level.
  """
  
  def __init__(self, func_type, initial_vals=None):
    self.func_type = func_type
    
    if self.func_type == SC.DOUBLE_GAUSSIAN:
      # Get the initial factor to be used as starting values
      self.f1 = initial_vals["f1"]
      self.f_m1 = initial_vals["f_m1"]
      self.f_m2 = initial_vals["f_m2"]
      self.f_s1 = initial_vals["f_s1"]
      self.f_s2 = initial_vals["f_s2"]
    
    