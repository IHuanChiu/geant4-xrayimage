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
    
  private:
    G4double kapton_radius = 139.8/2;
    G4double kapton_thick = 0.1;

    G4double tunnel_inner_radius=370/2.;
    G4double tunnel_outer_radius=520/2.;
    G4double tunnel_thick=67;
    G4double tunnel_dis=83;

    G4double Ge_radius = 50/2.;
    G4double Ge_thick = 50.;
    G4double Ge_dis = 1000.;

    G4double cover_radius = 40;//mm
    G4double cover_thick = 10;//mm
    G4double cover_lengh = 10;//mm
    G4double cover_dis = 60.;    
    G4double cu_cover_thick = 0.03;

    G4double Stand_x = 95;
    G4double Stand_y = 95;
    G4double Stand_z = 5;

    G4double Stand_line_x = 80;
    G4double Stand_line_y = 10;
    G4double Stand_line_z = 0.01;

    G4double fractionmass;
    G4int ncomponents;
    G4String name;
  public:
    G4double sample_radius_SiO2 = 0.564189;//1 mm^2
    G4double sample_thick_SiO2 = 1.;//1 mm
    G4double sample_radius_C = 0.021049640;
    G4double sample_thick_C = 0.25;
    G4double ryugu_x = 7;
    G4double ryugu_y = 2;
    G4double ryugu_z = 0.01;
    G4double sample_center = 0;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

