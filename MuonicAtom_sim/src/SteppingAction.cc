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
  VolumeMap["CdTe5"] = 11;
  VolumeMap["CdTe4"] = 10;
  VolumeMap["CdTe3"] = 9;
  VolumeMap["CdTe2"] = 8;
  VolumeMap["CdTe1"] = 7;
  VolumeMap["CdTe0"] = 6; 
  DetNumber = VolumeMap["CdTe0"];

  VolumeMap["Target1"] = 5;
  VolumeMap["Target2"] = 4;
  VolumeMap["Target3"] = 3;
  VolumeMap["Target4"] = 2;
  VolumeMap["KaptonTubs"] = 1;
  VolumeMap["World"] = 0;

  muhitSampleInThisEvent = false;
  muhitCdTeInThisEvent = false;
  muhitCollimatorInThisEvent = false;
  muhitShadowInThisEvent = false;
  muhitKaptonInThisEvent = false;
  muEscapeInThisEvent = false;
  
  particleHitCdTe = false;

  for(int i = 0; i<10; i++){
    ngammaHitVolume[i] = 0;
    neletronHitVolume[i] = 0;
    nneutronHitVolume[i] = 0;
  }
  ParticleID = 0;
  ParentID = 0;
  CurrentProcess = "";
  SignalType = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  CurrentVolumeName = volume->GetName();

  // get track
//  preStep       = step->GetPreStepPoint();
//  postStep      = step->GetPostStepPoint();//same with current step
//  kinenergy_pre = preStep->GetKineticEnergy();//for muon, kinenergy_pre = 0 is for stop (decay point?)
  aTrack        = step->GetTrack();
  particleName  = aTrack->GetDefinition()->GetParticleName();
  pdgID         = aTrack->GetDefinition()->GetPDGEncoding();
  ParentID      = aTrack->GetParentID();


//  std::cout << particleName   << "  pdgID : " << abs(pdgID) << " Particle ID : " << step->GetTrack()->GetTrackID() << " ParentID : " << aTrack->GetParentID()  << "  " << CurrentVolumeName << " number : " << VolumeMap[CurrentVolumeName] << " Time : " << Time << " Z : " << TrackPosition.z() <<  " energy : " << aTrack->GetKineticEnergy() << std::endl;

  // =========== store muon hit position ===============    
  if(abs(pdgID) == 13){// note: before touch physic volume, pdgID is random number
//  if((particleName == "mu-" || particleName == "mu+") && ParentID == 0){
    if(ParentID != 0) return;
    TrackPosition     = aTrack->GetPosition();//aTrack->GetPolarization();
    Time              = aTrack->GetGlobalTime()/CLHEP::microsecond;
    KineticEnergy     = aTrack->GetKineticEnergy()/CLHEP::MeV;
    myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);//return final stop position of muon
//    if(TrackPosition.z() > 70*CLHEP::mm) return;//kill long term muon

