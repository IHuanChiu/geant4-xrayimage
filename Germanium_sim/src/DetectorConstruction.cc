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
  
  // Envelope parameters
  //
  G4double env_sizeXY = 80*cm, env_sizeZ = 100*cm;//World volume
  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
//                        Vacuum,           //its material
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
//                        Vacuum,             //its material
                        env_mat,             //its material
                        "Envelope");         //its name
               

  // ***** Kapton *****
  G4Material* solid_kapton = nist->FindOrBuildMaterial("G4_KAPTON");
  G4VSolid* kapton_tubs = new G4Tubs("KaptonTubs",0*mm,(55)*mm,(0.05/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_kapton = G4ThreeVector(0, 0, 0.5*mm);//check beam position
  G4LogicalVolume* KaptonLog =
       new G4LogicalVolume(kapton_tubs,         //its solid
                           solid_kapton,          //its material
                           "KaptonTubs");           //its name
  new G4PVPlacement(0,                       //no rotation
                    pos_kapton,                    //at position
                    KaptonLog,             //its logical volume
                    "KaptonTubs",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking   

//  G4Material* solid_kapton2 = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* solid_kapton2 = nist->FindOrBuildMaterial("G4_Ge");
//  G4Material* solid_kapton2 = nist->FindOrBuildMaterial("G4_Al");
//  G4Material* solid_kapton2 = nist->FindOrBuildMaterial("G4_C");
  G4VSolid* kapton_tubs2 = new G4Tubs("Target",0*mm,(100)*mm,(100/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_kapton2 = G4ThreeVector(0, 0, (60+dis+10+5+50)*mm);//check beam position
  G4LogicalVolume* KaptonLog2 =
       new G4LogicalVolume(kapton_tubs2,         //its solid
                           solid_kapton2,          //its material
                           "Target");           //its name
  new G4PVPlacement(0,                       //no rotation
                    pos_kapton2,                    //at position
                    KaptonLog2,             //its logical volume
                    "Target",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking   
  
 
   
//  G4Material* solid_shelf = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* solid_shelf = nist->FindOrBuildMaterial("G4_C");
//  G4Material* solid_shelf = nist->FindOrBuildMaterial("G4_Al");
//  G4Material* solid_shelf = nist->FindOrBuildMaterial("G4_Fe");
//  G4Material* solid_shelf = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4Box* solidshelf = new G4Box("Shelf", (100/2)*mm, (100/2)*mm, (10/2)*mm);
  G4ThreeVector pos_shelf = G4ThreeVector(0, 0, (60+dis+5)*mm);  
  G4LogicalVolume* ShelfLog =
       new G4LogicalVolume(solidshelf,         //its solid
                           solid_shelf,          //its material
                           "Shelf");           //its name

  new G4PVPlacement(0,                       //no rotation
                    pos_shelf,                    //at position
                    ShelfLog,             //its logical volume
                    "Shelf",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

   


  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
