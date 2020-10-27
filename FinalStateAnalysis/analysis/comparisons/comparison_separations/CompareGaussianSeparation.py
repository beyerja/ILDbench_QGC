import logging as log
import matplotlib.pyplot as plt
import numpy as np

# Enable ROOT and turn on batch mode so drawn graphs aren't shown
import ROOT
ROOT.gROOT.SetBatch(True)

# Local modules
import DefaultFitSettings as DFS
import SeparationCalculation as SC

# ------------------------------------------------------------------------------

def main():
  """ Function which is executed when calling this script directly.
  """
  log.basicConfig(level=log.INFO)
  
  # Percentage of histgrams considered in Integral
  integral_perc = 100
  
  # Should the fitted histograms be saved
  save_fits = False
  
  # Get the default fit settings 
  settings = DFS.default_settings
  
  # Output directory
  output_dir = "/home/jakob/Documents/DESY/MountPoints/POOLMount/VBS/nunu_hadron/comparisons/comparison_separations"
  output_subdir_highQ2_l5 = "{}/highQ2_l".format(output_dir)
  output_subdir_highQ2_s5 = "{}/highQ2_s".format(output_dir)
  output_subdir_lowQ2_l5 = "{}/lowQ2_l".format(output_dir)
  output_subdir_lowQ2_s5 = "{}/lowQ2_s".format(output_dir)
  
  lowQ2_base = "/home/jakob/Documents/DESY/MountPoints/POOLMount/VBS/nunu_hadron/lowQ2range"
  highQ2_base = "/home/jakob/Documents/DESY/MountPoints/POOLMount/VBS/nunu_hadron/highQ2range"
  
  l5_subdir = "/v02-00-02_l5_o1_v02_output"
  s5_subdir = "/v02-00-02_s5_o1_v02_output"
  
  mm_subdir = "/mjjmjj_plots/_all_TH1Ds.root"
  icns_subdir = "/TJ/TJ_observ_to_icns/_all_TH1Ds.root"
  slds_subdir = "/mjj_vs_SLDecays/_all_TH1Ds.root"
  
  # Following: Extraction of all separation curves (sorry for copy-paste...)
  # Shortcuts used:
  #  rec ... full reconstruction
  #  oc ... overlay cheated
  #  jc ... jets cheated 
  #  bc ... bosons cheated
  #  sld ... semi-leptonic decays cheated
  #  gen ... generator level
  
  # TODO TODO TODO TODO TODO TODO
  # Create some settings helper classes for better functions
  # TODO TODO TODO TODO TODO TODO
  
  # ----------------------------------------------------------------------------
  # ----------------------------------------------------------------------------
  log.info("Calculating separations...")

  ## low-Q^2
  
  ### large
  lowQ2_l5_base = lowQ2_base + l5_subdir
  
  path_lowQ2_l5_mm = lowQ2_l5_base + mm_subdir
  path_lowQ2_l5_icns = lowQ2_l5_base + icns_subdir
  path_lowQ2_l5_slds = lowQ2_l5_base + slds_subdir
  
  sep_lowQ2_l5_rec = SC.get_separation(path_lowQ2_l5_mm, "m_WW_nocuts", "m_ZZ_nocuts", integral_perc, settings["low"]["rec"]["WW"], settings["low"]["rec"]["ZZ"], output_dir=output_subdir_lowQ2_l5, save_fits=save_fits, name_add="rec")
  sep_lowQ2_l5_oc = SC.get_separation(path_lowQ2_l5_icns, "m_WW_cheated_overlay_nocuts", "m_ZZ_cheated_overlay_nocuts", integral_perc, settings["low"]["oc"]["WW"], settings["low"]["oc"]["ZZ"], output_dir=output_subdir_lowQ2_l5, save_fits=save_fits)
  sep_lowQ2_l5_jc = SC.get_separation(path_lowQ2_l5_icns, "m_WW_custom_pairing_nocuts", "m_ZZ_custom_pairing_nocuts", integral_perc, settings["low"]["jc"]["WW"], settings["low"]["jc"]["ZZ"], output_dir=output_subdir_lowQ2_l5, save_fits=save_fits)
  sep_lowQ2_l5_bc = SC.get_separation(path_lowQ2_l5_icns, "m_WW_nocuts", "m_ZZ_nocuts", integral_perc, settings["low"]["bc"]["WW"], settings["low"]["bc"]["ZZ"], output_dir=output_subdir_lowQ2_l5, save_fits=save_fits, name_add="icn")
  sep_lowQ2_l5_sld = SC.get_separation(path_lowQ2_l5_slds, "m_WW_icn_noSLD", "m_ZZ_icn_noSLD", integral_perc, settings["low"]["sld"]["WW"], settings["low"]["sld"]["ZZ"], output_dir=output_subdir_lowQ2_l5, save_fits=save_fits)
  sep_lowQ2_l5_gen = SC.get_separation(path_lowQ2_l5_mm, "m_WW_genlevel", "m_ZZ_genlevel", integral_perc, settings["low"]["gen"]["WW"], settings["low"]["gen"]["ZZ"], output_dir=output_subdir_lowQ2_l5, save_fits=save_fits)
  
  ### small
  lowQ2_s5_base = lowQ2_base + s5_subdir
  
  path_lowQ2_s5_mm = lowQ2_s5_base + mm_subdir
  path_lowQ2_s5_icns = lowQ2_s5_base + icns_subdir
  path_lowQ2_s5_slds = lowQ2_s5_base + slds_subdir
  
  sep_lowQ2_s5_rec = SC.get_separation(path_lowQ2_s5_mm, "m_WW_nocuts", "m_ZZ_nocuts", integral_perc, settings["low"]["rec"]["WW"], settings["low"]["rec"]["ZZ"], output_dir=output_subdir_lowQ2_s5, save_fits=save_fits, name_add="rec")
  sep_lowQ2_s5_oc = SC.get_separation(path_lowQ2_s5_icns, "m_WW_cheated_overlay_nocuts", "m_ZZ_cheated_overlay_nocuts", integral_perc, settings["low"]["oc"]["WW"], settings["low"]["oc"]["ZZ"], output_dir=output_subdir_lowQ2_s5, save_fits=save_fits)
  sep_lowQ2_s5_jc = SC.get_separation(path_lowQ2_s5_icns, "m_WW_custom_pairing_nocuts", "m_ZZ_custom_pairing_nocuts", integral_perc, settings["low"]["jc"]["WW"], settings["low"]["jc"]["ZZ"], output_dir=output_subdir_lowQ2_s5, save_fits=save_fits)
  sep_lowQ2_s5_bc = SC.get_separation(path_lowQ2_s5_icns, "m_WW_nocuts", "m_ZZ_nocuts", integral_perc, settings["low"]["bc"]["WW"], settings["low"]["bc"]["ZZ"], output_dir=output_subdir_lowQ2_s5, save_fits=save_fits, name_add="icn")
  sep_lowQ2_s5_sld = SC.get_separation(path_lowQ2_s5_slds, "m_WW_icn_noSLD", "m_ZZ_icn_noSLD", integral_perc, settings["low"]["sld"]["WW"], settings["low"]["sld"]["ZZ"], output_dir=output_subdir_lowQ2_s5, save_fits=save_fits)
  sep_lowQ2_s5_gen = SC.get_separation(path_lowQ2_s5_mm, "m_WW_genlevel", "m_ZZ_genlevel", integral_perc, settings["low"]["gen"]["WW"], settings["low"]["gen"]["ZZ"], output_dir=output_subdir_lowQ2_s5, save_fits=save_fits)
  
  ## high-Q^2
  
  ### large
  
  highQ2_l5_base = highQ2_base + l5_subdir
  
  path_highQ2_l5_mm = highQ2_l5_base + mm_subdir
  path_highQ2_l5_icns = highQ2_l5_base + icns_subdir
  path_highQ2_l5_slds = highQ2_l5_base + slds_subdir
  
  sep_highQ2_l5_rec = SC.get_separation(path_highQ2_l5_mm, "m_WW_nocuts", "m_ZZ_nocuts", integral_perc, settings["high"]["rec"]["WW"], settings["high"]["rec"]["ZZ"], output_dir=output_subdir_highQ2_l5, save_fits=save_fits, name_add="rec")
  sep_highQ2_l5_oc = SC.get_separation(path_highQ2_l5_icns, "m_WW_cheated_overlay_nocuts", "m_ZZ_cheated_overlay_nocuts", integral_perc, settings["high"]["oc"]["WW"], settings["high"]["oc"]["ZZ"], output_dir=output_subdir_highQ2_l5, save_fits=save_fits)
  sep_highQ2_l5_jc = SC.get_separation(path_highQ2_l5_icns, "m_WW_custom_pairing_nocuts", "m_ZZ_custom_pairing_nocuts", integral_perc, settings["high"]["jc"]["WW"], settings["high"]["jc"]["ZZ"], output_dir=output_subdir_highQ2_l5, save_fits=save_fits)
  sep_highQ2_l5_bc = SC.get_separation(path_highQ2_l5_icns, "m_WW_nocuts", "m_ZZ_nocuts", integral_perc, settings["high"]["bc"]["WW"], settings["high"]["bc"]["ZZ"], output_dir=output_subdir_highQ2_l5, save_fits=save_fits, name_add="icn")
  sep_highQ2_l5_sld = SC.get_separation(path_highQ2_l5_slds, "m_WW_icn_noSLD", "m_ZZ_icn_noSLD", integral_perc, settings["high"]["sld"]["WW"], settings["high"]["sld"]["ZZ"], output_dir=output_subdir_highQ2_l5, save_fits=save_fits)
  sep_highQ2_l5_gen = SC.get_separation(path_highQ2_l5_mm, "m_WW_genlevel", "m_ZZ_genlevel", integral_perc, settings["high"]["gen"]["WW"], settings["high"]["gen"]["ZZ"], output_dir=output_subdir_highQ2_l5, save_fits=save_fits)
  
  ### small
  highQ2_s5_base = highQ2_base + s5_subdir
  
  path_highQ2_s5_mm = highQ2_s5_base + mm_subdir
  path_highQ2_s5_icns = highQ2_s5_base + icns_subdir
  path_highQ2_s5_slds = highQ2_s5_base + slds_subdir
  
  sep_highQ2_s5_rec = SC.get_separation(path_highQ2_s5_mm, "m_WW_nocuts", "m_ZZ_nocuts", integral_perc, settings["high"]["rec"]["WW"], settings["high"]["rec"]["ZZ"], output_dir=output_subdir_highQ2_s5, save_fits=save_fits, name_add="rec")
  sep_highQ2_s5_oc = SC.get_separation(path_highQ2_s5_icns, "m_WW_cheated_overlay_nocuts", "m_ZZ_cheated_overlay_nocuts", integral_perc, settings["high"]["oc"]["WW"], settings["high"]["oc"]["ZZ"], output_dir=output_subdir_highQ2_s5, save_fits=save_fits)
  sep_highQ2_s5_jc = SC.get_separation(path_highQ2_s5_icns, "m_WW_custom_pairing_nocuts", "m_ZZ_custom_pairing_nocuts", integral_perc, settings["high"]["jc"]["WW"], settings["high"]["jc"]["ZZ"], output_dir=output_subdir_highQ2_s5, save_fits=save_fits)
  sep_highQ2_s5_bc = SC.get_separation(path_highQ2_s5_icns, "m_WW_nocuts", "m_ZZ_nocuts", integral_perc, settings["high"]["bc"]["WW"], settings["high"]["bc"]["ZZ"], output_dir=output_subdir_highQ2_s5, save_fits=save_fits, name_add="icn")
  sep_highQ2_s5_sld = SC.get_separation(path_highQ2_s5_slds, "m_WW_icn_noSLD", "m_ZZ_icn_noSLD", integral_perc, settings["high"]["sld"]["WW"], settings["high"]["sld"]["ZZ"], output_dir=output_subdir_highQ2_s5, save_fits=save_fits)
  sep_highQ2_s5_gen = SC.get_separation(path_highQ2_s5_mm, "m_WW_genlevel", "m_ZZ_genlevel", integral_perc, settings["high"]["gen"]["WW"], settings["high"]["gen"]["ZZ"], output_dir=output_subdir_highQ2_s5, save_fits=save_fits)
  
  
  # ----------------------------------------------------------------------------
  # ----------------------------------------------------------------------------
  log.info("Plotting...")
  
  # Colors and markers
  lowQ2_color = [153./256., 142./256., 195./256.]
  highQ2_color = [241./256., 163./256., 64./256.]
  l_marker = "o"
  s_marker = "v"

  # Get the data ready and choose appropriate x axis labels
  n_points = 6
  x = np.arange(n_points)
  y_lowQ2_l5 = np.array([ sep_lowQ2_l5_rec, sep_lowQ2_l5_oc, sep_lowQ2_l5_jc, sep_lowQ2_l5_bc, sep_lowQ2_l5_sld, sep_lowQ2_l5_gen ])
  y_lowQ2_s5 = np.array([ sep_lowQ2_s5_rec, sep_lowQ2_s5_oc, sep_lowQ2_s5_jc, sep_lowQ2_s5_bc, sep_lowQ2_s5_sld, sep_lowQ2_s5_gen ])
  y_highQ2_l5 = np.array([ sep_highQ2_l5_rec, sep_highQ2_l5_oc, sep_highQ2_l5_jc, sep_highQ2_l5_bc, sep_highQ2_l5_sld, sep_highQ2_l5_gen ])
  y_highQ2_s5 = np.array([ sep_highQ2_s5_rec, sep_highQ2_s5_oc, sep_highQ2_s5_jc, sep_highQ2_s5_bc, sep_highQ2_s5_sld, sep_highQ2_s5_gen ])
  x_labels = ["Full rec.", "+ Cheat beam bkg's", "+ Cheat jet clustering", "+ Cheat boson pairing", "+ Remove SLDs", "Generator level"]
  
  # Plot
  fig, ax = plt.subplots(tight_layout=True, figsize=(4,4))
  ax.plot(x, y_lowQ2_l5, color=lowQ2_color, marker=l_marker, label=r"$m_{VV}<500$GeV, IDR-L", linestyle="None", mfc="None", ms=9, mew=1.5)
  ax.plot(x, y_lowQ2_s5, color=lowQ2_color, marker=s_marker, label=r"$m_{VV}<500$GeV, IDR-S", linestyle="None", mfc="None", ms=9, mew=1.5)
  ax.plot(x, y_highQ2_l5, color=highQ2_color, marker=l_marker, label=r"$m_{VV}>500$GeV, IDR-L", linestyle="None", mfc="None", ms=9, mew=1.5)
  ax.plot(x, y_highQ2_s5, color=highQ2_color, marker=s_marker, label=r"$m_{VV}>500$GeV, IDR-S", linestyle="None", mfc="None", ms=9, mew=1.5)
  
  # Set reasonable y limits and label
  ax.set_ylim(0.0, ax.get_ylim()[1])
  # ax.set_ylabel(r"Separation$_{{{}}}$".format(integral_perc))
  ax.set_ylabel("Separation")
  
  # Properly label the x axis
  # We want to show all ticks...
  ax.set_xticks(np.arange(n_points))
  # ... and label them with the respective list entries
  ax.set_xticklabels(x_labels)
  # Rotate the tick labels and set their alignment.
  plt.setp(ax.get_xticklabels(),rotation=45,ha="right",rotation_mode="anchor")
  
  # Show the legend
  # ax.legend(loc=0)
  ax.legend(loc=0, title=r"Fit: $f*G_{1}+(1-f)*G_{2}$")
  
  # Save the figure
  fig.savefig("{}/separations.pdf".format(output_dir))
  plt.close(fig)
  
  log.info("Done!")

# ------------------------------------------------------------------------------

if __name__ == "__main__":
  main()
  