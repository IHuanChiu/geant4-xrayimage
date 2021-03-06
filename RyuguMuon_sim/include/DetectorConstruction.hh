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

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class Messenger;
/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    static DetectorConstruction* GetDetInstance();
    virtual G4VPhysicalVolume* Construct();

    G4String SampleName;
    G4String BeamType;
    
  private:
    static DetectorConstruction* pointerToDet;
    Messenger* myMessenger; 
    //old collimator
//    G4double shape_depth_1 = 20;
//    G4double shape_depth_2 = 20;
//    G4double shape_depth_3 = 30;
//    G4double shape_depth_4 = 30;
//    G4double gap_dis = 10;
//    G4double thick   = 15;
//    G4double step    = 10;//step must smaller than thick
//    G4double duplicate = 5;
//    G4double kapton_radiu = 35;
//    G4double kapton_thick = 0.1;
//    G4double dis = 50;

//    G4double det_dx = 30; 
//    G4double det_dy = 2;
//    G4double det_dz = 30;
//    G4double det_dis = 30;
//    G4double angle = 45;

//    G4double shelf_dx = 30;
//    G4double shelf_dy = 30;
//    G4double shelf_dz = 5;
//
//    G4double sample_dis = 30;    
//    G4double sample_dx = 4.5;
//    G4double sample_dy = 6;
//    G4double sample_dz = 1;
//    G4double sample_angle = 45;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

