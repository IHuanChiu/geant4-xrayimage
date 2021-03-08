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
#include "PrimaryGeneratorAction.hh"

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
  G4double atomicNumber = 1.;
  G4double massOfMole = 1.008*g/mole;
  G4double density = 1.e-25*g/cm3;
  G4double temperature = 2.73*kelvin;
  G4double pressure = 3.e-18*pascal;
  G4Material* Vacuum = new G4Material( "Vacuum", atomicNumber, massOfMole, density, kStateGas, temperature, pressure);
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 80*cm;//World volume
  G4double world_sizeZ  = 120*cm;//World volume
  
  G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps); 
                     

  //     
  // ***** Kapton *****
  //     
  PrimaryGeneratorAction * prime = new PrimaryGeneratorAction;
  G4Material* solid_kapton = nist->FindOrBuildMaterial("G4_KAPTON");
  G4VSolid* kapton_tubs = new G4Tubs("KaptonTubs",0*mm,(kapton_radiu)*mm,(kapton_thick/2.)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_kapton = G4ThreeVector(0, 0, (prime->beam_initZ+1)*mm);//check beam position
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
  
  G4VSolid* kapton_tubs_sample = new G4Tubs("AirSample",0*mm,(kapton_radiu+10)*mm,(200/2.)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_kapton_sample = G4ThreeVector(0, 0, (sample_dis)*mm);
  G4LogicalVolume* AirLog_sample = new G4LogicalVolume(kapton_tubs_sample,Vacuum, "AirSample");       
//  G4LogicalVolume* AirLog_sample = new G4LogicalVolume(kapton_tubs_sample,elA, "AirSample");       
  new G4PVPlacement(0, pos_kapton_sample, AirLog_sample, "AirSample", logicWorld, false, 0, checkOverlaps);     

  //     
  // ***** Sample *****
  //     
  G4Material* solid_target = nist->FindOrBuildMaterial("G4_POLYPROPYLENE");//(C_2H_4)_N-Polypropylene

  G4Orb* solid_ball_L = new G4Orb("Ball", (1.27/2.)*cm);
  G4Orb* solid_ball_S = new G4Orb("samllBall", (0.635/2.)*cm);
  G4ThreeVector pos_sample1 = G4ThreeVector((0+(14.14/2.))*mm, (0+(14.14/2.))*mm, (0+5+sample_dz/2.)*mm);//L back
  G4ThreeVector pos_sample2 = G4ThreeVector((0-(14.14/2.))*mm, (0+(14.14/2.))*mm, (0-6+sample_dz/2.)*mm);//S back
  G4ThreeVector pos_sample3 = G4ThreeVector((0+(14.14/2.))*mm, (0-(14.14/2.))*mm, (0-9+sample_dz/2.)*mm);//S front
  G4ThreeVector pos_sample4 = G4ThreeVector((0-(14.14/2.))*mm, (0-(14.14/2.))*mm, (0-2+sample_dz/2.)*mm);//L front
  G4LogicalVolume* TargetLog1 = new G4LogicalVolume(solid_ball_L,solid_target,"Target1"); 
  G4LogicalVolume* TargetLog2 = new G4LogicalVolume(solid_ball_L,solid_target,"Target2"); 
  G4LogicalVolume* TargetLog3 = new G4LogicalVolume(solid_ball_S,solid_target,"Target3"); 
  G4LogicalVolume* TargetLog4 = new G4LogicalVolume(solid_ball_S,solid_target,"Target4"); 
  new G4PVPlacement(0, pos_sample1, TargetLog1, "Target1", AirLog_sample, false,0,checkOverlaps);
  new G4PVPlacement(0, pos_sample4, TargetLog2, "Target2", AirLog_sample, false,0,checkOverlaps);
  new G4PVPlacement(0, pos_sample2, TargetLog3, "Target3", AirLog_sample, false,0,checkOverlaps);
  new G4PVPlacement(0, pos_sample3, TargetLog4, "Target4", AirLog_sample, false,0,checkOverlaps);

  //     
  // ***** Al Baton *****
  //     
  G4Material* alBaton_mater = nist->FindOrBuildMaterial("G4_Al");
  G4Box* alBaton = new G4Box("alBaton", (65/2.)*mm, (150/2.)*mm, (65/2.)*mm);
  G4ThreeVector pos_alBaton = G4ThreeVector(0*mm, 0*mm, (sample_dis+5+sample_dz + 380)*mm);//300 is experiment setting
  G4LogicalVolume* alBatonLog = new G4LogicalVolume(alBaton,alBaton_mater,"AlBaton");
  new G4PVPlacement(0, pos_alBaton, alBatonLog, "AlBaton", logicWorld, false,0,checkOverlaps);

  //     
  // ***** CdTe *****
  //     
  G4double det_y = det_dy/2+det_dis+colli_dis;
  G4double colli_y = det_dy/2+colli_dis;
  G4double rota_angle = 60;

  G4Material* det_mater = nist->FindOrBuildMaterial("G4_CADMIUM_TELLURIDE");
  G4Material* det_mater_si = nist->FindOrBuildMaterial("G4_Si");
  G4Box* solidDet = new G4Box("CdTesource", (det_dx/2.)*mm, (det_dy/2.)*mm, (det_dz/2.)*mm);

  G4Material* collimator_mater = nist->FindOrBuildMaterial("G4_W");
  G4VSolid* cylinderDet = new G4Tubs("CdTeCollimator",0*mm,colli_radiu*mm,(8/2.)*mm,0.,2*M_PI*rad);
  G4Cons* collimator_cone=new G4Cons("collimator_cone",0*mm,(cone_size/2.)*mm, 0*mm, (cone_size*3/2.)*mm,(4)*mm, 0., shadow_angle*deg);//angle
  G4RotationMatrix* cone_rotation = new G4RotationMatrix(0*CLHEP::deg,180*CLHEP::deg,0*CLHEP::deg);
  G4VSolid* substractsolid_det_pre = new G4SubtractionSolid("CdTecollimator_pre", cylinderDet, collimator_cone, 0, G4ThreeVector(0.*cm, 0.* cm, 0.*cm));
  G4VSolid* substractsolid_det = new G4SubtractionSolid("CdTecollimator", substractsolid_det_pre, collimator_cone, cone_rotation, G4ThreeVector(0.*cm, 0.* cm, 0.*cm));

  G4Material* shield_mater_1 = nist->FindOrBuildMaterial("G4_Pb");
  G4Material* shield_mater_2 = nist->FindOrBuildMaterial("G4_Sn");
  G4Material* shield_mater_3 = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* shield_mater_4 = nist->FindOrBuildMaterial("G4_Al");
  G4VSolid* shield_up_1 = new G4Tubs("shield_up_1",20*mm,colli_radiu*mm,(2/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* shield_up_2 = new G4Tubs("shield_up_2",20*mm,colli_radiu*mm,(0.3/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* shield_up_3 = new G4Tubs("shield_up_3",20*mm,colli_radiu*mm,(0.3/2.)*mm,0.,2*M_PI*rad);

  G4VSolid* shield_down_1 = new G4Tubs("shield_down_1",(colli_radiu-6)*mm,(colli_radiu)*mm,(zshield_heigh/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* shield_down_2 = new G4Tubs("shield_down_2",(colli_radiu-6-2.)*mm,(colli_radiu-6)*mm,(zshield_heigh/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* shield_down_3 = new G4Tubs("shield_down_3",(colli_radiu-6-2-1)*mm,(colli_radiu-6-2.)*mm,(zshield_heigh/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* shield_down_4 = new G4Tubs("shield_down_4",(colli_radiu-6-2-1-6)*mm,(colli_radiu-6-2-1)*mm,(zshield_heigh/2.)*mm,0.,2*M_PI*rad);

  G4VSolid* Albox = new G4Box("Albox",(50/2.)*mm, (0.1/2.)*mm, (50/2.)*mm);
  G4VSolid* Alboxdown1 = new G4Box("AlboxDown1",(80/2.)*mm, (90/2.)*mm, (80/2.)*mm);
  G4VSolid* Alboxdown2 = new G4Box("AlboxDown2",(80/2.-10)*mm, (90/2.)*mm, (80/2.-10)*mm);
  G4VSolid* Alboxdown = new G4SubtractionSolid("AlboxDown",Alboxdown1, Alboxdown2, 0, G4ThreeVector(0.*mm, 10.* mm, 0.*mm));

  G4Material* plastic_mater = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material* epe_mater = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material* be_mater = nist->FindOrBuildMaterial("G4_Be");

  G4VSolid* Sihousing_up = new G4Tubs("Sihousing_up",26*mm,100*mm,(21/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* Sihousing_down = new G4Tubs("Sihousing_down",85*mm,100*mm,(102/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* Bewindow = new G4Tubs("Bewindow",0*mm,45*mm,(7/2.)*mm,0.,2*M_PI*rad);
  G4Box* plastic_cover_pre = new G4Box("plastic_cover_pre", (60/2.)*mm, (1/2.)*mm, (60/2.)*mm);
  G4Box* plastic_cover_sub = new G4Box("plastic_cover_sub", (30/2.)*mm, (1/2.)*mm, (30/2.)*mm);
  G4VSolid* plastic_cover = new G4SubtractionSolid("plastic_cover", plastic_cover_pre, plastic_cover_sub, 0, G4ThreeVector(0., 0., 0.));
  G4Box* kapton_cover = new G4Box("kapton_cover", (30/2.)*mm, (kapton_thick/2.)*mm, (30/2.)*mm);
  G4Box* epe_cover = new G4Box("epe_cover", (60/2.)*mm, (5/2.)*mm, (60/2.)*mm);

  // JPARC March
  //     
  // CdTe pos.
  //     
  G4LogicalVolume* DetLog = new G4LogicalVolume(solidDet,det_mater,"CdTe");          
  G4LogicalVolume* DetLog_colli = new G4LogicalVolume(substractsolid_det,collimator_mater,"Collimator");          
  G4LogicalVolume* DetLog_shield_up_1 = new G4LogicalVolume(shield_up_1,shield_mater_1,"shield_sn");          
  G4LogicalVolume* DetLog_shield_up_2 = new G4LogicalVolume(shield_up_2,shield_mater_2,"shield_cu");          
  G4LogicalVolume* DetLog_shield_up_3 = new G4LogicalVolume(shield_up_3,shield_mater_3,"shield_al");          
  G4LogicalVolume* DetLog_shield_down_1 = new G4LogicalVolume(shield_down_1,shield_mater_1,"shield_pb");          
  G4LogicalVolume* DetLog_shield_down_2 = new G4LogicalVolume(shield_down_2,shield_mater_2,"shield_sn");          
  G4LogicalVolume* DetLog_shield_down_3 = new G4LogicalVolume(shield_down_3,shield_mater_3,"shield_cu");          
  G4LogicalVolume* DetLog_shield_down_4 = new G4LogicalVolume(shield_down_4,shield_mater_4,"shield_al");          
  G4LogicalVolume* DetLog_Albox = new G4LogicalVolume(Albox,shield_mater_4,"Albox");          
  G4LogicalVolume* DetLog_AlboxDown = new G4LogicalVolume(Alboxdown,shield_mater_4,"AlboxDown");          

  G4ThreeVector pos_det_cdte = G4ThreeVector(0*mm, -det_y*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_collimator_cdte = G4ThreeVector(0*mm, -colli_y*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_shield_up_1 = G4ThreeVector(0*mm, (-colli_y-4.1-(2/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_shield_up_2 = G4ThreeVector(0*mm, (-colli_y-4.1-(2+0.3/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_shield_up_3 = G4ThreeVector(0*mm, (-colli_y-4.1-(2.3+0.3/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_shield_down_1 = G4ThreeVector(0*mm, (-colli_y-4.1-(4)-(zshield_heigh/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_shield_down_2 = G4ThreeVector(0*mm, (-colli_y-4.1-(4)-(zshield_heigh/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_shield_down_3 = G4ThreeVector(0*mm, (-colli_y-4.1-(4)-(zshield_heigh/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_shield_down_4 = G4ThreeVector(0*mm, (-colli_y-4.1-(4)-(zshield_heigh/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_albox = G4ThreeVector(0*mm, (-det_y+20)*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_alboxdown = G4ThreeVector(0*mm, (-det_y-10)*mm, (sample_dis+sample_dz/2.)*mm);

  G4RotationMatrix* angle_det;
  G4RotationMatrix* angle_collimator_cdte = new G4RotationMatrix(0,90*CLHEP::deg,0*CLHEP::deg);

   //  *** single CdTe ***
//  new G4PVPlacement(0,pos_det_cdte,DetLog,"CdTe",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_collimator_cdte,DetLog_colli,"Collimator",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_shield_up_1,DetLog_shield_up_1,"shield_sn",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_shield_up_2,DetLog_shield_up_2,"shield_cu",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_shield_up_3,DetLog_shield_up_3,"shield_al",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_shield_down_1,DetLog_shield_down_1,"shield_pb",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_shield_down_2,DetLog_shield_down_2,"shield_sn",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_shield_down_3,DetLog_shield_down_3,"shield_cu",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_shield_down_4,DetLog_shield_down_4,"shield_al",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(0,pos_albox,DetLog_Albox,"Albox",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(0,pos_alboxdown,DetLog_AlboxDown,"AlboxDown",logicWorld,false,0,checkOverlaps); 

   //  *** multi CdTe ***
  G4double current_angle;
  nDets=8;
  G4RotationMatrix* angle_albox;
  G4RotationMatrix* angle_shadow_cdte;
  for (int i=0; i < nDets; i++){
     auto idstr = std::to_string(i);
     current_angle=i*(2*CLHEP::pi/nDets)*CLHEP::rad;
     angle_det = new G4RotationMatrix(i*(360./nDets)*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
     angle_albox = new G4RotationMatrix((i*(360./nDets)+180)*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
     angle_shadow_cdte = new G4RotationMatrix(i*(360./nDets)*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);
     angle_collimator_cdte = new G4RotationMatrix(-i*(360./nDets)*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);

     pos_det_cdte = G4ThreeVector(det_y*std::sin(current_angle)*mm, det_y*-1*std::cos(current_angle)*mm, (sample_dis+sample_dz/2.)*mm);
     pos_collimator_cdte = G4ThreeVector(colli_y*std::sin(current_angle)*mm, -colli_y*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_shield_up_1 = G4ThreeVector((-colli_y-4.1-(2/2.))*std::sin(current_angle)*mm, (-colli_y-4.1-(2/2.))*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_shield_up_2 = G4ThreeVector((-colli_y-4.1-(2+0.3/2.))*std::sin(current_angle)*mm, (-colli_y-4.1-(2+0.3/2.))*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_shield_up_3 = G4ThreeVector((-colli_y-4.1-(2.3+0.3/2.))*std::sin(current_angle)*mm, (-colli_y-4.1-(2.3+0.3/2.))*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_shield_down_1 = G4ThreeVector((-colli_y-4.1-(4)-(zshield_heigh/2.))*std::sin(current_angle)*mm, (-colli_y-4.1-(4)-(zshield_heigh/2.))*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_shield_down_2 = G4ThreeVector((-colli_y-4.1-(4)-(zshield_heigh/2.))*std::sin(current_angle)*mm, (-colli_y-4.1-(4)-(zshield_heigh/2.))*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_shield_down_3 = G4ThreeVector((-colli_y-4.1-(4)-(zshield_heigh/2.))*std::sin(current_angle)*mm, (-colli_y-4.1-(4)-(zshield_heigh/2.))*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_shield_down_4 = G4ThreeVector((-colli_y-4.1-(4)-(zshield_heigh/2.))*std::sin(current_angle)*mm, (-colli_y-4.1-(4)-(zshield_heigh/2.))*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_albox = G4ThreeVector((-det_y+20)*std::sin(current_angle)*mm, (-det_y+20)*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);
     pos_alboxdown = G4ThreeVector((-det_y-10)*std::sin(current_angle)*mm, (-det_y-10)*(-1*std::cos(current_angle))*mm, (sample_dis+sample_dz/2.)*mm);

     DetLog = new G4LogicalVolume(solidDet,det_mater,"CdTe"+idstr);          
     new G4PVPlacement(angle_det,pos_det_cdte,DetLog,"CdTe"+idstr,logicWorld,false,0,checkOverlaps); 
//     new G4PVPlacement(angle_det,pos_det_cdte,DetLog,"CdTe",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_collimator_cdte,pos_collimator_cdte,DetLog_colli,"Collimator",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_shadow_cdte,pos_shield_up_1,DetLog_shield_up_1,"shield_sn",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_shadow_cdte,pos_shield_up_2,DetLog_shield_up_2,"shield_cu",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_shadow_cdte,pos_shield_up_3,DetLog_shield_up_3,"shield_al",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_shadow_cdte,pos_shield_down_1,DetLog_shield_down_1,"shield_pb",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_shadow_cdte,pos_shield_down_2,DetLog_shield_down_2,"shield_sn",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_shadow_cdte,pos_shield_down_3,DetLog_shield_down_3,"shield_cu",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_shadow_cdte,pos_shield_down_4,DetLog_shield_down_4,"shield_al",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_det,pos_albox,DetLog_Albox,"Albox",logicWorld,false,0,checkOverlaps); 
     new G4PVPlacement(angle_albox,pos_alboxdown,DetLog_AlboxDown,"AlboxDown",logicWorld,false,0,checkOverlaps); 
  }
  // */

  //     
  // Si pos.
  //     
  G4ThreeVector pos_colli_si = G4ThreeVector(0*mm, (colli_dis_si+(8/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_housing_up = G4ThreeVector(0*mm, (colli_dis_si+8+(21/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_housing_down = G4ThreeVector(0*mm, (colli_dis_si+8+21+(102/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_be = G4ThreeVector(0*mm, (colli_dis_si+8+21+(7/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_epe = G4ThreeVector(0*mm, (colli_dis_si+8+21+7+2+(5/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_pla = G4ThreeVector(0*mm, (colli_dis_si+8+21+7+2+5+(1/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_kapcover = G4ThreeVector(0*mm, (colli_dis_si+8+21+7+2+5+(1/2.))*mm, (sample_dis+sample_dz/2.)*mm);
  G4ThreeVector pos_det_si = G4ThreeVector(0*mm, (colli_dis_si+8+21+7+2+5+2+(det_dy/2.))*mm, (sample_dis+sample_dz/2.)*mm);

  G4LogicalVolume* DetLog_si = new G4LogicalVolume(solidDet,det_mater_si,"Si");         
  G4LogicalVolume* DetLog_housing_up = new G4LogicalVolume(Sihousing_up,shield_mater_4,"Housing");          
  G4LogicalVolume* DetLog_housing_down = new G4LogicalVolume(Sihousing_down,shield_mater_4,"Housing");          
  G4LogicalVolume* DetLog_Be = new G4LogicalVolume(Bewindow,be_mater,"Bewindow");          
  G4LogicalVolume* DetLog_epe = new G4LogicalVolume(epe_cover,epe_mater,"EPE");          
  G4LogicalVolume* DetLog_plastic = new G4LogicalVolume(plastic_cover,plastic_mater,"Plastic");          
  G4LogicalVolume* DetLog_kapcover = new G4LogicalVolume(kapton_cover,solid_kapton,"KaptonCover");          
 
//  new G4PVPlacement(angle_collimator_cdte,pos_colli_si,DetLog_colli,"Collimator",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_housing_up,DetLog_housing_up,"Housing",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_housing_down,DetLog_housing_down,"Housing",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_cdte,pos_be,DetLog_Be,"Bewindow",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(0,pos_epe,DetLog_epe,"EPE",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(0,pos_pla,DetLog_plastic,"Plastic",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(0,pos_kapcover,DetLog_kapcover,"KaptonCover",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(0,pos_det_si,DetLog_si,"Si",logicWorld,false,0,checkOverlaps); //Si Detector 



  // Six CdTes pos.
//  G4ThreeVector pos_det_0 = G4ThreeVector(det_y*std::sin(0*rota_angle*CLHEP::deg)*mm, det_y*std::cos(0*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_det_0 = new G4RotationMatrix(-0*rota_angle*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
//  G4ThreeVector pos_collimator_0 = G4ThreeVector(colli_y*std::sin(0*rota_angle*CLHEP::deg)*mm, colli_y*std::cos(0*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_collimator_0 = new G4RotationMatrix(-(0*rota_angle)*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);
//
//  G4ThreeVector pos_det_1 = G4ThreeVector(det_y*std::sin(1*rota_angle*CLHEP::deg)*mm, det_y*std::cos(1*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_det_1 = new G4RotationMatrix(-1*rota_angle*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
//  G4ThreeVector pos_collimator_1 = G4ThreeVector(colli_y*std::sin(1*rota_angle*CLHEP::deg)*mm, colli_y*std::cos(1*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_collimator_1 = new G4RotationMatrix(-(1*rota_angle)*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);
//
//  G4ThreeVector pos_det_2 = G4ThreeVector(det_y*std::sin(2*rota_angle*CLHEP::deg)*mm, det_y*std::cos(2*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_det_2 = new G4RotationMatrix(-2*rota_angle*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
//  G4ThreeVector pos_collimator_2 = G4ThreeVector(colli_y*std::sin(2*rota_angle*CLHEP::deg)*mm, colli_y*std::cos(2*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_collimator_2 = new G4RotationMatrix(-(2*rota_angle)*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);
//
//  G4ThreeVector pos_det_3 = G4ThreeVector(det_y*std::sin(3*rota_angle*CLHEP::deg)*mm, det_y*std::cos(3*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_det_3 = new G4RotationMatrix(-3*rota_angle*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
//  G4ThreeVector pos_collimator_3 = G4ThreeVector(colli_y*std::sin(3*rota_angle*CLHEP::deg)*mm, colli_y*std::cos(3*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_collimator_3 = new G4RotationMatrix(-(3*rota_angle)*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);
//
//  G4ThreeVector pos_det_4 = G4ThreeVector(det_y*std::sin(4*rota_angle*CLHEP::deg)*mm, det_y*std::cos(4*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_det_4 = new G4RotationMatrix(-4*rota_angle*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
//  G4ThreeVector pos_collimator_4 = G4ThreeVector(colli_y*std::sin(4*rota_angle*CLHEP::deg)*mm, colli_y*std::cos(4*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_collimator_4 = new G4RotationMatrix(-(4*rota_angle)*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);
//
//  G4ThreeVector pos_det_5 = G4ThreeVector(det_y*std::sin(5*rota_angle*CLHEP::deg)*mm, det_y*std::cos(5*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_det_5 = new G4RotationMatrix(-5*rota_angle*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);
//  G4ThreeVector pos_collimator_5 = G4ThreeVector(colli_y*std::sin(5*rota_angle*CLHEP::deg)*mm, colli_y*std::cos(5*rota_angle*CLHEP::deg)*mm, (sample_dis+sample_dz/2.)*mm);
//  G4RotationMatrix* angle_collimator_5 = new G4RotationMatrix(-(5*rota_angle)*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg);
//
//  G4LogicalVolume* DetLog_0 = new G4LogicalVolume(solidDet,det_mater,"CdTe0");           //its name
//  G4LogicalVolume* DetLog_1 = new G4LogicalVolume(solidDet,det_mater,"CdTe1");           //its name
//  G4LogicalVolume* DetLog_2 = new G4LogicalVolume(solidDet,det_mater,"CdTe2");           //its name
//  G4LogicalVolume* DetLog_3 = new G4LogicalVolume(solidDet,det_mater,"CdTe3");           //its name
//  G4LogicalVolume* DetLog_4 = new G4LogicalVolume(solidDet,det_mater,"CdTe4");           //its name
//  G4LogicalVolume* DetLog_5 = new G4LogicalVolume(solidDet,det_mater,"CdTe5");           //its name
//  G4LogicalVolume* DetColliLog_0 = new G4LogicalVolume(substractsolid_det,collimator_mater,"CdTeCollimator0");           
//  G4LogicalVolume* DetColliLog_1 = new G4LogicalVolume(substractsolid_det,collimator_mater,"CdTeCollimator1");           
//  G4LogicalVolume* DetColliLog_2 = new G4LogicalVolume(substractsolid_det,collimator_mater,"CdTeCollimator2");           
//  G4LogicalVolume* DetColliLog_3 = new G4LogicalVolume(substractsolid_det,collimator_mater,"CdTeCollimator3");           
//  G4LogicalVolume* DetColliLog_4 = new G4LogicalVolume(substractsolid_det,collimator_mater,"CdTeCollimator4");           
//  G4LogicalVolume* DetColliLog_5 = new G4LogicalVolume(substractsolid_det,collimator_mater,"CdTeCollimator5");           
//
//  new G4PVPlacement(angle_det_0,pos_det_0,DetLog_0,"CdTe0",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_det_1,pos_det_1,DetLog_1,"CdTe1",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_det_2,pos_det_2,DetLog_2,"CdTe2",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_det_3,pos_det_3,DetLog_3,"CdTe3",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_det_4,pos_det_4,DetLog_4,"CdTe4",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_det_5,pos_det_5,DetLog_5,"CdTe5",logicWorld,false,0,checkOverlaps); 
//  new G4PVPlacement(angle_collimator_0,pos_collimator_0,DetColliLog_0,"CdTeCollimator0",logicWorld,false,0,checkOverlaps);
//  new G4PVPlacement(angle_collimator_1,pos_collimator_1,DetColliLog_1,"CdTeCollimator1",logicWorld,false,0,checkOverlaps);
//  new G4PVPlacement(angle_collimator_2,pos_collimator_2,DetColliLog_2,"CdTeCollimator2",logicWorld,false,0,checkOverlaps);
//  new G4PVPlacement(angle_collimator_3,pos_collimator_3,DetColliLog_3,"CdTeCollimator3",logicWorld,false,0,checkOverlaps);
//  new G4PVPlacement(angle_collimator_4,pos_collimator_4,DetColliLog_4,"CdTeCollimator4",logicWorld,false,0,checkOverlaps);
//  new G4PVPlacement(angle_collimator_5,pos_collimator_5,DetColliLog_5,"CdTeCollimator5",logicWorld,false,0,checkOverlaps);

  


  // Set Detector as scoring volume
//  fScoringVolume = DetLog_1;
  fScoringVolume = DetLog;
  fSampleVolume = TargetLog1;
  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
