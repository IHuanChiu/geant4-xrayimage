import os,ROOT
from array import array
from ROOT import *
__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()
ROOT.gROOT.LoadMacro("langaus.C")

def getLatex(ch, x = 0.85, y = 0.85):
    _t = ROOT.TLatex()
    _t.SetNDC()
    _t.SetTextFont( 62 )
    _t.SetTextColor( 36 )
    _t.SetTextSize( 0.08 )
    _t.SetTextAlign( 12 )
    return _t

if __name__=="__main__":
   #f1=ROOT.TFile("./G4_MCsample/Output_white_xray.root","read"); width, _name=0.5, "eff_white"
   #f1=ROOT.TFile("./G4_MCsample/Output_black_xray.root","read"); width, _name=0.5, "eff_black"
   #f1=ROOT.TFile("./G4_MCsample/Output_dew_xray.root","read"); width, _name=1.75, "eff_dew"
   f1=ROOT.TFile("./G4_MCsample/Output_dew35_xray.root","read"); width, _name=1.75, "eff_dew_35"
   tree=f1.Get("tree")
   for i in range(6):
      tree.Draw("energy_init*1000 >> h0(680,10,180)","direction_id=={}".format(i+1),"")
      tree.Draw("energy*1000 >> h1(680,10,180)","detid=={}".format(i+1),"")
      h0=ROOT.gDirectory.Get("h0")
      h1=ROOT.gDirectory.Get("h1")
      h2=h1.Clone()
      h2.Divide(h0)
   
      h2.SetTitle(";Energy [keV]; Efficiency")
      h2.GetXaxis().CenterTitle(); h2.GetYaxis().CenterTitle();
      h2.SetLineColorAlpha(1,0.9)
      h2.SetMarkerColor(1)
      h2.SetMarkerStyle(2)
   
      c2=ROOT.TCanvas("c2_{}".format(i),"c2_{}".format(i),1200,1200)
      c2.cd()
      ROOT.gPad.SetLogy(0)
      h2.Draw("p")
   
      c2.SaveAs("/Users/chiu.i-huan/Desktop/c_terada_{}_{}.png".format(_name,i+1))
      h1.Delete();h2.Delete();h0.Delete()
