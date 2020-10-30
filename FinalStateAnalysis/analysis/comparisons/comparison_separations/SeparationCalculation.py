# ------------------------------------------------------------------------------

""" Functions needed to calculate the separation of a WW and a ZZ distribution.
"""

# ------------------------------------------------------------------------------

import logging as log
import math
import ROOT

import FitResultPlotting as FRP

# ------------------------------------------------------------------------------

# Some parameters that should be global in this namespace 
SINGLE_GAUSSIAN = "SingleGaussian"
DOUBLE_GAUSSIAN = "DoubleGaussian"

# ------------------------------------------------------------------------------

def get_percentage_borders(hist, integral_perc):
  """ Determine the lower and upper x values at which the inner distribution 
      integral contains the given percentage.
  """
  if (integral_perc == 100):
    x_axis = hist.GetXaxis()
    return [x_axis.GetXmin(), x_axis.GetXmax()]
  
  # Walk outwards from the mean (adding highest val bins) to see when integral 
  # becomes sufficient
  mean = hist.GetMean()
  start_bin = hist.FindBin(mean)
  bin_max = start_bin
  bin_min = start_bin
  
  current_integral = hist.GetBinContent(start_bin)
  goal_integral = hist.Integral() * integral_perc / 100.0
  while (current_integral < goal_integral):
    # First check on which side bins can still be added
    if (bin_min == 0):
      bin_max += 1 # Only upper bins can still be added
    elif (bin_min == hist.GetNbinsX()+2):
      bin_min -= 1 # Only lower bins can still be added
    else:
      # Determine if upper or lower bin has higher content
      if ( hist.GetBinContent(bin_max+1) > hist.GetBinContent(bin_min-1) ):
        bin_max += 1
      else:
        bin_min -= 1
      
    current_integral = hist.Integral(bin_min, bin_max)
    
  x_axis = hist.GetXaxis()
  x_max = x_axis.GetBinUpEdge(bin_max)
  x_min = x_axis.GetBinLowEdge(bin_min)
  
  return [x_min, x_max]

# ------------------------------------------------------------------------------

def fit_gaussian(hist, integral_perc, setting):
  """ Fit (TF1) a gaussian or double gaussian to the distribution.
      The integral_perc steers how much of the innermost part of the 
      distribution is supposed to be used (e.g. 80 innermost percent).
  """
  # Determine the range needed for the given percentage
  x_min, x_max = get_percentage_borders(hist, integral_perc)
  log.debug("Integrating {} from {} to {}".format(hist.GetName(),x_min,x_max))
  
  # String determining the fit function 
  # Pars: 0...Mean, 1...Sigma, 2...Amplitude
  gauss_str = "[2]*TMath::Gaus(x,[0],[1])"
  
  # For usefull parameter starting values
  ini_mean = hist.GetMean()
  ini_sigma = hist.GetStdDev()
  ini_ampl = hist.Integral() * integral_perc / 100.0
  
  # Set up fit function
  func = None
  func_type = setting.func_type
  func_name = "f_{}_{}".format(hist.GetName(),func_type)
  if func_type == SINGLE_GAUSSIAN:
    func_str = "[2]*TMath::Gaus(x,[0],[1])"
    func = ROOT.TF1(func_name, func_str, x_min, x_max)
    func.SetParNames("Mean", "Sigma", "Amplitude")
    func.SetParameters(ini_mean,ini_sigma,ini_ampl)
  elif func_type == DOUBLE_GAUSSIAN:
    func_str = "[0]/sqrt(2.0*3.141593) * ( [1]/[3] * exp(-0.5*pow((x-[2])/[3],2)) + (1-[1])/[5] * exp(-0.5*pow((x-[4])/[5],2)))"
    func = ROOT.TF1(func_name, func_str, x_min, x_max)
    func.SetParNames("Amplitude", "Fraction1", "Mean1", "Sigma1", "Mean2", "Sigma2")
    # Starting values such that first gaussian is expected to be the smaller one
    func.SetParameters(ini_ampl, setting.f1, setting.f_m1*ini_mean, setting.f_s1*ini_sigma, setting.f_m2*ini_mean, setting.f_s2*ini_sigma)
    func.SetParLimits(1, 0, 1) # Make sure the fraction is between 0 and 1
  else:
    raise ValueError("Unknown fit function type {}".format(func_type))
  
  # Perform the fit (S... store fit result, Q... quiet, N... don't draw)
  fit_result = hist.Fit(func_name, "SQN")
  
  if not fit_result.IsValid():
    log.warning("Fit result not valid for {}".format(hist.GetName()))
    
  chi2_ndf = func.GetChisquare()/func.GetNDF()
  if chi2_ndf > 4:
    log.warning("High chi^2/ndf for {} : {}".format(hist.GetName(),chi2_ndf))
  
  return func


# ------------------------------------------------------------------------------

def double_gauss_statistics(fit):
  """ Return the combined statistics of a double gaussian fit.
  """
  fraction = fit.GetParameter("Fraction1")
  mean1 = fit.GetParameter("Mean1")
  mean2 = fit.GetParameter("Mean2")
  mean = fraction * mean1 + (1.0-fraction) * mean2
  sigma = math.sqrt( fraction * fit.GetParameter("Sigma1")**2 + (1.0-fraction) * fit.GetParameter("Sigma2")**2 + fraction * (1.0-fraction) * (mean1 - mean2)**2 )
  return mean, sigma

# ------------------------------------------------------------------------------

def get_gauss_pars(fit):
  """ Get the mean and sigma from a fit function that may be a Gaussian or a 
      Double Gaussian.
  """
  mean = None
  sigma = None
  if (SINGLE_GAUSSIAN in fit.GetName()):
    mean = fit.GetParameter("Mean")
    sigma = fit.GetParameter("Sigma")
  elif (DOUBLE_GAUSSIAN in fit.GetName()):
    mean, sigma = double_gauss_statistics(fit)
  else:
    raise ValueError("Can't determine fit type from function name".format(fit.GetName()))
    
  return [mean,sigma]

# ------------------------------------------------------------------------------

def calculate_separation(fit_1,fit_2):
  """ Calculate the gaussian separation between two gaussian fits.
  """
  mean_1, sigma_1 = get_gauss_pars(fit_1)
  mean_2, sigma_2 = get_gauss_pars(fit_2)
  
  return abs(mean_1 - mean_2) / math.sqrt(sigma_1**2 + sigma_2**2)

# ------------------------------------------------------------------------------

def get_separation(path,WW_name,ZZ_name,integral_perc,settings_WW,settings_ZZ,output_dir=None,save_fits=False,name_add=""):
  """ Calculate the separate between the WW and ZZ histgrams (using the 
      innermost percentage given).
  """
  file = ROOT.TFile.Open(path, "READ")
  h_WW = file.Get(WW_name)
  h_ZZ = file.Get(ZZ_name)
  fit_WW = fit_gaussian(h_WW, integral_perc, settings_WW)
  fit_ZZ = fit_gaussian(h_ZZ, integral_perc, settings_ZZ)
  
  if save_fits:
    FRP.save_hist(h_WW, fit_WW, output_dir, name_add=name_add)
    FRP.save_hist(h_ZZ, fit_ZZ, output_dir, name_add=name_add)
  
  file.Close()
  return calculate_separation(fit_WW, fit_ZZ)