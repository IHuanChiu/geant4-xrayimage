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
using  namespace  std;

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

  VolumeMap["Target"] = 0;
  VolumeMap["GeTubs0"] = 1;
  VolumeMap["GeTubs1"] = 2;
  VolumeMap["GeTubs2"] = 3;
  VolumeMap["GeTubs3"] = 4;
  VolumeMap["GeTubs4"] = 5;
  VolumeMap["GeTubs5"] = 6;
  VolumeMap["World"] = -1;

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
  muhitTargetInThisEvent = false;
  muhitKaptonInThisEvent = false;
  muEscapeInThisEvent = false;
  xrayhitGeDetector = false;
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
     
//    std::cout << aTrack->GetDefinition()->GetParticleName()   << "  pdgID : " << abs(pdgID) << " ParticleID : " << aTrack->GetTrackID() << " ParentID : " <<  aTrack->GetParentID() << " step number : " << aTrack->GetCurrentStepNumber() << "  " << CurrentVolumeName << " number : " << VolumeMap[CurrentVolumeName] << " Time : " << aTrack->GetGlobalTime()/CLHEP::microsecond << " Z : " << TrackPosition.z() 
//   << " kin energy : " << aTrack->GetKineticEnergy() <<  " total energy : " << aTrack->GetTotalEnergy()/CLHEP::MeV  <<  " dep energy : " << step->GetTotalEnergyDeposit() 
//   << std::endl;
//   if (aTrack->GetCreatorProcess() != 0) std::cout << " CreatorProcess  : " << aTrack->GetCreatorProcess()->GetProcessName() << " name : " << aTrack->GetDefinition()->GetParticleName() << " ParticleID : " << aTrack->GetTrackID() << " ParentID : " <<  aTrack->GetParentID()  << std::endl;

     // =========== store muon hit position ===============    
     if(abs(pdgID) == 13 && ParentID == 0){// note: before touch physic volume, pdgID is random number
//     if(particleName == "mu-"){// note: before touch physic volume, pdgID is random number

       if(VolumeMap[CurrentVolumeName] == 0){//sample
       
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

       //global info.
       G4StepPoint* preStepPoint = step->GetPreStepPoint();
       G4StepPoint* postStepPoint = step->GetPostStepPoint();
       G4ThreeVector preStepPosition = preStepPoint->GetPosition();
       G4ThreeVector postStepPosition = postStepPoint->GetPosition();
       myRootOutput->SetDecayPolGlo(postStepPosition);
       myRootOutput->SetDecayTimeGlo(Time);

       myRootOutput->SetmuFinalVolume(VolumeMap[CurrentVolumeName]);//return final stop position of muon
       
     }//muon end

     if(VolumeMap[CurrentVolumeName] >= 1 && abs(pdgID) == 22 && !xrayhitGeDetector){//set sensitivity detectors
        xrayhitGeDetector=true;
        det_id=VolumeMap[CurrentVolumeName];
        myRootOutput->SetXrayInfo(det_id,KineticEnergy);      
//        myRootOutput->FillXrays();
     }//Ge det1~6

//     if(VolumeMap[CurrentVolumeName] >= 1){//set sensitivity detectors
//       // =========== store signal particle in detector ===============    
//       for (G4int j=0; j<nSignals; j++) {//loop current all signal particles (matching signal to current step)
//          if(std::fabs(Time-ahit_time_end[j]) < GeTimeResolution){ // same signal(macro second)
//             IsSameSignal       = true;
//             ahit_edep[j]       += step->GetTotalEnergyDeposit();
//             ahit_time_end[j]   = Time;   
//             ahit_nsteps[j]++;
//             ahit_length[j]     += step->GetStepLength();
//          }else{
//             IsSameSignal = false;
//          }
//       }
//
//       if(!IsSameSignal){//define a new signal
//          ahit_edep[nSignals]       = step->GetTotalEnergyDeposit();
//          ahit_start_x[nSignals] = TrackPosition.x();
//          ahit_start_y[nSignals] = TrackPosition.y();
//          ahit_start_z[nSignals] = TrackPosition.z();
//          ahit_time_start[nSignals] = Time;
//          ahit_time_end[nSignals]   = Time;
//          ahit_nsteps[nSignals]     = 1;     
//          ahit_length[nSignals]     = step->GetStepLength();
//          ahit_pdgid[nSignals]      = pdgID; 
//          if (aTrack->GetCreatorProcess() != 0){
//          ahit_process[nSignals]    = ProcessMap[aTrack->GetCreatorProcess()->GetProcessName()];
//          }else{ahit_process[nSignals] = ProcessMap["None"]; }
//          myRootOutput->h1_process->Fill(ahit_process[nSignals]);//fill process of signal
//          nSignals++;
//       }
//
//       myRootOutput->SetnMaxHit(nSignals);//set n signal
//       for (G4int i=0; i<nSignals; i++) {//loop all (merged) signals
//         myRootOutput->SetSignalInfo(i, ahit_edep[i], ahit_start_x[i], ahit_start_y[i], ahit_start_z[i], ahit_time_start[i], ahit_time_end[i], ahit_nsteps[i], ahit_length[i], ahit_pdgid[i], ahit_process[i]); //fill to root
//       } 
//     }//end Ge detector

  }// end : if (aTrack->GetDefinition())
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

