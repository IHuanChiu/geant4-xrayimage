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
         if (strcmp(tmpString0,"/command/sample")==0){ SampleName = tmpString1;
         //std::cout << "here: " << line[0] << " tmpString0 " << tmpString0 << tmpString1 << std::endl;
         }
         if (strcmp(tmpString0,"/command/beamtype")==0){
            BeamType = tmpString1;
            if (BeamType == "ri" && SampleName != "RI"){
               G4cout << "Please note that you used beamtype is ri, so the /command/sample should be RI !" << G4endl;
               SampleName = "RI";
            }
         }
      }
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
  G4double world_sizeXY = 1.2*30*cm;
  G4double world_sizeZ  = 1.2*90*cm;
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
                     
               
  G4double covor_thick = 0.3;// cu cover for chamber mm
  //
  // ***** Foil-1 *****
  //
//  G4Material* solid_foil1 = elA;//None
  G4Material* solid_foil1 = nist->FindOrBuildMaterial("G4_KAPTON");
//  G4Material* solid_foil1 = nist->FindOrBuildMaterial("G4_Al");
//  G4Material* solid_foil1 = nist->FindOrBuildMaterial("G4_Cu");
  G4double foil1_thick = 0.100;//mm
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
  // ***** Foil-2 (chamber window) *****
  //
  G4double foil2_thick;
//  G4Material* solid_foil2 = elA;//None
  G4Material* solid_foil2 = nist->FindOrBuildMaterial("G4_Al"); foil2_thick = 0.0125;//mm
