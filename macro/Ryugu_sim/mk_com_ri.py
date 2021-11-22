import os,ROOT
__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()

boolJPARCvsRIcenter=True

mypath="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/"
filelist=[
    "202111012_ch3_Am241_10cm_0pirad.root",
    "202111012_ch3_Am241_10cm_025pirad.root",
    "202111012_ch3_Am241_10cm_05pirad.root",
    "202111011_ch3_Eu152_10cm_0rad.root",
    "202111011_ch3_Eu152_10cm_025pirad.root",
    "202111011_ch3_Eu152_10cm_05pirad.root",
    "202111013_ch3_Ba133_10cm_0pirad.root",
    "202111011_ch3_Ba133_10cm_025pirad.root",
    "202111012_ch3_Ba133_10cm_05pirad.root",
    "20211107_ch2_Eu152_10cm_0rad.root",
    "20211107_ch2_Eu152_10cm_025pi.root",
    "20211107_ch2_Eu152_10cm_05pi.root",
    "20211108_ch2_Ba133_10cm_0rad.root",
    "20211108_ch2_Ba133_10cm_025pi.root",
    "20211110_ch2_Ba133_10cm_05pi.root"]
livetime=[3600, 3600, 3600, 3600, 3600, 3600, 14536, 63613, 63728, 3600, 3600, 3600, 61208, 61915, 10800]
casename=["CH3, ^{241}Am", "CH3, ^{152}Eu", "CH3, ^{133}Ba", "CH2, ^{152}Eu", "CH2, ^{133}Ba"]

f_ba=ROOT.TFile("./edb_data/MUSE203305_01_001_000_ene.root","read")
f_eu=ROOT.TFile("./edb_data/MUSE203303_01_001_000_ene.root","read")
t_ba=f_ba.Get("edbtree")
t_eu=f_eu.Get("edbtree")


def getLatex(ch, x = 0.85, y = 0.85):
    _t = ROOT.TLatex()
    _t.SetNDC()
    _t.SetTextFont( 62 )
    _t.SetTextColor( 36 )
    _t.SetTextSize( 0.08 )
    _t.SetTextAlign( 12 )
    return _t

