#include "PhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleWithCuts.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4DecayPhysics.hh"
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

//IH
#include "G4MuonicAtomDecayPhysics.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4StoppingPhysics.hh"
//Muonic Atom (check decay and stopping for G4MuonicAtomDecayPhysics and G4MuonMinusAtomicCapture)
//https://gitlab.physik.uni-kiel.de/geant4/geant4/-/tree/6aa23be5171b125c3363b5a4cfa00a57e524598b/source/physics_lists/constructors
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"
#include "G4OpticalPhysics.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4CoulombScattering.hh"
//#include "HadronPhysicsLHEP.hh"
//https://www.slac.stanford.edu/comp/physics/geant4/slac_physics_lists/ilc/LHEPlistdoc.html

PhysicsList::PhysicsList()
 : G4VModularPhysicsList()
{
  //SetVerboseLevel(1);
  SetVerboseLevel(0);

  // EM
//  emPhysicsList = new G4EmStandardPhysics();
//  emPhysicsList = new G4EmStandardPhysics_option1();
//  emPhysicsList = new G4EmStandardPhysics_option2();
//  emPhysicsList = new G4EmStandardPhysics_option3();
//  emPhysicsList = new G4EmStandardPhysics_option4();
  emPhysicsList = new G4EmLivermorePhysics();
  // Decay
  decPhysicsList = new G4DecayPhysics("decays");
  // Radioactive
  //raddecayList = new G4RadioactiveDecayPhysics();//take huge time
  // Muonic Atom decay   
   decMuonicPhysicsList = new G4MuonicAtomDecayPhysics();//IH
}

PhysicsList::~PhysicsList()
{
  delete emPhysicsList;
  delete decPhysicsList;
  delete decMuonicPhysicsList;
  delete raddecayList;
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
  decMuonicPhysicsList -> ConstructParticle();//IH
  emPhysicsList  -> ConstructParticle();
}

//IH
void PhysicsList::ConstructAdditionalProcess()
{
  stringParticleName = "gamma";
  particleDefinition = G4ParticleTable::GetParticleTable() -> FindParticle(stringParticleName);
  pManager = particleDefinition->GetProcessManager();
  pManager->AddDiscreteProcess(new G4PhotoElectricEffect);
  pManager->AddDiscreteProcess(new G4ComptonScattering);
  pManager->AddDiscreteProcess(new G4GammaConversion);
  pManager->AddDiscreteProcess(new G4RayleighScattering);
  G4cout<<"PhysicsList: Defining addional processes for "<<stringParticleName<<G4endl;

//Multiple scattering, ionization and bremsstrahlung processes should always be used together,
  stringParticleName = "e-";
  particleDefinition = G4ParticleTable::GetParticleTable() -> FindParticle(stringParticleName);
  pManager = particleDefinition->GetProcessManager();
  pManager->AddDiscreteProcess(new G4eMultipleScattering);
//  pManager->AddDiscreteProcess(new G4eIonisation);//take huge time
  pManager->AddDiscreteProcess(new G4eBremsstrahlung);
  G4cout<<"PhysicsList: Defining addional processes for "<<stringParticleName<<G4endl;

  stringParticleName = "e+";
  particleDefinition = G4ParticleTable::GetParticleTable() -> FindParticle(stringParticleName);
  pManager = particleDefinition->GetProcessManager();
  pManager->AddDiscreteProcess(new G4eMultipleScattering);
//  pManager->AddDiscreteProcess(new G4eIonisation);//take huge time
  pManager->AddDiscreteProcess(new G4eBremsstrahlung);
  pManager->AddDiscreteProcess(new G4eplusAnnihilation);
  G4cout<<"PhysicsList: Defining addional processes for "<<stringParticleName<<G4endl;

  stringParticleName = "mu-";
  particleDefinition = G4ParticleTable::GetParticleTable() -> FindParticle(stringParticleName);
  pManager = particleDefinition->GetProcessManager();
  pManager->AddDiscreteProcess(new G4MuMultipleScattering());
//  pManager->AddDiscreteProcess(new G4MuIonisation());
  pManager->AddDiscreteProcess(new G4MuBremsstrahlung());
  pManager->AddDiscreteProcess(new G4MuPairProduction());

}

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  emPhysicsList->ConstructProcess();
  decPhysicsList->ConstructProcess();
  decMuonicPhysicsList->ConstructProcess();//IH
//  raddecayList->ConstructProcess();

  ConstructAdditionalProcess();//IH

  // Hadron
  hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC());//was used to simulate the interaction of neutrons with matter
  hadronPhys.push_back( new G4EmExtraPhysics());
  hadronPhys.push_back( new G4HadronElasticPhysics());
  hadronPhys.push_back( new G4StoppingPhysics());
  hadronPhys.push_back( new G4IonBinaryCascadePhysics());
  hadronPhys.push_back( new G4NeutronTrackingCut());
  for (size_t i=0; i< hadronPhys.size(); i++) {
    hadronPhys[i] -> ConstructProcess();
  }
}
