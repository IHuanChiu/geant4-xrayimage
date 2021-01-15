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

   rootTree = new TTree("tree","Ge detector");
   SciTree = new TTree("scitree","Scintillator detector");
   TrackTree = new TTree("tracktree","Germanium simulation for particles");

   rootTree->Branch("eventID",&eventID,"eventID/I");
   rootTree->Branch("runID",&runID,"runID/I"); 
   rootTree->Branch("RunTime",&RunTime,"RunTime/D"); 
   // ===== muon position/hit =====
   rootTree->Branch("muSampleTime",&muSampleTime,"muSampleTime/D");
   rootTree->Branch("muSampleKineticEnergy",&muSampleKineticEnergy,"muSampleKineticEnergy/D");
   rootTree->Branch("muSamplePolX",&muSamplePolX,"muSamplePolX/D");
   rootTree->Branch("muSamplePolY",&muSamplePolY,"muSamplePolY/D");
   rootTree->Branch("muSamplePolZ",&muSamplePolZ,"muSamplePolZ/D");
   rootTree->Branch("muSampleMomX",&muSampleMomX,"muSampleMomX/D");
   rootTree->Branch("muSampleMomY",&muSampleMomY,"muSampleMomY/D");
   rootTree->Branch("muSampleMomZ",&muSampleMomZ,"muSampleMomZ/D");
   rootTree->Branch("muSampleEndTime",&muSampleEndTime,"muSampleEndTime/D");
   rootTree->Branch("muSampleEndKineticEnergy",&muSampleEndKineticEnergy,"muSampleEndKineticEnergy/D");
   rootTree->Branch("muSampleEndPolX",&muSampleEndPolX,"muSampleEndPolX/D");
   rootTree->Branch("muSampleEndPolY",&muSampleEndPolY,"muSampleEndPolY/D");
   rootTree->Branch("muSampleEndPolZ",&muSampleEndPolZ,"muSampleEndPolZ/D");
   rootTree->Branch("muSampleEndMomX",&muSampleEndMomX,"muSampleEndMomX/D");
   rootTree->Branch("muSampleEndMomY",&muSampleEndMomY,"muSampleEndMomY/D");
   rootTree->Branch("muSampleEndMomZ",&muSampleEndMomZ,"muSampleEndMomZ/D");
