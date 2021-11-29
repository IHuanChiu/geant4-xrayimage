import os,ROOT,ctypes
__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()

data_file="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/202111011_ch3_Eu152_10cm_0rad.root"#ch3
#data_file="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211107_ch2_Eu152_10cm_0rad.root"#ch2
mc_file="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/G4_data/RIcenter/Output_Eu152_ch3_200M.root"
ndecay_data=43741900
ndecay_mc=200_000_000

nbins=int(6800)

def getLatex(ch, x = 0.85, y = 0.85):
    _t = ROOT.TLatex()
    _t.SetNDC()
    _t.SetTextFont( 62 )
    _t.SetTextColor( 36 )
    _t.SetTextSize( 0.08 )
    _t.SetTextAlign( 12 )
    return _t

def fit(_h):
    overall_range_down=[39.8,43,119]
    overall_range_up=[42,46,124]
    signal_range_down=[39.8,45,121.3]
    signal_range_up=[40.3,45.8,122.5]
    three_peaks=[] 
    count_list=[]
    par0,par1,par2,par3,par4 = map(ctypes.c_double, (0,0,0,0,0))
   
    for _ip in range(len(overall_range_down)):
       _htemp,_hdraw=_h.Clone(), _h.Clone()
       _htemp.GetXaxis().SetRangeUser(overall_range_down[_ip],overall_range_up[_ip])
       _hdraw.GetXaxis().SetRangeUser(overall_range_down[_ip],overall_range_up[_ip])
       peak = ROOT.TF1("peak","gaus", signal_range_down[_ip], signal_range_up[_ip]);
       bkg = ROOT.TF1("bkg","pol1", overall_range_down[_ip],overall_range_up[_ip]);
       _htemp.Fit(peak)
       _htemp.Fit(bkg)
       par0=peak.GetParameter(0)
       par1=peak.GetParameter(1)
       par2=peak.GetParameter(2)
       par3=bkg.GetParameter(0)
       par4=bkg.GetParameter(1)

       total = ROOT.TF1("total","gaus(0)+pol1(3)", overall_range_down[_ip],overall_range_up[_ip] )
       total.SetParameters(par0,par1,par2,par3,par4)
       _htemp.Fit(total)
       par0,par1,par2,par3,par4=total.GetParameter(0),total.GetParameter(1),total.GetParameter(2),total.GetParameter(3),total.GetParameter(4)
       peak.SetParameters(par0,par1,par2)
       bkg.SetParameters(par3,par4)
       
       three_peaks.append(par1)#peak
       _bindown, _binup=_htemp.GetXaxis().FindBin(signal_range_down[_ip]), _htemp.GetXaxis().FindBin(signal_range_up[_ip])
       count_list.append(peak.Integral(_bindown,_binup))
       print(peak.Integral(_bindown,_binup), peak.Integral(overall_range_down[_ip],overall_range_up[_ip]))

       c=ROOT.TCanvas("c{}".format(_ip),"c{}".format(_ip),1200,800)
       _hdraw.SetLineColor(1)
       peak.SetLineColor(4)
       bkg.SetLineColor(3)
       total.SetLineColor(2)
       _hdraw.Draw("hist")
       peak.Draw("same")
       total.Draw("same")
       bkg.Draw("same")
       c.SaveAs("/Users/chiu.i-huan/Desktop/c_fit_peak{}.png".format(_ip))
       
    return three_peaks, count_list

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

  
  _peak, _cout = fit(h_data)
#  _peak_mc, _cout_mc = fit(h_mc)

