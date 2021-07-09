#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();
  
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void ConstructAdditionalProcess();
  void SetVerboseLevel(G4int);

private:
  G4VPhysicsConstructor*    emPhysicsList;
  G4VPhysicsConstructor*    decPhysicsList;
  G4VPhysicsConstructor*    decMuonicPhysicsList;
  G4VPhysicsConstructor*    raddecayList;
  std::vector<G4VPhysicsConstructor*> hadronPhys;

  G4ProcessManager* pManager;//IH
  G4ParticleDefinition* particleDefinition;//IH
  G4String stringParticleName;//IH
};


#endif
