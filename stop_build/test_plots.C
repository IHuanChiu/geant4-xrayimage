void test_plots(){
//   TFile* f1= new TFile("Output_none_none.root","read");
//   TFile* f2= new TFile("Output_kap100_al25.root","read");
//   TFile* f3= new TFile("Output_kap100_cu20.root","read");
//   TFile* f4= new TFile("Output_kap100_kap12p5.root","read");
//   TFile* f5= new TFile("Output_kap75_al25.root","read");
//   TFile* f6= new TFile("Output_kap75_cu20.root","read");
//   TFile* f7= new TFile("Output_kap75_kap12p5.root","read");
//   TFile* f8= new TFile("Output_kap50_al25.root","read");

//   TFile* f0= new TFile("Output_none_none.root","read");
//   TFile* f1= new TFile("Output_kap50_cu20.root","read");
//   TFile* f2= new TFile("Output_kap50_kap12p5.root","read");
//   TFile* f3= new TFile("Output_al150_al25.root","read");
//   TFile* f4= new TFile("Output_al150_cu20.root","read");
//   TFile* f5= new TFile("Output_al100_al25.root","read");
//   TFile* f6= new TFile("Output_al100_cu20.root","read");
//   TFile* f7= new TFile("Output_cu100_al25.root","read");
//   TFile* f8= new TFile("Output_cu100_cu20.root","read");

TFile* f0= new TFile("report10_files/Output_none_none.root","read");

//TFile* f1= new TFile("Output_none_none.root","read");
TFile* f1= new TFile("report10_files/Output_kap100_al25_cu10.root","read");
TFile* f2= new TFile("report10_files/Output_kap100_al25_cu5.root","read");
//TFile* f4= new TFile("Output_kap100_cu20.root","read");
TFile* f3= new TFile("report10_files/Output_kap75_cu20.root","read");
TFile* f4= new TFile("report10_files/Output_kap75_cu10.root","read");
//TFile* f6= new TFile("Output_kap50_cu20.root","read");

//TFile* f1= new TFile("report10_files/Output_kap100_kap12p5.root","read");
TFile* f5= new TFile("report10_files/Output_kap75_kap12p5.root","read");
TFile* f6= new TFile("report10_files/Output_al150_al25_27MeV.root","read");
TFile* f7= new TFile("report10_files/Output_al150_al20_27MeV.root","read");
//TFile* f5= new TFile("report10_files/Output_cu100_cu20_30MeV.root","read");
//TFile* f6= new TFile("report10_files/Output_cu100_cu20_35MeV.root","read");



   TTree* t0 = (TTree*)f0->Get("tree");
   TTree* t1 = (TTree*)f1->Get("tree");
   TTree* t2 = (TTree*)f2->Get("tree");
   TTree* t3 = (TTree*)f3->Get("tree");
   TTree* t4 = (TTree*)f4->Get("tree");
   TTree* t5 = (TTree*)f5->Get("tree");
   TTree* t6 = (TTree*)f6->Get("tree");
   TTree* t7 = (TTree*)f7->Get("tree");
//   TTree* t8 = (TTree*)f8->Get("tree");
   
   TH1D*h0;
   TH1D*h1;
   TH1D*h2;
   TH1D*h3;
   TH1D*h4;
   TH1D*h5;
   TH1D*h6;
   TH1D*h7;
   TH1D*h8;
   
   TCanvas*c1=new TCanvas("aa","aa",1000,800);
   gPad->SetLeftMargin(0.15);
   
   t0->Draw("     muInitX >>      h0(800,-80,80)");
   t1->Draw("muTargetPolX >> h1(800,-80,80)");
   t2->Draw("muTargetPolX >> h2(800,-80,80)");
   t3->Draw("muTargetPolX >> h3(800,-80,80)");
   t4->Draw("muTargetPolX >> h4(800,-80,80)");
   t5->Draw("muTargetPolX >> h5(800,-80,80)");
   t6->Draw("muTargetPolX >> h6(800,-80,80)");
   t7->Draw("muTargetPolX >> h7(800,-80,80)");
//   t8->Draw("muTargetPolX >> h8(1000,-100,100)");
   
   h0=(TH1D*)gDirectory->Get("h0");
   h1=(TH1D*)gDirectory->Get("h1");
   h2=(TH1D*)gDirectory->Get("h2");
   h3=(TH1D*)gDirectory->Get("h3");
   h4=(TH1D*)gDirectory->Get("h4");
   h5=(TH1D*)gDirectory->Get("h5");
   h6=(TH1D*)gDirectory->Get("h6");
   h7=(TH1D*)gDirectory->Get("h7");
//   h8=(TH1D*)gDirectory->Get("h8");

   h0->SetStats(0);
   h1->SetStats(0);
   h2->SetStats(0);
   h3->SetStats(0);
   h4->SetStats(0);
   h5->SetStats(0);
   h6->SetStats(0);
   h7->SetStats(0);
//   h8->SetStats(0);

   h0->SetLineColor(1);
   h1->SetLineColor(4);
   h2->SetLineColor(3);
   h3->SetLineColor(7);
   h4->SetLineColor(6);
   h5->SetLineColor(2);
   h6->SetLineColor(9);
   h7->SetLineColor(8);
//   h8->SetLineColor(9);

   h0->SetTitle("");
   h0->GetXaxis()->SetTitle("Y [mm]");  
   h0->GetYaxis()->SetTitle("Counts");

   h0->Draw();
   h1->Draw("same");
   h2->Draw("same");
   h3->Draw("same");
   h4->Draw("same");
   h5->Draw("same");
   h6->Draw("same");
   h7->Draw("same");
//   h8->Draw("same");

  TLegend* leg = new TLegend(.68,.50,.88,.88);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h0,  "Beam", "l");
  leg->AddEntry(h1,  "Case2", "l");
  leg->AddEntry(h2,  "Case3",   "l");
  leg->AddEntry(h3,  "Case5",   "l");
  leg->AddEntry(h4,  "Case8",   "l");
  leg->AddEntry(h5,  "Case11",   "l");
  leg->AddEntry(h6,  "Case16",   "l");
  leg->AddEntry(h7,  "Case18",   "l");
//  leg->AddEntry(h8,  "Case16",   "l");
  leg->Draw("same");

   c1->SaveAs("/Users/chiu.i-huan/Desktop/compare_addAl_X.pdf");
}
