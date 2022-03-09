"""
simple_fit.py used to find the counts of the peaks by fitting
bkg : pol1
signa : single gaus
"""

import os,ROOT,ctypes,argparse
ROOT.gErrorIgnoreLevel = ROOT.kFatal
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

def fit6ch(args):
    # === tree ===
    ROOT.gROOT.ProcessLine(
      "struct RootStruct {\
         Int_t      ch;\
         Double_t  peak;\
         Double_t  intensity;\
         Double_t  eff;\
      };"
    );
    from ROOT import RootStruct
    struct = RootStruct()
    tout=ROOT.TTree('tree','tree')
    tout.Branch( 'ch', ROOT.addressof( struct, 'ch' ),  'ch/I' )
    tout.Branch( 'peak', ROOT.addressof( struct, 'peak' ),  'peak/D' )
    tout.Branch( 'intensity', ROOT.addressof( struct, 'intensity' ),  'intensity/D' )
    tout.Branch( 'eff', ROOT.addressof( struct, 'eff' ),  'eff/D' )

    # === selectable regions ===
    if args.output.lower() == "co":
       overall_range_down=[13.8,121,135.5]
       overall_range_up=[14.8,122.8,137.5]
       emissivity=[9.16, 85.60, 10.68] #%
       cout_all=1222778353 # source in Ryugu Exp.
    elif args.output.lower() == "eu":
       overall_range_down=[39.7,44.8,46.1,121]
       overall_range_up=[40.6,45.8,47.2,122.6]
       emissivity=[38.3, 7.24+3.74, 2.39, 28.58] #%
       cout_all=2716632871
    elif args.output.lower() == "ba":
       overall_range_down=[34.6,35.6,80.4]
       overall_range_up=[35.4,36.2,81.6]
       emissivity=[5.99+11.6, 3.58, 2.62+34.06] #%
       cout_all=15770190
    else:
       print("check output name!")
       return [],[]

    f_int=ROOT.TFile(args.input,"read")
    t_int=f_int.Get(args.tree)

    nbins=int(6800)
    peaks_list,count_list=[],[]
    for ich in range(1,7):
       print("=====================================")
       print("CH{} : ".format(ich))
       t_int.Draw("energy >> h({},10,180)".format(nbins),"ch=={}".format(ich),"")
       _h=ROOT.gDirectory.Get("h")
       for _ip in range(len(overall_range_down)):
          _htemp,_hdraw=_h.Clone(), _h.Clone()
          _htemp.GetXaxis().SetRangeUser(overall_range_down[_ip],overall_range_up[_ip])
          _hdraw.GetXaxis().SetRangeUser(overall_range_down[_ip],overall_range_up[_ip])
          peak = ROOT.TF1("peak","gaus", overall_range_down[_ip], overall_range_up[_ip]);
          bkg = ROOT.TF1("bkg","pol1", overall_range_down[_ip],overall_range_up[_ip]);

          par0,par1,par2,par3,par4 = map(ctypes.c_double, (0,0,0,0,0))
          _htemp.Fit(peak,"Q")
          _htemp.Fit(bkg,"Q")
          par0=peak.GetParameter(0)
          par1=peak.GetParameter(1)
          par2=peak.GetParameter(2)
          par3=bkg.GetParameter(0)
          par4=bkg.GetParameter(1)
   
          total = ROOT.TF1("total","gaus(0)+pol1(3)", overall_range_down[_ip],overall_range_up[_ip] )
          total.SetParameters(par0,par1,par2,par3,par4)
          _htemp.Fit(total,"Q")
          par0,par1,par2,par3,par4=total.GetParameter(0),total.GetParameter(1),total.GetParameter(2),total.GetParameter(3),total.GetParameter(4)
          peak.SetParameters(par0,par1,par2)
          bkg.SetParameters(par3,par4)
          
          peaks_list.append(par1)#peak
          _bindown, _binup=_htemp.GetXaxis().FindBin(par1-3*par2), _htemp.GetXaxis().FindBin(par1+3*par2)
          _binwidth=_htemp.GetBinWidth(1)
          count_list.append(peak.Integral(par1-3*par2,par1+3*par2)/_binwidth)
          print("Peak : {0:.1f} | Hist. : {1:.2f} | Total : {2:.2f} | Signal : {3:.2f}".format(par1, _htemp.Integral(_bindown,_binup), total.Integral(par1-3*par2,par1+3*par2)/_binwidth, peak.Integral(par1-3*par2,par1+3*par2)/_binwidth))
   
          c=ROOT.TCanvas("c{}".format(_ip),"c{}".format(_ip),1200,800)
          _hdraw.SetLineColor(1)
          peak.SetLineColor(4)
          bkg.SetLineColor(3)
          total.SetLineColor(2)
          _hdraw.SetMinimum(0)
          _hdraw.Draw("ep")
          peak.Draw("same")
          total.Draw("same")
          bkg.Draw("same")
          c.SaveAs("/Users/chiu.i-huan/Desktop/temp_output/c_fit_{0}_peak{1}_CH{2}.png".format(args.output,_ip,ich))

          # === for tree ===
          struct.ch = ich
          struct.peak = par1
          struct.intensity = peak.Integral(par1-3*par2,par1+3*par2)/_binwidth
          struct.eff = struct.intensity/(cout_all*emissivity[_ip])
          tout.Fill()
    fout=ROOT.TFile("/Users/chiu.i-huan/Desktop/temp_output/output_fit_result_{}.root".format(args.output),"recreate")
    fout.cd()
    tout.Write()
    return peaks_list, count_list


