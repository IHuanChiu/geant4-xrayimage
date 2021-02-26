#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();
  
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  void SetVerboseLevel(G4int);

private:
  G4VPhysicsConstructor*    emPhysicsList;
  G4VPhysicsConstructor*    decPhysicsList;
  //G4VPhysicsConstructor*    raddecayList;
  std::vector<G4VPhysicsConstructor*> hadronPhys;
};


#endif
