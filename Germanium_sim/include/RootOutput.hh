#ifndef RootOutput_h
#define RootOutput_h 1
//#include "G4UserRunAction.hh"
#include <CLHEP/Units/PhysicalConstants.h>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "EventAction.hh"

//  ROOT
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TVectorD.h"
//
#include <map>

#include "G4ios.hh"
#include <fstream>
#include <vector>
#include <cmath>

class RootOutput{
  public:
    RootOutput();
    ~RootOutput();
    static RootOutput* GetRootInstance();

    void BeginOfRunAction();
    void EndOfRunAction();
    void FillEvent();
    void FillParticle();
    void ClearAllRootVariables();

    void SetRunID          (G4int id) {runID = id;};
    void SetEventID        (G4int id) {eventID = id;};
    void StoreTrack(G4int det_index, G4int pdgid, G4double energy_kin,  G4double energy_total, G4double energy_dep, G4double x, G4double y, G4double z, G4String tr_name, G4String tr_process) { 
          pdgId = pdgid; 
          TotalE = energy_total; 
          KineticE = energy_kin; 
          DepositE = energy_dep; 
          Det_X = x; 
          Det_Y = y; 
          Det_Z = z; 
          Track_Name  = tr_name;
          Track_Process  = tr_process;
    }

    static const int numberOfvolume = 10; 

