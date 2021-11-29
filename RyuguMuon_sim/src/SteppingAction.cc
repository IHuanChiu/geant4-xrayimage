//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RootOutput.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
//  fScoringVolume(0),
//  fScoringVolumeUp(0),
//  fSampleVolume(0)
{
  pointer=this;
  myRootOutput = RootOutput::GetRootInstance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

SteppingAction* SteppingAction::pointer=0;
SteppingAction* SteppingAction::GetInstance()
{
  return pointer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::InitializeInBeginningOfEvent(){
  ReNumber = 7;
  VolumeMap["FoilTubs1"] = 1;
  VolumeMap["intermediate1"] = 2;
  VolumeMap["FoilTubs2"] = 3;
  VolumeMap["intermediate2"] = 4;
  VolumeMap["FoilTubs3"] = 5;
  VolumeMap["Sample"] = 6;
  VolumeMap["Chamber"] = 7;
  VolumeMap["VirTubs"] = 8;
  VolumeMap["VirTubs2"] = 9;
  VolumeMap["BeWindowTubs"] = 10;
  VolumeMap["Holder"] = 11;
  VolumeMap["World"] = 0;
  for(int i=1; i<6+1;i++){
     auto idstr = std::to_string(i);
     VolumeMap["BeTubs"+idstr] = -1;     
     VolumeMap["Ge"+idstr] = -1-i;//-2~-7     
  }
  ProcessMap["muMinusCaptureAtRest"] = 1;
  ProcessMap["phot"] = 2;
  ProcessMap["compt"] = 3;
  ProcessMap["eBrem"] = 4;
  ProcessMap["neutronInelastic"] = 5;
  ProcessMap["muIoni"] = 6;
  ProcessMap["conv"] = 7;

  muhitSampleInThisEvent = false;
  muhitFoil1InThisEvent = false;
  muhitFoil2InThisEvent = false;
  muhitFoil3InThisEvent = false;
  muhitInter1InThisEvent = false;
  muhitInter2InThisEvent = false;
  muhitTargetInThisEvent = false;
  muhitTarget2InThisEvent = false;
  muEscapeInThisEvent = false;
  ngammaHitVolume = 0;
  neletronHitVolume = 0;
  nneutronHitVolume = 0;
  notherHitVolume = 0;
  ParticleID = 0;
  ParentID = 0;
  egammahitSampleInThisEvent = false;
  currentType = 0;
  for (int i = 0; i< nhitMax_indetector; i++){
     ahit_edep[i] = 0.;
     ahit_start_x[i] = -100.;
     ahit_start_y[i] = -100.;
     ahit_start_z[i] = -100.;
     ahit_time_start[i] = 0.;
     ahit_time_end[i] = 0.;
     ahit_nsteps[i] = 0;
     ahit_pdgid[i] = 0;
     ahit_process[i] = 0;
  }
  nSignals=0;//number of signal particles
  IsSameSignal = false;//same signal, depend on time resolution of detector
  IsFirstStep = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  CurrentVolumeName = volume->GetName();
  aTrack        = step->GetTrack();
  pdgID         = aTrack->GetDefinition()->GetPDGEncoding();
  TrackPosition = aTrack->GetPosition();
  TrackMomentum = aTrack->GetMomentum();//three vector
  particleName  = aTrack->GetDefinition()->GetParticleName();
  ParentID      = aTrack->GetParentID();
  Time          = aTrack->GetGlobalTime()/CLHEP::microsecond;
  KineticEnergy = aTrack->GetKineticEnergy()/CLHEP::MeV;

   // muMinusCaptureAtRest : Capture
   //     "compt":     Compton Scattering
   //     "rayleigh":  Rayleigh Scattering
   //     "phot" :     photoelectric effect
   //     "conv" :     pair Creation
   //     "tot"  :     total
   //     muIoni :    muon ionization
   //     eIoni :    eletron ionization
   //     "brems":    Bremsstrahlung

  // =========== init. energy of first particle ===============   
  if (IsFirstStep){ 
    myRootOutput->SetInitEnergy(KineticEnergy);//set n signal
    IsFirstStep = false;
    myRootOutput->h1_init_energy->Fill(KineticEnergy*1000);//[keV], fill first step energy (beam energy)
  }

  // =========== store muon hit position ===============   
  if(abs(pdgID) == 13 && ParentID == 0){// note: before touch physic volume, pdgID is random number
  //if(particleName == "mu-"){ 

    if (VolumeMap[CurrentVolumeName] == 1){//foil-1
       if(!muhitFoil1InThisEvent){
          muhitFoil1InThisEvent = true;
          myRootOutput->SetInitPolInFoil1(TrackPosition);
          myRootOutput->SetInitMomInFoil1(TrackMomentum);
          myRootOutput->SetInitTimeInFoil1(Time);
          myRootOutput->SetInitKineticEnergyInFoil1(KineticEnergy);
       }
    }else if (VolumeMap[CurrentVolumeName] == 2){//inter-1
       if(!muhitInter1InThisEvent){
          muhitInter1InThisEvent = true;
          myRootOutput->SetInitPolInInter1(TrackPosition);
          myRootOutput->SetInitMomInInter1(TrackMomentum);
          myRootOutput->SetInitTimeInInter1(Time);
          myRootOutput->SetInitKineticEnergyInInter1(KineticEnergy);
       }else{//end point 
          myRootOutput->SetEndPolInInter1(TrackPosition);
          myRootOutput->SetEndMomInInter1(TrackMomentum);
          myRootOutput->SetEndTimeInInter1(Time);            
          myRootOutput->SetEndKineticEnergyInInter1(KineticEnergy);            
       }
    }else if (VolumeMap[CurrentVolumeName] == 3){//foil-2
       if(!muhitFoil2InThisEvent){
          muhitFoil2InThisEvent = true;
          myRootOutput->SetInitPolInFoil2(TrackPosition);
          myRootOutput->SetInitMomInFoil2(TrackMomentum);
          myRootOutput->SetInitTimeInFoil2(Time);
          myRootOutput->SetInitKineticEnergyInFoil2(KineticEnergy);
       }
    }else if (VolumeMap[CurrentVolumeName] == 4){//inter-2
       if(!muhitInter2InThisEvent){
          muhitInter2InThisEvent = true;
          myRootOutput->SetInitPolInInter2(TrackPosition);
          myRootOutput->SetInitMomInInter2(TrackMomentum);
          myRootOutput->SetInitTimeInInter2(Time);
          myRootOutput->SetInitKineticEnergyInInter2(KineticEnergy);
       }else{//end point 
          myRootOutput->SetEndPolInInter2(TrackPosition);
          myRootOutput->SetEndMomInInter2(TrackMomentum);
          myRootOutput->SetEndTimeInInter2(Time);            
          myRootOutput->SetEndKineticEnergyInInter2(KineticEnergy);            
       }
    }else if (VolumeMap[CurrentVolumeName] == 5){//foil-3
       if(!muhitFoil3InThisEvent){
          muhitFoil3InThisEvent = true;
          myRootOutput->SetInitPolInFoil3(TrackPosition);
          myRootOutput->SetInitMomInFoil3(TrackMomentum);
          myRootOutput->SetInitTimeInFoil3(Time);
          myRootOutput->SetInitKineticEnergyInFoil3(KineticEnergy);
       }
    }else if (VolumeMap[CurrentVolumeName] == 8){//Vir-1
       if(!muhitTargetInThisEvent){
          muhitTargetInThisEvent = true;
          myRootOutput->SetInitPolInTarget(TrackPosition);
          myRootOutput->SetInitMomInTarget(TrackMomentum);
          myRootOutput->SetInitTimeInTarget(Time);
          myRootOutput->SetInitKineticEnergyInTarget(KineticEnergy);
       }
    }else if (VolumeMap[CurrentVolumeName] == 9){//vir-2
       if(!muhitTarget2InThisEvent){
          muhitTarget2InThisEvent = true;
          myRootOutput->SetInitPolInTarget2(TrackPosition);
          myRootOutput->SetInitMomInTarget2(TrackMomentum);
          myRootOutput->SetInitTimeInTarget2(Time);
          myRootOutput->SetInitKineticEnergyInTarget2(KineticEnergy);
       }

//    }else{//world
//       if(!muEscapeInThisEvent){
//          muEscapeInThisEvent = true;
//          myRootOutput->SetInitPolInWorld(TrackPosition);
//          myRootOutput->SetInitMomInWorld(TrackMomentum);
//          myRootOutput->SetInitTimeInWorld(Time);
//          myRootOutput->SetInitKineticEnergyInWorld(KineticEnergy);
//       }
    }//volume end
      myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);//return final stop position of muon
//      if(aTrack->GetPosition().z()/CLHEP::mm > 400) aTrack->SetTrackStatus(fKillTrackAndSecondaries);
  }//muon end

  // =========== store other particle ===============    
  if(VolumeMap[CurrentVolumeName] < -1){//set sensitivity detectors
    // =========== store signal particle in detector ===============
    for (G4int j=0; j<nSignals; j++) {//loop current all signal particles (matching signal to current step)
       if(std::fabs(Time-ahit_time_end[j]) < GeTimeResolution){ // same signal(macro second)
          IsSameSignal       = true;
          ahit_edep[j]       += step->GetTotalEnergyDeposit();
          ahit_time_end[j]   = Time;
          ahit_nsteps[j]++;
          ahit_length[j]     += step->GetStepLength();
       }else{
          IsSameSignal = false;
       }
    }

    if(!IsSameSignal){//define a new signal
       det_id=(VolumeMap[CurrentVolumeName]+2)*(-1);//which detector is this signal in? CH 0~5 
       ahit_edep[nSignals]       = step->GetTotalEnergyDeposit();
       ahit_start_x[nSignals] = TrackPosition.x();
       ahit_start_y[nSignals] = TrackPosition.y();
       ahit_start_z[nSignals] = TrackPosition.z();
       ahit_time_start[nSignals] = Time;
       ahit_time_end[nSignals]   = Time;
       ahit_nsteps[nSignals]     = 1;
       ahit_length[nSignals]     = step->GetStepLength();
       ahit_pdgid[nSignals]      = pdgID;
       if (aTrack->GetCreatorProcess() != 0){
       ahit_process[nSignals]    = ProcessMap[aTrack->GetCreatorProcess()->GetProcessName()];
       }else{ahit_process[nSignals] = ProcessMap["None"]; }
       myRootOutput->h1_process->Fill(ahit_process[nSignals]);//fill process of signal
       nSignals++;
    }
    myRootOutput->SetnMaxHit(nSignals);//set n signal
    for (G4int i=0; i<nSignals; i++) {//loop all (merged) signals
      //Fill
      myRootOutput->SetSignalInfo(det_id, i, ahit_edep[i], ahit_start_x[i], ahit_start_y[i], ahit_start_z[i], ahit_time_start[i], ahit_time_end[i], ahit_nsteps[i], ahit_length[i], ahit_pdgid[i], ahit_process[i]);
    }
  }//end Ge detector

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

