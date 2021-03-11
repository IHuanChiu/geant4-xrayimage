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

def createRatioCanvas(Name = "cs", w = 1000, h = 800):
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

    cv  = createRatioCanvas("cv", 1500, 800)

    ecut_s = TCut("Stop_VolumeID == {}".format(3))#sample
    ecut_b = TCut("Stop_VolumeID == {}".format(4))#Al
    tree.Draw("Hit_Energy_Reso*1000 >> h_a(600,0,150)","","")
    tree.Draw("Hit_Energy_Reso*1000 >> h_s(600,0,150)",ecut_s,"")
    tree.Draw("Hit_Energy_Reso*1000 >> h_b(600,0,150)",ecut_b,"")
    h_a, h_s, h_b=gDirectory.Get("h_a"), gDirectory.Get("h_s"), gDirectory.Get("h_b")
    h_a.GetXaxis().SetTitle("Energy[kev]")
    h_a.GetYaxis().SetTitle("Counts (/0.25keV)")
    h_a.SetLineColor(1)
    h_s.SetLineColor(2)
    h_b.SetLineColor(4)
    h_a.Draw()
    h_b.Draw("same")
    h_s.Draw("same")
    leg=ROOT.TLegend(.60,.70,.80,.85)
    leg.AddEntry(h_a,  "All", "l");
    leg.AddEntry(h_s,  "Signal", "l");
    leg.AddEntry(h_b,   "Bkg.",   "l");
    leg.Draw("same");
   
    name=args.input 
    name=name.replace(".root","_energy.pdf")
    name=name.replace("root/Output_","figs/Spec_")   
    cv.Print(name)

if __name__ == '__main__' : 
  parser = argparse.ArgumentParser(description='Process some integers.')
  parser.add_argument("input", type=str, default="./root/20151112_00009_001.root", help="Input File Name")
  args = parser.parse_args()

  plot( args )