    //-100 is for escape muon; -1000 is not hit muon
    void SetInitPolInSample(G4ThreeVector pol){muSamplePolX=pol.x(); muSamplePolY=pol.y(); muSamplePolZ=pol.z();}
    void SetInitPolInCdTe(G4int index, G4ThreeVector pol){muCdTePolX[index]=pol.x(); muCdTePolY[index]=pol.y(); muCdTePolZ[index]=pol.z();}
    void SetInitPolInTarget(G4ThreeVector pol){muTargetPolX=pol.x(); muTargetPolY=pol.y(); muTargetPolZ=pol.z();}
    void SetInitPolInKapton(G4ThreeVector pol){muKaptonPolX=pol.x(); muKaptonPolY=pol.y(); muKaptonPolZ=pol.z();}
    void SetInitPolInWorld(G4ThreeVector pol){muWorldPolX=pol.x(); muWorldPolY=pol.y(); muWorldPolZ=pol.z();}
    void SetInitMomInSample(G4ThreeVector pol){muSampleMomX=pol.x(); muSampleMomY=pol.y(); muSampleMomZ=pol.z();}
    void SetInitMomInCdTe(G4int index, G4ThreeVector pol){muCdTeMomX[index]=pol.x(); muCdTeMomY[index]=pol.y(); muCdTeMomZ[index]=pol.z();}
    void SetInitMomInTarget(G4ThreeVector pol){muTargetMomX=pol.x(); muTargetMomY=pol.y(); muTargetMomZ=pol.z();}
    void SetInitMomInKapton(G4ThreeVector pol){muKaptonMomX=pol.x(); muKaptonMomY=pol.y(); muKaptonMomZ=pol.z();}
    void SetInitMomInWorld(G4ThreeVector pol){muWorldMomX=pol.x(); muWorldMomY=pol.y(); muWorldMomZ=pol.z();}
    void SetInitTimeInSample(G4double time){muSampleTime = time;}
    void SetInitTimeInCdTe(G4int index, G4double time){muCdTeTime[index] = time;}
    void SetInitTimeInTarget(G4double time){muTargetTime = time;}
    void SetInitTimeInKapton(G4double time){muKaptonTime = time;}
    void SetInitTimeInWorld(G4double time){muWorldTime = time;}
    void SetInitKineticEnergyInSample(G4double energy){muSampleKineticEnergy = energy;}
    void SetInitKineticEnergyInCdTe(G4int index, G4double energy){muCdTeKineticEnergy[index] = energy;}
    void SetInitKineticEnergyInTarget(G4double energy){muTargetKineticEnergy = energy;}
    void SetInitKineticEnergyInKapton(G4double energy){muKaptonKineticEnergy = energy;}
    void SetInitKineticEnergyInWorld(G4double energy){muWorldKineticEnergy = energy;}
    void SetEndPolInSample(G4ThreeVector pol){muSampleEndPolX=pol.x(); muSampleEndPolY=pol.y(); muSampleEndPolZ=pol.z();}
    void SetEndPolInCdTe(G4int index, G4ThreeVector pol){muCdTeEndPolX[index]=pol.x(); muCdTeEndPolY[index]=pol.y(); muCdTeEndPolZ[index]=pol.z();}
    void SetEndPolInTarget(G4ThreeVector pol){muTargetEndPolX=pol.x(); muTargetEndPolY=pol.y(); muTargetEndPolZ=pol.z();}
    void SetEndPolInKapton(G4ThreeVector pol){muKaptonEndPolX=pol.x(); muKaptonEndPolY=pol.y(); muKaptonEndPolZ=pol.z();}
    void SetEndPolInWorld(G4ThreeVector pol){muWorldEndPolX=pol.x(); muWorldEndPolY=pol.y(); muWorldEndPolZ=pol.z();}    
    void SetEndMomInSample(G4ThreeVector pol){muSampleEndMomX=pol.x(); muSampleEndMomY=pol.y(); muSampleEndMomZ=pol.z();}
    void SetEndMomInCdTe(G4int index, G4ThreeVector pol){muCdTeEndMomX[index]=pol.x(); muCdTeEndMomY[index]=pol.y(); muCdTeEndMomZ[index]=pol.z();}
    void SetEndMomInTarget(G4ThreeVector pol){muTargetEndMomX=pol.x(); muTargetEndMomY=pol.y(); muTargetEndMomZ=pol.z();}
    void SetEndMomInKapton(G4ThreeVector pol){muKaptonEndMomX=pol.x(); muKaptonEndMomY=pol.y(); muKaptonEndMomZ=pol.z();}
    void SetEndMomInWorld(G4ThreeVector pol){muWorldEndMomX=pol.x(); muWorldEndMomY=pol.y(); muWorldEndMomZ=pol.z();}    
    void SetEndTimeInSample(G4double time){muSampleEndTime = time;}
    void SetEndTimeInCdTe(G4int index, G4double time){muCdTeEndTime[index] = time;}
    void SetEndTimeInTarget(G4double time){muTargetEndTime = time;}
    void SetEndTimeInKapton(G4double time){muKaptonEndTime = time;}
    void SetEndTimeInWorld(G4double time){muWorldEndTime = time;}
    void SetEndKineticEnergyInSample(G4double energy){muSampleEndKineticEnergy = energy;}
    void SetEndKineticEnergyInCdTe(G4int index, G4double energy){muCdTeEndKineticEnergy[index] = energy;}
    void SetEndKineticEnergyInTarget(G4double energy){muTargetEndKineticEnergy = energy;}
    void SetEndKineticEnergyInKapton(G4double energy){muKaptonEndKineticEnergy = energy;}
    void SetEndKineticEnergyInWorld(G4double energy){muWorldEndKineticEnergy = energy;}
    void SetInitialMuonParameters(G4double x,G4double y,G4double z, G4double px,G4double py,G4double pz,G4double time){
         muInitX = x;muInitY = y;muInitZ = z;muInitpX = px;muInitpY = py;muInitpZ = pz; muInitTime = time;}
    void SetInitialEletronParameters(G4double x,G4double y,G4double z, G4double px,G4double py,G4double pz){
         eInitX = x;eInitY = y;eInitZ = z;eInitpX = px;eInitpY = py;eInitpZ = pz;}

    void SetDecayPolGlo(G4ThreeVector pol){muDecayPolX=pol.x(); muDecayPolY=pol.y(); muDecayPolZ=pol.z();}    
    void SetDecayTimeGlo(G4double time){muDecayTime = time;}    

    void SetmuFinalVolume(G4int fpol){Stop_Volume = fpol;}
    void SetnParticleHitVolume(G4int ivolume, G4int ngamma, G4int neletron, G4int nneutron, G4int nother){
                               Ngamma = ngamma; Neletron = neletron; Nneutron = nneutron; Nother = nother;}

    TFile* rootFile;
    TTree* rootTree;
    TTree* TrackTree;

  // === variables for tree ===
  private:
    static RootOutput* pointerToRoot;

