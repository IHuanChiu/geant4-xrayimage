#!/usr/bin/env python    
#-*- coding:utf-8 -*-   
"""
This module provides the transformation from adc to energy.
"""
__author__    = "I-Huan CHIU"
__email__     = "ichiu@rirc.osaka-u.ac.jp"
__created__   = "2021-11-02"
__copyright__ = "Copyright 2021 I-Huan CHIU"
__license__   = "GPL http://www.gnu.org/licenses/gpl.html"

import sys,os,random,math,time,ROOT
from ROOT import TFile, TTree, TCut
from ROOT import gROOT, AddressOf, gPad, gDirectory
ROOT.gROOT.SetBatch(1)
import argparse
import math
sys.path.append('/Users/chiu.i-huan/Desktop/new_scientific/imageAna/macro/')
sys.path.append('/Users/chiu.i-huan/Desktop/new_scientific/imageAna/macro/utils/')
from helpers import ProgressBar

__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
#ROOT.SetAtlasStyle()
num_Det=6

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

def getfunc(name, _down, _up):
    return ROOT.TF1(name,"expo",_down,_up) 

def fit(args):
    fout = ROOT.TFile(args.output,"recreate")
    cv0  = createRatioCanvas("cv0", 1200, 800)

    f_ba=ROOT.TFile("./edb_data/MUSE203305_01_001_000_ene.root","read")
    f_co=ROOT.TFile("./edb_data/MUSE203302_01_001_000_ene.root","read") 
    f_eu=ROOT.TFile("./edb_data/MUSE203303_01_001_000_ene.root","read")
    t_ba=f_ba.Get("edbtree")
    t_co=f_co.Get("edbtree")
    t_eu=f_eu.Get("edbtree")
    fout.cd()

    prog = ProgressBar(ntotal=num_Det*3,text="Fitting...",init_t=time.time())
    nevproc=0
    if prog: prog.update(nevproc)
    p0_ba_list, p1_ba_list, time_ba_list, fuc_ba_list = [],[],[],[]
    p0_co_list, p1_co_list, time_co_list, fuc_co_list = [],[],[],[]
    p0_eu_list, p1_eu_list, time_eu_list, fuc_eu_list = [],[],[],[]
    for ich in range(num_Det):
       cv0.cd()
       t_ba.Draw("delta_t_det >> htemp_ba(50000,-1,1)","ch == {}".format(ich+1), "");nevproc+=1
       t_co.Draw("delta_t_det >> htemp_co(50000,-1,1)","ch == {}".format(ich+1), "");nevproc+=1
       t_eu.Draw("delta_t_det >> htemp_eu(50000,-1,1)","ch == {}".format(ich+1), "");nevproc+=1
       htemp_ba, htemp_co, htemp_eu = gDirectory.Get("htemp_ba"), gDirectory.Get("htemp_co"), gDirectory.Get("htemp_eu")
       htemp_ba.SetLineColor(1)
       htemp_co.SetLineColor(1)
       htemp_eu.SetLineColor(1)
       htemp_ba.SetTitle("Ba Ch{0};#Delta t [s]; Counts / 40#mus".format(ich+1))
       htemp_co.SetTitle("Co Ch{0};#Delta t [s]; Counts/40#mu s".format(ich+1))
       htemp_eu.SetTitle("Eu Ch{0};#Delta t [s]; Counts/40#mu s".format(ich+1))
       htemp_ba.SetName("htemp_ba_CH{}".format(ich+1))
       htemp_co.SetName("htemp_co_CH{}".format(ich+1))
       htemp_eu.SetName("htemp_eu_CH{}".format(ich+1))

       gb_ba=getfunc("gb_ba_"+str(ich),0,0.4)
       gb_co=getfunc("gb_co_"+str(ich),0,0.02)
       gb_eu=getfunc("gb_eu_"+str(ich),0,0.02)
       htemp_ba.Fit("gb_ba_"+str(ich),"QR")
       htemp_co.Fit("gb_co_"+str(ich),"QR")
       htemp_eu.Fit("gb_eu_"+str(ich),"QR")
       p0_ba_list.append(gb_ba.GetParameter(0))
       p1_ba_list.append(gb_ba.GetParameter(1))
       p0_co_list.append(gb_co.GetParameter(0))
       p1_co_list.append(gb_co.GetParameter(1))
       p0_eu_list.append(gb_eu.GetParameter(0))
       p1_eu_list.append(gb_eu.GetParameter(1))
       time_ba_list.append(htemp_ba.GetEntries()/(gb_ba.GetParameter(1)*(-1)))
       time_co_list.append(htemp_co.GetEntries()/(gb_co.GetParameter(1)*(-1)))
       time_eu_list.append(htemp_eu.GetEntries()/(gb_eu.GetParameter(1)*(-1)))
       fuc_ba_list.append(gb_ba)
       fuc_co_list.append(gb_co)
       fuc_eu_list.append(gb_eu)

       htemp_ba.Write()
       htemp_co.Write()
       htemp_eu.Write()
       gb_ba.Write()
       gb_co.Write()
       gb_eu.Write()
       if prog: prog.update(nevproc)

    # === make comparison ===
    ROOT.SetAtlasStyle()
    leg = ROOT.TLegend(.65,.7,.8,.88);
    leg.SetFillColor(0);
    leg.SetLineColor(0);
    leg.SetBorderSize(0);
    cv1  = createRatioCanvas("cv1", 1200, 800)
    cv2  = createRatioCanvas("cv2", 1200, 800)
    cv3  = createRatioCanvas("cv3", 1200, 800)
    for i in range(num_Det):
       cv1.cd()
       gPad.SetLogy(1)
       if i == 0:
          fuc_ba_list[i].SetTitle("Ba Ch{0};#Delta t [s]; Counts".format(i+1))
          fuc_ba_list[i].SetLineColor(i+1);fuc_ba_list[i].Draw()
       else:
          fuc_ba_list[i].SetLineColor(i+1);fuc_ba_list[i].Draw("same")
       leg.AddEntry(fuc_ba_list[i], "CH{}".format(i+1) , "l");
       if i == num_Det-1: leg.Draw("same");
    for i in range(num_Det):
       cv2.cd()
       gPad.SetLogy(1)
       if i == 0:
          fuc_co_list[i].SetTitle("Co Ch{0};#Delta t [s]; Counts".format(i+1))
          fuc_co_list[i].GetXaxis().SetNdivisions(5, 3, 0, False)
          fuc_co_list[i].SetLineColor(i+1);fuc_co_list[i].Draw()
       else:
          fuc_co_list[i].SetLineColor(i+1);fuc_co_list[i].Draw("same")
       if i == num_Det-1: leg.Draw("same");
    for i in range(num_Det):
       cv3.cd()
       gPad.SetLogy(1)
       if i == 0:
          fuc_eu_list[i].SetTitle("Eu Ch{0};#Delta t [s]; Counts".format(i+1))
          fuc_eu_list[i].GetXaxis().SetNdivisions(5, 3, 0, False)
          fuc_eu_list[i].SetLineColor(i+1);fuc_eu_list[i].Draw()
       else:
          fuc_eu_list[i].SetLineColor(i+1);fuc_eu_list[i].Draw("same")
       if i == num_Det-1: leg.Draw("same");

    cv1.Write()
    cv2.Write()
    cv3.Write()
    fout.Write()
    if prog: prog.finalize()
    cv1.SaveAs("./outfigs/fit_ba.pdf")
    cv2.SaveAs("./outfigs/fit_co.pdf")
    cv3.SaveAs("./outfigs/fit_eu.pdf")
    print("Ba CH1-6 => ", " p0 : ", p0_ba_list, " p1 : ", p1_ba_list, " livetime : ", time_ba_list)
    print("Co CH1-6 => ", " p0 : ", p0_co_list, " p1 : ", p1_co_list, " livetime : ", time_co_list)
    print("Eu CH1-6 => ", " p0 : ", p0_eu_list, " p1 : ", p1_eu_list, " livetime : ", time_eu_list)

if __name__ == '__main__' :
  parser = argparse.ArgumentParser(description='Process some integers.') 
  parser.add_argument("--output", type=str, default="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/outfigs/fit_output.root", help="Input File Name")
  args = parser.parse_args()

  fit( args )

