#include <iostream>
#include <fstream>
#include <sstream>

void Tran2txt(){
 TString fname="report12_files/Output_202103Expback_Ni_1M.root";

 TFile* f = new TFile(fname);
 TString phaDataName(fname.ReplaceAll(".root", ".dat"));
 ofstream phaDataFile(phaDataName.Data());

 TH1D*h;
 TH1D*hg;
 TH1D*he;
 TH1D*ho;
 TTree* t0 = (TTree*)f->Get("tree");
 t0->Draw("Hit_Energy_Reso*1000 >>      h(50000,0,5000)");
 t0->Draw("Hit_Energy_Reso*1000 >>      hg(500,0,5000)","Hit_pdgId == 22","");
 t0->Draw("Hit_Energy_Reso*1000 >>      he(500,0,5000)","Hit_pdgId == 11","");
 t0->Draw("Hit_Energy_Reso*1000 >>      ho(500,0,5000)","Hit_pdgId != 22 && Hit_pdgId != 11","");
 h=(TH1D*)gDirectory->Get("h");
 hg=(TH1D*)gDirectory->Get("hg");
 he=(TH1D*)gDirectory->Get("he");
 ho=(TH1D*)gDirectory->Get("ho");
 phaDataFile << "Total: " <<  h->GetEntries() << "; Photon: " << hg->GetEntries()<< "; Electron: " << he->GetEntries()<< "; Other: " << ho->GetEntries()<< endl; 

 Int_t nbin = h->GetNbinsX();
 for (Int_t i = 1; i < nbin + 1; i++)
 {
     Double_t binCenter = h->GetBinCenter(i);
     Double_t binContent = h->GetBinContent(i);
     phaDataFile << i << ", " << binCenter << ", " << binContent << endl;
 }
 phaDataFile.close();

}