    Double_t muSampleTime, muCdTeTime[6], muWorldTime, muKaptonTime, muTargetTime;
    Double_t muSampleKineticEnergy, muCdTeKineticEnergy[6], muWorldKineticEnergy, muKaptonKineticEnergy, muTargetKineticEnergy;
    Double_t muSamplePolX, muSamplePolY, muSamplePolZ;
    Double_t muCdTePolX[6], muCdTePolY[6], muCdTePolZ[6];
    Double_t muTargetPolX, muTargetPolY, muTargetPolZ;
    Double_t muKaptonPolX, muKaptonPolY, muKaptonPolZ;
    Double_t muWorldPolX, muWorldPolY, muWorldPolZ;
    Double_t muSampleMomX, muSampleMomY, muSampleMomZ;
    Double_t muCdTeMomX[6], muCdTeMomY[6], muCdTeMomZ[6];
    Double_t muTargetMomX, muTargetMomY, muTargetMomZ;
    Double_t muKaptonMomX, muKaptonMomY, muKaptonMomZ;
    Double_t muWorldMomX, muWorldMomY, muWorldMomZ;
    Double_t muSampleEndTime, muCdTeEndTime[6], muWorldEndTime, muKaptonEndTime, muTargetEndTime;
    Double_t muSampleEndKineticEnergy, muCdTeEndKineticEnergy[6], muWorldEndKineticEnergy, muKaptonEndKineticEnergy, muTargetEndKineticEnergy;
    Double_t muSampleEndPolX, muSampleEndPolY, muSampleEndPolZ;
    Double_t muCdTeEndPolX[6], muCdTeEndPolY[6], muCdTeEndPolZ[6];
    Double_t muKaptonEndPolX, muKaptonEndPolY, muKaptonEndPolZ;
    Double_t muWorldEndPolX, muWorldEndPolY, muWorldEndPolZ;
    Double_t muTargetEndPolX, muTargetEndPolY, muTargetEndPolZ;
    Double_t muSampleEndMomX, muSampleEndMomY, muSampleEndMomZ;
    Double_t muCdTeEndMomX[6], muCdTeEndMomY[6], muCdTeEndMomZ[6];
    Double_t muKaptonEndMomX, muKaptonEndMomY, muKaptonEndMomZ;
    Double_t muWorldEndMomX, muWorldEndMomY, muWorldEndMomZ;
    Double_t muTargetEndMomX, muTargetEndMomY, muTargetEndMomZ;
    Double_t muDecayPolX, muDecayPolY, muDecayPolZ, muDecayTime;
    Double_t muInitX ; Double_t muInitY ;Double_t muInitZ ;Double_t muInitpX ;Double_t muInitpY ;Double_t muInitpZ ; Double_t muInitTime;
    Double_t eInitX ; Double_t eInitY ;Double_t eInitZ ;Double_t eInitpX ;Double_t eInitpY ;Double_t eInitpZ ;  

    Int_t Det_nMax;
    Double_t eDep;
    Double_t eDep_e;
    Double_t eDep_gamma;
    Double_t eDep_other;
    Double_t egamma_hit_time;
    std::string Track_Name;
    std::string Track_Process;

    Int_t Hit_Volume;
    Int_t Stop_Volume;
    Double_t KineticE;
    Double_t TotalE;
    Double_t DepositE;
    Int_t    pdgId;
    Double_t Det_X;
    Double_t Det_Y;
    Double_t Det_Z;
    Double_t Hit_Time;

    Int_t Ngamma; 
    Int_t Neletron; 
    Int_t Nneutron;
    Int_t Nother;

    Int_t runID;
    Int_t eventID;
    Double_t RunTime;
    char RootOutputFileName[200];

    Int_t nSignals;
    Int_t hit_id;
    Double_t hit_energy[100];
    Double_t hit_timestart[100];
    Double_t hit_timeend[100];
    Int_t hit_nsteps[100];
    Double_t hit_length[100];
    Int_t hit_pdgId[100];

   // === public class to catch info. === 
  public:
    void SetDetectorInfo (G4double edep, G4double edep_e, G4double edep_gamma, G4double edep_other, G4double time);
    void SetnMaxHit (G4int nhits){nSignals = nhits;}
    void SetRunTime (G4double time) {RunTime = time;}
    void SetSignalInfo (G4int id, G4double energy, G4double time_start, G4double time_end, G4int nsteps, G4double length, G4int pdgId){
       hit_energy[id] = energy; 
       hit_timestart[id] = time_start; 
       hit_timeend[id] = time_end; 
       hit_nsteps[id] = nsteps; 
       hit_length[id] = length; 
       hit_pdgId[id] = pdgId;
    }
    
                                                                                                                     
};

#endif
