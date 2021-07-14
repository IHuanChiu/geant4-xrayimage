void beam_plots(){

TFile* f= new TFile("Output_0714_SigXY_0p5_3_errXY_0p025_0p025.root","read");

   TTree* t = (TTree*)f->Get("tree");
   
   TH1D*hx_take = new TH1D("","",120,0,120);
   TH1D*hy_take = new TH1D("","",120,0,120);
   hx_take->Fill(50,1.6);
   hx_take->Fill(80,2.6);
   hx_take->Fill(110,4.3);
   hy_take->Fill(50,12);
   hy_take->Fill(80,15);
   hy_take->Fill(110,23);

   TH1D*hx = new TH1D("","",120,0,120);
   TH1D*hy = new TH1D("","",120,0,120);
   
   TCanvas*c1=new TCanvas("aa","aa",1000,800);
   c1->SetGridy();
   
   TH1D* h0;
   TH1D* h1;
   TH1D* h2;
   TH1D* h3;
   t->Draw("    muInitX>>h0","muInitX!=-1000","");
   t->Draw("muFoil1PolX>>h1","muFoil1PolX!=-1000","");
   t->Draw("muFoil2PolX>>h2","muFoil2PolX!=-1000","");
   t->Draw("muFoil3PolX>>h3","muFoil3PolX!=-1000","");
   h0=(TH1D*)gDirectory->Get("h0");
   h1=(TH1D*)gDirectory->Get("h1");
   h2=(TH1D*)gDirectory->Get("h2");
   h3=(TH1D*)gDirectory->Get("h3");
   hx->Fill(1,h0->GetRMS());//Returns the Standard Deviation (Sigma).
   hx->Fill(50,h1->GetRMS());//Returns the Standard Deviation (Sigma).
   hx->Fill(80,h2->GetRMS());//Returns the Standard Deviation (Sigma).
   hx->Fill(110,h3->GetRMS());//Returns the Standard Deviation (Sigma).

   TH1D* h0_y;
   TH1D* h1_y;
   TH1D* h2_y;
   TH1D* h3_y;
   t->Draw("    muInitY>>h0_y","muInitY !=-1000","");
   t->Draw("muFoil1PolY>>h1_y","muFoil1PolY != -1000","");
   t->Draw("muFoil2PolY>>h2_y","muFoil2PolY != -1000","");
   t->Draw("muFoil3PolY>>h3_y","muFoil3PolY != -1000","");
   h0_y=(TH1D*)gDirectory->Get("h0_y");
   h1_y=(TH1D*)gDirectory->Get("h1_y");
   h2_y=(TH1D*)gDirectory->Get("h2_y");
   h3_y=(TH1D*)gDirectory->Get("h3_y");
   hy->Fill(1,h0_y->GetRMS());//Returns the Standard Deviation (Sigma).
   hy->Fill(50,h1_y->GetRMS());//Returns the Standard Deviation (Sigma).
   hy->Fill(80,h2_y->GetRMS());//Returns the Standard Deviation (Sigma).
   hy->Fill(110,h3_y->GetRMS());//Returns the Standard Deviation (Sigma).
   

   hx_take->SetStats(0);
   hy_take->SetStats(0);

   hx_take->SetMarkerStyle(20);
   hy_take->SetMarkerStyle(20);
   hx->SetMarkerStyle(20);
   hy->SetMarkerStyle(20);
   hx_take->SetMarkerSize(1.5);
   hy_take->SetMarkerSize(1.5);
   hx->SetMarkerSize(1.5);
   hy->SetMarkerSize(1.5);

   hx_take->SetMarkerColor(1);
   hy_take->SetMarkerColor(1);
   hx->SetMarkerColor(2);
   hy->SetMarkerColor(2);

   hx_take->SetTitle("#sigma_{x} of Beam vs. Z [mm]");
   hx_take->GetXaxis()->SetTitle("Z [mm]");  
   hx_take->GetYaxis()->SetTitle("#sigma_{x} /mm");
   hx_take->GetYaxis()->CenterTitle();
   hy_take->SetTitle("#sigma_{y} of Beam vs. Z [mm]");
   hy_take->GetXaxis()->SetTitle("Z [mm]");  
   hy_take->GetYaxis()->SetTitle("#sigma_{y} /mm");
   hy_take->GetYaxis()->CenterTitle();

   hx_take->Draw("hist p");
   hx->Draw("hist p same");
   TLegend* leg = new TLegend(.20,.75,.38,.88);
   leg->SetFillColor(0);
   leg->SetLineColor(0);
   leg->SetBorderSize(0);
   leg->AddEntry(hx_take,  "Exp.", "p");
   leg->AddEntry(hx,  "Sim.", "p");
   leg->Draw("same");
   TLine* line;
   line=new TLine(1,h0->GetRMS(),50,h1->GetRMS());line->SetLineColor(2);line->SetLineStyle(2);line->Draw("same");
   line=new TLine(50,h1->GetRMS(),80,h2->GetRMS());line->SetLineColor(2);line->SetLineStyle(2);line->Draw("same");
   line=new TLine(80,h2->GetRMS(),110,h3->GetRMS());line->SetLineColor(2);line->SetLineStyle(2);line->Draw("same");
   line=new TLine(50,1.6,80,2.6);line->SetLineColor(1);line->SetLineStyle(2);line->Draw("same");
   line=new TLine(80,2.6,110,4.3);line->SetLineColor(1);line->SetLineStyle(2);line->Draw("same");
   TGraph* errorband = new TGraph();
   errorband->SetLineWidth(0);
   errorband->SetLineColorAlpha(91,0.3);
   errorband->SetFillColorAlpha(91,0.3);
   errorband->SetPoint( 0, 1, h0->GetRMS()+h0->GetRMSError());
   errorband->SetPoint( 1, 50, h1->GetRMS()+h1->GetRMSError());
   errorband->SetPoint( 2, 80, h2->GetRMS()+h2->GetRMSError());
   errorband->SetPoint( 3, 110, h3->GetRMS()+h3->GetRMSError());
   errorband->SetPoint( 4, 110, h3->GetRMS()-h3->GetRMSError());
   errorband->SetPoint( 5, 80, h2->GetRMS()-h2->GetRMSError());
   errorband->SetPoint( 6, 50, h1->GetRMS()-h1->GetRMSError());
   errorband->SetPoint( 7, 1, h0->GetRMS()+h0->GetRMSError());
   errorband->Draw("F same");
   c1->SaveAs("/Users/chiu.i-huan/Desktop/beamstudy_X.pdf");


   TCanvas*c2=new TCanvas("kk","kk",1000,800);
   c2->SetGridy();
   hy_take->Draw("hist p ");
   hy->Draw("hist p same");
   TLegend* leg2 = new TLegend(.20,.75,.38,.88);
   leg2->SetFillColor(0);
   leg2->SetLineColor(0);
   leg2->SetBorderSize(0);
   leg2->AddEntry(hy_take,  "Exp.", "p");
   leg2->AddEntry(hy,  "Sim.", "p");
   leg2->Draw("same");
   line=new TLine(1,h0_y->GetRMS(),50,h1_y->GetRMS());line->SetLineColor(2);line->SetLineStyle(2);line->Draw("same");
   line=new TLine(50,h1_y->GetRMS(),80,h2_y->GetRMS());line->SetLineColor(2);line->SetLineStyle(2);line->Draw("same");
   line=new TLine(80,h2_y->GetRMS(),110,h3_y->GetRMS());line->SetLineColor(2);line->SetLineStyle(2);line->Draw("same");
   line=new TLine(50,12,80,15);line->SetLineColor(1);line->SetLineStyle(2);line->Draw("same");
   line=new TLine(80,15,110,23);line->SetLineColor(1);line->SetLineStyle(2);line->Draw("same");
   TGraph* errorband2 = new TGraph();
   errorband2->SetLineWidth(0);
   errorband2->SetLineColorAlpha(91,0.3);
   errorband2->SetFillColorAlpha(91,0.3);
   errorband2->SetPoint( 0, 1, h0_y->GetRMS()+h0_y->GetRMSError());
   errorband2->SetPoint( 1, 50, h1_y->GetRMS()+h1_y->GetRMSError());
   errorband2->SetPoint( 2, 80, h2_y->GetRMS()+h2_y->GetRMSError());
   errorband2->SetPoint( 3, 110, h3_y->GetRMS()+h3_y->GetRMSError());
   errorband2->SetPoint( 4, 110, h3_y->GetRMS()-h3_y->GetRMSError());
   errorband2->SetPoint( 5, 80, h2_y->GetRMS()-h2_y->GetRMSError());
   errorband2->SetPoint( 6, 50, h1_y->GetRMS()-h1_y->GetRMSError());
   errorband2->SetPoint( 7, 1, h0_y->GetRMS()+h0_y->GetRMSError());
   errorband2->Draw("F same");
   c2->SaveAs("/Users/chiu.i-huan/Desktop/beamstudy_Y.pdf");
}
