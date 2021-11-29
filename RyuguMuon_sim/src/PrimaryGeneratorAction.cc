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
#include "Parameters.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "RunAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Geantino.hh"
#include "G4IonTable.hh"

G4int PrimaryGeneratorAction::fractionOfEletronParticles = 10;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fParticleGunEle(0) 
{

   // ------------------------------------------------------------------------
   // get parameter
   // ------------------------------------------------------------------------
   //BeamType="muon";
   BeamType="gamma";
   char tmpString0[100]="Unset", tmpString1[100]="Unset";
   if (strcmp(Parameters::mySteeringFileName,"Unset")!=0){
      char charSteeringFileName[1000]; strcpy(charSteeringFileName,(Parameters::mySteeringFileName).c_str());
      FILE *fSteeringFile=fopen(charSteeringFileName,"r");
      char  line[501];
      while (!feof(fSteeringFile)) {
         fgets(line,500,fSteeringFile);
         if ((line[0]=='#')||(line[0]=='\n')||(line[0]=='\r')) continue;
         sscanf(&line[0],"%s %s",tmpString0,tmpString1);//command, sample name
         if (strcmp(tmpString0,"/command/beamtype")==0){ BeamType = tmpString1;
         }
      }
   }

  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  fParticleGunEle  = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  if(BeamType == "muon"){
     G4cout << Form("Beam type is %s",BeamType.c_str()) << G4endl;
     G4ParticleDefinition* particle=particleTable->FindParticle("mu-");
     fParticleGun->SetParticleDefinition(particle);
     muon_mass = fParticleGun->GetParticleDefinition()->GetPDGMass();

     G4ParticleDefinition* eletron=particleTable->FindParticle("e-");
     fParticleGunEle->SetParticleDefinition(eletron);
     ele_mass = fParticleGunEle->GetParticleDefinition()->GetPDGMass();
  }else if (BeamType == "gamma"){
     G4cout << Form("Beam type is %s",BeamType.c_str()) << G4endl;
     G4ParticleDefinition* particle = particleTable->FindParticle("gamma");
     fParticleGun->SetParticleDefinition(particle);
     gamma_mass = fParticleGun->GetParticleDefinition()->GetPDGMass();
  }else if (BeamType == "ri"){
     G4cout << Form("Beam type is %s",BeamType.c_str()) << G4endl;
  }else{
     BeamType = "ri";
     G4cout << Form("Set Beam type to %s",BeamType.c_str()) << G4endl;
  }

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
  RootOutput* myRootOutput = RootOutput::GetRootInstance();
  myRootOutput->ClearAllRootVariables(); 

  if(BeamType == "muon"){
  // ***** muon beam ***** 
     // === particle incident position ===
     //gauss for x and y
     x0 = G4RandGauss::shoot(poi_mean,poi_sigmaX)*CLHEP::mm;
     y0 = G4RandGauss::shoot(poi_mean,poi_sigmaY)*CLHEP::mm;
     z0 = -5*CLHEP::mm;
     //temp: only for this case (cut for beam)
     //if (std::fabs(x0)>60) x0 = SetCutforBeam(x0,poi_sigmaX);
     //if (std::fabs(y0)>60) y0 = SetCutforBeam(y0,poi_sigmaY);
     fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
   
     // == default particle momentum ==
     pSigma = p0*mom_error;
     p = G4RandGauss::shoot(p0,pSigma)*MeV;
     G4double ux = p*dir_error_x*2*(G4UniformRand()-0.5)*MeV,
              uy = p*dir_error_y*2*(G4UniformRand()-0.5)*MeV,
              uz = std::sqrt(p*p - ux*ux - uy*uy)*MeV;
     fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));//Momentum
     G4double particleEnergy = std::sqrt(p*p+muon_mass*muon_mass)-muon_mass;
     fParticleGun->SetParticleEnergy(particleEnergy);//IH 
     //this command will show previous particle energy and current particle momentum "EACH EVENT" !!
     //fParticleGun->SetParticleMomentum(p*MeV);//IH 
     fParticleGun->GeneratePrimaryVertex(anEvent);// === particle gen. ===
     G4double muInitTime = fParticleGun->GetParticleTime();
   
   // ***** electron *****
     //TODO try not to used random, very slow
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
       fParticleGunEle->GeneratePrimaryVertex(anEvent);// === particle gen. ===
     }
   
     myRootOutput->SetInitialMuonParameters(x0,y0,z0,ux,uy,uz,muInitTime);
     myRootOutput->SetInitialEletronParameters(x0_e,y0_e,z0,ux_e,uy_e,uz_e);

  }else if (BeamType == "gamma"){
  // ***** gamma *****
     count_event++;
     x0_ga = 0;
     y0_ga = 0;
     z0_ga = (275.113-3.28*0.5+7.90051e-01)*CLHEP::mm;
     //NOTE: 
     // 275.113 mm is center of sample; 
     // 3.28 mm is thickness of ryugy sample; 
     // depth of stopped muon in Ryugu sample is  7.90051e-01 mm;
     fParticleGun->SetParticlePosition(G4ThreeVector(x0_ga,y0_ga,z0_ga));
     //seed = int(G4UniformRand()*20);
     //fParticleGun->SetParticleEnergy(0.01+0.01*seed);//10~200 keV gamma
     gen_e_gamma = count_event%6800;
     fParticleGun->SetParticleEnergy(0.01+0.001*gen_e_gamma*0.025);//random 10~180 [keV] gamma
     //ux_ga = 2*(G4UniformRand()-0.5)*MeV;
     //uy_ga = 2*(G4UniformRand()-0.5)*MeV;
     //uz_ga = 2*(G4UniformRand()-0.5)*MeV;//random direction to xyz
     //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux_ga,uy_ga,uz_ga));//Momentum
     //fParticleGun->GeneratePrimaryVertex(anEvent);// === particle gen. ===

     //for(int iphi = -5; iphi < 5+1; iphi++){
     //   for(int itheta = 0; itheta < 360; itheta++){
     //      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(itheta),std::cos(itheta),std::sin(iphi/10.)));
     //   }//xy plane
     // }//z axis
     int itheta = int(G4UniformRand()*6); //random 0~5
     fParticleGun->SetParticleMomentumDirection(G4ThreeVector(std::sin(2*CLHEP::pi*(itheta*60+30)/360.*CLHEP::rad),std::cos(2*CLHEP::pi*(itheta*60+30)/360.*CLHEP::rad),0));
     fParticleGun->GeneratePrimaryVertex(anEvent);// === particle gen. ===

  }else{
  // ***** RI source *****
     if (fParticleGun->GetParticleDefinition() == G4Geantino::Geantino()) {
        //if not UI : Name and type is "geantino"
        //create vertex
        G4int Z = 27, A = 57;//init. source : Co57
        G4double excitEnergy = 0.*keV;
        G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
        fParticleGun->SetParticleDefinition(ion);
     }
     fParticleGun->SetParticlePosition(G4ThreeVector(0,0,275.15*CLHEP::mm));
     fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
     fParticleGun->SetParticleCharge(0.*eplus);
     fParticleGun->SetParticleEnergy(0*eV);
     fParticleGun->GeneratePrimaryVertex(anEvent);
  }//beam type
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

