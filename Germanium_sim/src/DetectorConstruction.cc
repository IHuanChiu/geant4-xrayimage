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
  G4Material* Vacuum= new G4Material( "Vacuum", CLHEP::universe_mean_density, 2 );//no possible for real vacuum
  Vacuum-> AddElement(elN, .7);
  Vacuum-> AddElement(elO, .3);
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
  G4ThreeVector pos_kapton_ori = G4ThreeVector(0, 0, 0*mm);
  G4ThreeVector pos_kapton_1 = G4ThreeVector(0, 0, 296*mm);//check beam position
  G4ThreeVector pos_kapton_2 = G4ThreeVector(0, 0, (296+27.5)*mm);//check beam position
  G4LogicalVolume* KaptonLog = new G4LogicalVolume(kapton_tubs, solid_kapton, "KaptonTubs");  
  new G4PVPlacement(0, pos_kapton_ori, KaptonLog, "KaptonTubs", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(0, pos_kapton_1, KaptonLog, "KaptonTubs", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(0, pos_kapton_2, KaptonLog, "KaptonTubs", logicWorld, false, 0, checkOverlaps);        

  // ***** Gas *****
  G4double placeforkapton = 3;
  G4VSolid* gas_tubs_1 = new G4Tubs("VacuumGas",0*mm,gas_radius_1*mm,(gas_thick_1/2-placeforkapton)*mm,0.,2*M_PI*rad);
  G4VSolid* gas_tubs_2 = new G4Tubs("AirGas",0*mm,gas_radius_2*mm,(gas_thick_2/2-placeforkapton)*mm,0.,2*M_PI*rad);
  G4VSolid* gas_tubs_3 = new G4Tubs("HeGas",0*mm,gas_radius_3*mm,(gas_thick_3/2-placeforkapton)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_gas_1 = G4ThreeVector(0, 0, (gas_thick_1/2)*mm);//check beam position
  G4ThreeVector pos_gas_2 = G4ThreeVector(0, 0, (gas_thick_2/2+gas_thick_1)*mm);//check beam position
  G4ThreeVector pos_gas_3 = G4ThreeVector(0, 0, (gas_thick_3/2+gas_thick_2+gas_thick_1)*mm);//check beam position
  G4LogicalVolume* GasLog_1 = new G4LogicalVolume(gas_tubs_1, Vacuum, "GasTubs_1");
  G4LogicalVolume* GasLog_2 = new G4LogicalVolume(gas_tubs_2, elA, "GasTubs_2");
  G4LogicalVolume* GasLog_3 = new G4LogicalVolume(gas_tubs_3, Vacuum, "GasTubs_3");
  new G4PVPlacement(0, pos_gas_1, GasLog_1, "GasTubs_1", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(0, pos_gas_2, GasLog_2, "GasTubs_2", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(0, pos_gas_3, GasLog_3, "GasTubs_3", logicWorld, false, 0, checkOverlaps);        

  // ***** Chamber *****
  G4Material* chamber_fe = nist->FindOrBuildMaterial("G4_Fe"); 
  G4VSolid* chamber_tube = new G4Tubs("GasChamber",(gas_radius_3+10)*mm,(gas_radius_3+chamber_thick+10)*mm,((gas_thick_3-300)/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_chamber_1 = G4ThreeVector(0, 0, (gas_thick_3*0.25+gas_thick_2+gas_thick_1+0)*mm);
  G4ThreeVector pos_chamber_2 = G4ThreeVector(0, 0, (gas_thick_3*0.75+gas_thick_2+gas_thick_1+0)*mm);
  G4LogicalVolume* ChamberLog = new G4LogicalVolume(chamber_tube, chamber_fe, "ChamberTubs");
//  new G4PVPlacement(0, pos_chamber_1, ChamberLog, "ChamberTubs", logicWorld, false, 0, checkOverlaps);         
//  new G4PVPlacement(0, pos_chamber_2, ChamberLog, "ChamberTubs", logicWorld, false, 0, checkOverlaps);          
  
  // ***** Sample *****
  G4Material* solid_sample_C = nist->FindOrBuildMaterial("G4_C");
  G4VSolid* sample_tubs_C = new G4Tubs("Sample_C",0*mm,(sample_radius_C)*mm,(sample_thick_C/2.)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_sample_C = G4ThreeVector((sample_radius_SiO2/2.)*mm, (sample_radius_SiO2/2.)*mm, (0)*mm);  
  G4LogicalVolume* SampleLog_C = new G4LogicalVolume(sample_tubs_C, solid_sample_C, "Sample_C");
  new G4PVPlacement(0, pos_sample_C, SampleLog_C, "Sample_C", GasLog_3, false, 0, checkOverlaps);

  G4Material* solid_sample_SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4VSolid* sample_tubs_SiO2_pre = new G4Tubs("Sample_SiO2_pre",0*mm,(sample_radius_SiO2)*mm,(sample_thick_SiO2/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* sample_tubs_SiO2 = new G4SubtractionSolid("Sample_SiO2",sample_tubs_SiO2_pre, sample_tubs_C, 0, G4ThreeVector((sample_radius_SiO2/2.)*mm, (sample_radius_SiO2/2.)*mm, 0.*mm));
  G4ThreeVector pos_sample_SiO2 = G4ThreeVector(0, 0, (0)*mm);  
  G4LogicalVolume* SampleLog_SiO2 = new G4LogicalVolume(sample_tubs_SiO2, solid_sample_SiO2, "Sample_SiO2");
  new G4PVPlacement(0, pos_sample_SiO2, SampleLog_SiO2, "Sample_SiO2", GasLog_3, false, 0, checkOverlaps);

  // ***** Stand *****  
  G4Material* solid_stand = nist->FindOrBuildMaterial("G4_Cu");
  G4VSolid* stand_tubs_main = new G4Box("Stand_main", (Stand_x/2)*mm, (Stand_y/2)*mm, (Stand_z/2)*mm);
  G4VSolid* stand_tubs_sub = new G4Box("Stand_sub", ((Stand_x-10)/2)*mm, ((Stand_y-10)/2)*mm, ((Stand_z+10)/2)*mm);
  G4VSolid* stand_tubs = new G4SubtractionSolid("Stand", stand_tubs_main, stand_tubs_sub, 0, G4ThreeVector(0.*cm, 0.* cm, 0.*cm));
  G4ThreeVector pos_stand = G4ThreeVector(0, 0, (0)*mm);
  G4LogicalVolume* StandLog = new G4LogicalVolume(stand_tubs, solid_stand, "Stand");
  new G4PVPlacement(0, pos_stand, StandLog, "Stand", GasLog_3, false, 0, checkOverlaps);

  G4VSolid* stand_line_main = new G4Box("Stand_line", (Stand_line_x/2)*mm, (Stand_line_y/2)*mm, (Stand_line_z/2)*mm);
  G4ThreeVector pos_standline_1 = G4ThreeVector(0, 0, (1)*mm);
  G4ThreeVector pos_standline_2 = G4ThreeVector(0, 0, (-1)*mm);
  G4LogicalVolume* StandLineLog = new G4LogicalVolume(stand_line_main, solid_stand, "StandLine");
  new G4PVPlacement(0, pos_standline_1, StandLineLog, "StandLine", GasLog_3, false, 0, checkOverlaps);
  new G4PVPlacement(0, pos_standline_2, StandLineLog, "StandLine", GasLog_3, false, 0, checkOverlaps);

  // ***** Ge *****
  G4Material* solid_ge = nist->FindOrBuildMaterial("G4_Ge");
  G4VSolid* ge_tubs = new G4Tubs("GeDet",0*mm,(Ge_radius)*mm,(Ge_thick/2.)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_ge_1 = G4ThreeVector(Ge_dis, 0, (0)*mm);//check beam position
  G4ThreeVector pos_ge_2 = G4ThreeVector(0, Ge_dis, (0)*mm);//check beam position
  G4ThreeVector pos_ge_3 = G4ThreeVector(0, (-1*Ge_dis), (0)*mm);//check beam position
  G4RotationMatrix* angle_ge_1 = new G4RotationMatrix(90*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg); 
  G4RotationMatrix* angle_ge_2 = new G4RotationMatrix(0*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg); 
  G4RotationMatrix* angle_ge_3 = new G4RotationMatrix(0*CLHEP::deg,90*CLHEP::deg,180*CLHEP::deg); 
  G4LogicalVolume* GeLog_1 = new G4LogicalVolume(ge_tubs, solid_ge, "GeTubs_1");//LogicalVolume need to be seperated!
  G4LogicalVolume* GeLog_2 = new G4LogicalVolume(ge_tubs, solid_ge, "GeTubs_2");
  G4LogicalVolume* GeLog_3 = new G4LogicalVolume(ge_tubs, solid_ge, "GeTubs_3");
  new G4PVPlacement(angle_ge_1, pos_ge_1, GeLog_1, "GeTubs_1", GasLog_3, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_ge_2, pos_ge_2, GeLog_2, "GeTubs_2", GasLog_3, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_ge_3, pos_ge_3, GeLog_3, "GeTubs_3", GasLog_3, false, 0, checkOverlaps);        

  // ***** Ge Cover *****
  G4Material* solid_al = nist->FindOrBuildMaterial("G4_Al");
  G4VSolid* al_tubs_main = new G4Tubs("AlDet_main",(cover_radius-cover_thick)*mm,(cover_radius)*mm,(cover_lengh/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* al_tubs_uni = new G4Tubs("AlDet_uni",0*mm,(cover_radius)*mm,(cover_thick/2.)*mm,0.,2*M_PI*rad);
  G4VSolid* al_tubs = new G4UnionSolid("AlDet", al_tubs_main, al_tubs_uni, 0, G4ThreeVector(0.*mm, 0.* mm, (cover_lengh/2.)*mm));
  G4ThreeVector pos_al_1 = G4ThreeVector(cover_dis, 0, (0)*mm);//check beam position
  G4ThreeVector pos_al_2 = G4ThreeVector(0, cover_dis, (0)*mm);//check beam position
  G4ThreeVector pos_al_3 = G4ThreeVector(0, (-1*cover_dis), (0)*mm);//check beam position
  G4RotationMatrix* angle_al_1 = new G4RotationMatrix(90*CLHEP::deg,-90*CLHEP::deg,0*CLHEP::deg); 
  G4RotationMatrix* angle_al_2 = new G4RotationMatrix(0*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg); 
  G4RotationMatrix* angle_al_3 = new G4RotationMatrix(0*CLHEP::deg,-90*CLHEP::deg,180*CLHEP::deg); 
  G4LogicalVolume* AlLog_1 = new G4LogicalVolume(al_tubs, solid_al, "AlTubs_1");
  G4LogicalVolume* AlLog_2 = new G4LogicalVolume(al_tubs, solid_al, "AlTubs_2");
  G4LogicalVolume* AlLog_3 = new G4LogicalVolume(al_tubs, solid_al, "AlTubs_3");
  new G4PVPlacement(angle_al_1, pos_al_1, AlLog_1, "AlTubs_1", GasLog_3, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_al_2, pos_al_2, AlLog_2, "AlTubs_2", GasLog_3, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_al_3, pos_al_3, AlLog_3, "AlTubs_3", GasLog_3, false, 0, checkOverlaps);       
   
  G4VSolid* cu_tubs_cover = new G4Tubs("CuDet",(cover_radius+1)*mm,(cover_radius+1+cu_cover_thick)*mm,((cover_lengh+cover_thick)/2.)*mm,0.,2*M_PI*rad);
  G4LogicalVolume* CuLog_1 = new G4LogicalVolume(cu_tubs_cover, solid_stand, "CuTubs_1");
  G4LogicalVolume* CuLog_2 = new G4LogicalVolume(cu_tubs_cover, solid_stand, "CuTubs_2");
  G4LogicalVolume* CuLog_3 = new G4LogicalVolume(cu_tubs_cover, solid_stand, "CuTubs_3");
  new G4PVPlacement(angle_al_1, pos_al_1, CuLog_1, "CuTubs_1", GasLog_3, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_al_2, pos_al_2, CuLog_2, "CuTubs_2", GasLog_3, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_al_3, pos_al_3, CuLog_3, "CuTubs_3", GasLog_3, false, 0, checkOverlaps);         

  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
