import os,ROOT
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
   f1=ROOT.TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211107_ch2_Eu152_10cm_0rad.root","read")
#   f1=ROOT.TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/202111011_ch3_Eu152_10cm_0rad.root","read")
   f2=ROOT.TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211208/202112013_ch2_152Eu_10cm_0pirad.root","read")
#   f2=ROOT.TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211208/20211130_ch3_152Eu_10cm_0pirad.root","read")
   t1=f1.Get("tree")
   t2=f2.Get("tree")
   t1.Draw("energy >> h1(6800,10,180)")
   t2.Draw("energy >> h2(6800,10,180)")
   h1=ROOT.gDirectory.Get("h1")
   h2=ROOT.gDirectory.Get("h2")
   live1=3600
   live2=72000
   h1.Scale(1./live1)
   h2.Scale(1./live2)

   h1.SetTitle(";Energy [keV]; Counts/s/25 eV")
   h1.SetLineColorAlpha(2,0.7)
   h2.SetLineColorAlpha(4,0.7)
   
   leg = ROOT.TLegend(.7,.65,.9,.9)
   leg.SetFillColor(0)
   leg.SetLineColor(0)
   leg.SetBorderSize(0)
   leg.AddEntry(h1, "CH2 old" , "l")
   leg.AddEntry(h2, "CH2 new" , "l")

   c2=ROOT.TCanvas("c2_{}".format(1),"c2_{}".format(1),1500,1200)
   c2.cd()
   ROOT.gPad.SetLogy(0)
   h1.Draw("hist")
   h2.Draw("hist same")
   leg.Draw("same")

   c2.SaveAs("/Users/chiu.i-huan/Desktop/c_ch2_RIdata_old_vs_new.png")


