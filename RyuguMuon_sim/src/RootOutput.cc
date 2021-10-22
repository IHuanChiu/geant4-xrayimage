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
   ResponseFile = new TFile("/Users/chiu.i-huan/Desktop/geant4WS/geant4-xrayimage/macro/Ge_ResResponse.root", "read");
   for (int i=0;i<6;i++){
      sprintf(GeName, "graph_Ge%d", i+1);
      gr_Ge[i]= (TGraph*)ResponseFile->Get(GeName);
   }

//   CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
//   auto tpo = std::chrono::high_resolution_clock::now();
//   G4double output_flag = tpo.time_since_epoch().count();
//   auto ima_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//   std::stringstream ss;
//   ss << std::put_time( std::localtime(&ima_time), "%Y%m%d_%H%M%S" ); 
//   auto RootOutputFileName = "./Output_"+ss.str()+".root";
//   rootFile = new TFile(RootOutputFileName.c_str(), "recreate");

   sprintf(RootOutputFileName, "./Output_%s.root", "1");
   rootFile = new TFile(RootOutputFileName, "recreate");

//   if(rootFile->IsZombie()) { 
//    char message[200];
//    sprintf(message,"musrRootOutput::BeginOfRunAction() Root output file %s can not be created",RootOutputFileName);
//    std::cout << message << std::endl;
//   }else rootFile->cd();

   G4cout << " RE-create file ! " <<G4endl; 
   rootFile->cd();

   rootTree = new TTree("tree","Germanium detector analysis");
   muonTree = new TTree("mutree","muon beam analysis");
   TrackTree = new TTree("tracktree","Germanium simulation for particles");

   muonTree->Branch("eventID",&eventID,"eventID/I");
   muonTree->Branch("runID",&runID,"runID/I"); 
   muonTree->Branch("RunTime",&RunTime,"RunTime/D"); 
   // ===== muon position/hit =====
//   muonTree->Branch("muSampleTime",&muSampleTime,"muSampleTime/D");
//   muonTree->Branch("muSampleKineticEnergy",&muSampleKineticEnergy,"muSampleKineticEnergy/D");
//   muonTree->Branch("muSamplePolX",&muSamplePolX,"muSamplePolX/D");
//   muonTree->Branch("muSamplePolY",&muSamplePolY,"muSamplePolY/D");
//   muonTree->Branch("muSamplePolZ",&muSamplePolZ,"muSamplePolZ/D");
//   muonTree->Branch("muSampleMomX",&muSampleMomX,"muSampleMomX/D");
//   muonTree->Branch("muSampleMomY",&muSampleMomY,"muSampleMomY/D");
//   muonTree->Branch("muSampleMomZ",&muSampleMomZ,"muSampleMomZ/D");
//   muonTree->Branch("muSampleEndTime",&muSampleEndTime,"muSampleEndTime/D");
//   muonTree->Branch("muSampleEndKineticEnergy",&muSampleEndKineticEnergy,"muSampleEndKineticEnergy/D");
//   muonTree->Branch("muSampleEndPolX",&muSampleEndPolX,"muSampleEndPolX/D");
//   muonTree->Branch("muSampleEndPolY",&muSampleEndPolY,"muSampleEndPolY/D");
//   muonTree->Branch("muSampleEndPolZ",&muSampleEndPolZ,"muSampleEndPolZ/D");
//   muonTree->Branch("muSampleEndMomX",&muSampleEndMomX,"muSampleEndMomX/D");
//   muonTree->Branch("muSampleEndMomY",&muSampleEndMomY,"muSampleEndMomY/D");
//   muonTree->Branch("muSampleEndMomZ",&muSampleEndMomZ,"muSampleEndMomZ/D");
//   muonTree->Branch("muWorldTime",&muWorldTime,"muWorldTime/D");
//   muonTree->Branch("muWorldKineticEnergy",&muWorldKineticEnergy,"muWorldKineticEnergy/D");
//   muonTree->Branch("muWorldPolX",&muWorldPolX,"muWorldPolX/D");
//   muonTree->Branch("muWorldPolY",&muWorldPolY,"muWorldPolY/D");
//   muonTree->Branch("muWorldPolZ",&muWorldPolZ,"muWorldPolZ/D");
//   muonTree->Branch("muWorldMomX",&muWorldMomX,"muWorldMomX/D");
//   muonTree->Branch("muWorldMomY",&muWorldMomY,"muWorldMomY/D");
//   muonTree->Branch("muWorldMomZ",&muWorldMomZ,"muWorldMomZ/D");
//   muonTree->Branch("muWorldEndTime",&muWorldEndTime,"muWorldEndTime/D");
//   muonTree->Branch("muWorldEndKineticEnergy",&muWorldEndKineticEnergy,"muWorldEndKineticEnergy/D");
//   muonTree->Branch("muWorldEndPolX",&muWorldEndPolX,"muWorldEndPolX/D");
//   muonTree->Branch("muWorldEndPolY",&muWorldEndPolY,"muWorldEndPolY/D");
//   muonTree->Branch("muWorldEndPolZ",&muWorldEndPolZ,"muWorldEndPolZ/D");
//   muonTree->Branch("muWorldEndMomX",&muWorldEndMomX,"muWorldEndMomX/D");
//   muonTree->Branch("muWorldEndMomY",&muWorldEndMomY,"muWorldEndMomY/D");
//   muonTree->Branch("muWorldEndMomZ",&muWorldEndMomZ,"muWorldEndMomZ/D");

