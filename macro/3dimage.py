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

import sys,os,random,math,time,ROOT
from ROOT import TFile, TTree, TCut
from ROOT import gROOT, AddressOf, gPad, gDirectory
ROOT.gROOT.SetBatch(1)
import argparse
from color import SetMyPalette
from logger import log, supports_color
import helpers
import numpy as np
ROOT.gErrorIgnoreLevel = ROOT.kWarning

__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()
DEG = u"\xb0"

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

def getContent(ibinx, ibiny, ibinz, h2, h2name, _h3):
    _angle=helpers.getangle(h2name)
    _content = h2.GetBinContent(ibinx,ibiny)
    _x,_y,_z = GetRotation(_h3.GetXaxis().GetBinCenter(ibinx), _h3.GetYaxis().GetBinCenter(ibiny), _h3.GetYaxis().GetBinCenter(ibinz),_angle)
    return _x,_y,_z, _content, _angle

def GetRotation(_x,_y,_z,_angle):
    c, s = np.cos(_angle), np.sin(_angle)
    R = np.matrix([[1,0,0], [0,c,-s], [0,s,c]]) # around x-axis
    v = np.matrix( [ _x, _y, _z ])
    new_v = R*v.reshape(3,1)
    return new_v[0,0], new_v[1,0], new_v[2,0]

def plot(args):
    f = ROOT.TFile(args.input,"read")   
    tree = f.Get("tree")
    emin,emax = 70,75 #keV
    emin2,emax2 = 10,15 #keV
    h2list = []
    log().info("Preparing 2D image...")
    for i in range(6):
       ecut = TCut("Position_x[{0}] != -1000 && Position_z[{0}]!= -1000 && ((energyDep[{0}]*1000 > {1} && energyDep[{0}]*1000 < {2}) || (energyDep[{0}]*1000 > {3} && energyDep[{0}]*1000 < {4}))".format(i,emin,emax, emin2, emax2))
       tree.Draw("Position_x[{0}]:(Position_z[{0}]-30.5) >> h_{0}(128,-16,16,128,-16,16)".format(i),ecut,"")
       hist = gDirectory.Get("h_{0}".format(i))
       hist.SetTitle("h_{0}".format(i))
       h2list.append(hist)
    

    h3d_t = ROOT.TH3D("solid_t","solid_t",32,-16,16,32,-16,16,32,-16,16)
    h3d_t.SetXTitle("x")
    h3d_t.SetYTitle("y")
    h3d_t.SetZTitle("z")
    if args.input3Dhist is None:
       log().info("Making 3D image...")
       h3d = ROOT.TH3D("solid","solid",128,-16,16,128,-16,16,128,-16,16)
       h3d.SetXTitle("x")
       h3d.SetYTitle("y")
       h3d.SetZTitle("z")
       numoff,ti=0,time.time()
       for h2 in h2list: 
          numoff+=1
          h2name = h2.GetTitle()
          for ibinx in range(1,h3d.GetXaxis().GetNbins()+1):
             for ibiny in range(1,h3d.GetYaxis().GetNbins()+1):
                for ibinz in range(1,h3d.GetZaxis().GetNbins()+1):
                   x,y,z,content, _angle = getContent(ibinx, ibiny, ibinz, h2, h2name, h3d)
                   h3d.Fill(x,y,z,content)
          log().info("Running time : %.1f s, (%s/%s) images, Angle : %.1f%s"%(time.time()-ti,numoff,len(h2list),math.degrees(_angle),DEG))
    else:
       r3dfile  =  ROOT.TFile(args.input3Dhist)
       h3d = r3dfile.Get("solid")

    log().info("Making tight 3D image...")
    _h3d_t = h3d.Clone()
    _h3d_t.Rebin3D(4,4,4)
    for _ix in range(1,_h3d_t.GetXaxis().GetNbins()+1):
       for _iy in range(1,_h3d_t.GetYaxis().GetNbins()+1):
          for _iz in range(1,_h3d_t.GetZaxis().GetNbins()+1):
             _bin = _h3d_t.GetBin(_ix,_iy,_iz)
             _x,_y,_z=_h3d_t.GetXaxis().GetBinCenter(_ix),_h3d_t.GetYaxis().GetBinCenter(_iy),_h3d_t.GetZaxis().GetBinCenter(_iz)
             if(_h3d_t.GetBinContent(_bin) > args.cut): h3d_t.Fill(_x,_y,_z,_h3d_t.GetBinContent(_bin))
    cv  = createRatioCanvas("cv", 1600, 1600)
    SetMyPalette("AD")     
    h3d_t.Draw("BOX2Z")
    cv.Print("./figs/quick_3dimage.pdf")


    log().info("Storing images...")
    fout = ROOT.TFile( "root/simulation_3dimage.root", 'recreate' )
    fout.cd()
    for _h2 in h2list:
       _h2.Write()
    h3d_t.Write()
    h3d.Write()
    fout.Write()

if __name__ == '__main__' : 
  parser = argparse.ArgumentParser(description='Process some integers.')
  parser.add_argument("-i", "--input", type=str, default="./data/Output_200M.root", help="Input File Name")
  parser.add_argument("-c", "--cut", type=int, default = 200, help="count cut for 3D image" )
  parser.add_argument("-p", "--input3Dhist", type=str, default=None, help="Input 3D file")
  args = parser.parse_args()

  plot( args )

