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
  VolumeMap["CdTe"] = 6; 
  VolumeMap["Si"] = 7; 
  DetNumber = VolumeMap["CdTe0"];

  VolumeMap["AlBaton"] = 4;
  VolumeMap["Target1"] = 3;
  VolumeMap["Target2"] = 3;
  VolumeMap["Target3"] = 3;
  VolumeMap["Target4"] = 3;
  VolumeMap["KaptonTubs"] = 1;
  VolumeMap["KaptonSample"] = 2;
  VolumeMap["World"] = 0;

  ProcessMap["muMinusCaptureAtRest"] = 1;
  ProcessMap["phot"] = 2;
  ProcessMap["compt"] = 3;
  ProcessMap["eBrem"] = 4;
  ProcessMap["neutronInelastic"] = 5;
  ProcessMap["muIoni"] = 6;
  ProcessMap["conv"] = 7;

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
  for (int i = 0; i< nhitMax_indetector; i++){
     ahit_edep[i] = 0.;
     ahit_time_start[i] = 0.;
     ahit_time_end[i] = 0.;
     ahit_nsteps[i] = 0;
     ahit_pdgid[i] = 0;
     ahit_process[i] = 0;
  }
  nSignals=0;//number of signal particles
  IsSameSignal = false;//same signal, depend on time resolution of detector

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  aTrack        = step->GetTrack();
  if (aTrack->GetDefinition()){
     //volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
     volume = aTrack->GetVolume()->GetLogicalVolume();
     CurrentVolumeName = volume->GetName();
   
     particleName  = aTrack->GetDefinition()->GetParticleName();
     pdgID         = aTrack->GetDefinition()->GetPDGEncoding();
     ParentID      = aTrack->GetParentID();
     Time          = aTrack->GetGlobalTime()/CLHEP::microsecond;
     KineticEnergy = aTrack->GetKineticEnergy()/CLHEP::MeV;
     TotalEnergy   = aTrack->GetTotalEnergy()/CLHEP::MeV;
     TrackPosition = aTrack->GetPosition()/CLHEP::mm;

  // =========== store muon hit position ===============   
     if(particleName == "mu-"){ 

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
   
       }else if (VolumeMap[CurrentVolumeName] == 2){//kapton in frond of sample
          if(!muhitCollimatorInThisEvent){
             muhitCollimatorInThisEvent = true;
             myRootOutput->SetInitPolInCollimator(TrackPosition);
             myRootOutput->SetInitTimeInCollimator(Time);
             myRootOutput->SetInitKineticEnergyInCollimator(KineticEnergy);
          }else{
             myRootOutput->SetEndPolInCollimator(TrackPosition);
             myRootOutput->SetEndTimeInCollimator(Time);            
             myRootOutput->SetEndKineticEnergyInCollimator(KineticEnergy);            
          }
   
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
        if(KineticEnergy == 0) myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);//return final stop position of muon
        if(aTrack->GetPosition().z()/CLHEP::mm > 100) aTrack->SetTrackStatus(fKillTrackAndSecondaries);
//        if (sqrt((aTrack->GetPosition().y()/CLHEP::mm)*(aTrack->GetPosition().y()/CLHEP::mm) + (aTrack->GetPosition().x()/CLHEP::mm)*(aTrack->GetPosition().x()/CLHEP::mm)) > 60) aTrack->SetTrackStatus(fKillTrackAndSecondaries);
  }//muon end


     // ============= Designated Position of The Particles ===================
//     if(aTrack->GetPosition().z()/CLHEP::mm < 0 || aTrack->GetPosition().z()/CLHEP::mm > 60) 
//        aTrack->SetTrackStatus(fKillTrackAndSecondaries);
   
     // =========== store detector info. ===============    
     //if (VolumeMap[CurrentVolumeName] >= DetNumber && particleName != "mu-"){//sensitivity volume
     if (VolumeMap[CurrentVolumeName] == 6 && particleName != "mu-"){//sensitivity volume
        // *** energy deposit info. ***
        for (G4int j=0; j<nSignals; j++) {//loop current all signal particles (matching signal to current step)
            if(std::fabs(Time-ahit_time_end[j]) < CdTeTimeResolution){ // same signal (macro second)
               IsSameSignal       = true;
               ahit_edep[j]       += step->GetTotalEnergyDeposit();
               ahit_time_end[j]   = Time;
               ahit_nsteps[j]++;
               ahit_length[j]     += step->GetStepLength();
            }else{
               IsSameSignal = false;
            }
         }
  
         if(!IsSameSignal){//current step isn't belong with pre-particle -> define a new signal
            ahit_edep[nSignals]       = step->GetTotalEnergyDeposit();
            ahit_time_start[nSignals] = Time;
            ahit_time_end[nSignals]   = Time;
            ahit_nsteps[nSignals]     = 1;
            ahit_length[nSignals]     = step->GetStepLength();
            ahit_pdgid[nSignals]      = pdgID;
            ahit_x[nSignals]          = aTrack->GetPosition().x()/CLHEP::mm;
            ahit_y[nSignals]          = aTrack->GetPosition().y()/CLHEP::mm;
            ahit_z[nSignals]          = aTrack->GetPosition().z()/CLHEP::mm;
            if (aTrack->GetCreatorProcess() != 0){
            ahit_process[nSignals]    = ProcessMap[aTrack->GetCreatorProcess()->GetProcessName()];
            }else{ahit_process[nSignals] = ProcessMap["None"]; }
            myRootOutput->h1_process->Fill(ahit_process[nSignals]);//fill process of signal
            nSignals++;
         }
  
         myRootOutput->SetnMaxHit(nSignals);//set n signal
         for (G4int i=0; i<nSignals; i++) {//loop all (merged) signals
           myRootOutput->SetSignalInfo(i, ahit_edep[i], ahit_time_start[i], ahit_time_end[i], ahit_nsteps[i], ahit_length[i], ahit_pdgid[i], ahit_process[i], ahit_x[i], ahit_y[i], ahit_z[i]); //fill to root
         }
 
   
        // *** track info. ***
        if(ParentID != 0 && step->GetTrack()->GetTrackID() != ParticleID){//skip muon & ignore same particle with different step
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
                                       det_x, det_y, det_z, aTrack->GetDefinition()->GetParticleName(), trackprocess, ProcessMap[trackprocess]);
              myRootOutput->FillParticle();
        }//first particle in sensitivity volume

//        /*
        // *** energy deposite & position of the first particle *** (Old method!)
         if(!particleHitCdTe){
            if(aTrack->GetDefinition()->GetParticleName() == "gamma") particleHitCdTe = true;//first position of gamma
            myRootOutput->SetParticlePositionInVolume(VolumeMap[CurrentVolumeName]-DetNumber, aTrack->GetPosition().x()/CLHEP::mm+0.0, aTrack->GetPosition().y()/CLHEP::mm+0.0, aTrack->GetPosition().z()/CLHEP::mm);
         }
         myRootOutput->SetEnergyDepositInVolume(VolumeMap[CurrentVolumeName]-DetNumber, aTrack->GetDefinition()->GetParticleName(), step->GetTotalEnergyDeposit()/CLHEP::MeV);
        // */
     }//end : if(VolumeMap[CurrentVolumeName] >= 2)
  }// end : if (aTrack->GetDefinition())

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