//    if (VolumeMap[CurrentVolumeName] >= 7){//cdte
//       detector_index = VolumeMap[CurrentVolumeName]-ReNumber; 
//       if(!muhitCdTeInThisEvent){//start point
//          muhitCdTeInThisEvent = true;
//          myRootOutput->SetInitPolInCdTe(detector_index,TrackPosition);
//          myRootOutput->SetInitTimeInCdTe(detector_index,Time);
//          myRootOutput->SetInitKineticEnergyInCdTe(detector_index,KineticEnergy);
//         }else{//end point
//     //  }else if(!kinenergy_pre){//end point => not first step(!muhitCdTeInThisEvent) and not middle step(kinenergy_pre == 0)
//          myRootOutput->SetEndPolInCdTe(detector_index,TrackPosition);
//          myRootOutput->SetEndTimeInCdTe(detector_index,Time);            
//          myRootOutput->SetEndKineticEnergyInCdTe(detector_index,KineticEnergy);            
//       }

    if (VolumeMap[CurrentVolumeName] == 1){//kapton
       if(!muhitKaptonInThisEvent){
          muhitKaptonInThisEvent = true;
          myRootOutput->SetInitPolInKapton(TrackPosition);
          myRootOutput->SetInitTimeInKapton(Time);
          myRootOutput->SetInitKineticEnergyInKapton(KineticEnergy);
       }else{
          myRootOutput->SetEndPolInKapton(TrackPosition);
          myRootOutput->SetEndTimeInKapton(Time);            
          myRootOutput->SetEndKineticEnergyInKapton(KineticEnergy);            
       }

//    }else if (VolumeMap[CurrentVolumeName] == 2){//collimator
//       if(!muhitCollimatorInThisEvent){
//          muhitCollimatorInThisEvent = true;
//          myRootOutput->SetInitPolInCollimator(TrackPosition);
//          myRootOutput->SetInitTimeInCollimator(Time);
//          myRootOutput->SetInitKineticEnergyInCollimator(KineticEnergy);
//       }else{
//          myRootOutput->SetEndPolInCollimator(TrackPosition);
//          myRootOutput->SetEndTimeInCollimator(Time);            
//          myRootOutput->SetEndKineticEnergyInCollimator(KineticEnergy);            
//       }

    }else if(CurrentVolumeName.find("Target") != std::string::npos){//sample
       if (!muhitSampleInThisEvent) {//start point
          muhitSampleInThisEvent = true;
          myRootOutput->SetInitPolInSample(TrackPosition);
          myRootOutput->SetInitTimeInSample(Time);
          myRootOutput->SetInitKineticEnergyInSample(KineticEnergy);
       }else{//end point 
          myRootOutput->SetEndPolInSample(TrackPosition);
          myRootOutput->SetEndTimeInSample(Time);            
          myRootOutput->SetEndKineticEnergyInSample(KineticEnergy);            
       }


//    }else if (VolumeMap[CurrentVolumeName] == 3 || VolumeMap[CurrentVolumeName] == 6){//shadow
//       if(!muhitShadowInThisEvent){
//          muhitShadowInThisEvent = true;
//          myRootOutput->SetInitPolInShadow(TrackPosition);
//          myRootOutput->SetInitTimeInShadow(Time);
//          myRootOutput->SetInitKineticEnergyInShadow(KineticEnergy);
//       }else{
//          myRootOutput->SetEndPolInShadow(TrackPosition);
//          myRootOutput->SetEndTimeInShadow(Time);            
//          myRootOutput->SetEndKineticEnergyInShadow(KineticEnergy);            
//       }

    }else{//world
       if(!muEscapeInThisEvent){
          muEscapeInThisEvent = true;
          myRootOutput->SetInitPolInWorld(TrackPosition);
          myRootOutput->SetInitTimeInWorld(Time);
          myRootOutput->SetInitKineticEnergyInWorld(KineticEnergy);
       }
//       }else{
//          myRootOutput->SetEndPolInWorld(TrackPosition);
//          myRootOutput->SetEndTimeInWorld(Time);            
//          myRootOutput->SetEndKineticEnergyInWorld(KineticEnergy);            
//       }
    }//volume end
     if (aTrack->GetPosition().z()/CLHEP::mm > 45) aTrack->SetTrackStatus(fKillTrackAndSecondaries);
     if (sqrt((aTrack->GetPosition().y()/CLHEP::mm)*(aTrack->GetPosition().y()/CLHEP::mm) + (aTrack->GetPosition().x()/CLHEP::mm)*(aTrack->GetPosition().x()/CLHEP::mm)) > 60) aTrack->SetTrackStatus(fKillTrackAndSecondaries);
  }//muon end
  else{

  // ============= Designated Position of The Particles ===================
  if(aTrack->GetPosition().z()/CLHEP::mm < 0 || aTrack->GetPosition().z()/CLHEP::mm > 60) 
     aTrack->SetTrackStatus(fKillTrackAndSecondaries);

//  if(aTrack->GetPosition().y() >= 0){
//     Solidangle_Width = (109+2.7-aTrack->GetPosition().y()/CLHEP::mm)*(1.5/2.7);
//     plane_width = std::sqrt((aTrack->GetPosition().x()/CLHEP::mm - 0)*(aTrack->GetPosition().x()/CLHEP::mm - 0) + (aTrack->GetPosition().z()/CLHEP::mm - 30.5)*(aTrack->GetPosition().z()/CLHEP::mm - 30.5));
//     if(plane_width > Solidangle_Width) aTrack->SetTrackStatus(fKillTrackAndSecondaries);
//  }else{
//     Solidangle_Width = (aTrack->GetPosition().y()/CLHEP::mm+109+2.7)*(1.5/2.7);
//     plane_width = std::sqrt((aTrack->GetPosition().x()/CLHEP::mm - 0)*(aTrack->GetPosition().x()/CLHEP::mm - 0) + (aTrack->GetPosition().z()/CLHEP::mm - 30.5)*(aTrack->GetPosition().z()/CLHEP::mm - 30.5));
//     if(plane_width > Solidangle_Width) aTrack->SetTrackStatus(fKillTrackAndSecondaries);
//  }

  // =========== store detector info. ===============    
  if (VolumeMap[CurrentVolumeName] >= DetNumber){//detector only
     // *** track info. ***
     if(ParentID != 0){//skip muon
        if(step->GetTrack()->GetTrackID() != ParticleID){//ignore same particle with different step
           ParticleID    = aTrack->GetTrackID();
           myRootOutput->ScannParticleHitVolume(VolumeMap[CurrentVolumeName]-DetNumber, aTrack->GetDefinition()->GetParticleName());//scan the number of hit particle 
   
           Kinetic_e         = aTrack->GetKineticEnergy()/CLHEP::MeV;
           Total_e           = aTrack->GetTotalEnergy()/CLHEP::MeV;
           det_x             = aTrack->GetPosition().x()/CLHEP::mm;
           det_y             = aTrack->GetPosition().y()/CLHEP::mm;
           det_z             = aTrack->GetPosition().z()/CLHEP::mm;
           det_pdgid         = pdgID;
   
           if (aTrack->GetCreatorProcess() != 0){ trackprocess   = aTrack->GetCreatorProcess()->GetProcessName();
           }else{ trackprocess = "None";}
   
           myRootOutput->StoreTrack(VolumeMap[CurrentVolumeName]-DetNumber, det_pdgid, 
                                    Kinetic_e, Total_e, 
                                    det_x, det_y, det_z, aTrack->GetDefinition()->GetParticleName(), trackprocess);
           myRootOutput->FillParticle();
        }
     }
     // *** energy deposite & position of the first particle ***
     if(!particleHitCdTe){
        if(aTrack->GetDefinition()->GetParticleName() == "gamma") particleHitCdTe = true;//first position of gamma
        myRootOutput->SetParticlePositionInVolume(VolumeMap[CurrentVolumeName]-DetNumber, aTrack->GetPosition().x()/CLHEP::mm+0.0, aTrack->GetPosition().y()/CLHEP::mm+0.0, aTrack->GetPosition().z()/CLHEP::mm);
     }
     myRootOutput->SetEnergyDepositInVolume(VolumeMap[CurrentVolumeName]-DetNumber, aTrack->GetDefinition()->GetParticleName(), step->GetTotalEnergyDeposit()/CLHEP::MeV);

  }// only CdTe detector

  }//other particles end

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

