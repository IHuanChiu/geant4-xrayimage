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

#include "EventAction.hh"
#include "RunAction.hh"
#include "RootOutput.hh"
#include "SteppingAction.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"
#include <sys/stat.h>

G4int  EventAction::nHowOftenToPrintEvent=10000;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  timeOfRunStart(-1000)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}


G4THitsMap<G4double>*
EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  G4cout << "\n============================================================\n"
	 << "Begin of event: "<< event->GetEventID() << G4endl
	 << "-------------------------------------------------------------\n";
	
  auto hitsCollection
    = static_cast<G4THitsMap<G4double>*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("B4dEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{   
  SteppingAction::GetInstance()->InitializeInBeginningOfEvent();

  for(int i = 0; i < numberOfdetectors; i++){
    fEdep[i] = 0.;
    fEdep_e[i] = 0.;
    fEdep_gamma[i] = 0.;
    fEdep_other[i] = 0.;
  }
  long thisEventNr = (long) (evt->GetEventID());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0.;
  for ( auto it : *hitsMap->GetMap() ) {
    sumValue += *(it.second);
  }
  return sumValue;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{   

  // accumulate statistics in run action
  long thisEventNr = (long) event->GetEventID();
  //information
  G4RunManager* fRunManager = G4RunManager::GetRunManager();
  RootOutput* myRootOutput = RootOutput::GetRootInstance();

  myRootOutput->SetRunID(fRunManager->GetCurrentRun()->GetRunID());
  myRootOutput->SetEventID(fRunManager->GetCurrentEvent()->GetEventID());

//  for(int idet = 0; idet < numberOfdetectors; idet++) {
//     if((0.026 < fEdep_gamma[idet] && fEdep_gamma[idet] < 0.033) || (0.026 < fEdep[idet] && fEdep[idet] < 0.033))std::cout <<  " id : " << idet <<" total : " << fEdep[idet] << " Sum : " << fEdep_e[idet] + fEdep_gamma[idet] + fEdep_other[idet] << " e : " << fEdep_e[idet] << " gamma : " << fEdep_gamma[idet] << " other : " << fEdep_other[idet] << std::endl; 
//     myRootOutput->SetDetectorInfo(idet,fEdep[idet], fEdep_e[idet], fEdep_gamma[idet], fEdep_other[idet]);//reset detector ID to ROOT 
//  }
  if(thisEventNr == 1) run_pre = std::chrono::high_resolution_clock::now();//ignore first event
  std::chrono::high_resolution_clock::time_point run_cur = std::chrono::high_resolution_clock::now();
  runtime  = double ( std::chrono::duration_cast<std::chrono::seconds>( run_cur - run_pre ).count() );
  myRootOutput->SetRunTime(runtime);
  myRootOutput->FillEvent();
  run_pre = std::chrono::high_resolution_clock::now();

  // time
  if (timeOfRunStart == -1000) timeOfRunStart=time(0);
  if ((thisEventNr != 0) && (thisEventNr%nHowOftenToPrintEvent == 0)) {
    time_t curr=time(0);
    //char * ctime(const time_t * tp);
    G4cout << ">>> Event " << event->GetEventID() <<". Running already for "<<curr-timeOfRunStart<<" seconds.  Present time: "<< ctime(&curr);
    G4cout.flush();
  }

//  #endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
