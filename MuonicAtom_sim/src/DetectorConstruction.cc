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
: G4VUserDetectorConstruction(),
fScoringVolume(0)
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
  
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double env_sizeXY = 80*cm, env_sizeZ = 100*cm;//World volume
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
                     

  // ***** Kapton *****
  G4Material* solid_kapton = nist->FindOrBuildMaterial("G4_KAPTON");
  G4VSolid* kapton_tubs = new G4Tubs("KaptonTubs",0*mm,(kapton_radiu)*mm,(kapton_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_kapton = G4ThreeVector(0, 0, (-70+1)*mm);//check beam position
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
  
 

  // ***** Sample *****
//    G4Material* solid_target = nist->FindOrBuildMaterial("G4_AIR");
//    G4Material* solid_target = nist->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");
    G4Material* solid_target = nist->FindOrBuildMaterial("G4_POLYPROPYLENE");//(C_2H_4)_N-Polypropylene

    G4Orb* solid_ball_L = new G4Orb("Ball", (1.27/2.)*cm);
    G4Orb* solid_ball_S = new G4Orb("samllBall", (0.635/2)*cm);
    G4ThreeVector pos_sample1 = G4ThreeVector((0+5)*mm, (0+5)*mm, (sample_dis+3+sample_dz/2)*mm);//front
    G4ThreeVector pos_sample2 = G4ThreeVector((0-5)*mm, (0+5)*mm, (sample_dis-3+sample_dz/2)*mm);//back
    G4ThreeVector pos_sample3 = G4ThreeVector((0+5)*mm, (0-5)*mm, (sample_dis+sample_dz/2)*mm);
    G4ThreeVector pos_sample4 = G4ThreeVector((0-5)*mm, (0-5)*mm, (sample_dis+sample_dz/2)*mm);
    G4LogicalVolume* TargetLog1 = new G4LogicalVolume(solid_ball_L,solid_target,"Target1"); 
    G4LogicalVolume* TargetLog2 = new G4LogicalVolume(solid_ball_L,solid_target,"Target2"); 
    G4LogicalVolume* TargetLog3 = new G4LogicalVolume(solid_ball_S,solid_target,"Target3"); 
    G4LogicalVolume* TargetLog4 = new G4LogicalVolume(solid_ball_S,solid_target,"Target4"); 
    new G4PVPlacement(0, pos_sample1, TargetLog1, "Target1", logicWorld, false,0,checkOverlaps);
    new G4PVPlacement(0, pos_sample4, TargetLog2, "Target2", logicWorld, false,0,checkOverlaps);
    new G4PVPlacement(0, pos_sample2, TargetLog3, "Target3", logicWorld, false,0,checkOverlaps);
    new G4PVPlacement(0, pos_sample3, TargetLog4, "Target4", logicWorld, false,0,checkOverlaps);


  // ***** CdTe *****
  G4Material* det_mater = nist->FindOrBuildMaterial("G4_CADMIUM_TELLURIDE");
  G4Box* solidDet = new G4Box("CdTesource", (det_dx/2)*mm, (det_dy/2)*mm, (det_dz/2)*mm);

  G4Material* collimator_mater = nist->FindOrBuildMaterial("G4_W");
  G4VSolid* cylinderDet = new G4Tubs("CdTeCollimator",1*mm,20*mm,(8/2)*mm,0.,2*M_PI*rad);
  G4Cons* collimator_cone=new G4Cons("collimator_cone",0*mm,(1./2)*mm, 0*mm, (5./2)*mm,(4./2)*mm, 0., shadow_angle*deg);//angle
  G4RotationMatrix* cone_rotation = new G4RotationMatrix(0*CLHEP::deg,180*CLHEP::deg,0*CLHEP::deg);
  G4VSolid* substractsolid_det_pre = new G4SubtractionSolid("CdTecollimator_pre", cylinderDet, collimator_cone, 0, G4ThreeVector(0.*cm, 0.* cm, 0.*cm));
  G4VSolid* substractsolid_det = new G4SubtractionSolid("CdTecollimator", substractsolid_det_pre, collimator_cone, cone_rotation, G4ThreeVector(0.*cm, 0.* cm, 0.*cm));

//  G4ThreeVector move_collimator = G4ThreeVector(0*mm, 30*mm, 0*mm);
//  G4RotationMatrix* rotation_collimator = new G4RotationMatrix(0*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);
//  G4VSolid* unionsolid_det = new G4UnionSolid("CdTe", solidDet, cylinderDet, rotation_collimator , move_collimator);
  
  G4ThreeVector pos_det_1 = G4ThreeVector(0, -(det_dy/2+det_dis)*mm, (sample_dis+sample_dz/2)*mm);
  G4RotationMatrix* angle_det_1 = new G4RotationMatrix(0*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
  G4ThreeVector pos_collimator_1 = G4ThreeVector(0, -(det_dy/2+det_dis-30)*mm, (sample_dis+sample_dz/2)*mm);
  G4RotationMatrix* angle_collimator_1 = new G4RotationMatrix(0*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);

  G4ThreeVector pos_det_2 = G4ThreeVector(0, (det_dy/2+det_dis)*mm, (sample_dis+sample_dz/2)*mm);
  G4RotationMatrix* angle_det_2 = new G4RotationMatrix(0*CLHEP::deg,180*CLHEP::deg,0*CLHEP::deg);
  G4ThreeVector pos_collimator_2 = G4ThreeVector(0, (det_dy/2+det_dis-30)*mm, (sample_dis+sample_dz/2)*mm);
  G4RotationMatrix* angle_collimator_2 = new G4RotationMatrix(0*CLHEP::deg,270*CLHEP::deg,0*CLHEP::deg);

  G4LogicalVolume* DetLog_1 = new G4LogicalVolume(solidDet,det_mater,"CdTe1");           //its name
  G4LogicalVolume* DetLog_2 = new G4LogicalVolume(solidDet,det_mater,"CdTe2");           //its name
  G4LogicalVolume* DetColliLog_1 = new G4LogicalVolume(substractsolid_det,collimator_mater,"CdTeCollimator1");           
  G4LogicalVolume* DetColliLog_2 = new G4LogicalVolume(substractsolid_det,collimator_mater,"CdTeCollimator2");           

  new G4PVPlacement(angle_det_1,pos_det_1,DetLog_1,"CdTe1",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_1,pos_collimator_1,DetColliLog_1,"CdTeCollimator1",logicWorld,false,0,checkOverlaps);
  new G4PVPlacement(angle_det_2,pos_det_2,DetLog_2,"CdTe2",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_2,pos_collimator_2,DetColliLog_2,"CdTeCollimator2",logicWorld,false,0,checkOverlaps);

  // Set Detector as scoring volume
  fScoringVolume = DetLog_1;
  fSampleVolume = TargetLog1;
  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
