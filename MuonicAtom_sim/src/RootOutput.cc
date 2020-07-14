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
   TrackTree = new TTree("tracktree","Germanium simulation for particles");

   rootTree->Branch("eventID",&eventID,"eventID/I");
   rootTree->Branch("runID",&runID,"runID/I"); 
   // ===== muon position/hit =====
   rootTree->Branch("muSampleTime",&muSampleTime,"muSampleTime/D");
   rootTree->Branch("muSampleKineticEnergy",&muSampleKineticEnergy,"muSampleKineticEnergy/D");
   rootTree->Branch("muSamplePolX",&muSamplePolX,"muSamplePolX/D");
   rootTree->Branch("muSamplePolY",&muSamplePolY,"muSamplePolY/D");
   rootTree->Branch("muSamplePolZ",&muSamplePolZ,"muSamplePolZ/D");
   rootTree->Branch("muSampleEndTime",&muSampleEndTime,"muSampleEndTime/D");
   rootTree->Branch("muSampleEndKineticEnergy",&muSampleEndKineticEnergy,"muSampleEndKineticEnergy/D");
   rootTree->Branch("muSampleEndPolX",&muSampleEndPolX,"muSampleEndPolX/D");
   rootTree->Branch("muSampleEndPolY",&muSampleEndPolY,"muSampleEndPolY/D");
   rootTree->Branch("muSampleEndPolZ",&muSampleEndPolZ,"muSampleEndPolZ/D");
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
   rootTree->Branch("muCollimatorTime",&muCollimatorTime,"muCollimatorTime/D");
   rootTree->Branch("muCollimatorKineticEnergy",&muCollimatorKineticEnergy,"muCollimatorKineticEnergy/D");
   rootTree->Branch("muCollimatorPolX",&muCollimatorPolX,"muCollimatorPolX/D");
   rootTree->Branch("muCollimatorPolY",&muCollimatorPolY,"muCollimatorPolY/D");
   rootTree->Branch("muCollimatorPolZ",&muCollimatorPolZ,"muCollimatorPolZ/D");
   rootTree->Branch("muCollimatorEndTime",&muCollimatorEndTime,"muCollimatorEndTime/D");
   rootTree->Branch("muCollimatorEndKineticEnergy",&muCollimatorEndKineticEnergy,"muCollimatorEndKineticEnergy/D");
   rootTree->Branch("muCollimatorEndPolX",&muCollimatorEndPolX,"muCollimatorEndPolX/D");
   rootTree->Branch("muCollimatorEndPolY",&muCollimatorEndPolY,"muCollimatorEndPolY/D");
   rootTree->Branch("muCollimatorEndPolZ",&muCollimatorEndPolZ,"muCollimatorEndPolZ/D");