//   rootTree->Branch("muCdTeTime",&muCdTeTime,"muCdTeTime[6]/D");
//   rootTree->Branch("muCdTeKineticEnergy",&muCdTeKineticEnergy,"muCdTeKineticEnergy[6]/D");
//   rootTree->Branch("muCdTePolX",&muCdTePolX,"muCdTePolX[6]/D");
//   rootTree->Branch("muCdTePolY",&muCdTePolY,"muCdTePolY[6]/D");
//   rootTree->Branch("muCdTePolZ",&muCdTePolZ,"muCdTePolZ[6]/D");
//   rootTree->Branch("muCdTeEndTime",&muCdTeEndTime,"muCdTeEndTime[6]/D");
//   rootTree->Branch("muCdTeEndKineticEnergy",&muCdTeEndKineticEnergy,"muCdTeEndKineticEnergy[6]/D");
//   rootTree->Branch("muCdTeEndPolX",&muCdTeEndPolX,"muCdTeEndPolX[6]/D");
//   rootTree->Branch("muCdTeEndPolY",&muCdTeEndPolY,"muCdTeEndPolY[6]/D");
//   rootTree->Branch("muCdTeEndPolZ",&muCdTeEndPolZ,"muCdTeEndPolZ[6]/D");
//   rootTree->Branch("muKaptonTime",&muKaptonTime,"muKaptonTime/D");
//   rootTree->Branch("muKaptonKineticEnergy",&muKaptonKineticEnergy,"muKaptonKineticEnergy/D");
//   rootTree->Branch("muKaptonPolX",&muKaptonPolX,"muKaptonPolX/D");
//   rootTree->Branch("muKaptonPolY",&muKaptonPolY,"muKaptonPolY/D");
//   rootTree->Branch("muKaptonPolZ",&muKaptonPolZ,"muKaptonPolZ/D");
//   rootTree->Branch("muKaptonMomX",&muKaptonMomX,"muKaptonMomX/D");
//   rootTree->Branch("muKaptonMomY",&muKaptonMomY,"muKaptonMomY/D");
//   rootTree->Branch("muKaptonMomZ",&muKaptonMomZ,"muKaptonMomZ/D");
//   rootTree->Branch("muKaptonEndTime",&muKaptonEndTime,"muKaptonEndTime/D");
//   rootTree->Branch("muKaptonEndKineticEnergy",&muKaptonEndKineticEnergy,"muKaptonEndKineticEnergy/D");
//   rootTree->Branch("muKaptonEndPolX",&muKaptonEndPolX,"muKaptonEndPolX/D");
//   rootTree->Branch("muKaptonEndPolY",&muKaptonEndPolY,"muKaptonEndPolY/D");
//   rootTree->Branch("muKaptonEndPolZ",&muKaptonEndPolZ,"muKaptonEndPolZ/D");
//   rootTree->Branch("muKaptonEndMomX",&muKaptonEndMomX,"muKaptonEndMomX/D");
//   rootTree->Branch("muKaptonEndMomY",&muKaptonEndMomY,"muKaptonEndMomY/D");
//   rootTree->Branch("muKaptonEndMomZ",&muKaptonEndMomZ,"muKaptonEndMomZ/D");
//   rootTree->Branch("muWorldTime",&muWorldTime,"muWorldTime/D");
//   rootTree->Branch("muWorldKineticEnergy",&muWorldKineticEnergy,"muWorldKineticEnergy/D");
//   rootTree->Branch("muWorldPolX",&muWorldPolX,"muWorldPolX/D");
//   rootTree->Branch("muWorldPolY",&muWorldPolY,"muWorldPolY/D");
//   rootTree->Branch("muWorldPolZ",&muWorldPolZ,"muWorldPolZ/D");
//   rootTree->Branch("muWorldMomX",&muWorldMomX,"muWorldMomX/D");
//   rootTree->Branch("muWorldMomY",&muWorldMomY,"muWorldMomY/D");
//   rootTree->Branch("muWorldMomZ",&muWorldMomZ,"muWorldMomZ/D");
//   rootTree->Branch("muWorldEndTime",&muWorldEndTime,"muWorldEndTime/D");
//   rootTree->Branch("muWorldEndKineticEnergy",&muWorldEndKineticEnergy,"muWorldEndKineticEnergy/D");
//   rootTree->Branch("muWorldEndPolX",&muWorldEndPolX,"muWorldEndPolX/D");
//   rootTree->Branch("muWorldEndPolY",&muWorldEndPolY,"muWorldEndPolY/D");
//   rootTree->Branch("muWorldEndPolZ",&muWorldEndPolZ,"muWorldEndPolZ/D");
//   rootTree->Branch("muWorldEndMomX",&muWorldEndMomX,"muWorldEndMomX/D");
//   rootTree->Branch("muWorldEndMomY",&muWorldEndMomY,"muWorldEndMomY/D");
//   rootTree->Branch("muWorldEndMomZ",&muWorldEndMomZ,"muWorldEndMomZ/D");
//   rootTree->Branch("muTargetTime",&muTargetTime,"muTargetTime/D");
//   rootTree->Branch("muTargetKineticEnergy",&muTargetKineticEnergy,"muTargetKineticEnergy/D");
//   rootTree->Branch("muTargetPolX",&muTargetPolX,"muTargetPolX/D");
//   rootTree->Branch("muTargetPolY",&muTargetPolY,"muTargetPolY/D");
//   rootTree->Branch("muTargetPolZ",&muTargetPolZ,"muTargetPolZ/D");
//   rootTree->Branch("muTargetMomX",&muTargetMomX,"muTargetMomX/D");
//   rootTree->Branch("muTargetMomY",&muTargetMomY,"muTargetMomY/D");
//   rootTree->Branch("muTargetMomZ",&muTargetMomZ,"muTargetMomZ/D");
//   rootTree->Branch("muTargetEndTime",&muTargetEndTime,"muTargetEndTime/D");
//   rootTree->Branch("muTargetEndKineticEnergy",&muTargetEndKineticEnergy,"muTargetEndKineticEnergy/D");
//   rootTree->Branch("muTargetEndPolX",&muTargetEndPolX,"muTargetEndPolX/D");
//   rootTree->Branch("muTargetEndPolY",&muTargetEndPolY,"muTargetEndPolY/D");
//   rootTree->Branch("muTargetEndPolZ",&muTargetEndPolZ,"muTargetEndPolZ/D");
//   rootTree->Branch("muTargetEndMomX",&muTargetEndMomX,"muTargetEndMomX/D");
//   rootTree->Branch("muTargetEndMomY",&muTargetEndMomY,"muTargetEndMomY/D");
//   rootTree->Branch("muTargetEndMomZ",&muTargetEndMomZ,"muTargetEndMomZ/D");
   rootTree->Branch("muDecayPolX",&muDecayPolX,"muDecayPolX/D");
   rootTree->Branch("muDecayPolY",&muDecayPolY,"muDecayPolY/D");
   rootTree->Branch("muDecayPolZ",&muDecayPolZ,"muDecayPolZ/D");
   rootTree->Branch("muDecayTime",&muDecayTime,"muDecayTime/D");
   rootTree->Branch("Stop_VolumeID",&Stop_Volume,"Stop_VolumeID/I");//muon hit
   rootTree->Branch("muInitX",&muInitX,"muInitX/D");//init muon 
   rootTree->Branch("muInitY",&muInitY,"muInitY/D");//init muon 
   rootTree->Branch("muInitZ",&muInitZ,"muInitZ/D");//init muon 
   rootTree->Branch("muInitpX",&muInitpX,"muInitpX/D");//init muon 
   rootTree->Branch("muInitpY",&muInitpY,"muInitpY/D");//init muon 
   rootTree->Branch("muInitpZ",&muInitpZ,"muInitpZ/D");//init muon 
   rootTree->Branch("muInitTime",&muInitTime,"muInitTime/D");//init muon 
   rootTree->Branch("eInitX",&eInitX,"eInitX/D");//init ele 
   rootTree->Branch("eInitY",&eInitY,"eInitY/D");//init ele 
   rootTree->Branch("eInitZ",&eInitZ,"eInitZ/D");//init ele 
   rootTree->Branch("eInitpX",&eInitpX,"eInitpX/D");//init ele 
   rootTree->Branch("eInitpY",&eInitpY,"eInitpY/D");//init ele 
   rootTree->Branch("eInitpZ",&eInitpZ,"eInitpZ/D");//init ele 

   rootTree->Branch("nSignals",&nSignals,"nSignals/I");//nsiganle in a event
   rootTree->Branch("Hit_Energy",&hit_energy,"Hit_Energy[nSignals]/D"); 
   rootTree->Branch("Hit_Energy_Reso",&hit_energy_reso,"Hit_Energy_Reso[nSignals]/D"); 
   rootTree->Branch("Hit_Time_Start",&hit_timestart,"Hit_Time_Start[nSignals]/D");
   rootTree->Branch("Hit_Time_End",&hit_timeend,"Hit_Time_End[nSignals]/D");
   rootTree->Branch("Hit_Nsteps",&hit_nsteps,"Hit_Nsteps[nSignals]/I");
   rootTree->Branch("Hit_Length",&hit_length,"Hit_Length[nSignals]/D");
   rootTree->Branch("Hit_pdgId",&hit_pdgId,"Hit_pdgId[nSignals]/I");
   rootTree->Branch("Hit_ProcessID",&hit_process,"Hit_ProcessID[nSignals]/I");

   // ===== detector info. =====
