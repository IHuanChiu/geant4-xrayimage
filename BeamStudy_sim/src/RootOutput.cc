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

   rootTree = new TTree("tree","Germanium detector analysis");
   TrackTree = new TTree("tracktree","Germanium simulation for particles");

   rootTree->Branch("eventID",&eventID,"eventID/I");
   rootTree->Branch("runID",&runID,"runID/I"); 
   rootTree->Branch("RunTime",&RunTime,"RunTime/D"); 

   // === init. poi ===
   rootTree->Branch("muFoil1Time",&muFoil1Time,"muFoil1Time/D");
   rootTree->Branch("muFoil1KineticEnergy",&muFoil1KineticEnergy,"muFoil1KineticEnergy/D");
   rootTree->Branch("muFoil1PolX",&muFoil1PolX,"muFoil1PolX/D");
   rootTree->Branch("muFoil1PolY",&muFoil1PolY,"muFoil1PolY/D");
   rootTree->Branch("muFoil1PolZ",&muFoil1PolZ,"muFoil1PolZ/D");
   rootTree->Branch("muFoil1MomX",&muFoil1MomX,"muFoil1MomX/D");
   rootTree->Branch("muFoil1MomY",&muFoil1MomY,"muFoil1MomY/D");
   rootTree->Branch("muFoil1MomZ",&muFoil1MomZ,"muFoil1MomZ/D");

   rootTree->Branch("muFoil2Time",&muFoil2Time,"muFoil2Time/D");
   rootTree->Branch("muFoil2KineticEnergy",&muFoil2KineticEnergy,"muFoil2KineticEnergy/D");
   rootTree->Branch("muFoil2PolX",&muFoil2PolX,"muFoil2PolX/D");
   rootTree->Branch("muFoil2PolY",&muFoil2PolY,"muFoil2PolY/D");
   rootTree->Branch("muFoil2PolZ",&muFoil2PolZ,"muFoil2PolZ/D");
   rootTree->Branch("muFoil2MomX",&muFoil2MomX,"muFoil2MomX/D");
   rootTree->Branch("muFoil2MomY",&muFoil2MomY,"muFoil2MomY/D");
   rootTree->Branch("muFoil2MomZ",&muFoil2MomZ,"muFoil2MomZ/D");

   rootTree->Branch("muFoil3Time",&muFoil3Time,"muFoil3Time/D");
   rootTree->Branch("muFoil3KineticEnergy",&muFoil3KineticEnergy,"muFoil3KineticEnergy/D");
   rootTree->Branch("muFoil3PolX",&muFoil3PolX,"muFoil3PolX/D");
   rootTree->Branch("muFoil3PolY",&muFoil3PolY,"muFoil3PolY/D");
   rootTree->Branch("muFoil3PolZ",&muFoil3PolZ,"muFoil3PolZ/D");
   rootTree->Branch("muFoil3MomX",&muFoil3MomX,"muFoil3MomX/D");
   rootTree->Branch("muFoil3MomY",&muFoil3MomY,"muFoil3MomY/D");
   rootTree->Branch("muFoil3MomZ",&muFoil3MomZ,"muFoil3MomZ/D");

//   rootTree->Branch("muInterTime",&muInter1Time,"muInterTime/D");
   rootTree->Branch("muInterKineticEnergy",&muInter1KineticEnergy,"muInterKineticEnergy/D");
   rootTree->Branch("muInterPolX",&muInter1PolX,"muInterPolX/D");
   rootTree->Branch("muInterPolY",&muInter1PolY,"muInterPolY/D");
   rootTree->Branch("muInterPolZ",&muInter1PolZ,"muInterPolZ/D");
   rootTree->Branch("muInterMomX",&muInter1MomX,"muInterMomX/D");
   rootTree->Branch("muInterMomY",&muInter1MomY,"muInterMomY/D");
   rootTree->Branch("muInterMomZ",&muInter1MomZ,"muInterMomZ/D");

   rootTree->Branch("muTargetTime",&muTargetTime,"muTargetTime/D");
   rootTree->Branch("muTargetKineticEnergy",&muTargetKineticEnergy,"muTargetKineticEnergy/D");
   rootTree->Branch("muTargetPolX",&muTargetPolX,"muTargetPolX/D");
   rootTree->Branch("muTargetPolY",&muTargetPolY,"muTargetPolY/D");
   rootTree->Branch("muTargetPolZ",&muTargetPolZ,"muTargetPolZ/D");
   rootTree->Branch("muTargetMomX",&muTargetMomX,"muTargetMomX/D");
   rootTree->Branch("muTargetMomY",&muTargetMomY,"muTargetMomY/D");
   rootTree->Branch("muTargetMomZ",&muTargetMomZ,"muTargetMomZ/D");

   rootTree->Branch("muSampleTime",&muSampleTime,"muSampleTime/D");
   rootTree->Branch("muSampleKineticEnergy",&muSampleKineticEnergy,"muSampleKineticEnergy/D");
   rootTree->Branch("muSamplePolX",&muSamplePolX,"muSamplePolX/D");
   rootTree->Branch("muSamplePolY",&muSamplePolY,"muSamplePolY/D");
   rootTree->Branch("muSamplePolZ",&muSamplePolZ,"muSamplePolZ/D");
   rootTree->Branch("muSampleMomX",&muSampleMomX,"muSampleMomX/D");
   rootTree->Branch("muSampleMomY",&muSampleMomY,"muSampleMomY/D");
   rootTree->Branch("muSampleMomZ",&muSampleMomZ,"muSampleMomZ/D");

   // === end poi ===
   rootTree->Branch("muInter1EndPolX",&muInter1EndPolX,"muInter1EndPolX/D");
   rootTree->Branch("muInter1EndPolY",&muInter1EndPolY,"muInter1EndPolY/D");
   rootTree->Branch("muInter1EndPolZ",&muInter1EndPolZ,"muInter1EndPolZ/D");

   // === muon beam ===
   rootTree->Branch("Stop_VolumeID",&Stop_Volume,"Stop_VolumeID/I");//muon hit
   rootTree->Branch("muInitX",&muInitX,"muInitX/D");//init muon 
   rootTree->Branch("muInitY",&muInitY,"muInitY/D");//init muon 
   rootTree->Branch("muInitZ",&muInitZ,"muInitZ/D");//init muon 
   rootTree->Branch("muInitpX",&muInitpX,"muInitpX/D");//init muon 
   rootTree->Branch("muInitpY",&muInitpY,"muInitpY/D");//init muon 
   rootTree->Branch("muInitpZ",&muInitpZ,"muInitpZ/D");//init muon 
   rootTree->Branch("muInitTime",&muInitTime,"muInitTime/D");//init muon 

   // ===== hist. info. =====
   h1_StopVol = new TH1F("h1_StopVol","Name of Volume",nh1bin,0,nh1bin);
   for (int i=1;i<=nh1bin;i++) h1_StopVol->GetXaxis()->SetBinLabel(i,var_name[i-1]);
   G4cout << "RootOutput::BeginOfRunAction()  The Root tree and branches were defined."<<G4endl;
}

void RootOutput::EndOfRunAction() {
  G4cout<<"RootOutput::EndOfRunAction() - Write Tree "<<G4endl;
  rootTree->Write();
  TrackTree->Write();
  h1_StopVol->Write();
  rootFile->Close();
  G4cout<<"RootOutput::EndOfRunAction() - Root tree written out."<<G4endl;
}

void RootOutput::FillEvent() {
  if(Stop_Volume != -1000) h1_StopVol->Fill(Stop_Volume);
  rootTree->Fill();
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
}