//  G4Material* solid_foil2 = nist->FindOrBuildMaterial("G4_KAPTON"); foil2_thick = 0.05;//mm
  G4VSolid* foil2_tubs = new G4Tubs("FoilTubs2",0*mm,((150-covor_thick)/2)*mm,(foil2_thick/2)*mm,0.,2*M_PI*rad);
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
  G4VSolid* inter_h_tubs = new G4Tubs("intermediate2",0*mm,((150-covor_thick)/2)*mm,(inter_h_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_inter_h = G4ThreeVector(0, 0, (foil2_thick+inter_air_thick+foil1_thick+inter_h_thick/2)*mm);//check here for photon beam position (270.113 mm)
  G4LogicalVolume* INTERLog2 = new G4LogicalVolume(inter_h_tubs, solid_He, "intermediate2");  
  new G4PVPlacement(0, pos_inter_h, INTERLog2, "intermediate2", logicWorld, false, 0,  checkOverlaps);        

  //
  // ***** Chamber *****
  //
  G4Material* solid_chamber = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");//SUS304
  G4Material* solid_chamber_cover = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* solid_chamber_window = nist->FindOrBuildMaterial("G4_Be");

  G4VSolid* chamber_tubs = new G4Tubs("Chamber_ori",(150/2)*mm,(165.2/2)*mm,(inter_h_thick/2)*mm,0.,2*M_PI*rad);
  G4VSolid* chamber_cover = new G4Tubs("Cover_ori",((150-covor_thick)/2)*mm,(150/2)*mm,(inter_h_thick/2)*mm,0.,2*M_PI*rad);
  G4VSolid* Chamber_Hole = new G4Tubs("ChamberHole",0*mm,(25/2)*mm,(20/2)*mm,0.,2*M_PI*rad);
  G4VSolid* Chamber_Window = new G4Tubs("BeWindow",0*mm,(23/2)*mm,(1./2)*mm,0.,2*M_PI*rad);
  G4double hole_dis=75+(15.2/2); 
  G4double current_angle_hole;
  G4ThreeVector pos_hole;
  G4ThreeVector pos_window_chamber;
  G4RotationMatrix* rot_hole;
  G4LogicalVolume* BeWindowLog;
  G4int nHoles = 6;
  for(int i=0; i<nHoles;i++){
     current_angle_hole=(i*(2*CLHEP::pi/nHoles)+(CLHEP::pi/6))*CLHEP::rad;
     pos_hole = G4ThreeVector(hole_dis*std::sin(current_angle_hole)*mm, hole_dis*std::cos(current_angle_hole)*mm, (5)*mm);
     rot_hole = new G4RotationMatrix((-i*(360./nHoles)-30)*CLHEP::deg,-90*CLHEP::deg,0*CLHEP::deg);
     chamber_tubs = new G4SubtractionSolid("Chamber", chamber_tubs, Chamber_Hole, rot_hole, pos_hole);
     chamber_cover = new G4SubtractionSolid("Cover", chamber_cover, Chamber_Hole, rot_hole, pos_hole);

     pos_window_chamber = G4ThreeVector((hole_dis-8/2.)*std::sin(current_angle_hole)*mm, (hole_dis-8/2.)*std::cos(current_angle_hole)*mm, ((foil2_thick+inter_air_thick+foil1_thick+inter_h_thick/2)+5)*mm);
     BeWindowLog = new G4LogicalVolume(Chamber_Window, solid_chamber_window, "BeWindowTubs");
     new G4PVPlacement(rot_hole, pos_window_chamber, BeWindowLog, "BeWindowTubs", logicWorld, false, 0, checkOverlaps);
  }
  G4LogicalVolume* ChamberLog = new G4LogicalVolume(chamber_tubs, solid_chamber, "Chamber");  
  G4LogicalVolume* CoverLog = new G4LogicalVolume(chamber_cover, solid_chamber_cover, "Cover");  
  new G4PVPlacement(0, pos_inter_h, ChamberLog, "Chamber", logicWorld, false, 0,  checkOverlaps);        
  new G4PVPlacement(0, pos_inter_h, CoverLog, "Cover", logicWorld, false, 0,  checkOverlaps);        

  //
  // ***** Virtual (in intermediate He) *****
  //
  G4double vir_thick = 0.010;//mm
  G4VSolid* Vir_tubs = new G4Tubs("VirTubs",0*mm,((150-covor_thick)/2)*mm,(vir_thick/2)*mm,0.,2*M_PI*rad);
  G4ThreeVector pos_vir = G4ThreeVector(0, 0, (-190)*mm);
  G4LogicalVolume* VirLog = new G4LogicalVolume(Vir_tubs, solid_He, "VirTubs");  
  new G4PVPlacement(0, pos_vir, VirLog, "VirTubs", INTERLog2, false, 0,  checkOverlaps);        

  //
  // ***** Sample *****
  //
  G4double sample_thick;
  G4double sample_width;
  G4Material* solid_sample;
  G4LogicalVolume* SampleLog;

  if(SampleName=="Al"){
     solid_sample = nist->FindOrBuildMaterial("G4_Al"); sample_width=25; sample_thick=1.2; //25*25*1.2 mm (change solid_foil2/3 to Kapton)
  }else if(SampleName=="Fe"){
     solid_sample = nist->FindOrBuildMaterial("G4_Fe"); sample_width=25; sample_thick=0.5;//25*25*0.5 mm (change solid_foil2/3 to Kapton)
  }else if(SampleName=="Ti"){
     solid_sample = nist->FindOrBuildMaterial("G4_Ti"); sample_width=25; sample_thick=1.0;//25*25*1.0 mm (change solid_foil2/3 to Kapton)
  }else if(SampleName=="Cu"){
     solid_sample = nist->FindOrBuildMaterial("G4_Cu"); sample_width=25; sample_thick=0.3;//25*25*0.3 mm 
  }else if(SampleName=="S"){
     solid_sample = nist->FindOrBuildMaterial("G4_S"); sample_width=23; sample_thick=0.98;//phi13*0.85 mm
  }else if(SampleName=="C"){
     solid_sample = nist->FindOrBuildMaterial("G4_C"); sample_width=23; sample_thick=1;//phi13*1 mm
  }else if(SampleName=="SiO2"){
     solid_sample = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"); sample_width=23; sample_thick=2;
  }else if(SampleName=="CaO"){
     solid_sample = nist->FindOrBuildMaterial("G4_CALCIUM_OXIDE"); sample_width=23 ; sample_thick=0.6; 
  }else if(SampleName=="Ni"){
     solid_sample = nist->FindOrBuildMaterial("G4_Ni"); sample_width=25; sample_thick=0.2;
  }else if(SampleName=="Mg"){
     solid_sample = nist->FindOrBuildMaterial("G4_Mg"); sample_width=25; sample_thick=0.76;
  }else if(SampleName=="Ryugu"){
     double ryugu_density = 2.0*g/cm3;
     int ncomponents;
     int natoms;
     solid_sample = new G4Material("Inseki", ryugu_density, ncomponents=4);
     solid_sample->AddElement(elO, natoms=45);
     solid_sample->AddElement(elMg, natoms=15);
     solid_sample->AddElement(elSi, natoms=20);
     solid_sample->AddElement(elFe, natoms=20);
  }
  if(SampleName!="RI" && SampleName!="Ryugu"){
     G4Box* solidsample = new G4Box("Sample", (sample_width/2.)*mm, (sample_width/2.)*mm, (sample_thick/2.)*mm);
     SampleLog = new G4LogicalVolume(solidsample, solid_sample, "Sample");
  }else if(SampleName=="Ryugu"){
     G4Box* solidsample = new G4Box("Target",0.5*8.65*mm, 0.5*5.12*mm, 0.5*3.28*mm);
     SampleLog = new G4LogicalVolume(solidsample, solid_sample, "Sample");
  }else if(SampleName=="RI"){
     // === RI source ===
     G4double isotope_plastic_radius = 12.5*mm;
     sample_thick = 6.0*mm;
     solid_sample = nist->FindOrBuildMaterial("G4_PLEXIGLASS"); // acrylic resin C5O2H8 density 1.18 g/cm3
     G4Tubs* solidsample = new G4Tubs("Sample", 0.0, isotope_plastic_radius, sample_thick*0.5, 0.0*deg, 360.0*deg);
     SampleLog = new G4LogicalVolume(solidsample, solid_sample, "Sample");
  }

  G4RotationMatrix* rot_sample = new G4RotationMatrix(90*CLHEP::deg,-20*CLHEP::deg,90*CLHEP::deg);
  G4ThreeVector pos_sample = G4ThreeVector(0, 0, (5)*mm);//center of sample is (270.113 or 270.15 + 5 mm for Al or Kapton foil)
  new G4PVPlacement(rot_sample,  pos_sample, SampleLog, "Sample", INTERLog2, false, 0, checkOverlaps);

  //
  // ***** Foil-3 (Cu cover) *****
  //
  G4Material* solid_foil3 = nist->FindOrBuildMaterial("G4_Cu");
//  G4Material* solid_foil3 = nist->FindOrBuildMaterial("G4_KAPTON");
  G4double foil3_thick = 0.005;//mm
//  G4Box* foil3_tubs = new G4Box("FoilTubs3", (138.4/2)*mm, (10/2)*mm, (foil3_thick/2)*mm);
  G4Box* foil3_tubs = new G4Box("FoilTubs3", ((138.4-10)/2)*mm, (20/2)*mm, (foil3_thick/2)*mm);
  G4ThreeVector pos_foil3_1 = G4ThreeVector(0, 0, 5+(foil3_thick/2+sample_thick/2)/std::cos((40/360.)*2*CLHEP::pi)*mm);//check beam position
  G4ThreeVector pos_foil3_2 = G4ThreeVector(0, 0, 5-(foil3_thick/2+sample_thick/2)/std::cos((40/360.)*2*CLHEP::pi)*mm);//check beam position
  G4LogicalVolume* FoilLog3 = new G4LogicalVolume(foil3_tubs, solid_foil3, "FoilTubs3");  
  new G4PVPlacement(rot_sample, pos_foil3_1, FoilLog3, "FoilTubs3", INTERLog2, false, 0,  checkOverlaps);        
  new G4PVPlacement(rot_sample, pos_foil3_2, FoilLog3, "FoilTubs3", INTERLog2, false, 0,  checkOverlaps);        

  //
  // ***** Holder *****
  //
  G4double holder_thick = 3;//mm
  G4Material* solid_holder = nist->FindOrBuildMaterial("G4_Cu");
//  G4Material* solid_holder = nist->FindOrBuildMaterial("G4_Fe");
//  G4VSolid* holder_tubs = new G4Tubs("Holder",(148.8/2-6.7)*mm,(148.8/2)*mm,(holder_thick/2)*mm,(0)*rad,(2*M_PI)-2*(0.5*M_PI-0.3761)*rad);
//  G4ThreeVector pos_holder = G4ThreeVector(0, -(148.8/2*std::sin(0.3761)), (5)*mm);  
  G4VSolid* holder_tubs = new G4Tubs("Holder",(138.8/2-6.7)*mm,(138.8/2)*mm,(holder_thick/2)*mm,(-0.3761)*rad,(M_PI+0.3761*2)*rad);
  G4ThreeVector pos_holder = G4ThreeVector(0, 0, (5)*mm);  
  G4LogicalVolume* HolderLog = new G4LogicalVolume(holder_tubs, solid_holder, "Holder");          
  new G4PVPlacement(rot_sample,  pos_holder, HolderLog, "Holder", INTERLog2, false, 0, checkOverlaps);

  //
  // ***** Ge detector *****
  //
  // common
  G4double Ge_thickness[6] = {10,9,10,10,10,10.3};
  G4double Ge_diameter[6] = {11.3,11.5,11.2,11.3,11.0,11.4};
  G4double groove_inner[6] = {3,3,3,3,2.5,3};//CH1,3,4,6 are unknown
  G4double groove_outer[6] = {5.5,5.5,5.5,5.5,5.5,5.5};//CH1,3,4,6 are unknown
  G4double groove_thickness[6] = {1.5,1.5,1.5,1.5,1,1.5};
  G4double deadlayer_thickness[6] = {0.5,0.5,0.5,0.5,0.5,0.5};
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
  G4double window_sample_dis=165.2/2+3;;//dis. of the sample center from the Be window surface
  G4double ge_dis_Z = (foil2_thick+inter_air_thick+foil1_thick+inter_h_thick/2+vir_thick+1.5+foil3_thick+0.2+5);//mm; pos. the sample on the z axis
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
  G4RotationMatrix* rot_ge;
  G4RotationMatrix* rot_deadlayer;
  G4ThreeVector pos_deadlayer;
  G4ThreeVector pos_ge;
  G4ThreeVector pos_endcap;
  G4ThreeVector pos_sncover;
  G4ThreeVector pos_window;
  
  for(int i=0; i<nDets;i++){
     auto idstr = std::to_string(i+1);
     solid_cryostat = nist->FindOrBuildMaterial("G4_Ge");
     solid_cryostatholder = nist->FindOrBuildMaterial(cryostat_material[i]);
     solid_endcap = nist->FindOrBuildMaterial(endcap_material[i]);
     solid_cover = nist->FindOrBuildMaterial("G4_Sn");
     solid_window = nist->FindOrBuildMaterial("G4_Be");
     Ge_Det_ori = new G4Tubs("GeDet_ori",0*mm,((Ge_diameter[i]/2.)-deadlayer_thickness[i])*mm,((Ge_thickness[i]-deadlayer_thickness[i])/2.)*mm,0.,2*M_PI*rad);//thinkness of deadlayer is 0.5 mm
     Ge_Det_groove = new G4Tubs("GeDet_groove",(groove_inner[i]/2.)*mm,(groove_outer[i]/2.)*mm,((groove_thickness[i]-deadlayer_thickness[i])/2.)*mm,0.,2*M_PI*rad); 
     Ge_Det_ori = new G4SubtractionSolid("GeDet", Ge_Det_ori, Ge_Det_groove, 0, G4ThreeVector(0*mm, 0*mm, -((Ge_thickness[i]-deadlayer_thickness[i])/2-(groove_thickness[i]-deadlayer_thickness[i])/2.)*mm));
     Ge_Det_dead = new G4Tubs("Ge_Det_dead_ori",0*mm,(Ge_diameter[i]/2.)*mm,(Ge_thickness[i]/2.)*mm,0.,2*M_PI*rad);
     Ge_Det_dead_sub = new G4Tubs("Ge_Det_dead_sub",0*mm,((Ge_diameter[i]-deadlayer_thickness[i]*2)/2.)*mm,((Ge_thickness[i]-deadlayer_thickness[i])/2.)*mm,0.,2*M_PI*rad);
     Ge_Det_groove = new G4Tubs("GeDet_groove",(groove_inner[i]/2.)*mm,(groove_outer[i]/2.)*mm,((deadlayer_thickness[i]/2.)*10)*mm,0.,2*M_PI*rad);//tips: 10 times for subtraction
     Ge_Det_dead = new G4SubtractionSolid("Ge_Det_dead", Ge_Det_dead, Ge_Det_dead_sub, 0, G4ThreeVector(0*mm, 0*mm, (deadlayer_thickness[i]/2.)*mm));
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
     det_angle=(i*(2*CLHEP::pi/nDets)+(CLHEP::pi/6))*CLHEP::rad;
     rot_ge = new G4RotationMatrix((-i*(360./nDets)-30+180)*CLHEP::deg,-90*CLHEP::deg,0*CLHEP::deg);
     rot_deadlayer = new G4RotationMatrix((-i*(360./nDets)-30+180)*CLHEP::deg,-90*CLHEP::deg,0*CLHEP::deg);
     pos_deadlayer=G4ThreeVector((window_sample_dis+window_thickness[i]/2.+5+Ge_thickness[i]/2.)*std::sin(det_angle)*mm, (window_sample_dis+window_thickness[i]/2.+5+Ge_thickness[i]/2.)*std::cos(det_angle)*mm, (ge_dis_Z)*mm);
     pos_ge=G4ThreeVector((window_sample_dis+window_thickness[i]/2.+5+Ge_thickness[i]/2.-deadlayer_thickness[i])*std::sin(det_angle)*mm, (window_sample_dis+window_thickness[i]/2.+5+Ge_thickness[i]/2.-deadlayer_thickness[i])*std::cos(det_angle)*mm, (ge_dis_Z)*mm);
     pos_sncover=G4ThreeVector((window_sample_dis+window_thickness[i]/2.+cover_lengh[i]/2.)*std::sin(det_angle)*mm, (window_sample_dis+window_thickness[i]/2.+cover_lengh[i]/2.)*std::cos(det_angle)*mm, (ge_dis_Z)*mm);
     pos_endcap=G4ThreeVector((window_sample_dis+window_thickness[i]/2.+endcap_lengh[i]/2.)*std::sin(det_angle)*mm, (window_sample_dis+window_thickness[i]/2.+endcap_lengh[i]/2.)*std::cos(det_angle)*mm, (ge_dis_Z)*mm);
     pos_window=G4ThreeVector(window_sample_dis*std::sin(det_angle)*mm, window_sample_dis*std::cos(det_angle)*mm, (ge_dis_Z)*mm);
     new G4PVPlacement(rot_deadlayer, pos_deadlayer, DeadlayerLog[i], "Deadlayer"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(rot_ge, pos_deadlayer, CrystalHolderLog[i], "Crystal_Holder"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(rot_ge, pos_ge, GeLog[i], "Ge"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(rot_ge, pos_endcap, EndcapLog[i], "Endcap"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(rot_ge, pos_sncover, SnCoverLog[i], "SnCover"+idstr, logicWorld, false, 0, checkOverlaps);
     new G4PVPlacement(rot_ge, pos_window, WindowLog[i], "Window"+idstr, logicWorld, false, 0, checkOverlaps);
  } 

  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
