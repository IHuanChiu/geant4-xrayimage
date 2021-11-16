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

#ifndef RunAction_h
#define RunAction_h 1

#include <CLHEP/Units/PhysicalConstants.h>
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "RootOutput.hh"
#include "DetectorConstruction.hh"

class G4Run;
class PrimaryGeneratorAction;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class RunAction : public G4UserRunAction
{
  public:
    RunAction(PrimaryGeneratorAction* kin);
    virtual ~RunAction();

    //virtual G4Run* GenerateRun();
    void BeginOfRunAction(const G4Run*);
    void   EndOfRunAction(const G4Run*);

    void AddEdep (G4double edep);
//    #ifdef G4ANALYSIS_USE_ROOT

  private:
    PrimaryGeneratorAction* fPrimary;
    G4Accumulable<G4double> fEdep;
    G4Accumulable<G4double> fEdep2;
    DetectorConstruction* myDetpointer;
    RootOutput* myRootOutput;

};

#endif

