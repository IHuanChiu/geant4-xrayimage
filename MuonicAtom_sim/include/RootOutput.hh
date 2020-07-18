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
    void RootEndOfRunAction();
    void FillEvent();
    void FillParticle();
    void ClearAllRootVariables();

    void SetRunID          (G4int id) {runID = id;};
    void SetEventID        (G4int id) {eventID = id;};
    void StoreTrack(G4int det_index, G4int pdgid, G4double energy_kin,  G4double energy_total, G4double x, G4double y, G4double z,  G4String tr_name, G4String tr_process, G4int tr_process_id) {
          det_ID = det_index;
          pdgId = pdgid; 
          TotalE = energy_total; 
          KineticE = energy_kin; 
          Det_X = x; 
          Det_Y = y; 
          Det_Z = z; 
          Track_Name  = tr_name;
          Track_Process  = tr_process;
          Track_ProcessID = tr_process_id;
    }

    static const int numberOfvolume = 6; 
    static const int nh1bin = 10;
    static const int nhitMax = 100;

    //-100 is for escape muon; -1000 is not hit muon
    void SetInitPolInSample(G4ThreeVector pol){muSamplePolX=pol.x(); muSamplePolY=pol.y(); muSamplePolZ=pol.z();}
    void SetInitPolInCdTe(G4int index, G4ThreeVector pol){muCdTePolX[index]=pol.x(); muCdTePolY[index]=pol.y(); muCdTePolZ[index]=pol.z();}
    void SetInitPolInCollimator(G4ThreeVector pol){muCollimatorPolX=pol.x(); muCollimatorPolY=pol.y(); muCollimatorPolZ=pol.z();}
    void SetInitPolInShadow(G4ThreeVector pol){muShadowPolX=pol.x(); muShadowPolY=pol.y(); muShadowPolZ=pol.z();}
    void SetInitPolInKapton(G4ThreeVector pol){muKaptonPolX=pol.x(); muKaptonPolY=pol.y(); muKaptonPolZ=pol.z();}
    void SetInitPolInWorld(G4ThreeVector pol){muWorldPolX=pol.x(); muWorldPolY=pol.y(); muWorldPolZ=pol.z();}
    void SetInitTimeInSample(G4double time){muSampleTime = time;}
    void SetInitTimeInCdTe(G4int index, G4double time){muCdTeTime[index] = time;}
    void SetInitTimeInCollimator(G4double time){muCollimatorTime = time;}
    void SetInitTimeInShadow(G4double time){muShadowTime = time;}
    void SetInitTimeInKapton(G4double time){muKaptonTime = time;}
    void SetInitTimeInWorld(G4double time){muWorldTime = time;}
    void SetInitKineticEnergyInSample(G4double energy){muSampleKineticEnergy = energy;}
    void SetInitKineticEnergyInCdTe(G4int index, G4double energy){muCdTeKineticEnergy[index] = energy;}
    void SetInitKineticEnergyInCollimator(G4double energy){muCollimatorKineticEnergy = energy;}
    void SetInitKineticEnergyInShadow(G4double energy){muShadowKineticEnergy = energy;}
    void SetInitKineticEnergyInKapton(G4double energy){muKaptonKineticEnergy = energy;}
    void SetInitKineticEnergyInWorld(G4double energy){muWorldKineticEnergy = energy;}
    void SetEndPolInSample(G4ThreeVector pol){muSampleEndPolX=pol.x(); muSampleEndPolY=pol.y(); muSampleEndPolZ=pol.z();}
    void SetEndPolInCdTe(G4int index, G4ThreeVector pol){muCdTeEndPolX[index]=pol.x(); muCdTeEndPolY[index]=pol.y(); muCdTeEndPolZ[index]=pol.z();}
    void SetEndPolInCollimator(G4ThreeVector pol){muCollimatorEndPolX=pol.x(); muCollimatorEndPolY=pol.y(); muCollimatorEndPolZ=pol.z();}
    void SetEndPolInShadow(G4ThreeVector pol){muShadowEndPolX=pol.x(); muShadowEndPolY=pol.y(); muShadowEndPolZ=pol.z();}
    void SetEndPolInKapton(G4ThreeVector pol){muKaptonEndPolX=pol.x(); muKaptonEndPolY=pol.y(); muKaptonEndPolZ=pol.z();}
    void SetEndPolInWorld(G4ThreeVector pol){muWorldEndPolX=pol.x(); muWorldEndPolY=pol.y(); muWorldEndPolZ=pol.z();}    
    void SetEndTimeInSample(G4double time){muSampleEndTime = time;}
    void SetEndTimeInCdTe(G4int index, G4double time){muCdTeEndTime[index] = time;}
    void SetEndTimeInCollimator(G4double time){muCollimatorEndTime = time;}
    void SetEndTimeInShadow(G4double time){muShadowEndTime = time;}
    void SetEndTimeInKapton(G4double time){muKaptonEndTime = time;}
    void SetEndTimeInWorld(G4double time){muWorldEndTime = time;}
    void SetEndKineticEnergyInSample(G4double energy){muSampleEndKineticEnergy = energy;}
    void SetEndKineticEnergyInCdTe(G4int index, G4double energy){muCdTeEndKineticEnergy[index] = energy;}
    void SetEndKineticEnergyInCollimator(G4double energy){muCollimatorEndKineticEnergy = energy;}
    void SetEndKineticEnergyInShadow(G4double energy){muShadowEndKineticEnergy = energy;}
    void SetEndKineticEnergyInKapton(G4double energy){muKaptonEndKineticEnergy = energy;}
    void SetEndKineticEnergyInWorld(G4double energy){muWorldEndKineticEnergy = energy;}

    void SetmuFinalVolume(G4int fpol){Stop_Volume = fpol;}
    void ScannParticleHitVolume(G4int ivolume, G4String particlename);
    void SetInitialMuonParameters(G4double x,G4double y,G4double z, G4double px,G4double py,G4double pz,G4double time){
         muInitX = x;muInitY = y;muInitZ = z;muInitpX = px;muInitpY = py;muInitpZ = pz; muInitTime = time;}
    void SetInitialEletronParameters(G4double x,G4double y,G4double z, G4double px,G4double py,G4double pz){
         eInitX = x;eInitY = y;eInitZ = z;eInitpX = px;eInitpY = py;eInitpZ = pz;}
    void SetEnergyDepositInVolume(G4int id, G4String name, G4double energy);
    void SetParticlePositionInVolume(G4int id, G4double x, G4double y , G4double z);
    void SetEnergyResolution ();

    TFile* rootFile;
    TTree* rootTree;
    TTree* muonTree;
    TTree* TrackTree;
    TH1F*  h1_process;

  // === variables for tree ===
  private:
    static RootOutput* pointerToRoot;

    const char *var_name[nh1bin] = {"None","muMinusCaptureAtRest","phot","compt","eBrem",
                                    "neutronInelastic","muIoni","conv","None","None"};

    Double_t muSampleTime, muCdTeTime[6], muCollimatorTime, muShadowTime, muWorldTime, muKaptonTime;
    Double_t muSampleKineticEnergy, muCdTeKineticEnergy[6], muCollimatorKineticEnergy, muShadowKineticEnergy, muWorldKineticEnergy, muKaptonKineticEnergy;
    Double_t muSamplePolX, muSamplePolY, muSamplePolZ;
    Double_t muCdTePolX[6], muCdTePolY[6], muCdTePolZ[6];
    Double_t muCollimatorPolX, muCollimatorPolY, muCollimatorPolZ;
    Double_t muShadowPolX, muShadowPolY, muShadowPolZ;
    Double_t muKaptonPolX, muKaptonPolY, muKaptonPolZ;
    Double_t muWorldPolX, muWorldPolY, muWorldPolZ;
    Double_t muSampleEndTime, muCdTeEndTime[6], muCollimatorEndTime, muShadowEndTime,muWorldEndTime, muKaptonEndTime;
    Double_t muSampleEndKineticEnergy, muCdTeEndKineticEnergy[6], muCollimatorEndKineticEnergy, muShadowEndKineticEnergy,muWorldEndKineticEnergy, muKaptonEndKineticEnergy;
    Double_t muSampleEndPolX, muSampleEndPolY, muSampleEndPolZ;
    Double_t muCdTeEndPolX[6], muCdTeEndPolY[6], muCdTeEndPolZ[6];
    Double_t muCollimatorEndPolX, muCollimatorEndPolY, muCollimatorEndPolZ;
    Double_t muShadowEndPolX, muShadowEndPolY, muShadowEndPolZ;
    Double_t muKaptonEndPolX, muKaptonEndPolY, muKaptonEndPolZ;
    Double_t muWorldEndPolX, muWorldEndPolY, muWorldEndPolZ;
    Double_t muInitX ; Double_t muInitY ;Double_t muInitZ ;Double_t muInitpX ;Double_t muInitpY ;Double_t muInitpZ ; Double_t muInitTime;
    Double_t eInitX ; Double_t eInitY ;Double_t eInitZ ;Double_t eInitpX ;Double_t eInitpY ;Double_t eInitpZ ;  

    Int_t Det_nMax;
    Double_t Position_x[numberOfvolume];
    Double_t Position_y[numberOfvolume];
    Double_t Position_z[numberOfvolume];
    Double_t eDep[numberOfvolume];
    Double_t eDep_e[numberOfvolume];
    Double_t eDep_gamma[numberOfvolume];
    Double_t eDep_other[numberOfvolume];

    Int_t Hit_Volume;
    Int_t Stop_Volume;

    Int_t    det_ID;
    Double_t KineticE;
    Double_t TotalE;
    Int_t    pdgId;
    Double_t Det_X;
    Double_t Det_Y;
    Double_t Det_Z;
    std::string Track_Name;
    std::string Track_Process;
    Int_t Track_ProcessID;

    Int_t nSignals;
    Int_t hit_id;
    Double_t hit_energy[nhitMax];
    Double_t hit_energy_reso[nhitMax];
    Double_t hit_timestart[nhitMax];
    Double_t hit_timeend[nhitMax];
    Int_t hit_nsteps[nhitMax];
    Double_t hit_length[nhitMax];
    Int_t hit_pdgId[nhitMax];
    Int_t hit_process[nhitMax];
    Double_t hit_x[nhitMax];
    Double_t hit_y[nhitMax];
    Double_t hit_z[nhitMax];

    Int_t nDet;
    Double_t Ngamma[numberOfvolume]; 
    Double_t Neletron[numberOfvolume]; 
    Double_t Nneutron[numberOfvolume];
    Double_t Nother[numberOfvolume];

    Int_t runID;
    Int_t eventID;
    Double_t RunTime;
    char RootOutputFileName[200];

    G4double reso_14keV;
    G4double reso_75keV;
    G4double reso_rate;

  public:
    void SetnMaxHit (G4int nhits){nSignals = nhits;}
    void SetRunTime (G4double time) {RunTime = time;}
    void SetSignalInfo (G4int id, G4double energy, G4double time_start, G4double time_end, G4int nsteps, G4double length, G4int _pdgid, G4int name_id, G4double x, G4double y, G4double z){
       hit_energy[id] = energy;
       hit_timestart[id] = time_start;
       hit_timeend[id] = time_end;
       hit_nsteps[id] = nsteps;
       hit_length[id] = length;
       hit_pdgId[id] = _pdgid;
       hit_process[id] = name_id;
       hit_x[id] = x;
       hit_y[id] = y;
       hit_z[id] = z;
    }
                                                                                                                    

};

#endif