def fit(args):
    # === selectable regions ===
    if args.output.lower() == "co":
       overall_range_down=[13.8,121,135.5]
       overall_range_up=[14.8,122.8,137.5]
    elif args.output.lower() == "eu":
       overall_range_down=[39.7,44.8,46.1,121]
       overall_range_up=[40.6,45.8,47.2,122.6]
    elif args.output.lower() == "ba":
       overall_range_down=[34.6,35.6,80.4]
       overall_range_up=[35.4,36.2,81.6]
    elif args.output.lower() == "other":
       overall_range_down=[10]
       overall_range_up=[20]
    else:
       print("check output name!")
       return [],[]

    f_int=ROOT.TFile(args.input,"read")
    t_int=f_int.Get(args.tree)

    nbins=int(6800)
    peaks_list,count_list=[],[]
    t_int.Draw("energy >> h({},10,180)".format(nbins),"","")
    _h=ROOT.gDirectory.Get("h")
    for _ip in range(len(overall_range_down)):
       _htemp,_hdraw=_h.Clone(), _h.Clone()
       _htemp.GetXaxis().SetRangeUser(overall_range_down[_ip],overall_range_up[_ip])
       _hdraw.GetXaxis().SetRangeUser(overall_range_down[_ip],overall_range_up[_ip])
       peak = ROOT.TF1("peak","gaus", overall_range_down[_ip], overall_range_up[_ip]);
       bkg = ROOT.TF1("bkg","pol1", overall_range_down[_ip],overall_range_up[_ip]);

       par0,par1,par2,par3,par4 = map(ctypes.c_double, (0,0,0,0,0))
       _htemp.Fit(peak,"Q")
       _htemp.Fit(bkg,"Q")
       par0=peak.GetParameter(0)
       par1=peak.GetParameter(1)
       par2=peak.GetParameter(2)
       par3=bkg.GetParameter(0)
       par4=bkg.GetParameter(1)
   
       total = ROOT.TF1("total","gaus(0)+pol1(3)", overall_range_down[_ip],overall_range_up[_ip] )
       total.SetParameters(par0,par1,par2,par3,par4)
       _htemp.Fit(total,"Q")
       par0,par1,par2,par3,par4=total.GetParameter(0),total.GetParameter(1),total.GetParameter(2),total.GetParameter(3),total.GetParameter(4)
       peak.SetParameters(par0,par1,par2)
       bkg.SetParameters(par3,par4)
       
       peaks_list.append(par1)#peak
       _bindown, _binup=_htemp.GetXaxis().FindBin(par1-3*par2), _htemp.GetXaxis().FindBin(par1+3*par2)
       _binwidth=_htemp.GetBinWidth(1)
       count_list.append(peak.Integral(par1-3*par2,par1+3*par2)/_binwidth)
       print("Peak : {0:.1f} | Hist. : {1:.2f} | Total : {2:.2f} | Signal : {3:.2f}".format(par1, _htemp.Integral(_bindown,_binup), total.Integral(par1-3*par2,par1+3*par2)/_binwidth, peak.Integral(par1-3*par2,par1+3*par2)/_binwidth))
   
       c=ROOT.TCanvas("c{}".format(_ip),"c{}".format(_ip),1200,800)
       _hdraw.SetLineColor(1)
       peak.SetLineColor(4)
       bkg.SetLineColor(3)
       total.SetLineColor(2)
       _hdraw.SetMinimum(0)
       _hdraw.Draw("ep")
       peak.Draw("same")
       total.Draw("same")
       bkg.Draw("same")
       c.SaveAs("/Users/chiu.i-huan/Desktop/temp_output/c_fit_{0}_peak{1}.png".format(args.output,_ip))
    return peaks_list, count_list

if __name__=="__main__":
  """
  Ryugu data
  Co57 : MUSE203302_01_001_000_ene.root
  Eu152 : MUSE203303_01_001_000_ene.root
  Ba133 : MUSE203305_01_001_000_ene.root

  MC 
  """
  parser = argparse.ArgumentParser(description='Processing') 
  parser.add_argument("-i","--input", type=str, default="/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/edb_data/MUSE203303_01_001_000_ene.root", help="Input File Name")
  parser.add_argument("-o","--output", type=str, default="Eu", help="Output Name")
  parser.add_argument("-t","--tree", type=str, default="edbtree", help="Input Tree Name")
  args = parser.parse_args()

  _peak, _cout = fit6ch( args )# for ryugu study
#  _peak, _cout = fit( args )# for other study

