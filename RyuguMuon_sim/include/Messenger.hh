#ifndef Messenger_h
#define Messenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Messenger: public G4UImessenger
{
  public:
    Messenger(DetectorConstruction*);
   ~Messenger();
    
  private:
    
    G4UIdirectory*             runDir;
    G4UIcmdWithAString*        Ignore1Cmd;
    G4UIcmdWithAString*        Ignore2Cmd;
    G4UIcmdWithAString*        Ignore3Cmd;
    G4UIcmdWithAnInteger*      RunIDSetCmd;

  public:

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

