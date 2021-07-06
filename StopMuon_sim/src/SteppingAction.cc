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
  VolumeMap["World"] = 0;
  for(int i=0; i<10;i++){
     auto idstr = std::to_string(i);
     VolumeMap["GeTubs"+idstr] = -1;     
  }

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
  EnergyDeposit = 0; 
  EnergyDeposit_gamma = 0; 
  EnergyDeposit_e = 0; 
  EnergyDeposit_other = 0; 
  egamma_hittime = -1000; 
  egammahitSampleInThisEvent = false;
  currentType = 0;
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

    //global info.
//    G4StepPoint* preStepPoint = step->GetPreStepPoint();
//    G4StepPoint* postStepPoint = step->GetPostStepPoint();
//    G4ThreeVector preStepPosition = preStepPoint->GetPosition();
//    G4ThreeVector postStepPosition = postStepPoint->GetPosition();
//    myRootOutput->SetDecayPolGlo(postStepPosition);
//    myRootOutput->SetDecayTimeGlo(Time);

    //if(KineticEnergy == 0) myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);
//      if(TrackMomentum.z()/CLHEP::mm <= -1 && (std::fabs(TrackMomentum.x()/CLHEP::mm)>60 || std::fabs(TrackMomentum.y()/CLHEP::mm)>60)){
//        aTrack->SetTrackStatus(fKillTrackAndSecondaries);
//       }
      myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);//return final stop position of muon
   
  }//muon end
  // =========== store other particle ===============    

 /*
   if(VolumeMap[CurrentVolumeName] == 3){//count number of particles

//     if(aTrack->GetCurrentStepNumber() == 1){//muMinusCaptureAtRest gamma is from step 3~5
     if(ParentID != 0 && step->GetTrack()->GetTrackID() != ParticleID){// not muon && not same particle
        ParticleID = aTrack->GetTrackID();
        if(aTrack->GetDefinition()->GetParticleName() == "gamma"){   
                  ngammaHitVolume++;
        }else if(aTrack->GetDefinition()->GetParticleName() == "e+" || aTrack->GetDefinition()->GetParticleName() == "e-"){   
                  neletronHitVolume++;
        }else if(aTrack->GetDefinition()->GetParticleName() == "neutron" || aTrack->GetDefinition()->GetParticleName() == "anti_neutron"){ 
                  nneutronHitVolume++;
        }else{    
                  notherHitVolume++;} 
        myRootOutput->SetnParticleHitVolume(VolumeMap[CurrentVolumeName], 
                                            ngammaHitVolume, 
                                            neletronHitVolume, 
                                            nneutronHitVolume,
                                            notherHitVolume);

        Kinetic_e     = aTrack->GetKineticEnergy()/CLHEP::MeV;
        Total_e       = aTrack->GetTotalEnergy()/CLHEP::MeV;
        det_x         = TrackPosition.x();
        det_y         = TrackPosition.y();
        det_z         = TrackPosition.z();

        myRootOutput->StoreTrack(0, pdgID, //detector_index is N/A
                                 KineticEnergy, Total_e, step->GetTotalEnergyDeposit(), 
                                 det_x, det_y, det_z, aTrack->GetGlobalTime()/CLHEP::microsecond);
        myRootOutput->FillParticle();
     }//first particle


    // Store Event Signal
    if(aTrack->GetCreatorProcess() != 0 && aTrack->GetCreatorProcess()->GetProcessName() == "muMinusCaptureAtRest"){
     if(!muhitSampleInThisEvent){ muhitSampleInThisEvent = true; egamma_hittime = Time;}
     if(aTrack->GetDefinition()->GetParticleName() == "gamma"){ currentType =1;
     }else if(aTrack->GetDefinition()->GetParticleName() == "e-"){ currentType =2;
     }else{currentType =3;}
    }

    if(currentType != 0) EnergyDeposit += step->GetTotalEnergyDeposit();
    if(currentType == 1) EnergyDeposit_gamma += step->GetTotalEnergyDeposit();
    if(currentType == 2) EnergyDeposit_e += step->GetTotalEnergyDeposit();
    if(currentType == 3) EnergyDeposit_other += step->GetTotalEnergyDeposit(); 

    myRootOutput->SetDetectorInfo(EnergyDeposit, EnergyDeposit_e, EnergyDeposit_gamma, EnergyDeposit_other, egamma_hittime);

   }//in target
// */

  // get track
//  preStep       = step->GetPreStepPoint();
//  postStep      = step->GetPostStepPoint();//same with current step
  
//  kinenergy_pre = preStep->GetKineticEnergy();//for muon, kinenergy_pre = 0 is for stop (decay point?)
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

