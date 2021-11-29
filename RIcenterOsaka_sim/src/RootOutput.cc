#include "RootOutput.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "Parameters.hh"

RootOutput::RootOutput() {
  TTree::SetMaxTreeSize(100000000000LL);      // Set maximum size of the tree file            
  pointerToRoot=this;
}

RootOutput::~RootOutput() {}

RootOutput* RootOutput::pointerToRoot=0;
RootOutput* RootOutput::GetRootInstance() {
  return pointerToRoot;
}

void RootOutput::BeginOfRunAction() {
   ResponseFile = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ge_ResResponse.root", "read");
   for (int i=0;i<6;i++){
      sprintf(GeName, "graph_Ge%d", i+1);
      gr_Ge[i]= (TGraph*)ResponseFile->Get(GeName);
   }

   // get parameter
   std::stringstream ss;ss.str("test");
   if (strcmp(Parameters::mySteeringFileName,"Unset")!=0){
      char charSteeringFileName[1000]; strcpy(charSteeringFileName,(Parameters::mySteeringFileName).c_str());
      FILE *fSteeringFile=fopen(charSteeringFileName,"r");
      char  line[501];
      G4cout << "\n\n....oooOO0OOooo........oooOO0OOooo......Configuration file used for this run....oooOO0OOooo........oooOO0OOooo......"<<G4endl;
      while (!feof(fSteeringFile)) {
         fgets(line,500,fSteeringFile);
         if(line[0]=='#/') continue;
         G4cout << line;
         if ((line[0]=='#')||(line[0]=='\n')||(line[0]=='\r')) continue;
         char tmpString0[100]="Unset", tmpString1[100]="Unset";
         sscanf(&line[0],"%s %s",tmpString0,tmpString1);
         if (strcmp(tmpString0,"/command/rootOutput")==0) ss << tmpString1;
      }
      G4cout <<"....oooOO0OOooo........oooOO0OOooo......End of the configuration file.....oooOO0OOooo........oooOO0OOooo......\n\n"<<G4endl;
   }
   auto RootOutputFileName = "./Output_"+ss.str()+".root";
   rootFile = new TFile(RootOutputFileName.c_str(), "recreate");

   G4cout << " RE-create file ! " <<G4endl; 
   rootFile->cd();

   rootTree = new TTree("tree","Germanium detector analysis");
   muonTree = new TTree("mutree","muon beam analysis");

   muonTree->Branch("eventID",&eventID,"eventID/I");
   muonTree->Branch("runID",&runID,"runID/I"); 
   muonTree->Branch("RunTime",&RunTime,"RunTime/D"); 

   // ===== detector info. =====
   rootTree->Branch("InitEnergy",&InitEnergy,"InitEnergy/D");//nsiganle in a event
   rootTree->Branch("nSignals",&nSignals,"nSignals/I");//nsiganle in a event
   rootTree->Branch("Det_ID",&Det_ID,"Det_ID[nSignals]/I");//nsiganle in a event
   rootTree->Branch("Hit_Energy",&hit_energy,"Hit_Energy[nSignals]/D");
   rootTree->Branch("Hit_Energy_Reso",&hit_energy_reso,"Hit_Energy_Reso[nSignals]/D"); //MeV
   rootTree->Branch("Hit_Start_X",&hit_startx,"Hit_Start_X[nSignals]/D");
   rootTree->Branch("Hit_Start_Y",&hit_starty,"Hit_Start_Y[nSignals]/D");
   rootTree->Branch("Hit_Start_Z",&hit_startz,"Hit_Start_Z[nSignals]/D");
//   rootTree->Branch("Hit_Time_Start",&hit_timestart,"Hit_Time_Start[nSignals]/D");
//   rootTree->Branch("Hit_Time_End",&hit_timeend,"Hit_Time_End[nSignals]/D");
//   rootTree->Branch("Hit_Nsteps",&hit_nsteps,"Hit_Nsteps[nSignals]/I");
//   rootTree->Branch("Hit_Length",&hit_length,"Hit_Length[nSignals]/D");
//   rootTree->Branch("Hit_pdgId",&hit_pdgId,"Hit_pdgId[nSignals]/I");
//   rootTree->Branch("Hit_ProcessID",&hit_process,"Hit_ProcessID[nSignals]/I");
//   rootTree->Branch("Stop_VolumeID",&Stop_Volume,"Stop_VolumeID/I");//muon hit 

   h1_init_energy = new TH1F("h1_init_energy","Init. input energy [keV]",6800,10,180);
   h1_ge_energy = new TH1F("h1_ge_energy","Deposite energy [keV]",6800,10,180);
   h1_ge_energy_reso = new TH1F("h1_ge_energy_reso","Deposite energy with resolution [keV]",6800,10,180);
}

