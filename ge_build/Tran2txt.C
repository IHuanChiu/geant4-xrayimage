#include <iostream>
#include <fstream>
#include <sstream>

void Tran2txt(){

 TString fname="Output_202103expfornt_Sn_1M.root";

 TFile* f = new TFile(fname);
 TString phaDataName(fname.ReplaceAll(".root", ".dat"));
 ofstream phaDataFile(phaDataName.Data());

 TH1D*h;
 TTree* t0 = (TTree*)f->Get("tree");
 t0->Draw("Hit_Energy_Reso*1000 >>      h(50000,0,5000)");
 h=(TH1D*)gDirectory->Get("h");

 Int_t nbin = h->GetNbinsX();
 for (Int_t i = 1; i < nbin + 1; i++)
 {
     Double_t binCenter = h->GetBinCenter(i);
     Double_t binContent = h->GetBinContent(i);
     phaDataFile << i << ", " << binCenter << ", " << binContent << endl;
 }
 phaDataFile.close();

}