//   rootTree->Branch("muShadowTime",&muShadowTime,"muShadowTime/D");
//   rootTree->Branch("muShadowKineticEnergy",&muShadowKineticEnergy,"muShadowKineticEnergy/D");
//   rootTree->Branch("muShadowPolX",&muShadowPolX,"muShadowPolX/D");
//   rootTree->Branch("muShadowPolY",&muShadowPolY,"muShadowPolY/D");
//   rootTree->Branch("muShadowPolZ",&muShadowPolZ,"muShadowPolZ/D");
//   rootTree->Branch("muShadowEndTime",&muShadowEndTime,"muShadowEndTime/D");
//   rootTree->Branch("muShadowEndKineticEnergy",&muShadowEndKineticEnergy,"muShadowEndKineticEnergy/D");
//   rootTree->Branch("muShadowEndPolX",&muShadowEndPolX,"muShadowEndPolX/D");
//   rootTree->Branch("muShadowEndPolY",&muShadowEndPolY,"muShadowEndPolY/D");
//   rootTree->Branch("muShadowEndPolZ",&muShadowEndPolZ,"muShadowEndPolZ/D");
//   rootTree->Branch("muKaptonTime",&muKaptonTime,"muKaptonTime/D");
//   rootTree->Branch("muKaptonKineticEnergy",&muKaptonKineticEnergy,"muKaptonKineticEnergy/D");
//   rootTree->Branch("muKaptonPolX",&muKaptonPolX,"muKaptonPolX/D");
//   rootTree->Branch("muKaptonPolY",&muKaptonPolY,"muKaptonPolY/D");
//   rootTree->Branch("muKaptonPolZ",&muKaptonPolZ,"muKaptonPolZ/D");
//   rootTree->Branch("muKaptonEndTime",&muKaptonEndTime,"muKaptonEndTime/D");
//   rootTree->Branch("muKaptonEndKineticEnergy",&muKaptonEndKineticEnergy,"muKaptonEndKineticEnergy/D");
//   rootTree->Branch("muKaptonEndPolX",&muKaptonEndPolX,"muKaptonEndPolX/D");
//   rootTree->Branch("muKaptonEndPolY",&muKaptonEndPolY,"muKaptonEndPolY/D");
//   rootTree->Branch("muKaptonEndPolZ",&muKaptonEndPolZ,"muKaptonEndPolZ/D");
//   rootTree->Branch("muWorldTime",&muWorldTime,"muWorldTime/D");
//   rootTree->Branch("muWorldKineticEnergy",&muWorldKineticEnergy,"muWorldKineticEnergy/D");
//   rootTree->Branch("muWorldPolX",&muWorldPolX,"muWorldPolX/D");
//   rootTree->Branch("muWorldPolY",&muWorldPolY,"muWorldPolY/D");
//   rootTree->Branch("muWorldPolZ",&muWorldPolZ,"muWorldPolZ/D");
//   rootTree->Branch("muWorldEndTime",&muWorldEndTime,"muWorldEndTime/D");
//   rootTree->Branch("muWorldEndKineticEnergy",&muWorldEndKineticEnergy,"muWorldEndKineticEnergy/D");
//   rootTree->Branch("muWorldEndPolX",&muWorldEndPolX,"muWorldEndPolX/D");
//   rootTree->Branch("muWorldEndPolY",&muWorldEndPolY,"muWorldEndPolY/D");
//   rootTree->Branch("muWorldEndPolZ",&muWorldEndPolZ,"muWorldEndPolZ/D");
   rootTree->Branch("Stop_VolumeID",&Stop_Volume,"Stop_VolumeID/I");//muon hit
   rootTree->Branch("muInitX",&muInitX,"muInitX/D");//init muon 
   rootTree->Branch("muInitY",&muInitY,"muInitY/D");//init muon 
   rootTree->Branch("muInitZ",&muInitZ,"muInitZ/D");//init muon 
   rootTree->Branch("muInitpX",&muInitpX,"muInitpX/D");//init muon 
   rootTree->Branch("muInitpY",&muInitpY,"muInitpY/D");//init muon 
   rootTree->Branch("muInitpZ",&muInitpZ,"muInitpZ/D");//init muon 
   rootTree->Branch("muInitTime",&muInitTime,"muInitTime/D");//init muon 
//   rootTree->Branch("eInitX",&eInitX,"eInitX/D");//init ele 
//   rootTree->Branch("eInitY",&eInitY,"eInitY/D");//init ele 
//   rootTree->Branch("eInitZ",&eInitZ,"eInitZ/D");//init ele 
//   rootTree->Branch("eInitpX",&eInitpX,"eInitpX/D");//init ele 
//   rootTree->Branch("eInitpY",&eInitpY,"eInitpY/D");//init ele 
//   rootTree->Branch("eInitpZ",&eInitpZ,"eInitpZ/D");//init ele 

   rootTree->Branch("nSignals",&nSignals,"nSignals/I");//nsiganle in a event
   rootTree->Branch("Hit_Energy",&hit_energy,"Hit_Energy[nSignals]/D");
   rootTree->Branch("Hit_Energy_Reso",&hit_energy_reso,"Hit_Energy_Reso[nSignals]/D");
   rootTree->Branch("Hit_Time_Start",&hit_timestart,"Hit_Time_Start[nSignals]/D");
   rootTree->Branch("Hit_Time_End",&hit_timeend,"Hit_Time_End[nSignals]/D");
   rootTree->Branch("Hit_Nsteps",&hit_nsteps,"Hit_Nsteps[nSignals]/I");
   rootTree->Branch("Hit_Length",&hit_length,"Hit_Length[nSignals]/D");
   rootTree->Branch("Hit_pdgId",&hit_pdgId,"Hit_pdgId[nSignals]/I");
   rootTree->Branch("Hit_ProcessID",&hit_process,"Hit_ProcessID[nSignals]/I");
   rootTree->Branch("Hit_x",&hit_x,"Hit_x[nSignals]/D");
   rootTree->Branch("Hit_y",&hit_y,"Hit_y[nSignals]/D");
   rootTree->Branch("Hit_z",&hit_z,"Hit_z[nSignals]/D");

   // ===== detector info. =====
   rootTree->Branch("Det_nMax",&Det_nMax,"Det_nMax/I");
   rootTree->Branch("energyDep",eDep,"energyDep[Det_nMax]/D");
   rootTree->Branch("energyDep_e",eDep_e,"energyDep_e[Det_nMax]/D");
   rootTree->Branch("energyDep_gamma",eDep_gamma,"energyDep_gamma[Det_nMax]/D");
   rootTree->Branch("energyDep_other",eDep_other,"energyDep_other[Det_nMax]/D");
   rootTree->Branch("Position_x",Position_x,"Position_x[Det_nMax]/D");
   rootTree->Branch("Position_y",Position_y,"Position_y[Det_nMax]/D");
   rootTree->Branch("Position_z",Position_z,"Position_z[Det_nMax]/D");

