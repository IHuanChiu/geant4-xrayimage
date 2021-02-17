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
//  VolumeMap["Shelf"] = 2;
//  VolumeMap["Target"] = 3;
  VolumeMap["FoilTubs1"] = 1;
  VolumeMap["FoilTubs2"] = 2;
  VolumeMap["AirTubs"] = 3;
  VolumeMap["FoilTubs3"] = 4;
  VolumeMap["Sample"] = 5;
  VolumeMap["World"] = 0;
  muhitSampleInThisEvent = false;
  muhitCdTeInThisEvent = false;
  muhitCollimatorInThisEvent = false;
  muhitTargetInThisEvent = false;
  muhitKaptonInThisEvent = false;
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


//    std::cout << aTrack->GetDefinition()->GetParticleName()   << "  pdgID : " << abs(pdgID) << " ParticleID : " << aTrack->GetTrackID() << " ParentID : " <<  aTrack->GetParentID() << " step number : " << aTrack->GetCurrentStepNumber() << "  " << CurrentVolumeName << " number : " << VolumeMap[CurrentVolumeName] << " Time : " << aTrack->GetGlobalTime()/CLHEP::microsecond << " Z : " << TrackPosition.z() 
//   << " kin energy : " << aTrack->GetKineticEnergy() <<  " total energy : " << aTrack->GetTotalEnergy()/CLHEP::MeV  <<  " dep energy : " << step->GetTotalEnergyDeposit() 
//   << std::endl;
//   if (aTrack->GetCreatorProcess() != 0) std::cout << " CreatorProcess  : " << aTrack->GetCreatorProcess()->GetProcessName() << " name : " << aTrack->GetDefinition()->GetParticleName() << " ParticleID : " << aTrack->GetTrackID() << " ParentID : " <<  aTrack->GetParentID()  << std::endl;

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
  //if(abs(pdgID) == 13 && ParentID == 0){// note: before touch physic volume, pdgID is random number
  if(particleName == "mu-"){ 


    if (VolumeMap[CurrentVolumeName] == 1){//foil-1
       if(!muhitKaptonInThisEvent){
          muhitKaptonInThisEvent = true;
          myRootOutput->SetInitPolInKapton(TrackPosition);
          myRootOutput->SetInitMomInKapton(TrackMomentum);
          myRootOutput->SetInitTimeInKapton(Time);
          myRootOutput->SetInitKineticEnergyInKapton(KineticEnergy);
       }else{
          myRootOutput->SetEndPolInKapton(TrackPosition);
          myRootOutput->SetEndMomInKapton(TrackMomentum);
          myRootOutput->SetEndTimeInKapton(Time);            
          myRootOutput->SetEndKineticEnergyInKapton(KineticEnergy);            
       }

    }else if(VolumeMap[CurrentVolumeName] == 2){//foil-2
       if (!muhitSampleInThisEvent) {//start point
          muhitSampleInThisEvent = true;
          myRootOutput->SetInitPolInSample(TrackPosition);
          myRootOutput->SetInitMomInSample(TrackMomentum);
          myRootOutput->SetInitTimeInSample(Time);
          myRootOutput->SetInitKineticEnergyInSample(KineticEnergy);
       }else{//end point 
          myRootOutput->SetEndPolInSample(TrackPosition);
          myRootOutput->SetEndMomInSample(TrackMomentum);
          myRootOutput->SetEndTimeInSample(Time);            
          myRootOutput->SetEndKineticEnergyInSample(KineticEnergy);            
       }


    }else if (VolumeMap[CurrentVolumeName] == 3){//foil-3
       if(!muhitTargetInThisEvent){
          muhitTargetInThisEvent = true;
          myRootOutput->SetInitPolInTarget(TrackPosition);
          myRootOutput->SetInitMomInTarget(TrackMomentum);
          myRootOutput->SetInitTimeInTarget(Time);
          myRootOutput->SetInitKineticEnergyInTarget(KineticEnergy);
       }else{
          myRootOutput->SetEndPolInTarget(TrackPosition);
          myRootOutput->SetEndMomInTarget(TrackMomentum);
          myRootOutput->SetEndTimeInTarget(Time);            
          myRootOutput->SetEndKineticEnergyInTarget(KineticEnergy);            
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
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    G4ThreeVector preStepPosition = preStepPoint->GetPosition();
    G4ThreeVector postStepPosition = postStepPoint->GetPosition();
//    myRootOutput->SetDecayPolGlo(postStepPosition);
//    myRootOutput->SetDecayTimeGlo(Time);

    //if(KineticEnergy == 0) myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);
//    if(TrackMomentum.z() >=0){
       myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);//return final stop position of muon
//    }
   
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

