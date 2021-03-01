void beam_sigma(){

   
   TH1D*hx_take = new TH1D("","",120,0,120);
   TH1D*hy_take = new TH1D("","",120,0,120);
   hx_take->Fill(50,1.6);
   hx_take->Fill(80,2.6);
   hx_take->Fill(110,4.3);
   hy_take->Fill(50,12);
   hy_take->Fill(80,15);
   hy_take->Fill(110,23);

   int n_samples=7;
   TH1D* hx[n_samples];
   TH1D* hy[n_samples];
//   for (int i = 0; i<n_samples; i++){
//   hx[i]->SetDirectory(0);
//   hy[i]->SetDirectory(0);
//   }
   Double_t sigma[n_samples][5][2];
   TH1D* h1;
   TH1D* h2;
   TH1D* h3;
   TH1D* h4;
   TH1D* h5;
   TH1D* h1y;
   TH1D* h2y;
   TH1D* h3y;
   TH1D* h4y;
   TH1D* h5y;
   TTree* t;
   TFile* f;
   TLegend* leg = new TLegend(.70,.25,.88,.48);
   leg->SetFillColor(0);
   leg->SetLineColor(0);
   leg->SetBorderSize(0);
   TLegend* leg2 = new TLegend(.20,.65,.38,.88);
   leg2->SetFillColor(0);
   leg2->SetLineColor(0);
   leg2->SetBorderSize(0);
   char name[50]="";
   TLine* line;

   for (int i = 0; i<n_samples; i++){
      if (i == 0) f= new TFile("Output_none_none.root","read");
      if (i == 1) f= new TFile("Output_kap100_al25.root","read");
      if (i == 2) f= new TFile("Output_kap75_cu20.root","read");
      if (i == 3) f= new TFile("Output_kap75_cu10.root","read");
      if (i == 4) f= new TFile("Output_kap75_kap12p5.root","read");
      if (i == 5) f= new TFile("Output_al150_al25.root","read");
      if (i == 6) f= new TFile("Output_al150_al20.root","read");
      t = (TTree*)f->Get("tree");
      t->Draw("    muInitX>>h1","muInitX!=-1000","");
      t->Draw("muFoil1PolX>>h2","muFoil1PolX!=-1000","");
      t->Draw("muFoil2PolX>>h3","muFoil2PolX!=-1000","");
      t->Draw("muTargetPolX>>h4","muTargetPolX!=-1000","");
      t->Draw("muTarget2PolX>>h5","muTarget2PolX!=-1000","");
      h1=(TH1D*)gDirectory->Get("h1");
      h2=(TH1D*)gDirectory->Get("h2");
      h3=(TH1D*)gDirectory->Get("h3");
      h4=(TH1D*)gDirectory->Get("h4");
      h5=(TH1D*)gDirectory->Get("h5");
      sigma[i][0][0]=h1->GetRMS();
      sigma[i][1][0]=h2->GetRMS();
      sigma[i][2][0]=h3->GetRMS();
      sigma[i][3][0]=h4->GetRMS();
      sigma[i][4][0]=h5->GetRMS();
      t->Draw("    muInitY>>h1y","muInitY!=-1000","");
      t->Draw("muFoil1PolY>>h2y","muFoil1PolY!=-1000","");
      t->Draw("muFoil2PolY>>h3y","muFoil2PolY!=-1000","");
      t->Draw("muTargetPolY>>h4y","muTargetPolY!=-1000","");
      t->Draw("muTarget2PolY>>h5y","muTarget2PolY!=-1000","");
      h1y=(TH1D*)gDirectory->Get("h1y");
      h2y=(TH1D*)gDirectory->Get("h2y");
      h3y=(TH1D*)gDirectory->Get("h3y");
      h4y=(TH1D*)gDirectory->Get("h4y");
      h5y=(TH1D*)gDirectory->Get("h5y");
      sigma[i][0][1]=h1y->GetRMS();
      sigma[i][1][1]=h2y->GetRMS();
      sigma[i][2][1]=h3y->GetRMS();
      sigma[i][3][1]=h4y->GetRMS();
      sigma[i][4][1]=h5y->GetRMS();

      sprintf(name,"hx_%d",i);
      hx[i]=new TH1D(name,name,260,-5,255);
      hx[i]->Fill(1,sigma[i][0][0]);
      hx[i]->Fill(5,sigma[i][1][0]);
      hx[i]->Fill(50,sigma[i][2][0]);
      hx[i]->Fill(90,sigma[i][3][0]);
      hx[i]->Fill(250,sigma[i][4][0]);
      hx[i]->SetStats(0);
      hx[i]->SetMarkerStyle(20);
      hx[i]->SetMarkerSize(1.5);
      hx[i]->SetMarkerColor(i+1);
      hx[i]->SetTitle("#sigma_{x} of Beam vs. Z [mm]");
      hx[i]->GetXaxis()->SetTitle("Z [mm]"); 
      hx[i]->GetYaxis()->SetTitle("#sigma_{x} /mm");
      hx[i]->GetYaxis()->CenterTitle();

      sprintf(name,"hy_%d",i);
      hy[i]=new TH1D(name,name,260,-5,255);
      hy[i]->Fill(1,sigma[i][0][1]);
      hy[i]->Fill(5,sigma[i][1][1]);
      hy[i]->Fill(50,sigma[i][2][1]);
      hy[i]->Fill(90,sigma[i][3][1]);
      hy[i]->Fill(250,sigma[i][4][1]);
      hy[i]->SetStats(0);
      hy[i]->SetMarkerStyle(20);
      hy[i]->SetMarkerSize(1.5);
      hy[i]->SetMarkerColor(i+1);
      hy[i]->SetTitle("#sigma_{y} of Beam vs. Z [mm]");
      hy[i]->GetXaxis()->SetTitle("Z [mm]");
      hy[i]->GetYaxis()->SetTitle("#sigma_{y} /mm");
      hy[i]->GetYaxis()->CenterTitle();

      sprintf(name,"Case %d",i+1);
      leg->AddEntry(hx[i],  name, "p");
      leg2->AddEntry(hx[i],  name, "p");
   }

   TCanvas*c1=new TCanvas("aa","aa",1000,800);
   c1->SetGridy();
   double binmax=0;
   for (int i = 0; i<n_samples; i++){
     if(sigma[i][4][0] > binmax) binmax = sigma[i][4][0];
   }
   hx[0]->SetMaximum(binmax*1.2);
   hx[0]->Draw("hist p");
   for (int i = 1; i<n_samples; i++)hx[i]->Draw("hist p same");
   for (int i = 0; i<n_samples; i++){
      for (int k =0; k<4;k++){ 
         if(k == 0) line=new TLine(1,sigma[i][k][0], 5,sigma[i][k+1][0]);
         if(k == 1) line=new TLine(5,sigma[i][k][0], 50,sigma[i][k+1][0]);
         if(k == 2) line=new TLine(50,sigma[i][k][0], 90,sigma[i][k+1][0]);
         if(k == 3) line=new TLine(90,sigma[i][k][0], 250,sigma[i][k+1][0]);
         line->SetLineColor(i+1);
         line->Draw("same");
   }}
   leg2->Draw("same");
   c1->SaveAs("/Users/chiu.i-huan/Desktop/D2_beamsigma_X.pdf");

   TCanvas*c2=new TCanvas("kk","kk",1000,800);
   c2->SetGridy();
   binmax=0;
   for (int i = 0; i<n_samples; i++){
     if(sigma[i][4][1] > binmax) binmax = sigma[i][4][1];
   }
   hy[0]->SetMaximum(binmax*1.2);
   hy[0]->Draw("hist p");
   for (int i = 1; i<n_samples; i++)hy[i]->Draw("hist p same");
   for (int i = 0; i<n_samples; i++){
      for (int k =0; k<4;k++){ 
         if(k == 0) line=new TLine(1,sigma[i][k][1], 5,sigma[i][k+1][1]);
         if(k == 1) line=new TLine(5,sigma[i][k][1], 50,sigma[i][k+1][1]);
         if(k == 2) line=new TLine(50,sigma[i][k][1], 90,sigma[i][k+1][1]);
         if(k == 3) line=new TLine(90,sigma[i][k][1], 250,sigma[i][k+1][1]);
         line->SetLineColor(i+1);
         line->Draw("same");
   }}
   leg->Draw("same");
   c2->SaveAs("/Users/chiu.i-huan/Desktop/D2_beamsigma_Y.pdf");
}
