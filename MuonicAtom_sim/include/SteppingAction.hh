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

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4ThreeVector.hh"
#include "globals.hh"

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "DetectorConstruction.hh"

#include "RootOutput.hh"

class EventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class SteppingAction : public G4UserSteppingAction
{
  public:
    static SteppingAction* GetInstance();
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);
    static const int numberOfvolume = 10;
    void InitializeInBeginningOfEvent();

    static const int nhitMax_indetector = 100;

  private:
    static SteppingAction* pointer;
    RootOutput* myRootOutput;

    G4bool muhitSampleInThisEvent;
    G4bool muhitCdTeInThisEvent;
    G4bool muhitCollimatorInThisEvent;
    G4bool muhitShadowInThisEvent;
    G4bool muhitKaptonInThisEvent;
    G4bool muEscapeInThisEvent;
    G4bool particleHitCdTe;
    G4int ngammaHitVolume[10];
    G4int neletronHitVolume[10];
    G4int nneutronHitVolume[10];

    EventAction*  fEventAction;
//    G4LogicalVolume* fScoringVolume;
//    G4LogicalVolume* fScoringVolumeUp;
//    G4LogicalVolume* fSampleVolume;
    G4LogicalVolume* volume;
    G4Track* aTrack;
//    G4StepPoint* preStep;
//    G4StepPoint* postStep; 
    
    std::map<G4String, G4int> VolumeMap;
    std::map<G4String, G4int> ProcessMap;

    G4String particleName;       
    G4ThreeVector TrackPosition;
    G4double Time; 
    G4double KineticEnergy; 
    G4double TotalEnergy; 
//    G4double kinenergy_pre; 
    G4int pdgID;                 
    G4int ParticleID;            
    G4int ParentID;             
    G4int DetNumber;            
//    G4double edepStep[numberOfvolume];           

    G4double Kinetic_e;          
    G4double Total_e;            
    G4double det_x;
    G4double det_y;
    G4double det_z;
    G4double det_pdgid;
    G4String trackprocess;

//    G4int detector_index;
    G4String CurrentVolumeName;
    G4String CurrentProcess;
    G4bool SignalType; 

    // === for energy deposit ===
    G4double CdTeTimeResolution = 0.002;//2ns
    G4bool IsSameSignal;

    G4int idet;
    G4double rot_angle;
    G4double ahit_edep[nhitMax_indetector];
    G4double ahit_time_start[nhitMax_indetector];
    G4double ahit_time_end[nhitMax_indetector];
    G4int ahit_nsteps[nhitMax_indetector];
    G4double ahit_length[nhitMax_indetector];
    G4double ahit_x[nhitMax_indetector];
    G4double ahit_y[nhitMax_indetector];
    G4double ahit_z[nhitMax_indetector];
    G4int ahit_pdgid[nhitMax_indetector];
    G4int ahit_process[nhitMax_indetector];
    G4int nSignals;//number of signal particles
    G4int det_id;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