void RootOutput::EndOfRunAction() {
  G4cout<<"RootOutput::EndOfRunAction() - Write Tree "<<G4endl;
  rootTree->Write();
  h1_init_energy->Write();
  h1_ge_energy->Write();
  h1_ge_energy_reso->Write();
//  muonTree->Write();//check beam profile (file size will be huge)
  rootFile->Close();
  G4cout<<Form("RootOutput::EndOfRunAction() - Root tree written out in %s",rootFile->GetName())<<G4endl;
}

void RootOutput::FillEvent() {
  double total_E = 0;
  for (int i = 0; i < nSignals; i++) total_E+=hit_energy[i];
  if(total_E*1000 > 3) rootTree->Fill();//only fill event with energy deposit larger than 3keV base on features of HPGe
//  muonTree->Fill();// check beam profile (file size will be huge)
}

void RootOutput::SetDetectorInfo (G4double edep, G4double edep_e, G4double edep_gamma, G4double edep_other, G4double time){
  eDep =  edep;
  eDep_e =  edep_e;
  eDep_gamma =  edep_gamma;
  eDep_other =  edep_other;
  egamma_hit_time =  time;
}

void RootOutput::SetEnergyResolution (){
   for (int i = 0; i < nSignals; i++){
      if(hit_energy[i] != 0){
//         std::cout << " E [keV]: " << hit_energy[i]*1000. << " sigma : " << gr_Ge[Det_ID[i]-1]->Eval(hit_energy[i]*1000.)/1000. << std::endl;
         double Esigma=gr_Ge[Det_ID[i]-1]->Eval(hit_energy[i]*1000.)/1000.;
         hit_energy_reso[i]=G4RandGauss::shoot(hit_energy[i],Esigma);//unit for G4(Gr_Ge) is [MeV]([keV])
//         hit_energy_reso[i]=G4RandGauss::shoot(hit_energy[i],(0.152/1000. + (hit_energy[i]-0.024)*0.0010545454545454547));
      }else{
         hit_energy_reso[i]=hit_energy[i];
      }
      h1_ge_energy->Fill(hit_energy[i]*1000);
      h1_ge_energy_reso->Fill(hit_energy_reso[i]*1000);
   }//loop signals in det.
}

