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

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "RunAction.hh"

G4int PrimaryGeneratorAction::fractionOfEletronParticles = 10;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fParticleGunEle(0), 
  fEnvelopeBox(0)
//  t0(0), tSigma(0), 
//  x0(0), y0(0), z0(-10*CLHEP::cm)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  fParticleGunEle  = new G4ParticleGun(n_particle);

  //define input particles
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="mu-");//IH
  fParticleGun->SetParticleDefinition(particle);
  muon_mass = fParticleGun->GetParticleDefinition()->GetPDGMass();

  G4String eletronName;
  G4ParticleDefinition* eletron
    = particleTable->FindParticle(eletronName="e-");//IH
  fParticleGunEle->SetParticleDefinition(eletron);
  ele_mass = fParticleGunEle->GetParticleDefinition()->GetPDGMass();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fParticleGunEle;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  RootOutput* myRootOutput = RootOutput::GetRootInstance();
  myRootOutput->ClearAllRootVariables(); 

  G4double envSizeXY = 0;
  G4double envSizeZ = 0;

//  if (!fEnvelopeBox)
//  {
//    G4LogicalVolume* envLV
//      = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
//    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
//  }
//
//  if ( fEnvelopeBox ) {
//    envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
//    envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
//  }  
//  else  {
//    G4ExceptionDescription msg;
//    msg << "Envelope volume of box shape not found.\n"; 
//    msg << "Perhaps you have changed geometry.\n";
//    msg << "The gun will be place at the center.";
//    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
//     "MyCode0002",JustWarning,msg);
//  }

  // default particle kinematic
  pSigma = p0*mom_error;
  p = G4RandGauss::shoot(p0,pSigma)*MeV;

//  G4double sigma_angle = 0.01*2*CLHEP::pi*G4UniformRand()*CLHEP::rad;
  G4double ux = p*dir_error*2*(G4UniformRand()-0.5)*MeV,
           uy = p*dir_error*2*(G4UniformRand()-0.5)*MeV,
           uz = std::sqrt(p*p - ux*ux - uy*uy)*MeV;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));//Momentum
  G4double particleEnergy = std::sqrt(p*p+muon_mass*muon_mass)-muon_mass;
  fParticleGun->SetParticleEnergy(particleEnergy);//IH 
  //this command will show previous particle energy and current particle momentum "EACH EVENT" !!
  //fParticleGun->SetParticleMomentum(p*MeV);//IH 

//  sigma_angle = 0.01*2*CLHEP::pi*G4UniformRand()*CLHEP::rad;
  G4double ux_e = p*dir_error*2*(G4UniformRand()-0.5)*MeV,
           uy_e = p*dir_error*2*(G4UniformRand()-0.5)*MeV,
           uz_e = std::sqrt(p*p - ux_e*ux_e - uy_e*uy_e)*MeV;
  fParticleGunEle->SetParticleMomentumDirection(G4ThreeVector(ux_e,uy_e,uz_e));
  particleEnergy = std::sqrt(p*p+ele_mass*ele_mass)-ele_mass;
  fParticleGunEle->SetParticleEnergy(particleEnergy);//IH 
  //fParticleGunEle->SetParticleMomentum(p*MeV);

  //particle incident position
  G4double radius = 2.5*CLHEP::cm;
  G4double rho = radius*std::sqrt(G4UniformRand());
  G4double theta = 2*CLHEP::pi*G4UniformRand()*CLHEP::rad;
  G4double y0 = rho * std::sin(theta);
  G4double x0 = rho * std::cos(theta);
  G4double z0 = -5*CLHEP::mm;
  
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
  G4double muInitTime = fParticleGun->GetParticleTime();

  long thisEventNr = (long) (anEvent->GetEventID());
  if ((thisEventNr != 0) && (thisEventNr%fractionOfEletronParticles == 0)) {  
    rho_e = radius*std::sqrt(G4UniformRand());
    theta_e = 2*CLHEP::pi*G4UniformRand()*CLHEP::rad;
    y0_e = rho_e * std::sin(theta_e);
    x0_e = rho_e * std::cos(theta_e);
    fParticleGunEle->SetParticlePosition(G4ThreeVector(x0_e,y0_e,z0));
    fParticleGunEle->GeneratePrimaryVertex(anEvent);
  }
  
  // Assign spin 
//  G4double ParticleTime;
//  if (tSigma>0)      {ParticleTime = G4RandGauss::shoot(t0,tSigma);}         //  Gaussian distribution       P.B. 13 May 2009
//  else if (tSigma<0) {ParticleTime = t0 + tSigma*(G4UniformRand()*2.-1.);}   //  Uniform step distribution   P.B. 13 May 2009
//  else               {ParticleTime = t0;}           
//  clearRootOutput->SetInitialMuonParameters(x0,y0,z0,px,py,pz,xpolaris,ypolaris,zpolaris,ParticleTime);
  myRootOutput->SetInitialMuonParameters(x0,y0,z0,ux,uy,uz,muInitTime);
  myRootOutput->SetInitialEletronParameters(x0_e,y0_e,z0,ux_e,uy_e,uz_e);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

