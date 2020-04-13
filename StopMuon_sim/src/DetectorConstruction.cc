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
               

  // ***** New Collimators *****
//  G4Material* shape_cover_mat = nist->FindOrBuildMaterial("G4_Pb");
//  G4Material* shape_cover_mat = nist->FindOrBuildMaterial("G4_Cu");
//  G4Tubs* Cylinder0 = new G4Tubs("Cylinder0",(5.0/2)*CLHEP::mm,(10.0/2)*CLHEP::mm,(4./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder1 = new G4Tubs("Cylinder1",(10.0/2)*CLHEP::mm,(20.0/2)*CLHEP::mm,(5./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder2 = new G4Tubs("Cylinder2",(10.0/2)*CLHEP::mm,(24.0/2)*CLHEP::mm,(9./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder3 = new G4Tubs("Cylinder3",(10.0/2)*CLHEP::mm,(34.0/2)*CLHEP::mm,(6./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder4 = new G4Tubs("Cylinder4",(20.0/2)*CLHEP::mm,(34.0/2)*CLHEP::mm,(8./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder5 = new G4Tubs("Cylinder5",(20.0/2)*CLHEP::mm,(44.0/2)*CLHEP::mm,(8./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder6 = new G4Tubs("Cylinder6",(30.0/2)*CLHEP::mm,(44.0/2)*CLHEP::mm,(6./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder7 = new G4Tubs("Cylinder7",(30.0/2)*CLHEP::mm,(60.0/2)*CLHEP::mm,(8./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder8 = new G4Tubs("Cylinder8",(30.0/2)*CLHEP::mm,(54.6/2)*CLHEP::mm,(2./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//  G4Tubs* Cylinder9 = new G4Tubs("Cylinder9",(40.0/2)*CLHEP::mm,(54.6/2)*CLHEP::mm,(16./2)*CLHEP::mm,0.*CLHEP::deg,360.*CLHEP::deg);
//
//  G4ThreeVector pos_cover0 = G4ThreeVector(0, 0, -(4./2)*mm);
//  G4ThreeVector pos_cover1 = G4ThreeVector(0, 0, -(5./2)*mm);
//  G4ThreeVector pos_cover2 = G4ThreeVector(0, 0, -(5+(9./2))*mm);
//  G4ThreeVector pos_cover3 = G4ThreeVector(0, 0, -(14.+(6./2))*mm);
//  G4ThreeVector pos_cover4 = G4ThreeVector(0, 0, -(20.+(8./2))*mm);
//  G4ThreeVector pos_cover5 = G4ThreeVector(0, 0, -(28.+(8./2))*mm);
//  G4ThreeVector pos_cover6 = G4ThreeVector(0, 0, -(36.+(6./2))*mm);
//  G4ThreeVector pos_cover7 = G4ThreeVector(0, 0, -(42.+(8./2))*mm);
//  G4ThreeVector pos_cover8 = G4ThreeVector(0, 0, -(50.+(2./2))*mm);
//  G4ThreeVector pos_cover9 = G4ThreeVector(0, 0, -(52.+(16./2))*mm);
//
//  G4LogicalVolume* CollimatorLog0 =
//       new G4LogicalVolume(Cylinder0,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator0");           //its name
//  G4LogicalVolume* CollimatorLog1 =
//       new G4LogicalVolume(Cylinder1,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator1");           //its name
//  G4LogicalVolume* CollimatorLog2 =
//       new G4LogicalVolume(Cylinder2,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator2");           //its name
//  G4LogicalVolume* CollimatorLog3 =
//       new G4LogicalVolume(Cylinder3,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator3");           //its name
//  G4LogicalVolume* CollimatorLog4 =
//       new G4LogicalVolume(Cylinder4,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator4");           //its name
//  G4LogicalVolume* CollimatorLog5 =
//       new G4LogicalVolume(Cylinder5,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator5");           //its name
//  G4LogicalVolume* CollimatorLog6 =
//       new G4LogicalVolume(Cylinder6,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator6");           //its name
//  G4LogicalVolume* CollimatorLog7 =
//       new G4LogicalVolume(Cylinder7,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator7");           //its name
//  G4LogicalVolume* CollimatorLog8 =
//       new G4LogicalVolume(Cylinder8,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator8");           //its name
//  G4LogicalVolume* CollimatorLog9 =
//       new G4LogicalVolume(Cylinder9,         //its solid
//                           shape_cover_mat,          //its material
//                           "Collimator9");           //its name
//
////  new G4PVPlacement(0,                       //no rotation
////                    pos_cover0,                    //at position
////                    CollimatorLog0,             //its logical volume
////                    "Collimator0",                //its name
////                    logicWorld,                //its mother  volume
////                    false,                   //no boolean operation
////                    0,                       //copy number
////                    checkOverlaps);          //overlaps checking
//
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover1,                    //at position
//                    CollimatorLog1,             //its logical volume
//                    "Collimator1",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking
//
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover2,                    //at position
//                    CollimatorLog2,             //its logical volume
//                    "Collimator2",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover3,                    //at position
//                    CollimatorLog3,             //its logical volume
//                    "Collimator3",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover4,                    //at position
//                    CollimatorLog4,             //its logical volume
//                    "Collimator4",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover5,                    //at position
//                    CollimatorLog5,             //its logical volume
//                    "Collimator5",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking   
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover6,                    //at position
//                    CollimatorLog6,             //its logical volume
//                    "Collimator6",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking   
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover7,                    //at position
//                    CollimatorLog7,             //its logical volume
//                    "Collimator7",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking   
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover8,                    //at position
//                    CollimatorLog8,             //its logical volume
//                    "Collimator8",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking   
//  new G4PVPlacement(0,                       //no rotation
//                    pos_cover9,                    //at position
//                    CollimatorLog9,             //its logical volume
//                    "Collimator9",                //its name
//                    logicWorld,                //its mother  volume
//                    false,                   //no boolean operation
//                    0,                       //copy number
//                    checkOverlaps);          //overlaps checking   
//
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
