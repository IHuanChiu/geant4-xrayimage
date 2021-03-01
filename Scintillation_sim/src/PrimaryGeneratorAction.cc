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

  //beam shape
//  G4double radius = 10.9*CLHEP::mm;
//  G4double rho = radius*std::sqrt(G4UniformRand());//random
//  G4double theta = 2*CLHEP::pi*G4UniformRand()*CLHEP::rad;
//  //beam incident position
//  y0 = rho * std::sin(theta);
//  x0 = rho * std::cos(theta);
//  z0 = -200*CLHEP::mm;
  //focus point (position of sample)
//  G4double sample_z0 = 0*CLHEP::mm;
//  G4double sample_x0 = 0*CLHEP::mm;
//  G4double sample_y0 = 0*CLHEP::mm;
//  G4double vector_x = (sample_x0 - x0)*(1+dir_error*2*(G4UniformRand()-0.5));
//  G4double vector_y = (sample_y0 - y0)*(1+dir_error*2*(G4UniformRand()-0.5));
//  G4double vector_z = (sample_z0 - z0)*(1+dir_error*2*(G4UniformRand()-0.5));

  //gauss for x and y
  x0 = G4RandGauss::shoot(poi_mean,poi_sigmaX)*CLHEP::mm;
  y0 = G4RandGauss::shoot(poi_mean,poi_sigmaY)*CLHEP::mm;
//  if (std::fabs(x0)>beamrange) x0 = SetCutforBeam(x0,poi_sigmaX);
//  if (std::fabs(y0)>beamrange) y0 = SetCutforBeam(y0,poi_sigmaY);
  z0 = -200*CLHEP::mm;
  // default particle kinematic
  pSigma = p0*mom_error;
  p = G4RandGauss::shoot(p0,pSigma)*MeV;

  //G4double ux = (vector_x/std::sqrt(vector_x*vector_x + vector_y*vector_y + vector_z*vector_z)),
  //         uy = (vector_y/std::sqrt(vector_x*vector_x + vector_y*vector_y + vector_z*vector_z)),
  //         uz = (vector_z/std::sqrt(vector_x*vector_x + vector_y*vector_y + vector_z*vector_z));
  G4double ux = p*dir_error*2*(G4UniformRand()-0.5)*MeV,
           uy = p*dir_error*2*(G4UniformRand()-0.5)*MeV,
           uz = std::sqrt(p*p - ux*ux - uy*uy)*MeV;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));//Momentum
  G4double particleEnergy = std::sqrt(p*p+muon_mass*muon_mass)-muon_mass;
  fParticleGun->SetParticleEnergy(particleEnergy);//IH

  G4double ux_e = p*dir_error*2*(G4UniformRand()-0.5)*MeV,
           uy_e = p*dir_error*2*(G4UniformRand()-0.5)*MeV,
           uz_e = std::sqrt(p*p - ux_e*ux_e - uy_e*uy_e)*MeV;
  fParticleGunEle->SetParticleMomentumDirection(G4ThreeVector(ux_e,uy_e,uz_e));
  particleEnergy = std::sqrt(p*p+ele_mass*ele_mass)-ele_mass;
  fParticleGunEle->SetParticleEnergy(particleEnergy);//IH 
  
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  for (int i=0;i< nPulseBeam; i++) fParticleGun->GeneratePrimaryVertex(anEvent);
  G4double muInitTime = fParticleGun->GetParticleTime()/CLHEP::nanosecond;

  long thisEventNr = (long) (anEvent->GetEventID());
  if ((thisEventNr != 0) && (thisEventNr%fractionOfEletronParticles == 0)) {  
    //random for x and y
//    rho_e = radius*std::sqrt(G4UniformRand());//random
//    theta_e = 2*CLHEP::pi*G4UniformRand()*CLHEP::rad;
//    y0_e = rho_e * std::sin(theta_e);
//    x0_e = rho_e * std::cos(theta_e);
    //gauss for x and y
    x0_e = G4RandGauss::shoot(poi_mean,poi_sigmaX)*CLHEP::mm;
    y0_e = G4RandGauss::shoot(poi_mean,poi_sigmaY)*CLHEP::mm;
//    if (std::fabs(x0_e)>beamrange) x0_e = SetCutforBeam(x0_e,poi_sigmaX);
//    if (std::fabs(y0_e)>beamrange) y0_e = SetCutforBeam(y0_e,poi_sigmaY);
    fParticleGunEle->SetParticlePosition(G4ThreeVector(x0_e,y0_e,z0));
    for (int i=0;i< nPulseBeam; i++) fParticleGunEle->GeneratePrimaryVertex(anEvent);
  }
  
  myRootOutput->SetInitialMuonParameters(x0,y0,z0,ux,uy,uz,muInitTime);
  myRootOutput->SetInitialEletronParameters(x0_e,y0_e,z0,ux_e,uy_e,uz_e);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