//   rootTree->Branch("energyDep",&eDep,"energyDep/D");
//   rootTree->Branch("energyDep_e",&eDep_e,"energyDep_e/D");
//   rootTree->Branch("energyDep_gamma",&eDep_gamma,"energyDep_gamma/D");
//   rootTree->Branch("energyDep_other",&eDep_other,"energyDep_other/D");
//   rootTree->Branch("egamma_hit_time",&egamma_hit_time,"egamma_hit_time/D");
//
//   rootTree->Branch("Ngamma",&Ngamma,"Ngamma/I");
//   rootTree->Branch("Neletron",&Neletron,"Neletron/I");
//   rootTree->Branch("Nneutron",&Nneutron,"Nneutron/I");
//   rootTree->Branch("Nother",&Nother,"Nother/I");
   SciTree->Branch("n_electrons_up",&n_electrons_up,"n_electrons_up/I");
   SciTree->Branch("n_photons_up",&n_photons_up,"n_photons_up/I");
   SciTree->Branch("n_electrons_down",&n_electrons_down,"n_electrons_down/I");
   SciTree->Branch("n_photons_donw",&n_photons_down,"n_photons_donw/I");

   // ===== track info. =====
   TrackTree->Branch("pdgId",&pdgId,"pdgId/I");
   TrackTree->Branch("kinetic_E",&KineticE,"kinetic_E/D");
   TrackTree->Branch("Total_E",&TotalE,"Total_E/D");
   TrackTree->Branch("Deposit_E",&DepositE,"Deposit_E/D");
   TrackTree->Branch("Detector_X",&Det_X,"Detector_X/D");
   TrackTree->Branch("Detector_Y",&Det_Y,"Detector_Y/D");
   TrackTree->Branch("Detector_Z",&Det_Z,"Detector_Z/D");
   TrackTree->Branch("Track_Name",&Track_Name);
   TrackTree->Branch("Track_Process",&Track_Process);
