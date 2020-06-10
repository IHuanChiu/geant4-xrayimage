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
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* elA = nist->FindOrBuildMaterial("G4_AIR"); 
  G4Material* Vacuum= new G4Material( "Vacuum", CLHEP::universe_mean_density, 2 );//no possible for real vacuum
  G4Material* HeA = nist->FindOrBuildMaterial("G4_He"); 
  
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
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,elA,"World");                                       
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);       
                     
  // ***** Envelope *****
  G4Box* solidEnv = new G4Box("Envelope", 0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ);      
  G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv,elA, "Envelope");    
               
  // ***** Kapton *****
  G4Material* solid_kapton = nist->FindOrBuildMaterial("G4_KAPTON");
  G4VSolid* kapton_tubs = new G4Tubs("KaptonTubs",0*mm,kapton_radius*mm,(kapton_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_kapton_1 = G4ThreeVector(0, 0, 296*mm);//check beam position
  G4ThreeVector pos_kapton_2 = G4ThreeVector(0, 0, (296+27.5)*mm);//check beam position
  G4LogicalVolume* KaptonLog = new G4LogicalVolume(kapton_tubs, solid_kapton, "KaptonTubs");  
  new G4PVPlacement(0, pos_kapton_1, KaptonLog, "KaptonTubs", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(0, pos_kapton_2, KaptonLog, "KaptonTubs", logicWorld, false, 0, checkOverlaps);        

  // ***** Gas *****
  G4VSolid* gas_tubs_1 = new G4Tubs("VacuumGas",0*mm,gas_radius_1*mm,(gas_thick_1/2)*mm,0.,2*M_PI*rad);
  G4VSolid* gas_tubs_2 = new G4Tubs("AirGas",0*mm,gas_radius_2*mm,(gas_thick_2/2)*mm,0.,2*M_PI*rad);
  G4VSolid* gas_tubs_3 = new G4Tubs("HeGas",0*mm,gas_radius_3*mm,(gas_thick_3/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_gas_1 = G4ThreeVector(0, 0, (gas_thick_1/2)*mm);//check beam position
  G4ThreeVector pos_gas_2 = G4ThreeVector(0, 0, (gas_thick_2/2+gas_thick_1)*mm);//check beam position
  G4ThreeVector pos_gas_3 = G4ThreeVector(0, 0, (gas_thick_3/2+gas_thick_2+gas_thick_1)*mm);//check beam position
  G4LogicalVolume* GasLog_1 = new G4LogicalVolume(gas_tubs_1, Vacuum, "GasTubs_1");
  G4LogicalVolume* GasLog_2 = new G4LogicalVolume(gas_tubs_2, elA, "GasTubs_2");
  G4LogicalVolume* GasLog_3 = new G4LogicalVolume(gas_tubs_3, HeA, "GasTubs_3");
  new G4PVPlacement(0, pos_gas_1, GasLog_1, "GasTubs_1", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(0, pos_gas_2, GasLog_2, "GasTubs_2", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(0, pos_gas_3, GasLog_3, "GasTubs_3", logicWorld, false, 0, checkOverlaps);        
  
  // ***** Sample *****  
  G4Material* solid_sample = nist->FindOrBuildMaterial("G4_C");
  G4VSolid* sample_tubs = new G4Tubs("Sample",0*mm,(sample_radius)*mm,(sample_thick/2.)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_sample = G4ThreeVector(0, 0, (sample_center)*mm);  
  G4LogicalVolume* SampleLog = new G4LogicalVolume(sample_tubs, solid_sample, "Sample");
  new G4PVPlacement(0, pos_sample, SampleLog, "Sample", logicWorld, false, 0, checkOverlaps);

  // ***** Ge *****
  G4Material* solid_ge = nist->FindOrBuildMaterial("G4_Ge");
  G4VSolid* ge_tubs = new G4Tubs("GeDet",0*mm,(Ge_radius)*mm,(Ge_thick/2.)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_ge_1 = G4ThreeVector(Ge_dis, 0, (sample_center)*mm);//check beam position
  G4ThreeVector pos_ge_2 = G4ThreeVector(0, Ge_dis, (sample_center)*mm);//check beam position
  G4ThreeVector pos_ge_3 = G4ThreeVector(0, (-1*Ge_dis), (sample_center)*mm);//check beam position
  G4RotationMatrix* angle_ge_1 = new G4RotationMatrix(90*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg); 
  G4RotationMatrix* angle_ge_2 = new G4RotationMatrix(0*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg); 
  G4LogicalVolume* GeLog = new G4LogicalVolume(ge_tubs, solid_ge, "GeTubs");
  new G4PVPlacement(angle_ge_1, pos_ge_1, GeLog, "GeTubs_1", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_ge_2, pos_ge_2, GeLog, "GeTubs_2", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_ge_2, pos_ge_3, GeLog, "GeTubs_3", logicWorld, false, 0, checkOverlaps);        


  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
