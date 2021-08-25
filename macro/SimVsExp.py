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
    standname = "/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/data/StandardSample/"
    f = ROOT.TFile(args.input)  
    fstand = ROOT.TFile(standname+args.atom+".root")
    tree = f.Get("tree")
    treestand = fstand.Get("tree")
    nbins = 1600
    maxenergy = 200 #keV

    ScaleType = "MaxBin"
    cut = TCut("1") # Stop_VolumeID = 6 is specified signal from sample
#    cut = TCut("Stop_VolumeID == 6") # Stop_VolumeID = 6 is specified signal from sample

#    ScaleType = "Nor"
#    cut = TCut("1") 

#    ScaleType = "Nmuon"
#    cut = TCut("1")
#    #muonStopNumberData=3183024.6#Fe(4-3)
#    muonStopNumberData=6897255.3#Al(3-2)
#    h0_test=f.Get("h1_StopVol")
#    muonStopNumberSim=h0_test.GetBinContent(7)# input muon stopping in Sample
#    print("Rate (Data/MC):",muonStopNumberData/muonStopNumberSim)

    treestand.Draw("energy >> h0_stand({0},0,{1})".format(nbins,maxenergy))
    tree.Draw("Hit_Energy_Reso*1000 >> h0_sim({0},0,{1})".format(nbins,maxenergy),cut,"")
    h0_stand, h0_sim = gDirectory.Get("h0_stand"), gDirectory.Get("h0_sim")
    h0_sim.GetXaxis().SetTitle("Energy[kev]")
    h0_sim.GetYaxis().SetTitle("Counts (/0.25keV)")

    stand_scale, sim_scale = 0., 0.
    if ScaleType == "MaxBin":
       stand_scale, sim_scale = 1./h0_stand.GetMaximum(), 1./h0_sim.GetMaximum()# scale with MaxBin
    elif ScaleType == "Nmuon": 
       stand_scale, sim_scale = 1., (muonStopNumberData/muonStopNumberSim)# scale with MaxBin
    else: 
       stand_scale, sim_scale = 1./h0_stand.GetEntries(), 1./h0_sim.GetEntries()# scale with MaxBin
    h0_stand.Scale(stand_scale)
    h0_sim.Scale(sim_scale)

    if h0_stand.GetMaximum() > h0_sim.GetMaximum():
       h0_stand.SetMaximum(h0_stand.GetMaximum()*1.15)  
    else: 
       h0_stand.SetMaximum(h0_sim.GetMaximum()*1.15)

    cv  = createRatioCanvas("cv", 1600, 1200)
    h0_sim.SetLineColorAlpha(2,0.7)
    h0_stand.SetLineColorAlpha(1,0.9)
    h0_stand.SetMarkerColor(1)
    if ScaleType == "Nor":
       h0_stand.DrawNormalized("hist")
       h0_sim.DrawNormalized("same hist")
    else:
       h0_stand.Draw("hist")
       h0_sim.Draw("same hist")
    leg=ROOT.TLegend(.60,.70,.80,.85)
    leg.AddEntry(h0_stand,  "Exp.", "l");
    leg.AddEntry(h0_sim,  "Sim.", "l");
    leg.Draw("same");

    name=args.input 
    name=name.replace(".root","_SIMvsEXP.pdf")
    cv.Print(name)

    # === Separation ===
    #stand_scale, sim_scale = stand_scale*2, sim_scale*2
    treestand.Draw("energy >> h0_stand_0({0},{1},{2})".format(nbins/4,maxenergy*0/4,maxenergy*1/4))
    tree.Draw("Hit_Energy_Reso*1000 >> h0_sim_0({0},{1},{2})".format(nbins/4,maxenergy*0/4,maxenergy*1/4),cut,"") 
    h0_stand_0, h0_sim_0 = gDirectory.Get("h0_stand_0"), gDirectory.Get("h0_sim_0")
    treestand.Draw("energy >> h0_stand_1({0},{1},{2})".format(nbins/4,maxenergy*1/4,maxenergy*2/4))
    tree.Draw("Hit_Energy_Reso*1000 >> h0_sim_1({0},{1},{2})".format(nbins/4,maxenergy*1/4,maxenergy*2/4),cut,"") 
    h0_stand_1, h0_sim_1 = gDirectory.Get("h0_stand_1"), gDirectory.Get("h0_sim_1")
    treestand.Draw("energy >> h0_stand_2({0},{1},{2})".format(nbins/4,maxenergy*2/4,maxenergy*3/4))
    tree.Draw("Hit_Energy_Reso*1000 >> h0_sim_2({0},{1},{2})".format(nbins/4,maxenergy*2/4,maxenergy*3/4),cut,"") 
    h0_stand_2, h0_sim_2 = gDirectory.Get("h0_stand_2"), gDirectory.Get("h0_sim_2")
    treestand.Draw("energy >> h0_stand_3({0},{1},{2})".format(nbins/4,maxenergy*3/4,maxenergy*4/4))
    tree.Draw("Hit_Energy_Reso*1000 >> h0_sim_3({0},{1},{2})".format(nbins/4,maxenergy*3/4,maxenergy*4/4),cut,"") 
    h0_stand_3, h0_sim_3 = gDirectory.Get("h0_stand_3"), gDirectory.Get("h0_sim_3")


    h0_stand_0.GetXaxis().SetTitle("Energy[kev]")
    h0_stand_1.GetXaxis().SetTitle("Energy[kev]")
    h0_stand_2.GetXaxis().SetTitle("Energy[kev]")
    h0_stand_3.GetXaxis().SetTitle("Energy[kev]")
    h0_stand_0.GetYaxis().SetTitle("Counts (/0.25keV)")
    h0_stand_1.GetYaxis().SetTitle("Counts (/0.25keV)")
    h0_stand_2.GetYaxis().SetTitle("Counts (/0.25keV)")
    h0_stand_3.GetYaxis().SetTitle("Counts (/0.25keV)")
    
    h0_stand_0.Scale(stand_scale)
    h0_stand_1.Scale(stand_scale)
    h0_stand_2.Scale(stand_scale)
    h0_stand_3.Scale(stand_scale)
    h0_sim_0.Scale(sim_scale)
    h0_sim_1.Scale(sim_scale)
    h0_sim_2.Scale(sim_scale)
    h0_sim_3.Scale(sim_scale)
    if h0_stand_0.GetMaximum() > h0_sim_0.GetMaximum(): h0_stand_0.SetMaximum(h0_stand_0.GetMaximum()*1.2)  
    else: h0_stand_0.SetMaximum(h0_sim_0.GetMaximum()*1.2)
    if h0_stand_1.GetMaximum() > h0_sim_1.GetMaximum(): h0_stand_1.SetMaximum(h0_stand_1.GetMaximum()*1.2)  
    else: h0_stand_1.SetMaximum(h0_sim_1.GetMaximum()*1.2)
    if h0_stand_2.GetMaximum() > h0_sim_2.GetMaximum(): h0_stand_2.SetMaximum(h0_stand_2.GetMaximum()*1.2)  
    else: h0_stand_2.SetMaximum(h0_sim_2.GetMaximum()*1.2)
    if h0_stand_3.GetMaximum() > h0_sim_3.GetMaximum(): h0_stand_3.SetMaximum(h0_stand_3.GetMaximum()*1.2)  
    else: h0_stand_3.SetMaximum(h0_sim_3.GetMaximum()*1.2)

    h0_sim_0.SetLineColorAlpha(2,0.7)
    h0_sim_1.SetLineColorAlpha(2,0.7)
    h0_sim_2.SetLineColorAlpha(2,0.7)
    h0_sim_3.SetLineColorAlpha(2,0.7)
    h0_stand_0.SetLineColorAlpha(1,0.9)
    h0_stand_1.SetLineColorAlpha(1,0.9)
    h0_stand_2.SetLineColorAlpha(1,0.9)
    h0_stand_3.SetLineColorAlpha(1,0.9)

    cv2  = createRatioCanvas("cv2", 2400, 1600)
    cv2.Divide(2,2)
    if ScaleType == "Nor":
       cv2.cd(1)
       h0_stand_0.DrawNormalized("hist")
       h0_sim_0.DrawNormalized("same hist")
       leg.Draw("same");
       cv2.cd(2)
       h0_stand_1.DrawNormalized("hist")
       h0_sim_1.DrawNormalized("same hist")
       leg.Draw("same");
       cv2.cd(3)
       h0_stand_2.DrawNormalized("hist")
       h0_sim_2.DrawNormalized("same hist")
       leg.Draw("same");
       cv2.cd(4)
       h0_stand_3.DrawNormalized("hist")
       h0_sim_3.DrawNormalized("same hist")
       leg.Draw("same");
    else:
       cv2.cd(1)
       h0_stand_0.Draw("hist")
       h0_sim_0.Draw("same hist")
       leg.Draw("same");
       cv2.cd(2)
       h0_stand_1.Draw("hist")
       h0_sim_1.Draw("same hist")
       leg.Draw("same");
       cv2.cd(3)
       h0_stand_2.Draw("hist")
       h0_sim_2.Draw("same hist")
       leg.Draw("same");
       cv2.cd(4)
       h0_stand_3.Draw("hist")
       h0_sim_3.Draw("same hist")
       leg.Draw("same");

    name=name.replace("_SIMvsEXP.pdf","_SIMvsEXP_sep.pdf")
    cv2.Print(name)

if __name__ == '__main__' : 
  parser = argparse.ArgumentParser(description='Process some integers.')
  parser.add_argument("input", type=str, default="./20151112_00009_001.root", help="Input File Name")
  parser.add_argument("-a", "--atom", type=str, default = "Al", help="Al or Fe or Ti" )
  args = parser.parse_args()

  plot( args )