//   muonTree->Branch("muFoil1Time",&muFoil1Time,"muFoil1Time/D");
   muonTree->Branch("muFoil1KineticEnergy",&muFoil1KineticEnergy,"muFoil1KineticEnergy/D");
   muonTree->Branch("muFoil1PolX",&muFoil1PolX,"muFoil1PolX/D");
   muonTree->Branch("muFoil1PolY",&muFoil1PolY,"muFoil1PolY/D");
   muonTree->Branch("muFoil1PolZ",&muFoil1PolZ,"muFoil1PolZ/D");
   muonTree->Branch("muFoil1MomX",&muFoil1MomX,"muFoil1MomX/D");
   muonTree->Branch("muFoil1MomY",&muFoil1MomY,"muFoil1MomY/D");
   muonTree->Branch("muFoil1MomZ",&muFoil1MomZ,"muFoil1MomZ/D");

//   muonTree->Branch("muFoil2Time",&muFoil2Time,"muFoil2Time/D");
   muonTree->Branch("muFoil2KineticEnergy",&muFoil2KineticEnergy,"muFoil2KineticEnergy/D");
   muonTree->Branch("muFoil2PolX",&muFoil2PolX,"muFoil2PolX/D");
   muonTree->Branch("muFoil2PolY",&muFoil2PolY,"muFoil2PolY/D");
   muonTree->Branch("muFoil2PolZ",&muFoil2PolZ,"muFoil2PolZ/D");
   muonTree->Branch("muFoil2MomX",&muFoil2MomX,"muFoil2MomX/D");
   muonTree->Branch("muFoil2MomY",&muFoil2MomY,"muFoil2MomY/D");
   muonTree->Branch("muFoil2MomZ",&muFoil2MomZ,"muFoil2MomZ/D");

//   muonTree->Branch("muFoil3Time",&muFoil3Time,"muFoil3Time/D");
   muonTree->Branch("muFoil3KineticEnergy",&muFoil3KineticEnergy,"muFoil3KineticEnergy/D");
   muonTree->Branch("muFoil3PolX",&muFoil3PolX,"muFoil3PolX/D");
   muonTree->Branch("muFoil3PolY",&muFoil3PolY,"muFoil3PolY/D");
   muonTree->Branch("muFoil3PolZ",&muFoil3PolZ,"muFoil3PolZ/D");
   muonTree->Branch("muFoil3MomX",&muFoil3MomX,"muFoil3MomX/D");
   muonTree->Branch("muFoil3MomY",&muFoil3MomY,"muFoil3MomY/D");
   muonTree->Branch("muFoil3MomZ",&muFoil3MomZ,"muFoil3MomZ/D");

