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

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "RootOutput.hh"
#include "Randomize.hh"

class G4ParticleGun;
class G4Event;
class G4Box;
//class RunAction;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    virtual ~PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
    const G4ParticleGun* GetParticleGunEle() const { return fParticleGunEle; }

    void SetMuonTime(G4double val)          {t0=val;}     //P.B. 13 May 2009
    void SetMuonTimeSigma(G4double val)     {tSigma=val;} //P.B. 13 May 2009
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4ParticleGun*  fParticleGunEle; // pointer a to G4 gun class
    G4Box* fEnvelopeBox;
    G4double t0, tSigma;
    G4double p, pSigma;

    //takeshita setting
    //G4double p0 = 35;//MeV/c
    //G4double mom_error = 0.05;//5%
    //G4double poi_mean = 0;//mm
    //G4double poi_sigmaX = 0.5;//mm
    //G4double poi_sigmaY = 7;//mm
    //G4double dir_error_x = 0.025;//momentum direction error : 0.025*2pi
    //G4double dir_error_y = 0.30;//momentum direction error : 0.30*2pi

    //small beam size
    G4double p0 = 35;//MeV/c
    G4double mom_error = 0.05;//5%
    G4double poi_mean = 0;//mm
    G4double poi_sigmaX = 0.01;//mm
    G4double poi_sigmaY = 0.01;//mm
    G4double dir_error_x = 0.001;//momentum direction error : 0.025*2pi
    G4double dir_error_y = 0.001;//momentum direction error : 0.30*2pi

    G4double rho_e;   
    G4double theta_e; 
    G4double y0_e;    
    G4double x0_e;   
    G4double muon_mass, ele_mass;

    G4double x0;
    G4double y0;
    G4double z0;
    G4int nPulseBeam = 1;
    G4int count_event;
    G4double gen_e_gamma;
    static G4int  fractionOfEletronParticles;//20% eletron

    int SetCutforBeam(G4double poi,G4double sigma){
       poi=G4RandGauss::shoot(poi_mean,sigma)*CLHEP::mm;
       if (std::fabs(poi) > 60){
        return SetCutforBeam(poi,sigma);
       }else{
        return poi;
       }
    }

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
