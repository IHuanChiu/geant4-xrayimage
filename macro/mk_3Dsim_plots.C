void mk_3Dsim_plots(){
   TH1F *h1,*h2,*h3,*h4,*h5,*h6,*h7,*h8;
   TFile* fint = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/mu_build/Output_CdTe_full_560M.root","read");
   TTree* tree = (TTree*)fint->Get("tree");
   tree->Draw("Hit_x:-Hit_z >> h0(128,-16,16,128,-16,16)","Det_ID == 1 && Stop_VolumeID == 3","");
   tree->Draw("Hit_x:-Hit_z >> h2(128,-16,16,128,-16,16)","Det_ID == 3 && Stop_VolumeID == 3","");
   tree->Draw("Hit_x:-Hit_z >> h4(128,-16,16,128,-16,16)","Det_ID == 5 && Stop_VolumeID == 3","");
   tree->Draw("Hit_x:-Hit_z >> h6(128,-16,16,128,-16,16)","Det_ID == 7 && Stop_VolumeID == 3","");
   tree->Draw("Hit_y:-Hit_z >> h1(128,-16,16,128,-16,16)","Det_ID == 2 && Stop_VolumeID == 3","");
   tree->Draw("Hit_y:-Hit_z >> h3(128,-16,16,128,-16,16)","Det_ID == 4 && Stop_VolumeID == 3","");
   tree->Draw("Hit_y:-Hit_z >> h5(128,-16,16,128,-16,16)","Det_ID == 6 && Stop_VolumeID == 3","");
   tree->Draw("Hit_y:-Hit_z >> h7(128,-16,16,128,-16,16)","Det_ID == 8 && Stop_VolumeID == 3","");

   h1=(TH1F*)gDirectory->Get("h0");
   h2=(TH1F*)gDirectory->Get("h1");
   h3=(TH1F*)gDirectory->Get("h2");
   h4=(TH1F*)gDirectory->Get("h3");
   h5=(TH1F*)gDirectory->Get("h4");
   h6=(TH1F*)gDirectory->Get("h5");
   h7=(TH1F*)gDirectory->Get("h6");
   h8=(TH1F*)gDirectory->Get("h7");
   TFile *fout = new TFile("./repro_MC.root","recreate");
   fout->cd();
   h1->Write();
   h2->Write();
   h3->Write();
   h4->Write();
   h5->Write();
   h6->Write();
   h7->Write();
   h8->Write();

}
