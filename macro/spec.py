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

def comparison(args):
    cut_signal = TCut("((x < 5 && x > -5) && (y < 11 && y > -7))")
    cut_basic = TCut("((trigger > 235 && trigger < 240) || (trigger > 247 && trigger < 253))")
    UTcut = TCut("((unixtime > 1583663336 && unixtime < 1583663640) || (unixtime > 1583665785 && unixtime < 1583668072) || (unixtime > 1583670126 && unixtime < 1583728926) || (unixtime > 1583797615 && unixtime < 1583807420) || (unixtime > 1583808902 && unixtime < 1583823904) || (unixtime > 1583825103 && unixtime < 1583837643) || (unixtime > 1583838416 && unixtime < 1583846500) || (unixtime > 1583847476 && unixtime < 1583872201))")
    ecut_s = TCut("Stop_VolumeID == {}".format(3))#sample

    f = ROOT.TFile(args.input)
    f_data = ROOT.TFile("/Users/chiu.i-huan/Desktop/new_scientific/imageAna/run/root/JPARC2020March_CdTe_sum.root","read") # paper result
    tree = f.Get("tree")
    mytree = f_data.Get("tree")

    tree.Draw("Hit_Energy_Reso*1000 >> h_s(520,10,140)",ecut_s,"")
    mytree.Draw("energy >> h_all_s(520,10,140)",cut_basic+UTcut+cut_signal,"")

    h_mc, h_data = gDirectory.Get("h_s"), gDirectory.Get("h_all_s")
    n_max_mc, n_max_data = h_mc.GetMaximum(), h_data.GetMaximum()
    h_mc.SetTitle(";Energy [keV];Normalized (/0.25keV)")
    h_mc.GetXaxis().CenterTitle(); h_mc.GetYaxis().CenterTitle();
    h_mc.SetLineColor(4)
    h_data.SetLineColor(2)

    cv  = createRatioCanvas("cv", 1200, 800)
    print("Ratio for scale : {:.2f}".format(n_max_data/n_max_mc))
    h_mc.Scale(n_max_data/n_max_mc)
    h_data.Draw("hist")
    h_mc.Draw("hist same")
#    h_mc.DrawNormalized("hist")
#    h_data.DrawNormalized("hist same")
    leg=ROOT.TLegend(.60,.65,.80,.85)
    leg.AddEntry(h_mc,  "MC", "l");
    leg.AddEntry(h_data,  "Data", "l");
    leg.Draw("same");
    cv.Print("/Users/chiu.i-huan/Desktop/Output_mc_vs_data.pdf")
    cv.Print("/Users/chiu.i-huan/Desktop/Output_mc_vs_data.png")
    

def plot(args):
    f = ROOT.TFile(args.input)   
    tree = f.Get("tree")

    cv  = createRatioCanvas("cv", 1200, 800)

    ecut_s = TCut("Stop_VolumeID == {}".format(3))#sample
    ecut_b = TCut("Stop_VolumeID == {}".format(4))#Al
    tree.Draw("Hit_Energy_Reso*1000 >> h_a(520,10,140)","","")
    tree.Draw("Hit_Energy_Reso*1000 >> h_s(520,10,140)",ecut_s,"")
    tree.Draw("Hit_Energy_Reso*1000 >> h_b(520,10,140)",ecut_b,"")
    h_a, h_s, h_b=gDirectory.Get("h_a"), gDirectory.Get("h_s"), gDirectory.Get("h_b")
    h_a.SetTitle(";Energy [keV];Counts (/0.25keV)")
    h_a.GetXaxis().CenterTitle(); h_a.GetYaxis().CenterTitle();
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

#  plot( args )
  comparison( args )

