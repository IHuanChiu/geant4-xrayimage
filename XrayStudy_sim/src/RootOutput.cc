#include "RootOutput.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

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
   sprintf(RootOutputFileName, "./Output_%s.root", "1");
   rootFile = new TFile(RootOutputFileName, "recreate");
   G4cout << " RE-create file ! " <<G4endl; 
   rootFile->cd();
//   if(rootFile->IsZombie()) { 
//    char message[200];
//    sprintf(message,"musrRootOutput::BeginOfRunAction() Root output file %s can not be created",RootOutputFileName);
//    std::cout << message << std::endl;
//   }else rootFile->cd();

   rootTree = new TTree("tree","CdTe detector analysis");
   muonTree = new TTree("muontree","Germanium simulation for particles");

   muonTree->Branch("eventID",&eventID,"eventID/I");
   muonTree->Branch("runID",&runID,"runID/I"); 
   muonTree->Branch("RunTime",&RunTime,"RunTime/D"); 
   // ===== muon position/hit =====
   muonTree->Branch("muSampleTime",&muSampleTime,"muSampleTime/D");
   muonTree->Branch("muSampleKineticEnergy",&muSampleKineticEnergy,"muSampleKineticEnergy/D");
   muonTree->Branch("muSamplePolX",&muSamplePolX,"muSamplePolX/D");
   muonTree->Branch("muSamplePolY",&muSamplePolY,"muSamplePolY/D");
   muonTree->Branch("muSamplePolZ",&muSamplePolZ,"muSamplePolZ/D");
   muonTree->Branch("muSampleMomX",&muSampleMomX,"muSampleMomX/D");
   muonTree->Branch("muSampleMomY",&muSampleMomY,"muSampleMomY/D");
   muonTree->Branch("muSampleMomZ",&muSampleMomZ,"muSampleMomZ/D");
   muonTree->Branch("muSampleEndTime",&muSampleEndTime,"muSampleEndTime/D");
   muonTree->Branch("muSampleEndKineticEnergy",&muSampleEndKineticEnergy,"muSampleEndKineticEnergy/D");
   muonTree->Branch("muSampleEndPolX",&muSampleEndPolX,"muSampleEndPolX/D");
   muonTree->Branch("muSampleEndPolY",&muSampleEndPolY,"muSampleEndPolY/D");
   muonTree->Branch("muSampleEndPolZ",&muSampleEndPolZ,"muSampleEndPolZ/D");
   muonTree->Branch("muSampleEndMomX",&muSampleEndMomX,"muSampleEndMomX/D");
   muonTree->Branch("muSampleEndMomY",&muSampleEndMomY,"muSampleEndMomY/D");
   muonTree->Branch("muSampleEndMomZ",&muSampleEndMomZ,"muSampleEndMomZ/D");

//   rootTree->Branch("nSignals",&nSignals,"nSignals/I");//nsiganle in a event
//   rootTree->Branch("Hit_Energy",&hit_energy,"Hit_Energy[nSignals]/D"); 
//   rootTree->Branch("Hit_Energy_Reso",&hit_energy_reso,"Hit_Energy_Reso[nSignals]/D"); //MeV 
//   rootTree->Branch("Hit_Start_X",&hit_startx,"Hit_Start_X[nSignals]/D");
//   rootTree->Branch("Hit_Start_Y",&hit_starty,"Hit_Start_Y[nSignals]/D");
//   rootTree->Branch("Hit_Start_Z",&hit_startz,"Hit_Start_Z[nSignals]/D");
//   rootTree->Branch("Hit_Time_Start",&hit_timestart,"Hit_Time_Start[nSignals]/D");
//   rootTree->Branch("Hit_Time_End",&hit_timeend,"Hit_Time_End[nSignals]/D");
//   rootTree->Branch("Hit_Nsteps",&hit_nsteps,"Hit_Nsteps[nSignals]/I");
//   rootTree->Branch("Hit_Length",&hit_length,"Hit_Length[nSignals]/D");
//   rootTree->Branch("Hit_pdgId",&hit_pdgId,"Hit_pdgId[nSignals]/I");
//   rootTree->Branch("Hit_ProcessID",&hit_process,"Hit_ProcessID[nSignals]/I");
//   rootTree->Branch("Stop_VolumeID",&Stop_Volume,"Stop_VolumeID/I");//muon hit

   // Terada Xray 
   rootTree->Branch("direction_id",&direction_id,"direction_id/I"); 
   rootTree->Branch("energy_init",&init_energy,"energy_init/D"); 
   rootTree->Branch("detid",&Ge_detid,"detid/I"); 
   rootTree->Branch("energy",&input_energy,"energy/D"); 

   h1_process = new TH1F("hit_process","Process of Signal",nh1bin,0,nh1bin);
   for (int i=1;i<=nh1bin;i++) h1_process->GetXaxis()->SetBinLabel(i,var_name[i-1]); 

   // ===== hist. info. =====
   h1_StopVol = new TH1F("h1_StopVol","Name of Volume",nh1bin+5,-5,nh1bin);

   // ===== Energy resolution =====
   reso_init = 0.14896551724137927/1000.;//14keV
   reso_rate = 0.0006769509981851182/1000.;

   G4cout << "RootOutput::BeginOfRunAction()  The Root tree and branches were defined."<<G4endl;
}

