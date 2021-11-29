import os,ROOT
__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()

data_file="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/202111011_ch3_Eu152_10cm_0rad.root"#ch3
#data_file="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211107_ch2_Eu152_10cm_0rad.root"#ch2
mc_file="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/G4_data/RIcenter/Output_Eu152_ch3_200M.root"
ndecay_data=43741900
ndecay_mc=200_000_000

nbins=int(6800/5)

def getLatex(ch, x = 0.85, y = 0.85):
    _t = ROOT.TLatex()
    _t.SetNDC()
    _t.SetTextFont( 62 )
    _t.SetTextColor( 36 )
    _t.SetTextSize( 0.08 )
    _t.SetTextAlign( 12 )
    return _t

def fit(_h):
    three_peaks=[40.17, 45.42, 121.78]
    count_list=[]
     
    return count_list
if __name__=="__main__":


  f_data=ROOT.TFile(data_file,"read")
  f_mc=ROOT.TFile(mc_file,"read")

  t_data=f_data.Get("tree")
  t_mc=f_mc.Get("tree")
  t_data.Draw("energy >> h_data({},10,180)".format(nbins))
  t_mc.Draw("Hit_Energy_Reso*1000 >> h_mc({},10,180)".format(nbins))
  h_data=ROOT.gDirectory.Get("h_data")
  h_mc=ROOT.gDirectory.Get("h_mc")

  h_data.SetLineColorAlpha(2,0.9)
  h_mc.SetLineColorAlpha(4,0.9)
  h_mc.Scale(ndecay_data/ndecay_mc)

  leg = ROOT.TLegend(.75,.75,.9,.9)
  leg.SetFillColor(0)
  leg.SetLineColor(0)
  leg.SetBorderSize(0)
  leg.AddEntry(h_data, "data" , "l")
  leg.AddEntry(h_mc, "mc" , "l")

  c=ROOT.TCanvas("c","c",1200,800)
  c.cd()
  ROOT.gPad.SetLogy(1)
  h_data.Draw("hist")
  h_mc.Draw("hist same")
  leg.Draw("same")
  c.SaveAs("/Users/chiu.i-huan/Desktop/c_data_vs_mc.png")

  c2=ROOT.TCanvas("c2","c2",1200,800)
  c2.cd()
  ROOT.gPad.SetLogy(0)
  h_data.SetMaximum(2500)
  h_data.Draw("hist")
  h_mc.Draw("hist same")
  leg.Draw("same")
  c2.SaveAs("/Users/chiu.i-huan/Desktop/c_data_vs_mc_linear.png")
