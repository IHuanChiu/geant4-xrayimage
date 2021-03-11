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
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"

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

//  G4Material* Vacuum= new G4Material( "Vacuum", CLHEP::universe_mean_density, 2 );//no possible for real vacuum
//  Vacuum-> AddElement(elN, .7);
//  Vacuum-> AddElement(elO, .3);

  G4double atomicNumber = 1.;
  G4double massOfMole = 1.008*g/mole;
  G4double density = 1.e-25*g/cm3;
  G4double temperature = 2.73*kelvin;
  G4double pressure = 3.e-18*pascal;
  G4Material* Vacuum = new G4Material( "Vacuum", atomicNumber, massOfMole, density, kStateGas, temperature, pressure); 
 
  // Envelope parameters
  //
  G4double env_sizeXY = 50*cm, env_sizeZ = 50*cm;//World volume
  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
  // ***** expain pressure *****
  //std::cout << "Air : " << elA->GetPressure() << std::endl;//Output is STP, 6.32421e+08
  //STP: 6.32421e+08 -> 101325 pascal -> 1atm 
  //if G4Material* the_He = nist->FindOrBuildMaterial("G4_He");
  //the_He is also 1atm
   

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        Vacuum,           //its material
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

  //
  // ***** Kapton *****
  //
  G4Material* solid_kap = nist->FindOrBuildMaterial("G4_KAPTON");
  G4double kap_thick = 0.100;//mm
  G4VSolid* kap_tubs = new G4Tubs("KaptonTubs",0*mm,(200/2)*mm,(kap_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_kap = G4ThreeVector(0, 0, -(kap_thick/2)*mm);//check beam position
  G4LogicalVolume* KapLog = new G4LogicalVolume(kap_tubs, solid_kap, "KaptonTubs");  
  new G4PVPlacement(0, pos_kap, KapLog, "KaptonTubs", logicWorld, false, 0,  checkOverlaps);        

  //
  // ***** intermediate Air *****
  //
  G4double inter_air_thick = 120;//mm
  G4VSolid* inter_air_tubs = new G4Tubs("intermediate",0*mm,(200/2)*mm,(inter_air_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_inter_air = G4ThreeVector(0, 0, (inter_air_thick/2)*mm);//check beam position
  G4LogicalVolume* INTERLog = new G4LogicalVolume(inter_air_tubs, elA, "intermediate");  
//  G4LogicalVolume* INTERLog = new G4LogicalVolume(inter_air_tubs, Vacuum, "intermediate");  
  new G4PVPlacement(0, pos_inter_air, INTERLog, "intermediate", logicWorld, false, 0,  checkOverlaps);       
  G4cout << elA << G4endl;

  //
  // ***** Foil-0 *****
  //
  G4VSolid* foil0_tubs = new G4Tubs("FoilTubs0",0*mm,(180/2)*mm,(0.01/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_foil0 = G4ThreeVector(0, 0, -(58)*mm);//check beam position, 2mm
  G4LogicalVolume* FoilLog0 = new G4LogicalVolume(foil0_tubs, elA, "FoilTubs0");  
  new G4PVPlacement(0, pos_foil0, FoilLog0, "FoilTubs0", INTERLog, false, 0,  checkOverlaps);        
  //
  // ***** Foil-1 *****
  //
  G4VSolid* foil1_tubs = new G4Tubs("FoilTubs1",0*mm,(180/2)*mm,(0.01/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_foil1 = G4ThreeVector(0, 0, -(10)*mm);//check beam position, 50mm
  G4LogicalVolume* FoilLog1 = new G4LogicalVolume(foil1_tubs, elA, "FoilTubs1");  
  new G4PVPlacement(0, pos_foil1, FoilLog1, "FoilTubs1", INTERLog, false, 0,  checkOverlaps);        
  //
  // ***** Foil-2 *****
  //
  G4VSolid* foil2_tubs = new G4Tubs("FoilTubs2",0*mm,(180/2)*mm,(0.01/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_foil2 = G4ThreeVector(0, 0, 20*mm);//check beam position, 80mm
  G4LogicalVolume* FoilLog2 = new G4LogicalVolume(foil2_tubs, elA, "FoilTubs2");  
  new G4PVPlacement(0, pos_foil2, FoilLog2, "FoilTubs2", INTERLog, false, 0,  checkOverlaps);        
  //
  // ***** Foil-3 *****
  //
  G4VSolid* foil3_tubs = new G4Tubs("FoilTubs3",0*mm,(180/2)*mm,(0.01/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_foil3 = G4ThreeVector(0, 0, 50*mm);//check beam position, 110mm
  G4LogicalVolume* FoilLog3 = new G4LogicalVolume(foil3_tubs, elA, "FoilTubs3");  
  new G4PVPlacement(0, pos_foil3, FoilLog3, "FoilTubs3", INTERLog, false, 0,  checkOverlaps);        
  //
  // ***** Sample poi *****
  //
  G4VSolid* sample_tubs = new G4Tubs("Sample",0*mm,(180/2)*mm,(0.01/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_sample = G4ThreeVector(0, 0, 250*mm);//check beam position, 250mm
  G4LogicalVolume* SampleLog = new G4LogicalVolume(sample_tubs, elA, "Sample");  
  new G4PVPlacement(0, pos_sample, SampleLog, "Sample", logicWorld, false, 0,  checkOverlaps);        

  //
  // ***** Magnetic field *****
  //
  G4MagneticField* magField = new G4UniformMagField(G4ThreeVector(1.*tesla,0.,0.)); //Uniform field

  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
