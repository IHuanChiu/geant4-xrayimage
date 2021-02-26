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
  VolumeMap["FoilTubs0"] = 0;
  VolumeMap["FoilTubs1"] = 1;
  VolumeMap["FoilTubs2"] = 2;
  VolumeMap["FoilTubs3"] = 3;
  VolumeMap["intermediate"] = 4;
  VolumeMap["Sample"] = 5;
  VolumeMap["World"] = -1;

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

  // =========== store muon hit position ===============    
  if(abs(pdgID) == 13 && ParentID == 0){// note: before touch physic volume, pdgID is random number

    if (VolumeMap[CurrentVolumeName] == 0){//1mm
       if(!muhitTargetInThisEvent){
          muhitTargetInThisEvent = true;
          myRootOutput->SetInitPolInTarget(TrackPosition);
          myRootOutput->SetInitMomInTarget(TrackMomentum);
          myRootOutput->SetInitTimeInTarget(Time);
          myRootOutput->SetInitKineticEnergyInTarget(KineticEnergy);
       }
    }else if (VolumeMap[CurrentVolumeName] == 1){//50mm
       if(!muhitFoil1InThisEvent){
          muhitFoil1InThisEvent = true;
          myRootOutput->SetInitPolInFoil1(TrackPosition);
          myRootOutput->SetInitMomInFoil1(TrackMomentum);
          myRootOutput->SetInitTimeInFoil1(Time);
          myRootOutput->SetInitKineticEnergyInFoil1(KineticEnergy);
       }
    }else if (VolumeMap[CurrentVolumeName] == 2){//80mm
       if(!muhitFoil2InThisEvent){
          muhitFoil2InThisEvent = true;
          myRootOutput->SetInitPolInFoil2(TrackPosition);
          myRootOutput->SetInitMomInFoil2(TrackMomentum);
          myRootOutput->SetInitTimeInFoil2(Time);
          myRootOutput->SetInitKineticEnergyInFoil2(KineticEnergy);
       }
    }else if (VolumeMap[CurrentVolumeName] == 3){//110mm
       if(!muhitFoil3InThisEvent){
          muhitFoil3InThisEvent = true;
          myRootOutput->SetInitPolInFoil3(TrackPosition);
          myRootOutput->SetInitMomInFoil3(TrackMomentum);
          myRootOutput->SetInitTimeInFoil3(Time);
          myRootOutput->SetInitKineticEnergyInFoil3(KineticEnergy);
       }
    }else if (VolumeMap[CurrentVolumeName] == 4){//Air
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
    }else if (VolumeMap[CurrentVolumeName] == 5){//Sample 250mm
       if(!muhitSampleInThisEvent){
          muhitSampleInThisEvent = true;
          myRootOutput->SetInitPolInSample(TrackPosition);
          myRootOutput->SetInitMomInSample(TrackMomentum);
          myRootOutput->SetInitTimeInSample(Time);
          myRootOutput->SetInitKineticEnergyInSample(KineticEnergy);
       }
    }//volume end

      myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);//return final stop position of muon
   
  }//muon end
  // =========== store other particle ===============    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

