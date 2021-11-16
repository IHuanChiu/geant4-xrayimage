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

#ifndef EventAction_h
#define EventAction_h 1

#include "RootOutput.hh"
#include "G4UserEventAction.hh"
#include "G4THitsMap.hh"
#include "globals.hh"

#include "TFile.h"
#include "TTree.h"
class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4int id, G4double edep) { fEdep[id] += edep; }
    void AddEdep_e(G4int id, G4double edep) { fEdep_e[id] += edep; }
    void AddEdep_gamma(G4int id, G4double edep) { fEdep_gamma[id] += edep; }
    void AddEdep_other(G4int id, G4double edep) { fEdep_other[id] += edep; }
    static const int numberOfdetectors = 2; 

  private:
    //IH
    G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
                                           const G4Event* event) const;
    G4double GetSum(G4THitsMap<G4double>* hitsMap) const;
    time_t   timeOfRunStart;
    time_t   curr;
    std::chrono::high_resolution_clock::time_point run_pre;
    double runtime;

    G4double     fEdep[numberOfdetectors];
    G4double     fEdep_e[numberOfdetectors];
    G4double     fEdep_gamma[numberOfdetectors];
    G4double     fEdep_other[numberOfdetectors];

    RunAction* fRunAction;
    G4int        store_det;
    Double_t energyDep;

    static G4int  nHowOftenToPrintEvent;

  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
