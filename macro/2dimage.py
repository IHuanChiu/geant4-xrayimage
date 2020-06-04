#!/usr/bin/env python    
#-*- coding:utf-8 -*-   
"""
This module provides the transformation from adc to energy.
"""
__author__    = "I-Huan CHIU"
__email__     = "ichiu@chem.sci.osaka-u.ac.jp"
__created__   = "2019-11-08"
__copyright__ = "Copyright 2019 I-Huan CHIU"
__license__   = "GPL http://www.gnu.org/licenses/gpl.html"

import sys,os,random,math,ROOT
from ROOT import TFile, TTree, TCut
from ROOT import gROOT, AddressOf, gPad, gDirectory
ROOT.gROOT.SetBatch(1)
import argparse
import math
from color import SetMyPalette

__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()

def createRatioCanvas(Name = "cs", w = 1200, h = 800):
    cRatioCanvas = ROOT.TCanvas(Name,"",0,0,int(w),int(h))
    cRatioCanvas.GetFrame().SetBorderMode(0)
    cRatioCanvas.GetFrame().SetBorderSize(0)
    cRatioCanvas.SetBorderMode(0)
    cRatioCanvas.SetBorderSize(0)
    cRatioCanvas.SetFillStyle(0)
    cRatioCanvas.SetFillColor(0)
    cRatioCanvas.SetRightMargin(0.15)
    cRatioCanvas.SetWindowSize( int(w + (w-cRatioCanvas.GetWw())), int(h + (h-cRatioCanvas.GetWh())) )
    return cRatioCanvas

def plot(args):
    f = ROOT.TFile(args.input)   
    tree = f.Get("tree")
    emin,emax = 70,75 #keV
    emin2,emax2 = 10,15 #keV
    cv  = createRatioCanvas("cv", 2400, 1600)

    cv.Divide(3,2)
    SetMyPalette("AD")     
    for i in range(6):
       cv.cd(i+1).SetRightMargin(0.18)
       ecut = TCut("Position_x[{0}] != -1000 && Position_z[{0}]!= -1000 && ((energyDep[{0}]*1000 > {1} && energyDep[{0}]*1000 < {2}) || (energyDep[{0}]*1000 > {3} && energyDep[{0}]*1000 < {4}))".format(i,emin,emax, emin2, emax2))
       tree.Draw("Position_x[{0}]:(Position_z[{0}]-30.5) >> h_{0}(64,-16,16,64,-16,16)".format(i),ecut,"")
       hist = gDirectory.Get("h_{0}".format(i))
       hist.GetXaxis().SetTitle("X[mm]")
       hist.GetYaxis().SetTitle("Y[mm]")
       hist.Draw("colz")
    cv.Print("./figs/quick_2dimage.pdf")

    cv2  = createRatioCanvas("cv2", 2400, 1600)
    cv2.Divide(3,2)
    for i in range(6):
       cv2.cd(i+1).SetRightMargin(0.1)
       ecut2 = TCut("energyDep[{0}]*1000 != 0".format(i))
       tree.Draw("energyDep[{0}]*1000 >> he_{0}(200,1,101)".format(i),ecut2,"")
       hist = gDirectory.Get("he_{0}".format(i))
       hist.GetXaxis().SetTitle("energy [keV]")
       hist.GetYaxis().SetTitle("Counts")
       hist.SetDirectory(0)
       hist.Draw("Hist")
       gPad.SetLogy()
    cv2.Print("./figs/quick_energy.pdf")


if __name__ == '__main__' : 
  parser = argparse.ArgumentParser(description='Process some integers.')
  parser.add_argument("input", type=str, default="./20151112_00009_001.root", help="Input File Name")
  args = parser.parse_args()

  plot( args )

