import sys,os,ROOT,time
import heapq
__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()

f = ROOT.TFile("./outfigs/cv_output.root","read");
source_list={"ba":[31.0,35.0,35.8,81.0],"co":[14.4, 122.1, 136.5],"eu":[40.1,45.4,121.8]}
_ratio_ch1, _ratio_ch2, _ratio_ch3, _ratio_ch4, _ratio_ch5, _ratio_ch6 = dict(),dict(),dict(),dict(),dict(),dict()
hr_1,hr_2,hr_3,hr_4,hr_5,hr_6=ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph(),ROOT.TGraph()

if __name__=="__main__":
   # === counts ===
   for iS in source_list:
      _name=iS
      e_list=source_list[_name]
      print("Source : ", _name)
      for ich in range(1,7):
         h1=f.Get("hist_data_{0}_ch{1}_range5".format(_name,ich))
         h2=f.Get("hist_g4_{0}_ch{1}_range5".format(_name,ich))
         print("CH",ich)
         for ie in e_list:         
            print("E : ", ie) 
            _downh1,_uph1=h1.GetXaxis().FindBin(ie-0.5), h1.GetXaxis().FindBin(ie+0.5)
            _downh2,_uph2=h2.GetXaxis().FindBin(ie-0.5), h2.GetXaxis().FindBin(ie+0.5)
            print("Counts (data):{:.0f}".format(h1.Integral(_downh1,_uph1)))
            print("Counts (g4):{:.1f}".format(h2.Integral(_downh2,_uph2)))
            print("Rate (data/g4):{}".format(h1.Integral(_downh1,_uph1)/h2.Integral(_downh2,_uph2)))
 
            if ich==1: _ratio_ch1.update({ie:(h1.Integral(_downh1,_uph1)/h2.Integral(_downh2,_uph2))})
            if ich==2: _ratio_ch2.update({ie:(h1.Integral(_downh1,_uph1)/h2.Integral(_downh2,_uph2))})
            if ich==3: _ratio_ch3.update({ie:(h1.Integral(_downh1,_uph1)/h2.Integral(_downh2,_uph2))})
            if ich==4: _ratio_ch4.update({ie:(h1.Integral(_downh1,_uph1)/h2.Integral(_downh2,_uph2))})
            if ich==5: _ratio_ch5.update({ie:(h1.Integral(_downh1,_uph1)/h2.Integral(_downh2,_uph2))})
            if ich==6: _ratio_ch6.update({ie:(h1.Integral(_downh1,_uph1)/h2.Integral(_downh2,_uph2))})

   # === plots ===
   _maxlist_ch1 = heapq.nsmallest(len(source_list["ba"])*len(source_list["co"])*len(source_list["eu"]),_ratio_ch1)
   _maxlist_ch2 = heapq.nsmallest(len(source_list["ba"])*len(source_list["co"])*len(source_list["eu"]),_ratio_ch2)
   _maxlist_ch3 = heapq.nsmallest(len(source_list["ba"])*len(source_list["co"])*len(source_list["eu"]),_ratio_ch3)
   _maxlist_ch4 = heapq.nsmallest(len(source_list["ba"])*len(source_list["co"])*len(source_list["eu"]),_ratio_ch4)
   _maxlist_ch5 = heapq.nsmallest(len(source_list["ba"])*len(source_list["co"])*len(source_list["eu"]),_ratio_ch5)
   _maxlist_ch6 = heapq.nsmallest(len(source_list["ba"])*len(source_list["co"])*len(source_list["eu"]),_ratio_ch6)

   _maxratio=0.
   _minratio=10000.
   for _ich in range(len(_maxlist_ch1)):
      _energy=_maxlist_ch1[_ich]
      _ratio=_ratio_ch1[_energy]
      if _ratio > _maxratio: _maxratio = _ratio
      if _ratio < _minratio: _minratio = _ratio
      hr_1.SetPoint(_ich, _energy, _ratio)
   for _ich in range(len(_maxlist_ch2)):
      _energy=_maxlist_ch2[_ich]
      _ratio=_ratio_ch2[_energy]
      if _ratio > _maxratio: _maxratio = _ratio
      if _ratio < _minratio: _minratio = _ratio
      hr_2.SetPoint(_ich, _energy, _ratio)
   for _ich in range(len(_maxlist_ch3)):
      _energy=_maxlist_ch3[_ich]
      _ratio=_ratio_ch3[_energy]
      if _ratio > _maxratio: _maxratio = _ratio
      if _ratio < _minratio: _minratio = _ratio
      hr_3.SetPoint(_ich, _energy, _ratio)
   for _ich in range(len(_maxlist_ch4)):
      _energy=_maxlist_ch4[_ich]
      _ratio=_ratio_ch4[_energy]
      if _ratio > _maxratio: _maxratio = _ratio
      if _ratio < _minratio: _minratio = _ratio
      hr_4.SetPoint(_ich, _energy, _ratio)
   for _ich in range(len(_maxlist_ch5)):
      _energy=_maxlist_ch5[_ich]
      _ratio=_ratio_ch5[_energy]
      if _ratio > _maxratio: _maxratio = _ratio
      if _ratio < _minratio: _minratio = _ratio
      hr_5.SetPoint(_ich, _energy, _ratio)
   for _ich in range(len(_maxlist_ch6)):
      _energy=_maxlist_ch6[_ich]
      _ratio=_ratio_ch6[_energy]
      if _ratio > _maxratio: _maxratio = _ratio
      if _ratio < _minratio: _minratio = _ratio
      hr_6.SetPoint(_ich, _energy, _ratio)

   cv1=ROOT.TCanvas("cv1","cv1",0,0,1200,800)
   cv1.cd()
   hr_1.SetTitle("Ratio; Energy [keV]; Ratio (data/MC)")
   hr_1.GetXaxis().CenterTitle()
   hr_1.GetYaxis().CenterTitle()
   hr_1.SetMaximum(_maxratio*1.2)
   hr_1.SetMinimum(_minratio*0.2)
   hr_1.SetLineColor(1+1)
   hr_2.SetLineColor(2+1)
   hr_3.SetLineColor(3+1)
   hr_4.SetLineColor(4+1)
   hr_5.SetLineColor(5+1)
   hr_6.SetLineColor(6+1)
   line0=ROOT.TLine(hr_1.GetXaxis().GetXmin(),1,hr_1.GetXaxis().GetXmax(),1)
   line0.SetLineColor(1)
   line0.SetLineStyle(9)
   leg = ROOT.TLegend(.78,.6,.88,.88)
   leg.SetFillColor(0)
   leg.SetLineColor(0)
   leg.SetBorderSize(0)
   leg.AddEntry(hr_1,  "CH1", "l")
   leg.AddEntry(hr_2,  "CH2", "l")
   leg.AddEntry(hr_3,  "CH3", "l")
   leg.AddEntry(hr_4,  "CH4", "l")
   leg.AddEntry(hr_5,  "CH5", "l")
   leg.AddEntry(hr_6,  "CH6", "l")
   hr_1.Draw("");hr_2.Draw("same");hr_3.Draw("same");hr_4.Draw("same");hr_5.Draw("same");hr_6.Draw("same");leg.Draw("same");line0.Draw("same");
   cv1.SaveAs("./outfigs/hist_ratio.pdf")
   
