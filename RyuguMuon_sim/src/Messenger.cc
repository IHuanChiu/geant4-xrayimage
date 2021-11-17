#include "DetectorConstruction.hh"
#include "Messenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

#include "G4RunManager.hh"   //cks  included in order to be able to change run ID
#include "Randomize.hh"      //cks  included in order to initialise the random nr. generator by time
#include <time.h>            //cks   -----------------------------||-------------------------------
#include <vector>
#include "globals.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


Messenger::Messenger(DetectorConstruction* myDet)
{ 
  Ignore1Cmd = new G4UIcmdWithAString("/command/sample",this);
  Ignore1Cmd->SetGuidance("This command is ignored by the messenger, but used for the detector construction.");
  Ignore1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  Ignore2Cmd = new G4UIcmdWithAString("/command/rootOutput",this);
  Ignore2Cmd->SetGuidance("This command is ignored by the messenger, but used for the detector construction.");
  Ignore2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  Ignore3Cmd = new G4UIcmdWithAString("/command/beamtype",this);
  Ignore3Cmd->SetGuidance("This command is ignored by the messenger, but used for the detector construction.");
  Ignore3Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Messenger::~Messenger()
{
  delete Ignore1Cmd;
  delete Ignore2Cmd;
  delete Ignore3Cmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