//   muonTree->Branch("muInter1Time",&muInter1Time,"muInter1Time/D");
   muonTree->Branch("muInter1KineticEnergy",&muInter1KineticEnergy,"muInter1KineticEnergy/D");
   muonTree->Branch("muInter1PolX",&muInter1PolX,"muInter1PolX/D");
   muonTree->Branch("muInter1PolY",&muInter1PolY,"muInter1PolY/D");
   muonTree->Branch("muInter1PolZ",&muInter1PolZ,"muInter1PolZ/D");
   muonTree->Branch("muInter1MomX",&muInter1MomX,"muInter1MomX/D");
   muonTree->Branch("muInter1MomY",&muInter1MomY,"muInter1MomY/D");
   muonTree->Branch("muInter1MomZ",&muInter1MomZ,"muInter1MomZ/D");

//   muonTree->Branch("muInter2Time",&muInter2Time,"muInter2Time/D");
   muonTree->Branch("muInter2KineticEnergy",&muInter2KineticEnergy,"muInter2KineticEnergy/D");
   muonTree->Branch("muInter2PolX",&muInter2PolX,"muInter2PolX/D");
   muonTree->Branch("muInter2PolY",&muInter2PolY,"muInter2PolY/D");
   muonTree->Branch("muInter2PolZ",&muInter2PolZ,"muInter2PolZ/D");
   muonTree->Branch("muInter2MomX",&muInter2MomX,"muInter2MomX/D");
   muonTree->Branch("muInter2MomY",&muInter2MomY,"muInter2MomY/D");
   muonTree->Branch("muInter2MomZ",&muInter2MomZ,"muInter2MomZ/D");

//   muonTree->Branch("muTargetTime",&muTargetTime,"muTargetTime/D");
   muonTree->Branch("muTargetKineticEnergy",&muTargetKineticEnergy,"muTargetKineticEnergy/D");
   muonTree->Branch("muTargetPolX",&muTargetPolX,"muTargetPolX/D");
   muonTree->Branch("muTargetPolY",&muTargetPolY,"muTargetPolY/D");
   muonTree->Branch("muTargetPolZ",&muTargetPolZ,"muTargetPolZ/D");
   muonTree->Branch("muTargetMomX",&muTargetMomX,"muTargetMomX/D");
   muonTree->Branch("muTargetMomY",&muTargetMomY,"muTargetMomY/D");
   muonTree->Branch("muTargetMomZ",&muTargetMomZ,"muTargetMomZ/D");

//   muonTree->Branch("muTarget2Time",&muTarget2Time,"muTarget2Time/D");
   muonTree->Branch("muTarget2KineticEnergy",&muTarget2KineticEnergy,"muTarget2KineticEnergy/D");
   muonTree->Branch("muTarget2PolX",&muTarget2PolX,"muTarget2PolX/D");
   muonTree->Branch("muTarget2PolY",&muTarget2PolY,"muTarget2PolY/D");
   muonTree->Branch("muTarget2PolZ",&muTarget2PolZ,"muTarget2PolZ/D");
   muonTree->Branch("muTarget2MomX",&muTarget2MomX,"muTarget2MomX/D");
   muonTree->Branch("muTarget2MomY",&muTarget2MomY,"muTarget2MomY/D");
   muonTree->Branch("muTarget2MomZ",&muTarget2MomZ,"muTarget2MomZ/D");
   // === end poi ===
   muonTree->Branch("muInter1EndPolX",&muInter1EndPolX,"muInter1EndPolX/D");
   muonTree->Branch("muInter1EndPolY",&muInter1EndPolY,"muInter1EndPolY/D");
   muonTree->Branch("muInter1EndPolZ",&muInter1EndPolZ,"muInter1EndPolZ/D");
