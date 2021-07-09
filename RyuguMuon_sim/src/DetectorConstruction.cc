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
  G4double env_sizeXY = 30*cm, env_sizeZ = 90*cm;//World volume
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
  G4ThreeVector pos_world = G4ThreeVector(0, 0, 0);
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        Vacuum,           //its material
//                        elA,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      pos_world,       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
//  G4Box* solidEnv =    
//    new G4Box("Envelope",                    //its name
//        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
//      
//  G4LogicalVolume* logicEnv =                         
//    new G4LogicalVolume(solidEnv,            //its solid
////                        Vacuum,             //its material
//                        env_mat,             //its material
//                        "Envelope");         //its name
               

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
  // ***** Foil-1 *****
  //
//  G4Material* solid_foil1 = elA;//None
  G4Material* solid_foil1 = nist->FindOrBuildMaterial("G4_KAPTON");
//  G4Material* solid_foil1 = nist->FindOrBuildMaterial("G4_Al");
//  G4Material* solid_foil1 = nist->FindOrBuildMaterial("G4_Cu");
  G4double foil1_thick = 0.150;//mm
  G4VSolid* foil1_tubs = new G4Tubs("FoilTubs1",0*mm,(120/2)*mm,(foil1_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_foil1 = G4ThreeVector(0, 0, -(foil1_thick/2)*mm);//check beam position
  G4LogicalVolume* FoilLog1 = new G4LogicalVolume(foil1_tubs, solid_foil1, "FoilTubs1");  
  new G4PVPlacement(0, pos_foil1, FoilLog1, "FoilTubs1", logicWorld, false, 0,  checkOverlaps);        

  //
  // ***** intermediate Air *****
  //
  G4double inter_air_thick = 45;//mm
  G4VSolid* inter_air_tubs = new G4Tubs("intermediate1",0*mm,(240/2)*mm,(inter_air_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_inter_air = G4ThreeVector(0, 0, (foil1_thick+inter_air_thick/2)*mm);//check beam position
  G4LogicalVolume* INTERLog1 = new G4LogicalVolume(inter_air_tubs, elA, "intermediate1");  
  new G4PVPlacement(0, pos_inter_air, INTERLog1, "intermediate1", logicWorld, false, 0,  checkOverlaps);        
  
  //
  // ***** Foil-2 *****
  //
//  G4Material* solid_foil2 = elA;//None
  G4Material* solid_foil2 = nist->FindOrBuildMaterial("G4_KAPTON");
//  G4Material* solid_foil2 = nist->FindOrBuildMaterial("G4_Al");
//  G4Material* solid_foil2 = nist->FindOrBuildMaterial("G4_Cu");
  G4double foil2_thick = 0.020;//mm
  G4VSolid* foil2_tubs = new G4Tubs("FoilTubs2",0*mm,(150/2)*mm,(foil2_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_foil2 = G4ThreeVector(0, 0, (foil2_thick/2+inter_air_thick+foil1_thick)*mm);//check beam position
  G4LogicalVolume* FoilLog2 = new G4LogicalVolume(foil2_tubs, solid_foil2, "FoilTubs2");  
  new G4PVPlacement(0, pos_foil2, FoilLog2, "FoilTubs2", logicWorld, false, 0,  checkOverlaps);        

  //
  // ***** intermediate He *****
  //
  G4Material* solid_He = nist->FindOrBuildMaterial("G4_He");
    // === or ===
  //G4double He_atomicNumber = 2.;
  //G4double He_massOfMole = 4.003*g/mole;
  //G4double He_density = 0.166*g/cm3;
  //G4double He_temperature = 293.15*kelvin;
  //G4double He_pressure = (3.e-18)*pascal;//1atm
  //G4Material* solid_He = new G4Material( "My_He", He_atomicNumber, He_massOfMole, He_density, kStateGas, He_temperature, He_pressure); 

  G4double inter_h_thick = 450;//mm
  G4VSolid* inter_h_tubs = new G4Tubs("intermediate2",0*mm,(150/2)*mm,(inter_h_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_inter_h = G4ThreeVector(0, 0, (foil2_thick+inter_air_thick+foil1_thick+inter_h_thick/2)*mm);//check beam position
  G4LogicalVolume* INTERLog2 = new G4LogicalVolume(inter_h_tubs, solid_He, "intermediate2");  
  new G4PVPlacement(0, pos_inter_h, INTERLog2, "intermediate2", logicWorld, false, 0,  checkOverlaps);        

  //
  // ***** Chamber *****
  //
  G4Material* solid_chamber = nist->FindOrBuildMaterial("G4_Cu");
  G4VSolid* chamber_tubs = new G4Tubs("Chamber_ori",(150/2)*mm,(165.2/2)*mm,(inter_h_thick/2)*mm,0.,2*M_PI*rad);

  G4VSolid* Chamber_Hole = new G4Tubs("ChamberHole",0*mm,(25/2)*mm,(20/2)*mm,0.,2*M_PI*rad);
  G4double hole_dis=75+(15.2/2); 
  G4double current_angle_hole;
  G4ThreeVector pos_hole;
  G4RotationMatrix* rot_hole;
  G4int nHoles = 6;
  for(int i=0; i<nHoles;i++){
     current_angle_hole=(i*(2*CLHEP::pi/nHoles)+(CLHEP::pi/6))*CLHEP::rad;
     pos_hole = G4ThreeVector(hole_dis*std::sin(current_angle_hole)*mm, hole_dis*std::cos(current_angle_hole)*mm, (5)*mm);
     rot_hole = new G4RotationMatrix((-i*(360./nHoles)-30)*CLHEP::deg,-90*CLHEP::deg,0*CLHEP::deg);
     chamber_tubs = new G4SubtractionSolid("Chamber", chamber_tubs, Chamber_Hole, rot_hole, pos_hole);
  }
  G4LogicalVolume* ChamberLog = new G4LogicalVolume(chamber_tubs, solid_chamber, "Chamber");  
  new G4PVPlacement(0, pos_inter_h, ChamberLog, "Chamber", logicWorld, false, 0,  checkOverlaps);        

  //
  // ***** Virtual (in intermediate He) *****
  //
  G4double vir_thick = 0.010;//mm
  G4VSolid* Vir_tubs = new G4Tubs("VirTubs",0*mm,(150/2)*mm,(vir_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_vir = G4ThreeVector(0, 0, (-190)*mm);
  G4LogicalVolume* VirLog = new G4LogicalVolume(Vir_tubs, solid_He, "VirTubs");  
  new G4PVPlacement(0, pos_vir, VirLog, "VirTubs", INTERLog2, false, 0,  checkOverlaps);        

  //
  // ***** Virtual (not used) *****
  //
  G4VSolid* Vir_tubs2 = new G4Tubs("VirTubs2",0*mm,(150/2)*mm,(vir_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_vir2 = G4ThreeVector(0, 0, (foil2_thick+inter_air_thick+foil1_thick+inter_h_thick+vir_thick/2)*mm);
  G4LogicalVolume* VirLog2 = new G4LogicalVolume(Vir_tubs2, solid_He, "VirTubs2");  
//  new G4PVPlacement(0, pos_vir2, VirLog2, "VirTubs2", logicWorld, false, 0,  checkOverlaps);        

  // ***** Sample *****
  G4double sample_thick;
  G4double sample_width;
  // === SiO2 ===
//  G4Material* solid_sample = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"); sample_width=25; sample_thick=1.0;
  // === Al ===
//  G4Material* solid_sample = nist->FindOrBuildMaterial("G4_Al"); sample_width=25; sample_thick=1.2; //25*25*1.2 mm
  // === Fe ===
  G4Material* solid_sample = nist->FindOrBuildMaterial("G4_Fe"); sample_width=25; sample_thick=0.5;//25*25*0.5 mm
  // === Ti ===
//  G4Material* solid_sample = nist->FindOrBuildMaterial("G4_Ti"); sample_width=25; sample_thick=1.0;//25*25*1.0 mm

  G4RotationMatrix* rot_sample = new G4RotationMatrix(90*CLHEP::deg,40*CLHEP::deg,90*CLHEP::deg);
  G4Box* solidsample = new G4Box("Sample", (sample_width/2.)*mm, (sample_width/2.)*mm, (sample_thick/2.)*mm);
  G4ThreeVector pos_sample = G4ThreeVector(0, 0, (5)*mm);  
  G4LogicalVolume* SampleLog = new G4LogicalVolume(solidsample, solid_sample, "Sample");          
  new G4PVPlacement(rot_sample,  pos_sample, SampleLog, "Sample", INTERLog2, false, 0, checkOverlaps);


  // ***** Foil-3 (Cu cover) *****
  G4Material* solid_foil3 = nist->FindOrBuildMaterial("G4_Cu");
  G4double foil3_thick = 0.005;//mm
  G4Box* foil3_tubs = new G4Box("FoilTubs3", (138.4/2)*mm, (10/2)*mm, (foil3_thick/2)*mm);
  G4ThreeVector pos_foil3_1 = G4ThreeVector(0, 0, 5+(foil3_thick/2+sample_thick/2)/std::cos((40/360.)*2*CLHEP::pi)*mm);//check beam position
  G4ThreeVector pos_foil3_2 = G4ThreeVector(0, 0, 5-(foil3_thick/2+sample_thick/2)/std::cos((40/360.)*2*CLHEP::pi)*mm);//check beam position
  G4LogicalVolume* FoilLog3 = new G4LogicalVolume(foil3_tubs, solid_foil3, "FoilTubs3");  
  new G4PVPlacement(rot_sample, pos_foil3_1, FoilLog3, "FoilTubs3", INTERLog2, false, 0,  checkOverlaps);        
  new G4PVPlacement(rot_sample, pos_foil3_2, FoilLog3, "FoilTubs3", INTERLog2, false, 0,  checkOverlaps);        

  // ***** Holder *****
  //TODO


  // ***** Ge detector *****
  G4Material* solid_common;
  solid_common=nist->FindOrBuildMaterial("G4_Ge");
  G4VSolid* Ge_Det = new G4Tubs("GeDet",0*mm,(5.641895835477563)*mm,(5./2.)*mm,0.,2*M_PI*rad);
  G4double ge_dis=(foil2_thick+inter_air_thick+foil1_thick+inter_h_thick/2+vir_thick+1.5+foil3_thick+0.2+5);//mm; same with pos. of sample
  G4double ge_sample_dis=165.2/2+1+5./2.; // same with chamber thinkness+5mm
//  G4double ge_angle=2*CLHEP::pi*(45./360)*CLHEP::rad;
  G4double nDets=6;
  G4double current_angle;
  G4ThreeVector pos_ge;
  G4LogicalVolume* GeLog;
  G4RotationMatrix* rot_ge;
  for(int i=0; i<nDets;i++){
     auto idstr = std::to_string(i);
     current_angle=(i*(2*CLHEP::pi/nDets)+(CLHEP::pi/6))*CLHEP::rad;
//     pos_ge = G4ThreeVector(ge_dis*std::sin(ge_angle)*std::sin(current_angle)*mm, ge_dis*std::sin(ge_angle)*std::cos(current_angle)*mm, (ge_dis*std::cos(ge_angle))*mm);
     pos_ge = G4ThreeVector(ge_sample_dis*std::sin(current_angle)*mm, ge_sample_dis*std::cos(current_angle)*mm, (ge_dis)*mm);
     GeLog = new G4LogicalVolume(Ge_Det, solid_common, "GeTubs"+idstr);
     rot_ge = new G4RotationMatrix((-i*(360./nDets)-30)*CLHEP::deg,-90*CLHEP::deg,0*CLHEP::deg);
     new G4PVPlacement(rot_ge, pos_ge, GeLog, "GeTubs"+idstr, logicWorld, false, 0, checkOverlaps);
  }


  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
