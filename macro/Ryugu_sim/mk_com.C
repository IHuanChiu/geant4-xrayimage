#include <TROOT.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "THStack.h"
#include "TLegend.h"
#include "TChain.h"
#include "TFile.h"
#include "TH2.h"
#include "TH2.h"
#include "TH3.h"
#include "THn.h"
#include "TCanvas.h"
#include "TLatex.h"

#include "TH1F.h"
#include "TDirectory.h"
#include "TDirectoryFile.h"
#include "TArrow.h"
#include "TRef.h"
#include "TApplication.h"
#include "TError.h"
#include "TMath.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TLine.h"

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <TGraph.h>

#include "ATLASStyle/AtlasStyle.C"
#include "ATLASStyle/AtlasLabels.C"
#include "ATLASStyle/AtlasUtils.C"

void mk_com(){
  #ifdef __CINT__
    gROOT->LoadMacro("AtlasLabels.C");
    gROOT->LoadMacro("AtlasUtils.C");
  #endif
  SetAtlasStyle();
  
  TFile* f = new TFile("./outfigs/cv_output.root","read");
  TH1D *h1, *h2;
  TLegend* leg;
  TCanvas *c0;
//  TString sourcename="co";
//  TString sourcename="ba";
  TString sourcename="eu";

  for (int ich = 1 ; ich < 7; ich++){
     for (int ir = 1; ir < 5; ir++){
       h1=(TH1D*)f->Get(Form("hist_data_%s_ch%d_range%d",sourcename.Data(),ich,ir));
       h2=(TH1D*)f->Get(Form("hist_g4_%s_ch%d_range%d",sourcename.Data(),ich,ir));
       c0 = new TCanvas(Form("cv0_ch%d_range%d",ich,ir),Form("cv0_ch%d_range%d",ich,ir),1200,800);
       gPad->SetLogy(1);
       h1->SetLineColor(1);
       h2->SetLineColorAlpha(4,0.85);
       h1->Draw("hist");
       h2->Draw("hist same");

       leg = new TLegend(.75,.75,.9,.9);
       leg->SetFillColor(0);
       leg->SetLineColor(0);
       leg->SetBorderSize(0);
       leg->AddEntry(h1, "data" , "l");
       leg->AddEntry(h2, "G4" , "l");
       leg->Draw("same");
      
       c0->SaveAs(Form("./outfigs/comparison_%s_ch%d_range%d.pdf",sourcename.Data(),ich,ir));
       
     }
  }

  
}
