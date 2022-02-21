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
  G4ParticleDefinition* particle;

  // muon beam
//  particle  = particleTable->FindParticle(particleName="mu-");
//  fParticleGun->SetParticleDefinition(particle);
//  muon_mass = fParticleGun->GetParticleDefinition()->GetPDGMass();

  // gamma source
  particle = particleTable->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particle);

  G4String eletronName;
  G4ParticleDefinition* eletron
    = particleTable->FindParticle(eletronName="e-");//IH
  fParticleGunEle->SetParticleDefinition(eletron);
  ele_mass = fParticleGunEle->GetParticleDefinition()->GetPDGMass();

  count_event=0;
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

   /* muon pulse beam
  for (int i=0;i< nPulseBeam; i++){
  // === default particle kinematic ===
  pSigma = p0*mom_error;
  p = G4RandGauss::shoot(p0,pSigma)*MeV;

  // === particle init. position ===
  // ** gauss **
  x0 = G4RandGauss::shoot(poi_mean,poi_sigmaX)*CLHEP::mm;
  y0 = G4RandGauss::shoot(poi_mean,poi_sigmaY)*CLHEP::mm;
  z0 = -50*CLHEP::mm;
  //temp: only for this case (cut for beam)
  //if (std::fabs(x0)>60) x0 = SetCutforBeam(x0,poi_sigmaX);
  //if (std::fabs(y0)>60) y0 = SetCutforBeam(y0,poi_sigmaY);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  // === particle init. momentum and energy ===
  G4double ux = p*dir_error_x*2*(G4UniformRand()-0.5)*MeV,
           uy = p*dir_error_y*2*(G4UniformRand()-0.5)*MeV,
           uz = std::sqrt(p*p - ux*ux - uy*uy)*MeV;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));//Momentum
  G4double particleEnergy = std::sqrt(p*p+muon_mass*muon_mass)-muon_mass;
  fParticleGun->SetParticleEnergy(particleEnergy);//IH
  G4double muInitTime = fParticleGun->GetParticleTime()/CLHEP::nanosecond;
  fParticleGun->GeneratePrimaryVertex(anEvent);

  // === electron ===
  G4double ux_e = p*(dir_error_x/10)*2*(G4UniformRand()-0.5)*MeV,
           uy_e = p*(dir_error_y/10)*2*(G4UniformRand()-0.5)*MeV,
           uz_e = std::sqrt(p*p - ux_e*ux_e - uy_e*uy_e)*MeV;
  fParticleGunEle->SetParticleMomentumDirection(G4ThreeVector(ux_e,uy_e,uz_e));
  particleEnergy = std::sqrt(p*p+ele_mass*ele_mass)-ele_mass;
  fParticleGunEle->SetParticleEnergy(particleEnergy);//IH 
  long thisEventNr = (long) (anEvent->GetEventID());
  if ((thisEventNr != 0) && (thisEventNr%fractionOfEletronParticles == 0)) { 
    // ** gauss **
    x0_e = G4RandGauss::shoot(poi_mean,poi_sigmaX)*CLHEP::mm;
    y0_e = G4RandGauss::shoot(poi_mean,poi_sigmaY)*CLHEP::mm;
    //temp: only for this case (cut for beam)
    //if (std::fabs(x0_e)>60) x0_e = SetCutforBeam(x0_e,poi_sigmaX);
    //if (std::fabs(y0_e)>60) y0_e = SetCutforBeam(y0_e,poi_sigmaY);
    fParticleGunEle->SetParticlePosition(G4ThreeVector(x0_e,y0_e,z0));
//    fParticleGunEle->GeneratePrimaryVertex(anEvent);
  }
  myRootOutput->SetInitialMuonParameters(x0,y0,z0,ux,uy,uz,muInitTime);
  myRootOutput->SetInitialEletronParameters(x0_e,y0_e,z0,ux_e,uy_e,uz_e);
  }//Pulse Beam
  // */

//   /* gamma generation
//  G4double poi_z = -0.5+G4RandGauss::shoot(0.422585,0.0723287)*CLHEP::mm; G4double length_x = 10;G4double length_y = 10;//white
//  G4double poi_z = -0.5+G4RandGauss::shoot(0.433646,0.0777046)*CLHEP::mm; G4double length_x = 20;G4double length_y = 10;//black
//  G4double poi_z = -1.75+G4RandGauss::shoot(0.426187,0.0715852)*CLHEP::mm;G4double length_x = 30;G4double length_y = 10;//dew
//  G4double poi_z = -1.75+G4RandGauss::shoot(1.05532,0.176408)*CLHEP::mm; G4double length_x = 30;G4double length_y = 10;//dew 35
  G4double poi_z = -0.5*3.28+G4RandGauss::shoot(0.7567,0.1337)*CLHEP::mm; G4double length_x = 8.65; G4double length_y = 5.12;//Ryugu
  G4double poi_x = (G4UniformRand()-0.5)*2*(length_x/2.)*CLHEP::mm;
  G4double poi_y = (G4UniformRand()-0.5)*2*(length_y/2.)*CLHEP::mm;//(G4UniformRand()-0.5)*2 is -1 ~ 1 | length_x,length_y is size 
  G4double rot_rad=(20./360)*2*CLHEP::pi*CLHEP::rad;
  G4double poi_x_prime=poi_x*std::cos(rot_rad)-poi_z*std::sin(rot_rad);
  G4double poi_z_prime=poi_x*std::sin(rot_rad)+poi_z*std::cos(rot_rad);
//  G4double Ge_x[6] = {17.6777, 35.3553, 17.6777, -17.6777, -35.3553, -17.6777};
//  G4double Ge_y[6] = {30.6186, 0, -30.6186, -30.6186, 0, 30.6186};
  G4double Ge_x[6] = {58.407, 116.814, 58.407, -58.407, -116.814, -58.407};
  G4double Ge_y[6] = {101.164, 0, -101.164, -101.164, 0, 101.164};
  
  fParticleGun->SetParticlePosition(G4ThreeVector(poi_x_prime,poi_y,poi_z_prime));
  count_event++;
  gen_e_gamma = count_event%6800;
  int idet = int(G4UniformRand()*6);//0~5
  myRootOutput->SetInitEnergy(idet+1,0.01+0.001*gen_e_gamma*0.025);
  fParticleGun->SetParticleEnergy(0.01+0.001*gen_e_gamma*0.025);//random 10~180 [keV] gamma
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(Ge_x[idet]-poi_x_prime,Ge_y[idet]-poi_y,0));
  fParticleGun->GeneratePrimaryVertex(anEvent);
  // */

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

