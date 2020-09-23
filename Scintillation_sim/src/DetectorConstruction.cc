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
  G4ThreeVector pos_kapton_ori = G4ThreeVector(0, 0, -80*mm);
  G4LogicalVolume* KaptonLog = new G4LogicalVolume(kapton_tubs, solid_kapton, "KaptonTubs");  
  new G4PVPlacement(0, pos_kapton_ori, KaptonLog, "KaptonTubs", logicWorld, false, 0, checkOverlaps);        

  // ***** Tunnel *****
  G4Material* tunnel_fe = nist->FindOrBuildMaterial("G4_Fe"); 
  G4VSolid* tunnel = new G4Tubs("Tunnel",tunnel_inner_radius*mm,tunnel_outer_radius*mm,(tunnel_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_tunnel_left = G4ThreeVector(0, 0, -1*(tunnel_dis+tunnel_thick/2)*mm);
  G4ThreeVector pos_tunnel_right = G4ThreeVector(0, 0, (tunnel_dis+tunnel_thick/2)*mm);
  G4LogicalVolume* TunnelLog_left = new G4LogicalVolume(tunnel, tunnel_fe, "TunnelLog_left");
  G4LogicalVolume* TunnelLog_right = new G4LogicalVolume(tunnel, tunnel_fe, "TunnelLog_right");
  new G4PVPlacement(0, pos_tunnel_left, TunnelLog_left, "Tunnel", logicWorld, false, 0, checkOverlaps);
  new G4PVPlacement(0, pos_tunnel_right, TunnelLog_right, "Tunnel", logicWorld, false, 0, checkOverlaps);

  // ***** Sample *****
  G4Material* solid_sample_C = nist->FindOrBuildMaterial("G4_C");
  G4Material* solid_sample_SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  G4double density = 2*mg/cm3;
  G4Material* solid_sample_SiO2_C = new G4Material(name="Sample"  , density, ncomponents=2);
  solid_sample_SiO2_C->AddMaterial( solid_sample_SiO2,              fractionmass = 0.97);
  solid_sample_SiO2_C->AddMaterial( solid_sample_C,              fractionmass = 0.03);
  
  G4VSolid* sample_box = new G4Box("Sample", (ryugu_x/2)*mm, (ryugu_y/2)*mm, (ryugu_z/2)*mm);
  G4ThreeVector pos_sample_SiO2C = G4ThreeVector(0, 0, (0)*mm);
  G4RotationMatrix* angle_sample = new G4RotationMatrix(0*CLHEP::deg,0*CLHEP::deg,90*CLHEP::deg); 
  G4LogicalVolume* SampleLog = new G4LogicalVolume(sample_box, solid_sample_SiO2_C, "Sample");
  new G4PVPlacement(angle_sample, pos_sample_SiO2C, SampleLog, "Sample", logicWorld, false, 0, checkOverlaps);
   
  // ***** Ge *****
  G4Material* solid_ge = nist->FindOrBuildMaterial("G4_Ge");
  G4VSolid* ge_tubs = new G4Tubs("GeDet",0*mm,(Ge_radius)*mm,(Ge_thick/2.)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_ge_1 = G4ThreeVector(0, -Ge_dis*mm, 0);//check beam position
  G4ThreeVector pos_ge_2 = G4ThreeVector(0, Ge_dis*mm, 0);//check beam position
  G4RotationMatrix* angle_ge_1 = new G4RotationMatrix(90*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg); 
  G4RotationMatrix* angle_ge_2 = new G4RotationMatrix(0*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg); 
  G4LogicalVolume* GeLog_1 = new G4LogicalVolume(ge_tubs, solid_ge, "GeTubs_1");//LogicalVolume need to be seperated!
  G4LogicalVolume* GeLog_2 = new G4LogicalVolume(ge_tubs, solid_ge, "GeTubs_2");
  new G4PVPlacement(angle_ge_1, pos_ge_1, GeLog_1, "GeTubs_1", logicWorld, false, 0, checkOverlaps);        
  new G4PVPlacement(angle_ge_2, pos_ge_2, GeLog_2, "GeTubs_2", logicWorld, false, 0, checkOverlaps);        

  // ***** Ge Cover *****
//  G4Material* solid_al = nist->FindOrBuildMaterial("G4_Al");
//  G4VSolid* al_tubs_main = new G4Tubs("AlDet_main",(cover_radius-cover_thick)*mm,(cover_radius)*mm,(cover_lengh/2.)*mm,0.,2*M_PI*rad);
//  G4VSolid* al_tubs_uni = new G4Tubs("AlDet_uni",0*mm,(cover_radius)*mm,(cover_thick/2.)*mm,0.,2*M_PI*rad);
//  G4VSolid* al_tubs = new G4UnionSolid("AlDet", al_tubs_main, al_tubs_uni, 0, G4ThreeVector(0.*mm, 0.* mm, (cover_lengh/2.)*mm));
//  G4ThreeVector pos_al_1 = G4ThreeVector(cover_dis, 0, (0)*mm);//check beam position
//  G4ThreeVector pos_al_2 = G4ThreeVector(0, cover_dis, (0)*mm);//check beam position
//  G4ThreeVector pos_al_3 = G4ThreeVector(0, (-1*cover_dis), (0)*mm);//check beam position
//  G4RotationMatrix* angle_al_1 = new G4RotationMatrix(90*CLHEP::deg,-90*CLHEP::deg,0*CLHEP::deg); 
//  G4RotationMatrix* angle_al_2 = new G4RotationMatrix(0*CLHEP::deg,90*CLHEP::deg,0*CLHEP::deg); 
//  G4RotationMatrix* angle_al_3 = new G4RotationMatrix(0*CLHEP::deg,-90*CLHEP::deg,180*CLHEP::deg); 
//  G4LogicalVolume* AlLog_1 = new G4LogicalVolume(al_tubs, solid_al, "AlTubs_1");
//  G4LogicalVolume* AlLog_2 = new G4LogicalVolume(al_tubs, solid_al, "AlTubs_2");
//  G4LogicalVolume* AlLog_3 = new G4LogicalVolume(al_tubs, solid_al, "AlTubs_3");
//  new G4PVPlacement(angle_al_1, pos_al_1, AlLog_1, "AlTubs_1", GasLog_3, false, 0, checkOverlaps);        
//  new G4PVPlacement(angle_al_2, pos_al_2, AlLog_2, "AlTubs_2", GasLog_3, false, 0, checkOverlaps);        
//  new G4PVPlacement(angle_al_3, pos_al_3, AlLog_3, "AlTubs_3", GasLog_3, false, 0, checkOverlaps);       
//   
//  G4VSolid* cu_tubs_cover = new G4Tubs("CuDet",(cover_radius+1)*mm,(cover_radius+1+cu_cover_thick)*mm,((cover_lengh+cover_thick)/2.)*mm,0.,2*M_PI*rad);
//  G4LogicalVolume* CuLog_1 = new G4LogicalVolume(cu_tubs_cover, solid_stand, "CuTubs_1");
//  G4LogicalVolume* CuLog_2 = new G4LogicalVolume(cu_tubs_cover, solid_stand, "CuTubs_2");
//  G4LogicalVolume* CuLog_3 = new G4LogicalVolume(cu_tubs_cover, solid_stand, "CuTubs_3");
//  new G4PVPlacement(angle_al_1, pos_al_1, CuLog_1, "CuTubs_1", GasLog_3, false, 0, checkOverlaps);        
//  new G4PVPlacement(angle_al_2, pos_al_2, CuLog_2, "CuTubs_2", GasLog_3, false, 0, checkOverlaps);        
//  new G4PVPlacement(angle_al_3, pos_al_3, CuLog_3, "CuTubs_3", GasLog_3, false, 0, checkOverlaps);         

  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