void RootOutput::EndOfRunAction() {
  G4cout<<"RootOutput::EndOfRunAction() - Write Tree "<<G4endl;
  rootTree->Write();
//  muonTree->Write();//TODO for muon stop position
  rootFile->Close();
  G4cout<<"RootOutput::EndOfRunAction() - Root tree written out."<<G4endl;
}

void RootOutput::FillEvent() {
  double total_E = 0;
  for (int i = 0; i < nSignals; i++) total_E+=hit_energy[i];
//  if(total_E != 0) rootTree->Fill();//only fill event with energy deposit
  muonTree->Fill();
   if(Stop_Volume != -1000) h1_StopVol->Fill(Stop_Volume);
}

void RootOutput::FillXrays() {
  rootTree->Fill();
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
//      hit_energy_reso[i]= G4RandGauss::shoot(hit_energy[i],(reso_init + hit_energy[i]*reso_rate));
      hit_energy_reso[i]= G4RandGauss::shoot(hit_energy[i],(0.152/1000. + (hit_energy[i]-0.024)*0.0010545454545454547));
   }
}

void RootOutput::ClearAllRootVariables() {
  Stop_Volume = -1000;
  eDep = 0.;eDep_e = 0.; eDep_gamma = 0.; eDep_other = 0.;
  Track_Name = "None";
  Track_Process = "None";
  egamma_hit_time = -1000.;
  runID=-1000;
  eventID=-1000;
  RunTime=-1000;
  for (int j = 0; j < 6; j++){
   muCdTeTime[j] = -1000; muCdTePolX[j] = -1000; muCdTePolY[j] = -1000; muCdTePolZ[j] = -1000;
   muCdTeKineticEnergy[j] = -1000; muCdTeEndKineticEnergy[j] = -1000;
   muCdTeEndTime[j] = -1000; muCdTeEndPolZ[j] = -1000; muCdTeEndPolY[j] = -1000; muCdTeEndPolX[j]  = -1000;
  }
   KineticE = -1000; TotalE = -1000; DepositE = -1000; pdgId = -1000;
   Det_X = -1000; Det_Y = -1000; Det_Z = -1000;
   Ngamma=-1000;
   Neletron=-1000;
   Nneutron=-1000;  
   Nother=-1000;  
  muSampleTime = -1000; muWorldTime = -1000; muKaptonTime = -1000; muTargetTime = -1000;
  muSampleKineticEnergy = -1000; muWorldKineticEnergy = -1000;
  muSamplePolX = -1000; muSamplePolY = -1000; muSamplePolZ = -1000; 
  muKaptonKineticEnergy = -1000; muTargetKineticEnergy = -1000;
  muWorldPolX = -1000; muWorldPolY = -1000; muWorldPolZ = -1000;
  muKaptonPolX = -1000; muKaptonPolY = -1000; muKaptonPolZ = -1000;
  muSampleEndTime = -1000; muWorldEndTime = -1000; muTargetEndTime = -1000;
  muTargetPolX = -1000; muTargetPolY = -1000; muTargetPolZ = -1000;
  muSampleEndKineticEnergy = -1000; muWorldEndKineticEnergy = -1000; muKaptonEndKineticEnergy = -1000; muTargetEndTime = -1000;
  muSampleEndPolX = -1000; muSampleEndPolY = -1000; muSampleEndPolZ = -1000;
  muWorldEndPolX = -1000; muWorldEndPolY = -1000; muWorldEndPolZ = -1000;
  muKaptonEndPolX = -1000; muKaptonEndPolY = -1000; muKaptonEndPolZ = -1000;
  muTargetEndPolX = -1000; muTargetEndPolY = -1000; muTargetEndPolZ = -1000;
  muDecayPolX = -1000; muDecayPolY = -1000; muDecayPolZ = -1000; muDecayTime = -1000;
  Hit_Time = -1000;
  muSampleMomX = -1000; muSampleMomY = -1000; muSampleMomZ = -1000; 
  muWorldMomX = -1000; muWorldMomY = -1000; muWorldMomZ = -1000;
  muKaptonMomX = -1000; muKaptonMomY = -1000; muKaptonMomZ = -1000;
  muSampleEndMomX = -1000; muSampleEndMomY = -1000; muSampleEndMomZ = -1000;
  muWorldEndMomX = -1000; muWorldEndMomY = -1000; muWorldEndMomZ = -1000;
  muKaptonEndMomX = -1000; muKaptonEndMomY = -1000; muKaptonEndMomZ = -1000;
  muTargetEndMomX = -1000; muTargetEndMomY = -1000; muTargetEndMomZ = -1000;
  muTargetMomX = -1000; muTargetMomY = -1000; muTargetMomZ = -1000;
  muInitX = -1000; muInitY = -1000;muInitZ = -1000;muInitpX = -1000;muInitpY = -1000;muInitpZ = -1000;  
  eInitX = -1000; eInitY = -1000;eInitZ = -1000;eInitpX = -1000;eInitpY = -1000;eInitpZ = -1000;  
  muInitTime = -1000;

  for (int i = 0; i < nhitMax; i++){
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
  init_energy = -100.;
  input_energy = -100.;
  Ge_detid = -1;
}