//   muonTree->Branch("muInter1EndTime",&muInter1EndTime,"muInter1EndTime/D");
//   muonTree->Branch("muInter1EndKineticEnergy",&muInter1EndKineticEnergy,"muInter1EndKineticEnergy/D");
//   muonTree->Branch("muInter1EndMomX",&muInter1EndMomX,"muInter1EndMomX/D");
//   muonTree->Branch("muInter1EndMomY",&muInter1EndMomY,"muInter1EndMomY/D");
//   muonTree->Branch("muInter1EndMomZ",&muInter1EndMomZ,"muInter1EndMomZ/D");

   muonTree->Branch("muInter2EndPolX",&muInter2EndPolX,"muInter2EndPolX/D");
   muonTree->Branch("muInter2EndPolY",&muInter2EndPolY,"muInter2EndPolY/D");
   muonTree->Branch("muInter2EndPolZ",&muInter2EndPolZ,"muInter2EndPolZ/D");
//   muonTree->Branch("muInter2EndTime",&muInter2EndTime,"muInter2EndTime/D");
//   muonTree->Branch("muInter2EndKineticEnergy",&muInter2EndKineticEnergy,"muInter2EndKineticEnergy/D");
//   muonTree->Branch("muInter2EndMomX",&muInter2EndMomX,"muInter2EndMomX/D");
//   muonTree->Branch("muInter2EndMomY",&muInter2EndMomY,"muInter2EndMomY/D");
//   muonTree->Branch("muInter2EndMomZ",&muInter2EndMomZ,"muInter2EndMomZ/D");

//   muonTree->Branch("muDecayPolX",&muDecayPolX,"muDecayPolX/D");
//   muonTree->Branch("muDecayPolY",&muDecayPolY,"muDecayPolY/D");
//   muonTree->Branch("muDecayPolZ",&muDecayPolZ,"muDecayPolZ/D");
//   muonTree->Branch("muDecayTime",&muDecayTime,"muDecayTime/D");
   muonTree->Branch("Stop_VolumeID",&Stop_Volume,"Stop_VolumeID/I");//muon hit
   muonTree->Branch("muInitX",&muInitX,"muInitX/D");//init muon 
   muonTree->Branch("muInitY",&muInitY,"muInitY/D");//init muon 
   muonTree->Branch("muInitZ",&muInitZ,"muInitZ/D");//init muon 
   muonTree->Branch("muInitpX",&muInitpX,"muInitpX/D");//init muon 
   muonTree->Branch("muInitpY",&muInitpY,"muInitpY/D");//init muon 
   muonTree->Branch("muInitpZ",&muInitpZ,"muInitpZ/D");//init muon 
   muonTree->Branch("muInitTime",&muInitTime,"muInitTime/D");//init muon 
//   muonTree->Branch("eInitX",&eInitX,"eInitX/D");//init ele 
//   muonTree->Branch("eInitY",&eInitY,"eInitY/D");//init ele 
//   muonTree->Branch("eInitZ",&eInitZ,"eInitZ/D");//init ele 
//   muonTree->Branch("eInitpX",&eInitpX,"eInitpX/D");//init ele 
//   muonTree->Branch("eInitpY",&eInitpY,"eInitpY/D");//init ele 
//   muonTree->Branch("eInitpZ",&eInitpZ,"eInitpZ/D");//init ele

   // ===== detector info. =====
   rootTree->Branch("InitEnergy",&InitEnergy,"InitEnergy/D");//nsiganle in a event
   rootTree->Branch("nSignals",&nSignals,"nSignals/I");//nsiganle in a event
   rootTree->Branch("Det_ID",&Det_ID,"Det_ID[nSignals]/I");//nsiganle in a event
   rootTree->Branch("Hit_Energy",&hit_energy,"Hit_Energy[nSignals]/D");
   rootTree->Branch("Hit_Energy_Reso",&hit_energy_reso,"Hit_Energy_Reso[nSignals]/D"); //MeV
   rootTree->Branch("Hit_Start_X",&hit_startx,"Hit_Start_X[nSignals]/D");
   rootTree->Branch("Hit_Start_Y",&hit_starty,"Hit_Start_Y[nSignals]/D");
   rootTree->Branch("Hit_Start_Z",&hit_startz,"Hit_Start_Z[nSignals]/D");
   rootTree->Branch("Hit_Time_Start",&hit_timestart,"Hit_Time_Start[nSignals]/D");
   rootTree->Branch("Hit_Time_End",&hit_timeend,"Hit_Time_End[nSignals]/D");
   rootTree->Branch("Hit_Nsteps",&hit_nsteps,"Hit_Nsteps[nSignals]/I");
   rootTree->Branch("Hit_Length",&hit_length,"Hit_Length[nSignals]/D");
   rootTree->Branch("Hit_pdgId",&hit_pdgId,"Hit_pdgId[nSignals]/I");
   rootTree->Branch("Hit_ProcessID",&hit_process,"Hit_ProcessID[nSignals]/I");
   rootTree->Branch("Stop_VolumeID",&Stop_Volume,"Stop_VolumeID/I");//muon hit 