void RootOutput::ClearAllRootVariables() {
     Stop_Volume = -1000;
     eDep = 0.;eDep_e = 0.; eDep_gamma = 0.; eDep_other = 0.;
     hitparticle = "None"; 
     egamma_hit_time = -1000.;
     runID=-1000;
     eventID=-1000;
     RunTime=-1000;

     KineticE = -1000; TotalE = -1000; DepositeE = -1000; pdgId = -1000;
     Det_X = -1000; Det_Y = -1000; Det_Z = -1000;
     Ngamma=-1000;
     Neletron=-1000;
     Nneutron=-1000;  
     Nother=-1000;  

     muSamplePolX=-1000; muSamplePolY=-1000; muSamplePolZ=-1000;
     muWorldPolX=-1000; muWorldPolY=-1000; muWorldPolZ=-1000;
     muFoil1PolX=-1000; muFoil1PolY=-1000; muFoil1PolZ=-1000;
     muFoil2PolX=-1000; muFoil2PolY=-1000; muFoil2PolZ=-1000;
     muFoil3PolX=-1000; muFoil3PolY=-1000; muFoil3PolZ=-1000;
     muInter1PolX=-1000; muInter1PolY=-1000; muInter1PolZ=-1000;
     muInter2PolX=-1000; muInter2PolY=-1000; muInter2PolZ=-1000;
     muTargetPolX=-1000; muTargetPolY=-1000; muTargetPolZ=-1000;
     muTarget2PolX=-1000; muTarget2PolY=-1000; muTarget2PolZ=-1000;
     muSampleMomX=-1000; muSampleMomY=-1000; muSampleMomZ=-1000;
     muWorldMomX=-1000; muWorldMomY=-1000; muWorldMomZ=-1000;
     muFoil1MomX=-1000; muFoil1MomY=-1000; muFoil1MomZ=-1000;
     muFoil2MomX=-1000; muFoil2MomY=-1000; muFoil2MomZ=-1000;
     muFoil3MomX=-1000; muFoil3MomY=-1000; muFoil3MomZ=-1000;
     muInter1MomX=-1000; muInter1MomY=-1000; muInter1MomZ=-1000;
     muInter2MomX=-1000; muInter2MomY=-1000; muInter2MomZ=-1000;
     muTargetMomX=-1000; muTargetMomY=-1000; muTargetMomZ=-1000;
     muTarget2MomX=-1000; muTarget2MomY=-1000; muTarget2MomZ=-1000;
     muSampleTime=-1000; muWorldTime=-1000; muFoil1Time=-1000; muFoil2Time=-1000; muFoil3Time=-1000; muInter1Time=-1000; muInter2Time=-1000; muTargetTime=-1000; muTarget2Time=-1000;
     muSampleKineticEnergy=-1000; muWorldKineticEnergy=-1000; muFoil1KineticEnergy=-1000; muFoil2KineticEnergy=-1000; muFoil3KineticEnergy=-1000; muInter1KineticEnergy=-1000; muInter2KineticEnergy=-1000; muTargetKineticEnergy=-1000; muTarget2KineticEnergy=-1000;

     muSampleEndPolX=-1000; muSampleEndPolY=-1000; muSampleEndPolZ=-1000;
     muWorldEndPolX=-1000; muWorldEndPolY=-1000; muWorldEndPolZ=-1000;
     muInter1EndPolX=-1000; muInter1EndPolY=-1000; muInter1EndPolZ=-1000;
     muInter2EndPolX=-1000; muInter2EndPolY=-1000; muInter2EndPolZ=-1000;
     muSampleEndMomX=-1000; muSampleEndMomY=-1000; muSampleEndMomZ=-1000;
     muWorldEndMomX=-1000; muWorldEndMomY=-1000; muWorldEndMomZ=-1000;
     muInter1EndMomX=-1000; muInter1EndMomY=-1000; muInter1EndMomZ=-1000;
     muInter2EndMomX=-1000; muInter2EndMomY=-1000; muInter2EndMomZ=-1000;
     muSampleEndTime=-1000; muWorldEndTime=-1000; muInter1EndTime=-1000; muInter2EndTime=-1000;
     muSampleEndKineticEnergy=-1000; muWorldEndKineticEnergy=-1000; muInter1EndKineticEnergy=-1000; muInter2EndKineticEnergy=-1000;

     muDecayPolX = -1000; muDecayPolY = -1000; muDecayPolZ = -1000; muDecayTime = -1000;
     Hit_Time = -1000;
     muInitX = -1000; muInitY = -1000;muInitZ = -1000;muInitpX = -1000;muInitpY = -1000;muInitpZ = -1000;  
     eInitX = -1000; eInitY = -1000;eInitZ = -1000;eInitpX = -1000;eInitpY = -1000;eInitpZ = -1000;  
     muInitTime = -1000;

     InitEnergy=0.;
     for (int i = 0; i < nhitMax; i++){
      Det_ID[i]=0;
      hit_energy[i] = 0.;
      hit_energy_reso[i] = 0.;
      hit_startx[i] = 0.;
      hit_starty[i] = 0.;
      hit_startz[i] = 0.;
      hit_timestart[i] = 0.;
      hit_timeend[i] = 0.;
      hit_nsteps[i] = 0;
      hit_length[i] = 0.;
      hit_pdgId[i] = 0;
      hit_process[i] = 0;
     }
}
