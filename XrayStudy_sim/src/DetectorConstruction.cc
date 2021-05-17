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

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
   G4double A, Z;
   // ------------------------------------------------------------------------
   // Elements
   // ------------------------------------------------------------------------
   G4Element* elH  = new G4Element("Hydrogen","H",  Z=1.,  A=1.00794*g/mole);
   G4Element* elC  = new G4Element("Carbon",  "C",  Z=6.,  A= 12.011 *g/mole);
   G4Element* elN  = new G4Element("Nitrogen","N",  Z=7.,  A= 14.00674*g/mole);
   G4Element* elO  = new G4Element("Oxygen",  "O",  Z=8.,  A= 15.9994*g/mole);
   G4Element* elNa = new G4Element("Sodium",  "Na", Z=11., A= 22.989768*g/mole);
   G4Element* elSi = new G4Element("Silicon", "Si", Z=14., A= 28.0855*g/mole);
   G4Element* elAr = new G4Element("Argon",   "Ar", Z=18., A= 39.948*g/mole);
   G4Element* elI  = new G4Element("Iodine",  "I",  Z=53., A= 126.90447*g/mole);
   G4Element* elCs = new G4Element("Cesium",  "Cs", Z=55., A= 132.90543*g/mole);

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* elA = nist->FindOrBuildMaterial("G4_AIR"); 
  G4Material* HeA = nist->FindOrBuildMaterial("G4_He");

  G4double atomicNumber = 1.;
  G4double massOfMole = 1.008*g/mole;
  G4double density = 1.e-25*g/cm3;
  G4double temperature = 2.73*kelvin;
  G4double pressure = 3.e-18*pascal;
  G4Material* Vacuum = new G4Material( "Vacuum", atomicNumber, massOfMole, density, kStateGas, temperature, pressure); 
  
  // Envelope parameters
  //
  G4double env_sizeXY = 80*cm, env_sizeZ = 100*cm;//World volume
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  // ***** World *****
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;  
  G4Box* solidWorld = new G4Box("World",0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);      
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,Vacuum,"World");                                       
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);                            
  // ***** Envelope *****
  G4Box* solidEnv = new G4Box("Envelope", 0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ);      
  G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv,elA, "Envelope");    

  // /* 2021/3 D2 experiment
  G4Material* solid_common;
  // ***** Pb Target *****
//  solid_common=nist->FindOrBuildMaterial("G4_C");
  solid_common=nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4double Pb_dis=5;//mm
//  G4VSolid* Pb_Target = new G4Box("PbTarget",(7./2)*mm, (4./2)*mm, (1./2)*mm);
  G4VSolid* Pb_Target = new G4Box("PbTarget",(50./2)*mm, (50./2)*mm, (10./2)*mm);
  G4ThreeVector pos_pb = G4ThreeVector(0, 0, 0*mm);
  G4LogicalVolume* PbLog = new G4LogicalVolume(Pb_Target, solid_common, "PbTarget");
  new G4PVPlacement(0, pos_pb, PbLog, "PbTarget", logicWorld, false, 0, checkOverlaps);
  // ***** Ge detector *****
  solid_common=nist->FindOrBuildMaterial("G4_Ge");
  G4VSolid* Ge_Det = new G4Tubs("GeDet",0*mm,(50./2)*mm,(50./2.)*mm,0.,2*M_PI*rad);
  G4double ge_dis=100.+50./2.;//mm
  G4double ge_angle=2*CLHEP::pi*(45./360)*CLHEP::rad;
  G4double nDets=6;
  G4double current_angle;
  G4ThreeVector pos_ge;
  G4LogicalVolume* GeLog;
  G4RotationMatrix* rot_ge;
  for(int i=0; i<nDets;i++){
     auto idstr = std::to_string(i);
     current_angle=i*(2*CLHEP::pi/nDets)*CLHEP::rad;
     pos_ge = G4ThreeVector(ge_dis*std::sin(ge_angle)*std::sin(current_angle)*mm, ge_dis*std::sin(ge_angle)*std::cos(current_angle)*mm, (ge_dis*std::cos(ge_angle))*mm);
     GeLog = new G4LogicalVolume(Ge_Det, solid_common, "GeTubs"+idstr);
     rot_ge = new G4RotationMatrix(-i*(360./nDets)*CLHEP::deg,-45*CLHEP::deg,0*CLHEP::deg);
     new G4PVPlacement(rot_ge, pos_ge, GeLog, "GeTubs"+idstr, logicWorld, false, 0, checkOverlaps);
  }
  ge_angle=(-1)*2*CLHEP::pi*(45./360)*CLHEP::rad;
  for(int i=0; i<nDets;i++){
     auto idstr = std::to_string(i);
     current_angle=i*(2*CLHEP::pi/nDets)*CLHEP::rad;
     pos_ge = G4ThreeVector(ge_dis*std::sin(ge_angle)*std::sin(current_angle)*mm, ge_dis*std::sin(ge_angle)*std::cos(current_angle)*mm, (-1*ge_dis*std::cos(ge_angle))*mm);
     GeLog = new G4LogicalVolume(Ge_Det, solid_common, "GeTubs2"+idstr);
     rot_ge = new G4RotationMatrix(-i*(360./nDets)*CLHEP::deg,-45*CLHEP::deg,0*CLHEP::deg);
     new G4PVPlacement(rot_ge, pos_ge, GeLog, "GeTubs2"+idstr, logicWorld, false, 0, checkOverlaps);
  }

  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