//   rootTree->Branch("energyDep",&eDep,"energyDep/D");
//   rootTree->Branch("particlename",&hitparticle,"particlename/D");
//   rootTree->Branch("energyDep_e",&eDep_e,"energyDep_e/D");
//   rootTree->Branch("energyDep_gamma",&eDep_gamma,"energyDep_gamma/D");
//   rootTree->Branch("energyDep_other",&eDep_other,"energyDep_other/D");
//   rootTree->Branch("egamma_hit_time",&egamma_hit_time,"egamma_hit_time/D");

//   rootTree->Branch("Ngamma",&Ngamma,"Ngamma/I");
//   rootTree->Branch("Neletron",&Neletron,"Neletron/I");
//   rootTree->Branch("Nneutron",&Nneutron,"Nneutron/I");
//   rootTree->Branch("Nother",&Nother,"Nother/I");
//
//   // ===== track info. =====
//   TrackTree->Branch("pdgId",&pdgId,"pdgId/I");
//   TrackTree->Branch("kinetic_E",&KineticE,"kinetic_E/D");
//   TrackTree->Branch("Total_E",&TotalE,"Total_E/D");
//   TrackTree->Branch("Deposite_E",&DepositeE,"Deposite_E/D");
//   TrackTree->Branch("Detector_X",&Det_X,"Detector_X/D");
//   TrackTree->Branch("Detector_Y",&Det_Y,"Detector_Y/D");
//   TrackTree->Branch("Detector_Z",&Det_Z,"Detector_Z/D");
//   TrackTree->Branch("Hit_Time",&Hit_Time,"Hit_Time/D");

   // ===== hist. info. =====
   h1_StopVol = new TH1F("h1_StopVol","Name of Volume",nh1bin,0,nh1bin);
   for (int i=1;i<=nh1bin;i++) h1_StopVol->GetXaxis()->SetBinLabel(i,var_name[i-1]);
   h1_process = new TH1F("hit_process","Process of Signal",nh1bin,0,nh1bin);
   for (int i=1;i<=nh1bin;i++) h1_process->GetXaxis()->SetBinLabel(i,pro_name[i-1]);
   G4cout << "RootOutput::BeginOfRunAction()  The Root tree and branches were defined."<<G4endl;
}

void RootOutput::EndOfRunAction() {
  G4cout<<"RootOutput::EndOfRunAction() - Write Tree "<<G4endl;
  rootTree->Write();
//  muonTree->Write();//check beam profile (file size will be huge)
//  TrackTree->Write();
  h1_StopVol->Write();
  h1_process->Write();
  rootFile->Close();
  G4cout<<"RootOutput::EndOfRunAction() - Root tree written out."<<G4endl;
}

void RootOutput::FillEvent() {
  if(Stop_Volume != -1000) h1_StopVol->Fill(Stop_Volume);
  double total_E = 0;
  for (int i = 0; i < nSignals; i++) total_E+=hit_energy[i];
  if(total_E*1000 > 3) rootTree->Fill();//only fill event with energy deposit larger than 3keV base on features of HPGe
//  muonTree->Fill();// check beam profile (file size will be huge)
}

void RootOutput::FillParticle() {
  TrackTree->Fill();
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
