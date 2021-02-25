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
  VolumeMap["Sample"] = 1;
  VolumeMap["GeTubs_down"] = 2;
  VolumeMap["GeTubs_up"] = 3;
  VolumeMap["SciUPLog"] = 4;
  VolumeMap["SciDownLog"] = 5;
  VolumeMap["World"] = 0;
  VolumeMap["BeamColli"] = -1;
  ProcessMap["muMinusCaptureAtRest"] = 1;
  ProcessMap["phot"] = 2;
  ProcessMap["compt"] = 3;
  ProcessMap["eBrem"] = 4;
  ProcessMap["neutronInelastic"] = 5;
  ProcessMap["muIoni"] = 6;
  ProcessMap["conv"] = 7;
  muhitSampleInThisEvent = false;
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
  EndTimeUp=0;
  EndTimeDown=0;
  IsSameElectronUp = false;//same electron, depend on time resolution of detector
  IsSameElectronDown = false;//same electron, depend on time resolution of detector
  sciID=0;
  neleHitSci_UP=0; 
  nphotonHitSci_UP=0; 
  neleHitSci_DOWN=0; 
  nphotonHitSci_DOWN=0; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  aTrack        = step->GetTrack();
  if (aTrack->GetDefinition()){

     volume = aTrack->GetVolume()->GetLogicalVolume();
     CurrentVolumeName = volume->GetName();
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
//     if(abs(pdgID) == 13 && ParentID == 0){// note: before touch physic volume, pdgID is random number
     if(particleName == "mu-"){// note: before touch physic volume, pdgID is random number
     //TODO check if these two lines are same

       if(VolumeMap[CurrentVolumeName] == 1){//sample
       
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

       }//volume end

       if(KineticEnergy == 0) myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);//return final stop position of muon
       if (aTrack->GetPosition().z()/CLHEP::mm > 83) aTrack->SetTrackStatus(fKillTrackAndSecondaries);//escape muon
       if (VolumeMap[CurrentVolumeName] == 2 || VolumeMap[CurrentVolumeName] == 3) aTrack->SetTrackStatus(fKillTrackAndSecondaries);//muon hit Ge
       
     }//muon end

     if((VolumeMap[CurrentVolumeName] == 2 || VolumeMap[CurrentVolumeName] == 3) && particleName != "mu-"){//Ge sensitivity detectors
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
          ahit_edep[nSignals]       = step->GetTotalEnergyDeposit();
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
         myRootOutput->SetSignalInfo(i, ahit_edep[i], ahit_time_start[i], ahit_time_end[i], ahit_nsteps[i], ahit_length[i], ahit_pdgid[i], ahit_process[i]); //fill to root
       } 

       // =========== all particles in detector ==============
       if(ParentID != 0 && step->GetTrack()->GetTrackID() != ParticleID){// not muon && not same particle
          ParticleID = aTrack->GetTrackID();

          Kinetic_e     = aTrack->GetKineticEnergy()/CLHEP::MeV;
          Total_e       = aTrack->GetTotalEnergy()/CLHEP::MeV;
          det_x         = TrackPosition.x();
          det_y         = TrackPosition.y();
          det_z         = TrackPosition.z();
          if (aTrack->GetCreatorProcess() != 0){ trackprocess   = aTrack->GetCreatorProcess()->GetProcessName();
          }else{ trackprocess = "None";}

          myRootOutput->StoreTrack(0, pdgID, //detector_index is N/A
                                   KineticEnergy, Total_e, step->GetTotalEnergyDeposit(), 
                                   det_x, det_y, det_z, aTrack->GetDefinition()->GetParticleName(), trackprocess);
          myRootOutput->FillParticle();
       }//first particle
     }//end Ge detectors
     
     if(VolumeMap[CurrentVolumeName] == 4 && particleName != "mu-"){//Scintillator sensitivity detectors
        if(abs(pdgID) == 11 && KineticEnergy > 0.1 ){
           if(std::fabs(Time-EndTimeUp) < SciTimeResolution){
             IsSameElectronUp=true;
           }else{
             IsSameElectronUp=false;
           }
           EndTimeUp = Time;
           if(!IsSameElectronUp) neleHitSci_UP++;
        }
        if(abs(pdgID) == 22) nphotonHitSci_UP++;
        myRootOutput->CountUpSciDet(neleHitSci_UP, nphotonHitSci_UP);
     }
     if(VolumeMap[CurrentVolumeName] == 5 && particleName != "mu-"){//Scintillator sensitivity detectors
        if(abs(pdgID) == 11 && KineticEnergy > 0.1) {
           if(std::fabs(Time-EndTimeDown) < SciTimeResolution){
             IsSameElectronDown=true;
           }else{
             IsSameElectronDown=false;
           }
           EndTimeDown = Time;
           if(!IsSameElectronDown) neleHitSci_DOWN++;
        }
        if(abs(pdgID) == 22) nphotonHitSci_DOWN++;
        myRootOutput->CountDownSciDet(neleHitSci_DOWN, nphotonHitSci_DOWN);
     }
     

  }// end : if (aTrack->GetDefinition())
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

