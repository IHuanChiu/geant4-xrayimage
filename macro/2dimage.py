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
from ROOT import gROOT, AddressOf, gPad, gDirectory, gStyle
from root_numpy import hist2array, array2hist, tree2array
import numpy as np
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
    nbin=32
    if tree.GetEntries() > 500000: nbin=128
    elif tree.GetEntries() > 100000: nbin=64
    else:nbin=32

    ecut_base=TCut("Hit_y < {}".format(-180))
    ecut_s = TCut("Stop_VolumeID == {}".format(3))#sample
    ecut_b = TCut("Stop_VolumeID == {}".format(4))#Al
   
    tree.Draw("Hit_z:Hit_x >> h_s({0},-16,16,{1},-16,16)".format(nbin,nbin),ecut_base+ecut_s,"")
    tree.Draw("Hit_z:Hit_x >> h_b({0},-16,16,{1},-16,16)".format(nbin,nbin),ecut_base+ecut_b,"")
    _h_s,_h_b=gDirectory.Get("h_s"),gDirectory.Get("h_b")

    h_s=_h_s.Clone()
    _array=hist2array(_h_s)
    _array[np.where(_array == 0)]=0.001
    array2hist(_array,h_s)
    h_b=_h_b.Clone()
    _arrayb=hist2array(_h_b)
    _arrayb[np.where(_arrayb == 0)]=0.001
    array2hist(_arrayb,h_b)

    h_s.GetXaxis().SetTitle("X[mm]")
    h_s.GetYaxis().SetTitle("Y[mm]")
    h_b.GetXaxis().SetTitle("X[mm]")
    h_b.GetYaxis().SetTitle("Y[mm]")

    cvs  = createRatioCanvas("cvs", 1200, 1000)
#    SetMyPalette("AD")
    gStyle.SetPalette(62)
    h_s.Draw("colz")
    name=args.input 
    name=name.replace(".root","_2d_s.pdf")
    name=name.replace("root/Output_","figs/Image_")   
    cvs.Print(name)

    cvb  = createRatioCanvas("cvb", 1200, 1000)
#    SetMyPalette("AD")
    gStyle.SetPalette(62)
    h_b.Draw("colz")
    name=name.replace("_2d_s.pdf","_2d_b.pdf")
    cvb.Print(name)
    
if __name__ == '__main__' : 
  parser = argparse.ArgumentParser(description='Process some integers.')
  parser.add_argument("input", type=str, default="./20151112_00009_001.root", help="Input File Name")
  args = parser.parse_args()

  plot( args )

