void mkplots_forNabe(){

     TH1D *h1, *h2, *h3, *h4, *h5, *h6;
     TH1D *h1d, *h2d, *h3d, *h4d, *h5d, *h6d;
     double scale_0 = 873122400/100000000;
     double scale_45 = 436561200/100000000;

     TFile *f1 = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/G4_data/RIcenter/Output_Eu152_ch2_100M_nabe.root","read"); 
     TTree * t1 = (TTree*)f1->Get("tree"); 
     t1->Draw("Hit_Energy_Reso*1000 >> h1(8000,0,200)");
     h1=(TH1D*)gDirectory->Get("h1");
     h1->Scale(scale_0);
     h1->SetDirectory(0);

     TFile *f2 = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/G4_data/RIcenter/Output_Eu152_ch2_100M_nabe_45.root","read"); 
     TTree * t2 = (TTree*)f2->Get("tree"); 
     t2->Draw("Hit_Energy_Reso*1000 >> h2(8000,0,200)");
     h2=(TH1D*)gDirectory->Get("h2");
     h2->Scale(scale_45);
     h2->SetDirectory(0);

     TFile *f3 = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/G4_data/RIcenter/Output_Eu152_ch2_100M_nabe_90.root","read"); 
     TTree * t3 = (TTree*)f3->Get("tree"); 
     t3->Draw("Hit_Energy_Reso*1000 >> h3(8000,0,200)");
     h3=(TH1D*)gDirectory->Get("h3");
     h3->Scale(scale_45);
     h3->SetDirectory(0);

     TFile *f4 = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/G4_data/RIcenter/Output_Eu152_ch3_100M_nabe.root","read"); 
     TTree * t4 = (TTree*)f4->Get("tree"); 
     t4->Draw("Hit_Energy_Reso*1000 >> h4(8000,0,200)");
     h4=(TH1D*)gDirectory->Get("h4");
     h4->Scale(scale_0);
     h4->SetDirectory(0);

     TFile *f5 = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/G4_data/RIcenter/Output_Eu152_ch3_100M_nabe_45.root","read"); 
     TTree * t5 = (TTree*)f5->Get("tree"); 
     t5->Draw("Hit_Energy_Reso*1000 >> h5(8000,0,200)");
     h5=(TH1D*)gDirectory->Get("h5");
     h5->Scale(scale_45);
     h5->SetDirectory(0);

     TFile *f6 = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/G4_data/RIcenter/Output_Eu152_ch3_100M_nabe_90.root","read"); 
     TTree * t6 = (TTree*)f6->Get("tree"); 
     t6->Draw("Hit_Energy_Reso*1000 >> h6(8000,0,200)");
     h6=(TH1D*)gDirectory->Get("h6");
     h6->Scale(scale_45);
     h6->SetDirectory(0);

     TFile *f1d = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211208/202112013_ch2_152Eu_10cm_0pirad.root","read"); 
     TTree * t1d = (TTree*)f1d->Get("tree"); 
     t1d->Draw("energy >> h1d(8000,0,200)");
     h1d=(TH1D*)gDirectory->Get("h1d");
     h1d->SetDirectory(0);

     TFile *f2d = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211208/202112013_ch2_152Eu_10cm_025pirad.root","read"); 
     TTree * t2d = (TTree*)f2d->Get("tree"); 
     t2d->Draw("energy >> h2d(8000,0,200)");
     h2d=(TH1D*)gDirectory->Get("h2d");
     h2d->SetDirectory(0);

     TFile *f3d = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211208/202112013_ch2_152Eu_10cm_05pirad.root","read"); 
     TTree * t3d = (TTree*)f3d->Get("tree"); 
     t3d->Draw("energy >> h3d(8000,0,200)");
     h3d=(TH1D*)gDirectory->Get("h3d");
     h3d->SetDirectory(0);

     TFile *f4d = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211208/20211130_ch3_152Eu_10cm_0pirad.root","read"); 
     TTree * t4d = (TTree*)f4d->Get("tree"); 
     t4d->Draw("energy >> h4d(8000,0,200)");
     h4d=(TH1D*)gDirectory->Get("h4d");
     h4d->SetDirectory(0);

     TFile *f5d = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211208/20211130_ch3_152Eu_10cm_025pirad.root","read"); 
     TTree * t5d = (TTree*)f5d->Get("tree"); 
     t5d->Draw("energy >> h5d(8000,0,200)");
     h5d=(TH1D*)gDirectory->Get("h5d");
     h5d->SetDirectory(0);

     TFile *f6d = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ryugu_sim/RIcsv_data/20211208/20211130_ch3_152Eu_10cm_05pirad.root","read"); 
     TTree * t6d = (TTree*)f6d->Get("tree"); 
     t6d->Draw("energy >> h6d(8000,0,200)");
     h6d=(TH1D*)gDirectory->Get("h6d");
     h6d->SetDirectory(0);

     TFile *fout = new TFile("~/Desktop/output_Ryugu_Sim.root","recreate");
     fout->cd();
     h1->SetName("MC_ch2_0");
     h2->SetName("MC_ch2_45");
     h3->SetName("MC_ch2_90");
     h4->SetName("MC_ch3_0");
     h5->SetName("MC_ch3_45");
     h6->SetName("MC_ch3_90");
     h1d->SetName("DATA_ch2_0");
     h2d->SetName("DATA_ch2_45");
     h3d->SetName("DATA_ch2_90");
     h4d->SetName("DATA_ch3_0");
     h5d->SetName("DATA_ch3_45");
     h6d->SetName("DATA_ch3_90");
     h1->Write();
     h2->Write();
     h3->Write();
     h4->Write();
     h5->Write();
     h6->Write();
     h1d->Write();
     h2d->Write();
     h3d->Write();
     h4d->Write();
     h5d->Write();
     h6d->Write();
}
