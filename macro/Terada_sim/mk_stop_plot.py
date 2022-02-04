import os,ROOT
from array import array
from ROOT import *
__location__ = os.path.realpath(
        os.path.join(os.getcwd(), os.path.dirname(__file__)))
ROOT.gROOT.LoadMacro( __location__+'/AtlasStyle/AtlasStyle.C')
ROOT.SetAtlasStyle()
ROOT.gROOT.LoadMacro("langaus.C")

def LanGausFit(hist, fitmin, fitmax):
    fitrange, initial_pars, parlimits_lo, parlimits_hi, ret_pars, ret_errors, ret_ChiSqr = array('d'), array('d'), array('d'), array('d'), array('d'), array('d'), array('d')
    ret_NDF = array('i')

    LINE_WIDTH = 5

    fitrange.append(fitmin)
    fitrange.append(fitmax)
    print(hist.GetRMS()/6, hist.GetBinCenter(hist.GetMaximumBin()), hist.GetSum())
    initial_pars.append(hist.GetRMS()/6)
    initial_pars.append(hist.GetBinCenter(hist.GetMaximumBin()))
    initial_pars.append(hist.GetSum())
    initial_pars.append(1.5)
    parlimits_lo.append(0.001)
    parlimits_lo.append(0.001)
    parlimits_lo.append(0.001)
    parlimits_lo.append(0.001)#gaus sigma
    parlimits_hi.append(500)
    parlimits_hi.append(5000)
    parlimits_hi.append(9999999999)
    parlimits_hi.append(1000)#gaus sigma
    ret_pars.append(0)
    ret_pars.append(0)
    ret_pars.append(0)
    ret_pars.append(0)
    ret_errors.append(0)
    ret_errors.append(0)
    ret_errors.append(0)
    ret_errors.append(0)
    ret_ChiSqr.append(0)
    ret_NDF.append(0)

    print ('LanGaus Fit:')
    convolfunc = ROOT.langaufit(hist,fitrange, initial_pars, parlimits_lo, parlimits_hi, ret_pars, ret_errors, ret_ChiSqr, ret_NDF)
    convolfunc.SetNpx(1000)
    convolfunc.SetLineStyle(1)
    convolfunc.SetLineColor(kRed)
    convolfunc.SetLineWidth(LINE_WIDTH)

    try:
        chisqr_over_NDF = ret_ChiSqr[0]/ret_NDF[0]
    except:
        chisqr_over_NDF = -1
    print ("Chisqr / NDF = " + str(ret_ChiSqr[0]) + ' / ' + str(ret_NDF[0]) + ' = ' + str(chisqr_over_NDF) +'\n')

    print ("Final pars:\n")
    print ("par0 = " + str(ret_pars[0]))
    print ("par1 = " + str(ret_pars[1]))
    print ("par2 = " + str(ret_pars[2]))
    print ("par3 = " + str(ret_pars[3]))
    print ("chisqr_over_NDF = ", chisqr_over_NDF)
    return convolfunc


def getLatex(ch, x = 0.85, y = 0.85):
    _t = ROOT.TLatex()
    _t.SetNDC()
    _t.SetTextFont( 62 )
    _t.SetTextColor( 36 )
    _t.SetTextSize( 0.08 )
    _t.SetTextAlign( 12 )
    return _t

if __name__=="__main__":
   #f1=ROOT.TFile("./MuonStop/Output_white_stop_27.root","read")
   #f1=ROOT.TFile("./MuonStop/Output_black_stop_27.root","read")
   #f1=ROOT.TFile("./MuonStop/Output_dew_stop_27.root","read")
   f1=ROOT.TFile("./MuonStop/Output_dew_stop_35.root","read")
   width, _name=1.75, "dew_35" # 0.5 mm for white/black; 1.75 mm for dew
   tree=f1.Get("muontree")
   tree.Draw("muSampleEndPolZ+{} >> h1(2000,0,2)".format(width),"muSampleEndPolZ != -1000","")
   h1=ROOT.gDirectory.Get("h1")
#   _func=LanGausFit(h1,0,1)

   h1.SetTitle(";Position from surface [mm]; Counts/0.1 #mum")
   h1.SetLineColorAlpha(1,0.9)

   c2=ROOT.TCanvas("c2_{}".format(1),"c2_{}".format(1),1200,1200)
   c2.cd()
   ROOT.gPad.SetLogy(0)
   h1.Draw("hist")
#   _func.SetLineColor(2)
#   _func.Draw("same")

   c2.SaveAs("/Users/chiu.i-huan/Desktop/c_terada_stop_{}.png".format(_name))


