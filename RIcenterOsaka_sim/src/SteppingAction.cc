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
  VolumeMap["Sample"] = 1;
  VolumeMap["World"] = 0;
  for(int i=1; i<6+1;i++){
     auto idstr = std::to_string(i);
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

  // =========== ONLY for gamma source ===============   
  if (IsFirstStep){ 
    myRootOutput->SetnInitEnergy(KineticEnergy);//set n signal
    IsFirstStep = false;
  }

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
       nSignals++;
    }
    myRootOutput->SetnMaxHit(nSignals);//set n signal
    for (G4int i=0; i<nSignals; i++) {//loop all (merged) signals
      myRootOutput->SetSignalInfo(det_id, i, ahit_edep[i], ahit_start_x[i], ahit_start_y[i], ahit_start_z[i], ahit_time_start[i], ahit_time_end[i], ahit_nsteps[i], ahit_length[i], ahit_pdgid[i], ahit_process[i]); //fill to root
    }
  }//end Ge detector
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

