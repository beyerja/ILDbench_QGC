# ------------------------------------------------------------------------------

""" Functions and classes to properly plot a given histogram with its fit 
    result.
"""

# ------------------------------------------------------------------------------

import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import quad

import SeparationCalculation as SC
import StatisticsHelpers as SH

# ------------------------------------------------------------------------------

def get_plot_text(hist, fit):
  """ Get the text describing the metadata of a given histogram and fit.
  """
  # TODO Separate hist and fit
  
  std_dev = hist.GetStdDev()
  mean = hist.GetMean()
  
  fit_name = fit.GetName()
  
  fit_mean = None
  fit_sigma = None
  # if SC.SINGLE_GAUSSIAN:
  if SC.DOUBLE_GAUSSIAN in fit_name:
    fit_mean, fit_sigma = SC.double_gauss_statistics(fit)
  else:
    raise ValueError("No known fit func in fit name {}".format(fit_name))
  
  evt_type = None
  if "WW" in fit_name:
    evt_type = "WW"
  elif "ZZ" in fit_name:
    evt_type = "ZZ"
  else:
    raise ValueError("Couldn't identify fit event type {}".format(fit_name))
    
  text = r"""{0}
$\mu_{{hist}}$={1}
$\sigma_{{hist}}$={2}
$\mu_{{fit}}$={3}
$\sigma_{{fit}}$={4}
$\chi^2/ndf$={5}""".format( 
    evt_type,
    round(mean,1),
    round(std_dev,1),
    round(fit_mean,1),
    round(fit_sigma,1),
    round(fit.GetChisquare()/fit.GetNDF(),1))
  return text
  
# ------------------------------------------------------------------------------

def extract_hist_data(hist):
  """ Extract the x and y data and the bin edges from a TH1 ROOT histogram.
  """
  hist_x = []
  hist_y = []
  bin_edges = []
  
  for b in range(hist.GetNbinsX()):
    i_b = b + 1 # TH1: 0 is underflow bins -> offset by 1
    hist_x.append(hist.GetBinCenter(i_b))
    hist_y.append(hist.GetBinContent(i_b))
    bin_edges.append(hist.GetXaxis().GetBinLowEdge(i_b))
    
  bin_edges.append(hist.GetXaxis().GetBinUpEdge(hist.GetNbinsX())) # upper edge
  
  return np.array(hist_x), np.array(hist_y), np.array(bin_edges)

# ------------------------------------------------------------------------------

def extract_fit_data(fit, hist, n_bins):
  """ Extract the y data from a TF1 ROOT fit that belongs to a histogram.
  """
  x_axis = hist.GetXaxis()
  x_min = x_axis.GetBinLowEdge(1)
  x_max = x_axis.GetBinUpEdge(hist.GetNbinsX())
  fit_x = np.linspace(x_min, x_max, num=n_bins)
  
  # Histogram is (evenly) binned -> Need to integrate function over bin width
  bin_width = hist.GetBinWidth(1)
  fit_y = np.array([fit.Integral(x-bin_width/2,x+bin_width/2) for x in fit_x])
  return fit_x, fit_y
  
# ------------------------------------------------------------------------------

def extract_doublegaussian_fit_data(fit, hist, n_bins):
  """ Extract the y data's for the individual gaussians from a TF1 ROOT fit that 
      is a double gaussian and that belongs to a histogram.
  """
  x_axis = hist.GetXaxis()
  x_min = x_axis.GetBinLowEdge(1)
  x_max = x_axis.GetBinUpEdge(hist.GetNbinsX())
  fit_x = np.linspace(x_min, x_max, num=n_bins)

  # Get the parameters for both Gaussians
  A = fit.GetParameter("Amplitude")
  f1 = fit.GetParameter("Fraction1")
  mean_1 = fit.GetParameter("Mean1")
  sigma_1 = fit.GetParameter("Sigma1")
  mean_2 = fit.GetParameter("Mean2")
  sigma_2 = fit.GetParameter("Sigma2")
  A1 = A * f1
  A2 = A * (1.0 - f1)
  
  # Determine which one is the tail and which one the peak (peak is thinner)
  peak_par = {}
  tail_par = {}
  if (sigma_1 < sigma_2):
    peak_par = {"A": A1, "mean": mean_1, "sigma": sigma_1} 
    tail_par = {"A": A2, "mean": mean_2, "sigma": sigma_2} 
  else:
    peak_par = {"A": A2, "mean": mean_2, "sigma": sigma_2} 
    tail_par = {"A": A1, "mean": mean_1, "sigma": sigma_1} 
    
  peak_gaussian = lambda x : SH.gaussian(x, peak_par["A"], peak_par["mean"], 
                                         peak_par["sigma"])
  tail_gaussian = lambda x : SH.gaussian(x, tail_par["A"], tail_par["mean"], 
                                         tail_par["sigma"])
  
  # Integrate over bin content to get bin value
  bin_width = hist.GetBinWidth(1)
  peak_y = np.array([quad(peak_gaussian,x-bin_width/2,x+bin_width/2) for x in fit_x])
  tail_y = np.array([quad(tail_gaussian,x-bin_width/2,x+bin_width/2) for x in fit_x])

  return fit_x, peak_y, tail_y

# ------------------------------------------------------------------------------

def plot_hist(axis, hist):
  """ Draw the given histogram on the given axis.
  """
  hist_x, hist_y, bin_edges = extract_hist_data(hist)
  return axis.hist(hist_x, bins=bin_edges, weights=hist_y, histtype="step", lw=2, color="black")

# ------------------------------------------------------------------------------

def plot_fit(axis, fit, hist, n_bins):
  """ Draw the given fit to the given histogram on the given axis.
  """
  # Plot the full fit
  fit_x, fit_y = extract_fit_data(fit, hist, n_bins)
  axis.plot(fit_x, fit_y, color="#DC267F")

  # Draw individual Gaussians in case of combined Gaussian
  if (SC.DOUBLE_GAUSSIAN in fit.GetName()):
    fit_x, peak_y, tail_y = extract_doublegaussian_fit_data(fit, hist, n_bins)
    axis.plot(fit_x, peak_y, color="#785EF0")
    axis.plot(fit_x, tail_y, color="#FE6100")
    
    
# ------------------------------------------------------------------------------

def save_hist(hist, fit, output_dir, extensions=["pdf","png"],name_add=""):
  """ Plots and saves the given histogram.
      Any number of different output formats can be given.
  """
  hist_name = hist.GetName()
  if not name_add == "":
    hist_name += "_" + name_add
    
  fig, ax = plt.subplots(tight_layout=True, figsize=(4,3.5))

  _, bin_edges, _ = plot_hist(ax, hist)
  plot_fit(ax, fit, hist, 300)
  
  ax.set_xlim(bin_edges[0],bin_edges[-1])
  
  ax.set_xlabel(r"$(m_{V} + m_{V})/2$", ha='right', x=1.0)
  ax.set_ylabel("Entries", ha='right', y=1.0)
  
  # Create descriptive text box
  x_rel = 0.7
  y_rel = 0.6
  x_min, x_max = ax.get_xlim()
  y_min, y_max = ax.get_ylim()
  coord_x = x_min + x_rel * (x_max - x_min)
  coord_y = y_min + y_rel * (y_max - y_min)
  
  text = get_plot_text(hist, fit)
  plt.text(coord_x,coord_y,text,clip_on=True)
  
  for extension in extensions:
    fig.savefig("{}/{}.{}".format(output_dir,hist_name,extension))
    plt.close()

# ------------------------------------------------------------------------------