//   TrackTree->Branch("Hit_Time",&Hit_Time,"Hit_Time/D");

   h1_process = new TH1F("hit_process","Process of Signal",nh1bin,0,nh1bin);
   for (int i=1;i<=nh1bin;i++) h1_process->GetXaxis()->SetBinLabel(i,var_name[i-1]);

   // ===== Energy resolution =====
   reso_14keV = 0.000014*2;//14eV, 2%
   reso_75keV = 0.000075*2;//75eV
   reso_rate = (reso_75keV-reso_14keV)/(75-14); 

   G4cout << "RootOutput::BeginOfRunAction()  The Root tree and branches were defined."<<G4endl;
}

void RootOutput::EndOfRunAction() {
  G4cout<<"RootOutput::EndOfRunAction() - Write Tree "<<G4endl;
  rootTree->Write();
  SciTree->Write();
  TrackTree->Write();
  h1_process->Write();
  rootFile->Close();
  G4cout<<"RootOutput::EndOfRunAction() - Root tree written out."<<G4endl;
}

void RootOutput::FillEvent() {
  double total_E = 0;
  for (int i = 0; i < nSignals; i++) total_E+=hit_energy[i];
  if(total_E != 0) rootTree->Fill();//only fill event with energy deposit
  SciTree->Fill();
}

void RootOutput::FillParticle() {
  TrackTree->Fill();
}

//void RootOutput::SetDetectorInfo (G4double edep, G4double edep_e, G4double edep_gamma, G4double edep_other, G4double time){
//  eDep =  edep;
//  eDep_e =  edep_e;
//  eDep_gamma =  edep_gamma;
//  eDep_other =  edep_other;
//  egamma_hit_time =  time;
//}

void RootOutput::SetEnergyResolution (){
   for (int i = 0; i < nSignals; i++){
      if((hit_energy[i]*1000 - 14) < 0){
         hit_energy_reso[i] = G4RandGauss::shoot(hit_energy[i],reso_14keV);
      }else{
         hit_energy_reso[i] = G4RandGauss::shoot(hit_energy[i],(reso_14keV + (hit_energy[i]*1000 - 14)*reso_rate));
      }
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

  n_electrons_up = -100;
  n_photons_up = -100; 
  n_electrons_down = -100;
  n_photons_down = -100; 

  for (int i = 0; i < nhitMax; i++){
   hit_energy[i] = 0.;
   hit_timestart[i] = 0.;
   hit_timeend[i] = 0.;
   hit_nsteps[i] = 0;
   hit_length[i] = 0.;
   hit_pdgId[i] = 0;
   hit_process[i] = 0;
  }
}
