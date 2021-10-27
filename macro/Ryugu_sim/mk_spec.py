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

__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
#ROOT.SetAtlasStyle()

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
    fout = ROOT.TFile("./cv_output.root","recreate")

    fc_ba=ROOT.TFile("./comptonsoft_data/ri_ba133_simulation_specfile.root","read")
    fc_co=ROOT.TFile("./comptonsoft_data/ri_co57_simulation_specfile.root","read")
    fc_eu=ROOT.TFile("./comptonsoft_data/ri_eu152_simulation_specfile.root","read")
    
    f_ba=ROOT.TFile("./pha_data/203297_self_Ba.root","read")
    f_co=ROOT.TFile("./pha_data/203296_self_Co.root","read") 
    f_eu=ROOT.TFile("./pha_data/203299_self_Eu.root","read")
    t_ba=f_ba.Get("tree")
    t_co=f_co.Get("tree")
    t_eu=f_eu.Get("tree")
    
    fg_ba=ROOT.TFile("./G4_data/Output_Ba133.root","read")
    fg_co=ROOT.TFile("./G4_data/Output_Co57.root","read")
    fg_eu=ROOT.TFile("./G4_data/Output_Eu152.root","read")
    tg_ba=fg_ba.Get("tree")
    tg_co=fg_co.Get("tree")
    tg_eu=fg_eu.Get("tree")

    cv0  = createRatioCanvas("cv0", 800, 800)
    cv1  = createRatioCanvas("cv1", 2500, 800)
    cv2  = createRatioCanvas("cv2", 2500, 800)
    cv3  = createRatioCanvas("cv3", 2500, 800)
    cv1.Divide(3,2)
    cv2.Divide(3,2)
    cv3.Divide(3,2)

    fout.cd()
    for ich in range(6):
       if ich+1 < 6:
          _index = 6-(ich+1)
       else: 
          _index = 6
       hc_ba=fc_ba.Get("spec_det{}".format(_index))
       hc_co=fc_co.Get("spec_det{}".format(_index))
       hc_eu=fc_eu.Get("spec_det{}".format(_index))

       cv0.cd()
       t_ba.Draw("energy_ori >> h_ba(6800,10,180)","detID == {}".format(ich+1),"") 
       t_co.Draw("energy_ori >> h_co(6800,10,180)","detID == {}".format(ich+1),"") 
       t_eu.Draw("energy_ori >> h_eu(6800,10,180)","detID == {}".format(ich+1),"") 
       h_ba=gDirectory.Get("h_ba")
       h_co=gDirectory.Get("h_co")
       h_eu=gDirectory.Get("h_eu")

       tg_ba.Draw("Hit_Energy_Reso*1000 >> hg_ba(6800,10,180)","Det_ID == {}".format(ich+1),"")
       tg_co.Draw("Hit_Energy_Reso*1000 >> hg_co(6800,10,180)","Det_ID == {}".format(ich+1),"")
       tg_eu.Draw("Hit_Energy_Reso*1000 >> hg_eu(6800,10,180)","Det_ID == {}".format(ich+1),"")
       hg_ba=gDirectory.Get("hg_ba")
       hg_co=gDirectory.Get("hg_co")
       hg_eu=gDirectory.Get("hg_eu")

       h_ba.SetTitle("Data Ba CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       h_co.SetTitle("Data Co CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       h_eu.SetTitle("Data Eu CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       h_ba.GetXaxis().CenterTitle()
       h_co.GetXaxis().CenterTitle()
       h_eu.GetXaxis().CenterTitle()
       hg_ba.SetTitle("G4 Ba CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       hg_co.SetTitle("G4 Co CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       hg_eu.SetTitle("G4 Eu CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       hg_ba.GetXaxis().CenterTitle()
       hg_co.GetXaxis().CenterTitle()
       hg_eu.GetXaxis().CenterTitle()
       hc_ba.SetTitle("ComptonSoft Ba CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       hc_co.SetTitle("ComptonSoft Co CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       hc_eu.SetTitle("ComptonSoft Eu CH{};Energy[keV];Counts/0.025keV".format(ich+1))
       hc_ba.GetXaxis().CenterTitle()
       h_co.GetXaxis().CenterTitle()
       h_eu.GetXaxis().CenterTitle()
       h_ba.SetStats(0)
       h_co.SetStats(0)
       h_eu.SetStats(0)

       h_ba.SetLineColor(1)
       h_co.SetLineColor(1)
       h_eu.SetLineColor(1)
       hc_ba.SetLineColor(2)
       hc_co.SetLineColor(2)
       hc_eu.SetLineColor(2)
       hg_ba.SetLineColor(4)
       hg_co.SetLineColor(4)
       hg_eu.SetLineColor(4)
       #hc_ba.SetLineColorAlpha(2,0.7)
       #hc_co.SetLineColorAlpha(2,0.7)
       #hc_eu.SetLineColorAlpha(2,0.7)
       #hg_ba.SetLineColorAlpha(4,0.7)
       #hg_co.SetLineColorAlpha(4,0.7)
       #hg_eu.SetLineColorAlpha(4,0.7)
       
       leg=ROOT.TLegend(.60,.70,.80,.85)
       leg.AddEntry(h_ba,  "Data", "l");
       leg.AddEntry(hc_ba,  "ComptonSoft", "l");
       leg.AddEntry(hg_ba,   "Geant4",   "l");
       cv1.cd(ich+1)
       gPad.SetLogy(1);
       h_ba.DrawNormalized()
       hc_ba.DrawNormalized("same")
       hg_ba.DrawNormalized("same")
       leg.Draw("same");

       cv2.cd(ich+1)
       gPad.SetLogy(1);
       h_co.DrawNormalized()
       hc_co.DrawNormalized("same")
       hg_co.DrawNormalized("same")
       leg.Draw("same");

       cv3.cd(ich+1)
       gPad.SetLogy(1);
       h_eu.DrawNormalized()
       hc_eu.DrawNormalized("same")
       hg_eu.DrawNormalized("same")
       leg.Draw("same");

       h_ba.Write()
       h_co.Write()
       h_eu.Write()
       hg_ba.Write()
       hg_co.Write()
       hg_eu.Write()
       hc_ba.Write()
       hc_co.Write()
       hc_eu.Write()

    cv1.SaveAs("/Users/chiu.i-huan/Desktop/hist_ba.pdf")
    cv2.SaveAs("/Users/chiu.i-huan/Desktop/hist_co.pdf")
    cv3.SaveAs("/Users/chiu.i-huan/Desktop/hist_eu.pdf")
    cv1.Write()
    cv2.Write()
    cv3.Write()
    fout.Write()

if __name__ == '__main__' :
  parser = argparse.ArgumentParser(description='Process some integers.') 
  parser.add_argument("--input", type=str, default="./root/20151112_00009_001.root", help="Input File Name")
  args = parser.parse_args()

  plot( args )