if __name__=="__main__":
  nbins=int(6800/2)
  for i in range(5):
     f0=ROOT.TFile(mypath+filelist[i*3],"read")
     f025=ROOT.TFile(mypath+filelist[i*3+1],"read")
     f05=ROOT.TFile(mypath+filelist[i*3+2],"read")
     t0=f0.Get("tree")
     t025=f025.Get("tree")
     t05=f05.Get("tree")
     t0.Draw("energy >> h0({},10,180)".format(nbins))
     t025.Draw("energy >> h025({},10,180)".format(nbins))
     t05.Draw("energy >> h05({},10,180)".format(nbins))
     h0=ROOT.gDirectory.Get("h0")
     h025=ROOT.gDirectory.Get("h025")
     h05=ROOT.gDirectory.Get("h05")
     
     h0.SetLineColorAlpha(1,0.9)
     h025.SetLineColorAlpha(4,0.7)
     h05.SetLineColorAlpha(2,0.7)
     h0_live, h025_live, h05_live = livetime[i*3],livetime[i*3+1],livetime[i*3+2]
     h0.Scale(1./h0_live)
     h025.Scale(1./h025_live)
     h05.Scale(1./h05_live)
     h0.SetTitle(";Energy [keV];Counts/s/0.5 keV")

     leg = ROOT.TLegend(.75,.75,.9,.9)
     leg.SetFillColor(0)
     leg.SetLineColor(0)
     leg.SetBorderSize(0)
     leg.AddEntry(h0, "angle 0" , "l")
     leg.AddEntry(h025, "angle #pi/4" , "l")
     leg.AddEntry(h05, "angle #pi/2" , "l")

     latex = getLatex(i,400,8000)

     if not boolJPARCvsRIcenter:
        c=ROOT.TCanvas("c_{}".format(i),"c_{}".format(i),2200,800)
        c.cd()
        ROOT.gPad.SetLogy(1)
   
        h0.Draw("hist")
        h025.Draw("hist same")
        h05.Draw("hist same")
        leg.Draw("same")
        latex.DrawLatex(0.5,0.85,casename[i])
        newname=casename[i].replace(", ^","_")
        c.SaveAs("/Users/chiu.i-huan/Desktop/c_case_{}.pdf".format(newname))
        c.SaveAs("/Users/chiu.i-huan/Desktop/c_case_{}.png".format(newname))
     else:    
        scale_factor=(pow(100,2)/pow(85,2))
        if i == 1:
           t_eu.Draw("energy >> h_ryu({},10,180)".format(nbins),"ch == 3","")
           h_ryu=ROOT.gDirectory.Get("h_ryu") 
           h_ryu.Scale(1./(3947*688277.9))
           h0.Scale(scale_factor*(1./(12150.5)))
           h025.Scale(scale_factor*(1./(12150.5)))
           h05.Scale(scale_factor*(1./(12150.5)))
           h_ryu.SetLineColorAlpha(1,1)
           h0.SetLineColorAlpha(2,0.9)
           h025.SetLineColorAlpha(4,0.5)
           h05.SetLineColorAlpha(3,0.5)
           
           c2=ROOT.TCanvas("c2_{}".format(i),"c2_{}".format(i),2200,800)
           c2.cd()
           leg2 = ROOT.TLegend(.75,.75,.9,.9)
           leg2.SetFillColor(0)
           leg2.SetLineColor(0)
           leg2.SetBorderSize(0)
           leg2.AddEntry(h_ryu, "J-PARC" , "l")
           leg2.AddEntry(h0, "RI center, 0" , "l")
           leg2.AddEntry(h025, "RI center, #pi/4" , "l")
           leg2.AddEntry(h05, "RI center, #pi/2" , "l")
           ROOT.gPad.SetLogy(1)
           h_ryu.SetTitle(";Energy [keV];Eff./0.5 keV")
           h_ryu.Draw("hist")
           h0.Draw("hist same")
           h025.Draw("hist same")
           h05.Draw("hist same")
           leg2.Draw("same")
           c2.SaveAs("/Users/chiu.i-huan/Desktop/c_Ryugu_CH3_Eu.png")
   
        if i == 3:
           t_eu.Draw("energy >> h_ryu({},10,180)".format(nbins),"ch == 2","")
           h_ryu=ROOT.gDirectory.Get("h_ryu") 
           h_ryu.Scale(1./(3947*688277.9))
           h0.Scale(scale_factor*(1./(12150.5)))
           h025.Scale(scale_factor*(1./(12150.5)))
           h05.Scale(scale_factor*(1./(12150.5)))
           h_ryu.SetLineColorAlpha(1,1)
           h0.SetLineColorAlpha(2,0.9)
           h025.SetLineColorAlpha(4,0.5)
           h05.SetLineColorAlpha(3,0.5)
           
           c2=ROOT.TCanvas("c2_{}".format(i),"c2_{}".format(i),2200,800)
           c2.cd()
           leg2 = ROOT.TLegend(.75,.75,.9,.9)
           leg2.SetFillColor(0)
           leg2.SetLineColor(0)
           leg2.SetBorderSize(0)
           leg2.AddEntry(h_ryu, "J-PARC" , "l")
           leg2.AddEntry(h0, "RI center, 0" , "l")
           leg2.AddEntry(h025, "RI center, #pi/4" , "l")
           leg2.AddEntry(h05, "RI center, #pi/2" , "l")
           ROOT.gPad.SetLogy(1)
           h_ryu.SetTitle(";Energy [keV];Eff./0.5 keV")
           h_ryu.Draw("hist")
           h0.Draw("hist same")
           h025.Draw("hist same")
           h05.Draw("hist same")
           leg2.Draw("same")
           c2.SaveAs("/Users/chiu.i-huan/Desktop/c_Ryugu_CH2_Eu.png")
        

