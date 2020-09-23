#include "PhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleWithCuts.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4DecayPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmProcessOptions.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsFTF_BIC.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"
#include "G4HadronPhysicsQGS_BIC.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4ProcessManager.hh"
#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4ParallelWorldPhysics.hh"

PhysicsList::PhysicsList()
 : G4VModularPhysicsList()
{
  //SetVerboseLevel(1);
  SetVerboseLevel(0);

  // EM
  emPhysicsList = new G4EmStandardPhysics_option4();
  // Decay
  decPhysicsList = new G4DecayPhysics("decays");
  // Radioactive
  //raddecayList = new G4RadioactiveDecayPhysics();
}

PhysicsList::~PhysicsList()
{
  delete emPhysicsList;
  delete decPhysicsList;
  //delete raddecayList;
  for(size_t i=0; i<hadronPhys.size(); i++) {
    delete hadronPhys[i];
  }
}

void PhysicsList::SetVerboseLevel(G4int i)
{
  verboseLevel=i;
}

void PhysicsList::ConstructParticle()
{
  decPhysicsList -> ConstructParticle();
  emPhysicsList  -> ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  emPhysicsList->ConstructProcess();
  decPhysicsList->ConstructProcess();
  //raddecayList->ConstructProcess();

  // Hadron
  hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC());
  hadronPhys.push_back( new G4EmExtraPhysics());
  hadronPhys.push_back( new G4HadronElasticPhysics());
  hadronPhys.push_back( new G4StoppingPhysics());
  hadronPhys.push_back( new G4IonBinaryCascadePhysics());
  hadronPhys.push_back( new G4NeutronTrackingCut());
  for (size_t i=0; i< hadronPhys.size(); i++) {
    hadronPhys[i] -> ConstructProcess();
  }
}
