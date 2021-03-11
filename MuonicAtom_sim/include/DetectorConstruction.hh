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
/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    G4LogicalVolume* GetSampleVolume() const { return fSampleVolume; }

    G4int nDets;
    G4double sample_dis = 30;//20 is experiment setting 
    G4double sample_dx = 4.5;
    G4double sample_dy = 6;
    G4double sample_dz = 1;
    G4double sample_angle = 45;

  protected:
    G4LogicalVolume*  fScoringVolume;
    G4LogicalVolume*  fScoringVolumeUp;
    G4LogicalVolume*  fSampleVolume;

  private:
    //old collimator
//    G4double shape_depth_1 = 20;
//    G4double shape_depth_2 = 20;
//    G4double shape_depth_3 = 30;
//    G4double shape_depth_4 = 30;
//    G4double gap_dis = 10;
//    G4double thick   = 15;
//    G4double step    = 10;//step must smaller than thick
//    G4double duplicate = 5;
    G4double kapton_radiu = 35;
    G4double kapton_thick = 0.1;

    G4double det_dx = 32; 
    G4double det_dy = 0.75;
    G4double det_dz = 32;
    G4double det_dis = 74;// dis between colli. & det
    G4double colli_dis = 109;//dis between cdte colli. & sample
    G4double colli_dis_si = 70;//dis between si colli. & sample
    G4double colli_radiu = 37;//collimator size
    G4double angle = 45;
    G4double cone_size = 3;

    G4double shadow_front = 15;
    G4double shadow_depth = 50;
    G4double shadow_thick = 2;
    G4double shadow_inner_front = 10;
    G4double shadow_outer_front = 20;
//    G4double shadow_inner_back = 5;
//    G4double shadow_outer_back = 6;
    G4double shadow_inner_back = 33;
    G4double shadow_outer_back = 43;
    G4double shadow_angle = 360.;
    G4double zshield_heigh = 25.;

    G4double shelf_dx = 30;
    G4double shelf_dy = 30;
    G4double shelf_dz = 5;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