//   rootTree->Branch("nDet",nDet,"nDet/I");
//   rootTree->Branch("Ngamma",Ngamma,"Ngamma[nDet]/D");
//   rootTree->Branch("Neletron",Neletron,"Neletron[nDet]/D");
//   rootTree->Branch("Nneutron",Nneutron,"Nneutron[nDet]/D");
//   rootTree->Branch("Nother",Nother,"Nother[nDet]/D");

   // ===== track info. =====
   TrackTree->Branch("det_ID", &det_ID, "det_ID/I");
   TrackTree->Branch("pdgId",&pdgId,"pdgId/I");
   TrackTree->Branch("kinetic_E",&KineticE,"kinetic_E/D");
   TrackTree->Branch("Total_E",&TotalE,"Total_E/D");
   TrackTree->Branch("Detector_X",&Det_X,"Detector_X/D");
   TrackTree->Branch("Detector_Y",&Det_Y,"Detector_Y/D");
   TrackTree->Branch("Detector_Z",&Det_Z,"Detector_Z/D");
   TrackTree->Branch("Track_Name",&Track_Name);
   TrackTree->Branch("Track_Process",&Track_Process);

   // ===== histogram =====
   h1_process = new TH1F("hit_process","Process of Signal",nh1bin,0,nh1bin);
   for (int i=1;i<=nh1bin;i++) h1_process->GetXaxis()->SetBinLabel(i,var_name[i-1]);

   G4cout << "RootOutput::BeginOfRunAction()  The Root tree and branches were defined."<<G4endl;
}

void RootOutput::RootEndOfRunAction() {
  G4cout<<"RootOutput::RootEndOfRunAction() - Write Tree "<<G4endl;
  rootTree->Write();
  TrackTree->Write();
  rootFile->Close();
  G4cout<<"RootOutput::RootEndOfRunAction() - Root tree written out."<<G4endl;
}

void RootOutput::FillEvent() {
  double total_E = 0;
  for (int i = 0; i < nSignals; i++) total_E+=hit_energy[i];
  if(total_E != 0) rootTree->Fill();
}

void RootOutput::FillParticle() {
  TrackTree->Fill();
}

//void RootOutput::SetDetectorInfo (G4int det_id, G4double edep, G4double edep_e, G4double edep_gamma, G4double edep_other){
//  Det_nMax = numberOfvolume; 
//  eDep[det_id] =  edep;
//  eDep_e[det_id] =  edep_e;
//  eDep_gamma[det_id] =  edep_gamma;
//  eDep_other[det_id] =  edep_other;
//}

void RootOutput::ScannParticleHitVolume(G4int id, G4String name){
   nDet = numberOfvolume;
   if(name == "gamma"){ Ngamma[id]++;
   }else if(name == "e+" || name == "e-"){ Neletron[id]++;
   }else if(name == "neutron" || name == "anti_neutron"){ Nneutron[id]++;
   }else{ Nother[id]++;
   }
}

void RootOutput::SetEnergyDepositInVolume(G4int id, G4String particleName, G4double energy){  
  Det_nMax = numberOfvolume; // find max id (number of detectors)
  eDep[id] +=  energy;
  if (particleName=="e-" || particleName == "e+") { eDep_e[id] +=  energy;
  }else if(particleName == "gamma"){ eDep_gamma[id] += energy;
  }else{ eDep_other[id] += energy;}

}

void RootOutput::SetParticlePositionInVolume(G4int id, G4double x, G4double y , G4double z){
     // xy for plane of detector; z for depth
     int currentstep = id;     
     Position_x[id] = x*std::cos(id*(-60)*CLHEP::deg) + y*std::sin(id*(-60)*CLHEP::deg); 
     Position_y[id] = x*(-std::sin(id*(-60)*CLHEP::deg)) + y*std::cos(id*(-60)*CLHEP::deg); 
     Position_z[id] = z;
}

