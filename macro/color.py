import sys,ROOT,types,array,os,shutil
import numpy as np

def SetMyPalette( color = 'Bird', alpha = 1.0 ):
    ROOT.gStyle.SetPalette(1)
    NRGBs = 5
    NCont = 999
    ROOT.gStyle.SetNumberContours(NCont)

    ### Nominal ###
    Stops = np.array([0.00, 0.10, 0.50, 0.84, 1.00])
    Red   = np.array([1.00, 0.00, 0.87, 1.00, 0.51])
    Green = np.array([1.00, 0.81, 1.00, 0.20, 0.00])
    Blue  = np.array([1.00, 1.00, 0.12, 0.00, 0.00])
    if 'Bird' in color :
        Stops = np.array([0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000])
        Red   = np.array([0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764])
        Green = np.array([0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832])
        Blue  = np.array([0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539])
        ColorTable = ROOT.TColor.CreateGradientColorTable( 9, Stops, Red, Green, Blue, 100, alpha)
        return
    if 'RB' in color:
        ### Blue & Red ###
        NCont = 999
        Stops = np.array([0.0, 0.1, 0.5, 0.9, 1.0])
        Red   = np.array([0.0, 0.5, 1.0, 1.0, 1.0])
        Blue  = np.array([1.0, 1.0, 1.0, 0.5, 0.0])
        Green = np.array([0.0, 0.5, 1.0, 0.5, 0.0])
        NRGBs =len(Stops)
        ColorTable = ROOT.TColor.CreateGradientColorTable(NRGBs, Stops, Red, Green, Blue, NCont, alpha)
        pass
    if 'AD' in color:
        ### White & ligh Blue & Deep Red ###
        NCont = 999
        Stops = np.array([0.00, 0.2, 0.5, 0.8, 1.00])
        Red   = np.array([1.0, 0.8, 0.0, 1.0, 1.0])
        Green = np.array([1.0, 0.8, 0.0, 0.3, 0.0])
        Blue  = np.array([1.0, 1.0, 1.0, 0.3, 0.0])
        NRGBs =len(Stops)
        ColorTable = ROOT.TColor.CreateGradientColorTable(NRGBs, Stops, Red, Green, Blue, NCont, alpha)
        pass

    ColorTable = ROOT.TColor.CreateGradientColorTable(NRGBs,Stops,Red,Green,Blue,NCont)
    return ColorTable
