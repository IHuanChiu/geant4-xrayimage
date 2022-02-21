"""
self absolution check
"""
import os,ROOT
from array import array
from ROOT import *
__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()

def getLatex(ch, x = 0.85, y = 0.85):
    _t = ROOT.TLatex()
    _t.SetNDC()
    _t.SetTextFont( 62 )
    _t.SetTextColor( 36 )
    _t.SetTextSize( 0.08 )
    _t.SetTextAlign( 12 )
    return _t

if __name__=="__main__":
   fout = ROOT.TFile("./output_eff_plots_Ryugu.root","recreate")

   f1, _name=ROOT.TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/xray_build/Output_xray_self.root","read"), "ryugu"
   tree=f1.Get("tree")
   h_eff_list=[]
   for i in range(6):
       tree.Draw("energy_init*1000 >> h0_{}_{}(680,10,180)".format(_name,i+1),"direction_id=={} && detid !=-1".format(i+1),"")
       tree.Draw("energy*1000 >> h1_{}_{}(680,10,180)".format(_name,i+1),"detid=={}".format(i+1),"")
       h0=ROOT.gDirectory.Get("h0_{}_{}".format(_name,i+1))
       h1=ROOT.gDirectory.Get("h1_{}_{}".format(_name,i+1))
       h2=h1.Clone()
       h2.Divide(h0)
   
       h2.SetTitle(";Energy [keV]; Efficiency")
       h2.GetXaxis().CenterTitle(); h2.GetYaxis().CenterTitle();
       h2.SetLineColorAlpha(1,0.9)
       h2.SetMarkerColor(1)
       h2.SetMarkerStyle(2)
       fout.cd()
       h2.Write()
   
       c2=ROOT.TCanvas("c2_{}".format(i),"c2_{}".format(i),1200,1200)
       c2.cd()
       ROOT.gPad.SetLogy(0)
       h2.Draw("p")
       
       c2.SaveAs("/Users/chiu.i-huan/Desktop/c_{}_{}.png".format(_name,i+1))
       h_eff_list.append(h2)

   c3, index=ROOT.TCanvas("c3","c3",1200,1200), 1
   c3.cd()
   leg = ROOT.TLegend(.78,.3,.88,.58)
   leg.SetFillColor(0)
   leg.SetLineColor(0)
   leg.SetBorderSize(0)
   for ih in h_eff_list:
      ih.SetMarkerColorAlpha(7-index,0.7)
      ih.Draw("same p")
      leg.AddEntry(ih,  "CH{}".format(index), "p")
      index+=1
   leg.Draw("same")
   c3.SaveAs("/Users/chiu.i-huan/Desktop/c_all_combined.png")


