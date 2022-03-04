"""
for detector eff. calculation
Ryugu fitting result 
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

   f1=ROOT.TFile("/Users/chiu.i-huan/Desktop/temp_output/output_fit_sum.root","read")
   tree=f1.Get("tree")
   g1,g2,g3,g4,g5,g6 = ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph()
   index1,index2,index3,index4,index5,index6=0,0,0,0,0,0
   for ie in range(tree.GetEntries()):
      tree.GetEntry(ie)
      if tree.ch == 1: 
         g1.SetPoint(index1,tree.peak,tree.eff)
         index1+=1
      if tree.ch == 2: 
         g2.SetPoint(index2,tree.peak,tree.eff)
         index2+=1
      if tree.ch == 3: 
         g3.SetPoint(index3,tree.peak,tree.eff)
         index3+=1
      if tree.ch == 4: 
         g4.SetPoint(index4,tree.peak,tree.eff)
         index4+=1
      if tree.ch == 5: 
         g5.SetPoint(index5,tree.peak,tree.eff)
         index5+=1
      if tree.ch == 1: 
         g6.SetPoint(index6,tree.peak,tree.eff)
         index6+=1
   c1=ROOT.TCanvas("c1","c1",1000,800)
   g1.Draw("AP")
   c1.SaveAs("/Users/chiu.i-huan/Desktop/c_ch1_eff.pdf")
   c2=ROOT.TCanvas("c2","c2",1000,800)
   g2.Draw("AP")
   c2.SaveAs("/Users/chiu.i-huan/Desktop/c_ch2_eff.pdf")
   c3=ROOT.TCanvas("c3","c3",1000,800)
   g3.Draw("AP")
   c3.SaveAs("/Users/chiu.i-huan/Desktop/c_ch3_eff.pdf")
   c4=ROOT.TCanvas("c4","c4",1000,800)
   g4.Draw("AP")
   c4.SaveAs("/Users/chiu.i-huan/Desktop/c_ch4_eff.pdf")
   c5=ROOT.TCanvas("c5","c5",1000,800)
   g5.Draw("AP")
   c5.SaveAs("/Users/chiu.i-huan/Desktop/c_ch5_eff.pdf")
   c6=ROOT.TCanvas("c6","c6",1000,800)
   g6.Draw("AP")
   c6.SaveAs("/Users/chiu.i-huan/Desktop/c_ch6_eff.pdf")