void RootOutput::SetEnergyResolution (){
   for (int i = 0; i < nSignals; i++){
      if(hit_energy[i]*1000 > 9 && hit_energy[i]*1000 < 19){  hit_energy_reso[i] = G4RandGauss::shoot(hit_energy[i],0.00143);
      }else if(hit_energy[i]*1000 > 70 && hit_energy[i]*1000 < 80){ hit_energy_reso[i] = G4RandGauss::shoot(hit_energy[i],0.00165);
      }else{ hit_energy_reso[i] = hit_energy[i]; }
   }
}

void RootOutput::ClearAllRootVariables() {
  Stop_Volume = -1000;

  runID=-1000;
  eventID=-1000;
  for (int j = 0; j < 6; j++){
   muCdTeTime[j] = -1000; muCdTePolX[j] = -1000; muCdTePolY[j] = -1000; muCdTePolZ[j] = -1000;
   muCdTeKineticEnergy[j] = -1000; muCdTeEndKineticEnergy[j] = -1000;
   muCdTeEndTime[j] = -1000; muCdTeEndPolZ[j] = -1000; muCdTeEndPolY[j] = -1000; muCdTeEndPolX[j]  = -1000;
  }
  KineticE = -1000; TotalE = -1000; pdgId = -1000;
  Det_X = -1000; Det_Y = -1000; Det_Z = -1000;
  Track_Name = "None";
  Track_Process = "None";

  nDet = -1000;
  for (int j = 0; j < 10; j++){
   Ngamma[j]=0;
   Neletron[j]=0;
   Nneutron[j]=0;  
   Nother[j]=0;  
  }
  Det_nMax = 0;
  for (int i = 0; i < numberOfvolume; i++){
    eDep[i] = 0.;
    eDep_e[i] = 0.;
    eDep_gamma[i] = 0.;
    eDep_other[i] = 0.;
    Position_x[i] = -1000.;
    Position_y[i] = -1000.;
    Position_z[i] = -1000.;
  }

  for (int i = 0; i < nhitMax; i++){
     hit_energy[i] = 0.;
     hit_timestart[i] = 0.;
     hit_timeend[i] = 0.;
     hit_nsteps[i] = 0;
     hit_length[i] = 0.;
     hit_pdgId[i] = 0;
     hit_process[i] = 0;
     hit_x[i] = 0;
     hit_y[i] = 0;
     hit_z[i] = 0;
    }
  
  muSampleTime = -1000; muCollimatorTime = -1000; muWorldTime = -1000; muShadowTime = -1000; muKaptonTime = -1000;
  muSampleKineticEnergy = -1000; muCollimatorKineticEnergy = -1000; muWorldKineticEnergy = -1000; muShadowKineticEnergy = -1000;
  muSamplePolX = -1000; muSamplePolY = -1000; muSamplePolZ = -1000; muKaptonKineticEnergy = -1000;
  muCollimatorPolX = -1000; muCollimatorPolY = -1000; muCollimatorPolZ = -1000;
  muWorldPolX = -1000; muWorldPolY = -1000; muWorldPolZ = -1000;
  muShadowPolX = -1000; muShadowPolY = -1000; muShadowPolZ = -1000;
  muKaptonPolX = -1000; muKaptonPolY = -1000; muKaptonPolZ = -1000;
  muSampleEndTime = -1000; muCollimatorEndTime = -1000; muWorldEndTime = -1000; muShadowEndTime = -1000;
  muSampleEndKineticEnergy = -1000; muCollimatorEndKineticEnergy = -1000; muWorldEndKineticEnergy = -1000; muShadowEndKineticEnergy = -1000; muKaptonEndKineticEnergy = -1000;
  muSampleEndPolX = -1000; muSampleEndPolY = -1000; muSampleEndPolZ = -1000;
  muCollimatorEndPolX = -1000; muCollimatorEndPolY = -1000; muCollimatorEndPolZ = -1000;
  muWorldEndPolX = -1000; muWorldEndPolY = -1000; muWorldEndPolZ = -1000;
  muShadowEndPolX = -1000; muShadowEndPolY = -1000; muShadowEndPolZ = -1000;
  muKaptonEndPolX = -1000; muKaptonEndPolY = -1000; muKaptonEndPolZ = -1000;
  muInitX = -1000; muInitY = -1000;muInitZ = -1000;muInitpX = -1000;muInitpY = -1000;muInitpZ = -1000;  
  eInitX = -1000; eInitY = -1000;eInitZ = -1000;eInitpX = -1000;eInitpY = -1000;eInitpZ = -1000;  
  muInitTime = -1000;
}
