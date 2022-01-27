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
#include "Parameters.hh"
#include "Messenger.hh"

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
{
   myMessenger = new Messenger(this);
   pointerToDet=this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 
   delete myMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction* DetectorConstruction::pointerToDet=0;
DetectorConstruction* DetectorConstruction::GetDetInstance() {return pointerToDet;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{ 

   // ------------------------------------------------------------------------
   // get parameter
   // ------------------------------------------------------------------------
   char tmpString0[100]="Unset", tmpString1[100]="Unset";
   SampleName="RI";
   rot_angle=0;
   chID=6;
   if (strcmp(Parameters::mySteeringFileName,"Unset")!=0){
      char charSteeringFileName[1000]; strcpy(charSteeringFileName,(Parameters::mySteeringFileName).c_str());
      //std::cout << "here11: " << charSteeringFileName << std::endl;
      FILE *fSteeringFile=fopen(charSteeringFileName,"r");
      char  line[501];
      while (!feof(fSteeringFile)) {
         fgets(line,500,fSteeringFile);
         //std::cout << "here: " << line << std::endl;
         if ((line[0]=='#')||(line[0]=='\n')||(line[0]=='\r')) continue;
         sscanf(&line[0],"%s %s",tmpString0,tmpString1);//command, sample name
         if (strcmp(tmpString0,"/command/sample")==0)SampleName = tmpString1;
         if (strcmp(tmpString0,"/command/rot")==0)rot_angle = std::stoi(tmpString1);
         if (strcmp(tmpString0,"/command/chID")==0)chID = std::stoi(tmpString1);
      }
      std::cout << "ROT. angle of RI : " << rot_angle << " detector ch : " << chID << std::endl;
   }
   //std::cout << "Sample Name: " << SampleName << std::endl;
   // ------------------------------------------------------------------------
   // Elements
   // ------------------------------------------------------------------------
   G4double A, Z;
   G4Element* elH  = new G4Element("Hydrogen","H",  Z=1.,  A=1.00794*g/mole);
   G4Element* elC  = new G4Element("Carbon",  "C",  Z=6.,  A= 12.011 *g/mole);
   G4Element* elN  = new G4Element("Nitrogen","N",  Z=7.,  A= 14.00674*g/mole);
   G4Element* elO  = new G4Element("Oxygen",  "O",  Z=8.,  A= 15.9994*g/mole);
   G4Element* elNa = new G4Element("Sodium",  "Na", Z=11., A= 22.989768*g/mole);
   G4Element* elSi = new G4Element("Silicon", "Si", Z=14., A= 28.0855*g/mole);
   G4Element* elAr = new G4Element("Argon",   "Ar", Z=18., A= 39.948*g/mole);
   G4Element* elI  = new G4Element("Iodine",  "I",  Z=53., A= 126.90447*g/mole);
   G4Element* elCs = new G4Element("Cesium",  "Cs", Z=55., A= 132.90543*g/mole);
   G4Element* elMg  = new G4Element("Magnesium", "Mg",Z=12.,A= 24.3050    *g/mole);
   G4Element* elFe  = new G4Element("Iron",      "Fe",Z=26.,A= 55.847     *g/mole);

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* elA = nist->FindOrBuildMaterial("G4_AIR"); 

  G4double atomicNumber = 1.;
  G4double massOfMole = 1.008*g/mole;
  G4double density = 1.e-25*g/cm3;
  G4double temperature = 2.73*kelvin;
  G4double pressure = 3.e-18*pascal;
  G4Material* Vacuum = new G4Material( "Vacuum", atomicNumber, massOfMole, density, kStateGas, temperature, pressure); 
 
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY =  90*cm;
  G4double world_sizeZ  =  90*cm;
  G4ThreeVector pos_world = G4ThreeVector(0, 0, 0);
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
//                        Vacuum,           //its material
                        elA,           //its material
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
  // ***** Table *****
  //
  G4Material* solid_table = nist->FindOrBuildMaterial("G4_Al");
  G4Box* solidTable = new G4Box("Table", 200*mm,50*mm,300*mm);
  G4LogicalVolume* TableLog = new G4LogicalVolume(solidTable, solid_table, "Table");
  G4ThreeVector pos_table = G4ThreeVector(0, -(90+50)*mm, -100*mm);
  new G4PVPlacement(0,  pos_table, TableLog, "Table", logicWorld, false, 0, checkOverlaps);

  //
  // ***** Sample *****
  //
  G4double rot_sample=(rot_angle/360.)*2*CLHEP::pi*CLHEP::rad;
  G4RotationMatrix* rot_angle_sample = new G4RotationMatrix(90*CLHEP::deg,rot_angle*CLHEP::deg,90*CLHEP::deg);;
  G4double sample_thick;
  G4double sample_width;
  G4Material* solid_sample;
  G4LogicalVolume* SampleLog;
  G4LogicalVolume* SampleLogCover;
  G4double isotope_plastic_radius = ((24/2.)-5.5)*mm;
  sample_thick = 0.01*mm;
  solid_sample = nist->FindOrBuildMaterial("G4_PLEXIGLASS"); // acrylic resin C5O2H8 density 1.18 g/cm3
  G4Tubs* solidsample = new G4Tubs("Sample", 0.0, isotope_plastic_radius, sample_thick*0.5, 0.0*deg, 360.0*deg);
  G4Tubs* solidsamplecover = new G4Tubs("SampleCover", ((24/2.)-5)*mm, (24/2.)*mm, 4*0.5, 0.0*deg, 360.0*deg);
  SampleLog = new G4LogicalVolume(solidsample, solid_sample, "Sample");
  SampleLogCover = new G4LogicalVolume(solidsamplecover, solid_sample, "SampleCover");
  G4ThreeVector pos_sample = G4ThreeVector(std::sin(rot_sample)*100, 0, -100+(100*std::cos(rot_sample))*mm);  
  new G4PVPlacement(rot_angle_sample,  pos_sample, SampleLog, "Sample", logicWorld, false, 0, checkOverlaps);
  new G4PVPlacement(rot_angle_sample,  pos_sample, SampleLogCover, "SampleCover", logicWorld, false, 0, checkOverlaps);


  //
  // ***** Ge detector *****
  //
  // common
  G4double Ge_thickness[6] = {10,9,10,10,10,10.3};
  G4double Ge_diameter[6] = {11.3,11.5,11.2,11.3,11.0,11.4};
  G4double groove_inner[6] = {3,3,3,3,2.5,3};//CH1,3,4,6 are unknown
  G4double groove_outer[6] = {5.5,5.5,5.5,5.5,5.5,5.5};//CH1,3,4,6 are unknown
  G4double groove_thickness[6] = {1.5,1.5,1.5,1.5,1,1.5};
  //G4double deadlayer_thickness[6] = {0.5,0.5,0.5,0.5,0.5,0.5};
  G4double deadlayer_thickness[6] = {0.5,1.68,0.015,0.5,0.5,0.5};//watanebe test TODO need to be fixed
  G4String cryostat_material[6] = {"G4_Cu","G4_Cu","G4_Al","G4_Cu","G4_Cu","G4_Cu"};//CH1,4,6 are unknown
  G4double cryostat_holder_length[6] = {8.7,8.7,10,8.7,8.7,8.7};//CH1,4,6 are unknown
  G4double cryostat_holder_thickness[6] = {2.4,2.4,2.4,2.4,2.4,2.4};//CH1,4,6 are unknown
  G4String endcap_material[6] = {"G4_Al","G4_Al","G4_STAINLESS-STEEL","G4_Al","G4_Al","G4_Al"};//CH1,4,6 are unknown
  G4double endcap_lengh[6] = {50,50,50,50,50,50};
  G4double endcap_thickness[6] = {1.6,1.6,0.9,1.6,1.6,1.6};//CH1,4,6 are unknown
  G4double endcap_diameter[6] = {25.4,25.4,25.4,25.4,25.4,25.4};
  G4double cover_lengh[6] = {30,30,30,30,30,30};
  G4double cover_thickness[6] = {3,3,3,3,3,3};
  G4double window_thickness[6] = {0.03,0.08,0.025,0.025,0.03,0.025};
  G4double distance_from_window[6] = {5,5,5,5,5,5};
  G4double distance_from_cover[6] = {0.1,0.1,0.1,0.1,0.1,0.1};//guess
  G4double window_sample_dis=0;;//dis. of the sample center from the Be window surface
  G4double ge_dis_Z = 100;//mm; pos. the sample on the z axis
  G4double nDets=6;
  
  G4Material* solid_cryostat;
  G4Material* solid_cryostatholder;
  G4Material* solid_endcap; 
  G4Material* solid_window; 
  G4Material* solid_cover; 
  G4VSolid* Ge_Det_ori;
  G4VSolid* Ge_Det_dead;
  G4VSolid* Ge_Det_dead_sub;
  G4VSolid* Ge_Det_groove;
  G4VSolid* Crystal_Holder;
  G4VSolid* Cover_Tubes;
  G4VSolid* Endcap_Tubes;
  G4VSolid* Window_Foil;
  G4LogicalVolume* DeadlayerLog[6];
  G4LogicalVolume* GeLog[6];
  G4LogicalVolume* CrystalHolderLog[6];
  G4LogicalVolume* EndcapLog[6];
  G4LogicalVolume* SnCoverLog[6];
  G4LogicalVolume* WindowLog[6];
  G4double det_angle;
  G4RotationMatrix* rot_deadlayer;
  G4ThreeVector pos_deadlayer;
  G4ThreeVector pos_ge;
  G4ThreeVector pos_endcap;
  G4ThreeVector pos_sncover;
  G4ThreeVector pos_window;
  
  for(int i=0; i<nDets;i++){
     if(i != chID-1) continue;//ch selection
     auto idstr = std::to_string(i+1);
     solid_cryostat = nist->FindOrBuildMaterial("G4_Ge");
     solid_cryostatholder = nist->FindOrBuildMaterial(cryostat_material[i]);
     solid_endcap = nist->FindOrBuildMaterial(endcap_material[i]);
     solid_cover = nist->FindOrBuildMaterial("G4_Sn");
     solid_window = nist->FindOrBuildMaterial("G4_Be");
//     Ge_Det_ori = new G4Tubs("GeDet_ori",0*mm,((Ge_diameter[i]/2.)-deadlayer_thickness[i])*mm,((Ge_thickness[i]-deadlayer_thickness[i])/2.)*mm,0.,2*M_PI*rad);//thinkness of deadlayer is 0.5 mm
     Ge_Det_ori = new G4Tubs("GeDet_ori",0*mm,((Ge_diameter[i]/2.)-deadlayer_thickness[i])*mm,((Ge_thickness[i]-0.5)/2.)*mm,0.,2*M_PI*rad);//thinkness of deadlayer is 0.5 mm
//     Ge_Det_groove = new G4Tubs("GeDet_groove",(groove_inner[i]/2.)*mm,(groove_outer[i]/2.)*mm,((groove_thickness[i]-deadlayer_thickness[i])/2.)*mm,0.,2*M_PI*rad); 
     Ge_Det_groove = new G4Tubs("GeDet_groove",(groove_inner[i]/2.)*mm,(groove_outer[i]/2.)*mm,((groove_thickness[i]-0.5)/2.)*mm,0.,2*M_PI*rad); 
//     Ge_Det_ori = new G4SubtractionSolid("GeDet", Ge_Det_ori, Ge_Det_groove, 0, G4ThreeVector(0*mm, 0*mm, -((Ge_thickness[i]-deadlayer_thickness[i])/2-(groove_thickness[i]-deadlayer_thickness[i])/2.)*mm));
     Ge_Det_ori = new G4SubtractionSolid("GeDet", Ge_Det_ori, Ge_Det_groove, 0, G4ThreeVector(0*mm, 0*mm, -((Ge_thickness[i]-0.5)/2-(groove_thickness[i]-0.5)/2.)*mm));
     Ge_Det_dead = new G4Tubs("Ge_Det_dead_ori",0*mm,(Ge_diameter[i]/2.)*mm,(Ge_thickness[i]/2.)*mm,0.,2*M_PI*rad);
//     Ge_Det_dead_sub = new G4Tubs("Ge_Det_dead_sub",0*mm,((Ge_diameter[i]-deadlayer_thickness[i]*2)/2.)*mm,((Ge_thickness[i]-deadlayer_thickness[i])/2.)*mm,0.,2*M_PI*rad);
     Ge_Det_dead_sub = new G4Tubs("Ge_Det_dead_sub",0*mm,((Ge_diameter[i]-deadlayer_thickness[i]*2)/2.)*mm,((Ge_thickness[i]-0.5)/2.)*mm,0.,2*M_PI*rad);
//     Ge_Det_groove = new G4Tubs("GeDet_groove",(groove_inner[i]/2.)*mm,(groove_outer[i]/2.)*mm,((deadlayer_thickness[i]/2.)*10)*mm,0.,2*M_PI*rad);//tips: 10 times for subtraction
//     Ge_Det_dead = new G4SubtractionSolid("Ge_Det_dead", Ge_Det_dead, Ge_Det_dead_sub, 0, G4ThreeVector(0*mm, 0*mm, (deadlayer_thickness[i]/2.)*mm));
     Ge_Det_groove = new G4Tubs("GeDet_groove",(groove_inner[i]/2.)*mm,(groove_outer[i]/2.)*mm,((0.5/2.)*10)*mm,0.,2*M_PI*rad);//tips: 10 times for subtraction
     Ge_Det_dead = new G4SubtractionSolid("Ge_Det_dead", Ge_Det_dead, Ge_Det_dead_sub, 0, G4ThreeVector(0*mm, 0*mm, (0.5/2.)*mm));
     Ge_Det_dead = new G4SubtractionSolid("Ge_Det_dead", Ge_Det_dead, Ge_Det_groove, 0, G4ThreeVector(0*mm, 0*mm, -(Ge_thickness[i]/2.)*mm));
     Crystal_Holder = new G4Tubs("Crystal_Holder",(Ge_diameter[i]/2.)*mm,((Ge_diameter[i]/2.)+cryostat_holder_thickness[i])*mm,(cryostat_holder_length[i]/2.)*mm,0.,2*M_PI*rad);//thinkness of holder is 2.4 mm
     Endcap_Tubes = new G4Tubs("Endcap_Tubes",((endcap_diameter[i]-endcap_thickness[i]*2)/2.)*mm,(endcap_diameter[i]/2.)*mm,(endcap_lengh[i]/2.)*mm,0.,2*M_PI*rad);
     Cover_Tubes = new G4Tubs("Cover_Tubes",((endcap_diameter[i]/2.)+distance_from_cover[i])*mm,((25.4/2.)+distance_from_cover[i]+cover_thickness[i])*mm,(cover_lengh[i]/2.)*mm,0.,2*M_PI*rad);
     Window_Foil = new G4Tubs("BeWin",0*mm,(endcap_diameter[i]/2.)*mm,(window_thickness[i]/2.)*mm,0.,2*M_PI*rad);

     // set logical volume
     DeadlayerLog[i] = new G4LogicalVolume(Ge_Det_dead, solid_cryostat, "Deadlayer"+idstr);
     GeLog[i] = new G4LogicalVolume(Ge_Det_ori, solid_cryostat, "Ge"+idstr);
     CrystalHolderLog[i] = new G4LogicalVolume(Crystal_Holder, solid_cryostatholder, "Crystal_Holder"+idstr);
     EndcapLog[i] = new G4LogicalVolume(Endcap_Tubes, solid_endcap, "Endcap"+idstr);
     SnCoverLog[i] = new G4LogicalVolume(Cover_Tubes, solid_cover, "SnCover"+idstr);
     WindowLog[i] = new G4LogicalVolume(Window_Foil, solid_window, "Window"+idstr);

     // place
     pos_deadlayer=G4ThreeVector(0,0,(-1)*(ge_dis_Z+window_thickness[i]/2.+5+Ge_thickness[i]/2.)*mm);
     //pos_ge=G4ThreeVector(0,0,(-1)*(ge_dis_Z+window_thickness[i]/2.+5+Ge_thickness[i]/2.-deadlayer_thickness[i])*mm);
     pos_ge=G4ThreeVector(0,0,(-1)*(ge_dis_Z+window_thickness[i]/2.+5+Ge_thickness[i]/2.-0.5)*mm);
     pos_sncover=G4ThreeVector(0,0,(-1)*(ge_dis_Z+window_thickness[i]/2.+cover_lengh[i]/2.)*mm);
     pos_endcap=G4ThreeVector(0,0,(-1)*(ge_dis_Z+window_thickness[i]/2.+endcap_lengh[i]/2.)*mm);
     pos_window=G4ThreeVector(0,0,(-1)*(ge_dis_Z)*mm);
     rot_deadlayer = new G4RotationMatrix((0)*CLHEP::deg,0*CLHEP::deg,0*CLHEP::deg);

     new G4PVPlacement(rot_deadlayer, pos_deadlayer, DeadlayerLog[i], "Deadlayer"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(0, pos_deadlayer, CrystalHolderLog[i], "Crystal_Holder"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(0, pos_ge, GeLog[i], "Ge"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(0, pos_endcap, EndcapLog[i], "Endcap"+idstr, logicWorld, false, 0, checkOverlaps);
//     new G4PVPlacement(0, pos_sncover, SnCoverLog[i], "SnCover"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(0, pos_window, WindowLog[i], "Window"+idstr, logicWorld, false, 0, checkOverlaps);
  } 

  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
