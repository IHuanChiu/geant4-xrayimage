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
    void StoreTrack(G4int det_index, G4int pdgid, G4double energy_kin,  G4double energy_total, G4double energy_dep, G4double x, G4double y, G4double z, G4double time) { 
          pdgId = pdgid; 
          TotalE = energy_total; 
          KineticE = energy_kin; 
          DepositeE = energy_dep; 
          Det_X = x; 
          Det_Y = y; 
          Det_Z = z; 
          Hit_Time = time;
    }

    static const int numberOfvolume = 10; 
    static const int nh1bin = 15;
    static const int nhitMax = 10;
    static const int nDets = 6;

    //-100 is for escape muon; -1000 is not hit muon
    void SetInitPolInSample(G4ThreeVector pol){muSamplePolX=pol.x(); muSamplePolY=pol.y(); muSamplePolZ=pol.z();}
    void SetInitPolInWorld(G4ThreeVector pol){muWorldPolX=pol.x(); muWorldPolY=pol.y(); muWorldPolZ=pol.z();}
    void SetInitPolInFoil1(G4ThreeVector pol){muFoil1PolX=pol.x(); muFoil1PolY=pol.y(); muFoil1PolZ=pol.z();}
    void SetInitPolInFoil2(G4ThreeVector pol){muFoil2PolX=pol.x(); muFoil2PolY=pol.y(); muFoil2PolZ=pol.z();}
    void SetInitPolInFoil3(G4ThreeVector pol){muFoil3PolX=pol.x(); muFoil3PolY=pol.y(); muFoil3PolZ=pol.z();}
    void SetInitPolInInter1(G4ThreeVector pol){muInter1PolX=pol.x(); muInter1PolY=pol.y(); muInter1PolZ=pol.z();}
    void SetInitPolInInter2(G4ThreeVector pol){muInter2PolX=pol.x(); muInter2PolY=pol.y(); muInter2PolZ=pol.z();}
    void SetInitPolInTarget(G4ThreeVector pol){muTargetPolX=pol.x(); muTargetPolY=pol.y(); muTargetPolZ=pol.z();}
    void SetInitPolInTarget2(G4ThreeVector pol){muTarget2PolX=pol.x(); muTarget2PolY=pol.y(); muTarget2PolZ=pol.z();}

    void SetInitMomInSample(G4ThreeVector pol){muSampleMomX=pol.x(); muSampleMomY=pol.y(); muSampleMomZ=pol.z();}
    void SetInitMomInWorld(G4ThreeVector pol){muWorldMomX=pol.x(); muWorldMomY=pol.y(); muWorldMomZ=pol.z();}
    void SetInitMomInFoil1(G4ThreeVector pol){muFoil1MomX=pol.x(); muFoil1MomY=pol.y(); muFoil1MomZ=pol.z();}
    void SetInitMomInFoil2(G4ThreeVector pol){muFoil2MomX=pol.x(); muFoil2MomY=pol.y(); muFoil2MomZ=pol.z();}
    void SetInitMomInFoil3(G4ThreeVector pol){muFoil3MomX=pol.x(); muFoil3MomY=pol.y(); muFoil3MomZ=pol.z();}
    void SetInitMomInInter1(G4ThreeVector pol){muInter1MomX=pol.x(); muInter1MomY=pol.y(); muInter1MomZ=pol.z();}
    void SetInitMomInInter2(G4ThreeVector pol){muInter2MomX=pol.x(); muInter2MomY=pol.y(); muInter2MomZ=pol.z();}
    void SetInitMomInTarget(G4ThreeVector pol){muTargetMomX=pol.x(); muTargetMomY=pol.y(); muTargetMomZ=pol.z();}
    void SetInitMomInTarget2(G4ThreeVector pol){muTarget2MomX=pol.x(); muTarget2MomY=pol.y(); muTarget2MomZ=pol.z();}

    void SetInitTimeInSample(G4double time){muSampleTime = time;}
    void SetInitTimeInWorld(G4double time){muWorldTime = time;}
    void SetInitTimeInFoil1(G4double time){muFoil1Time = time;}
    void SetInitTimeInFoil2(G4double time){muFoil2Time = time;}
    void SetInitTimeInFoil3(G4double time){muFoil3Time = time;}
    void SetInitTimeInInter1(G4double time){muInter1Time = time;}
    void SetInitTimeInInter2(G4double time){muInter2Time = time;}
    void SetInitTimeInTarget(G4double time){muTargetTime = time;}
    void SetInitTimeInTarget2(G4double time){muTarget2Time = time;}

    void SetInitKineticEnergyInSample(G4double energy){muSampleKineticEnergy = energy;}
    void SetInitKineticEnergyInWorld(G4double energy){muWorldKineticEnergy = energy;}
    void SetInitKineticEnergyInFoil1(G4double energy){muFoil1KineticEnergy = energy;}
    void SetInitKineticEnergyInFoil2(G4double energy){muFoil2KineticEnergy = energy;}
    void SetInitKineticEnergyInFoil3(G4double energy){muFoil3KineticEnergy = energy;}
    void SetInitKineticEnergyInInter1(G4double energy){muInter1KineticEnergy = energy;}
    void SetInitKineticEnergyInInter2(G4double energy){muInter2KineticEnergy = energy;}
    void SetInitKineticEnergyInTarget(G4double energy){muTargetKineticEnergy = energy;}
    void SetInitKineticEnergyInTarget2(G4double energy){muTarget2KineticEnergy = energy;}

    void SetEndPolInSample(G4ThreeVector pol){muSampleEndPolX=pol.x(); muSampleEndPolY=pol.y(); muSampleEndPolZ=pol.z();}
    void SetEndPolInWorld(G4ThreeVector pol){muWorldEndPolX=pol.x(); muWorldEndPolY=pol.y(); muWorldEndPolZ=pol.z();}    
    void SetEndPolInInter1(G4ThreeVector pol){muInter1EndPolX=pol.x(); muInter1EndPolY=pol.y(); muInter1EndPolZ=pol.z();}    
    void SetEndPolInInter2(G4ThreeVector pol){muInter2EndPolX=pol.x(); muInter2EndPolY=pol.y(); muInter2EndPolZ=pol.z();}   

    void SetEndMomInSample(G4ThreeVector pol){muSampleEndMomX=pol.x(); muSampleEndMomY=pol.y(); muSampleEndMomZ=pol.z();}
    void SetEndMomInWorld(G4ThreeVector pol){muWorldEndMomX=pol.x(); muWorldEndMomY=pol.y(); muWorldEndMomZ=pol.z();}    
    void SetEndMomInInter1(G4ThreeVector pol){muInter1EndMomX=pol.x(); muInter1EndMomY=pol.y(); muInter1EndMomZ=pol.z();}    
    void SetEndMomInInter2(G4ThreeVector pol){muInter2EndMomX=pol.x(); muInter2EndMomY=pol.y(); muInter2EndMomZ=pol.z();}    

    void SetEndTimeInSample(G4double time){muSampleEndTime = time;}
    void SetEndTimeInWorld(G4double time){muWorldEndTime = time;}
    void SetEndTimeInInter1(G4double time){muInter1EndTime = time;}
    void SetEndTimeInInter2(G4double time){muInter2EndTime = time;}

    void SetEndKineticEnergyInSample(G4double energy){muSampleEndKineticEnergy = energy;}
    void SetEndKineticEnergyInWorld(G4double energy){muWorldEndKineticEnergy = energy;}
    void SetEndKineticEnergyInInter1(G4double energy){muInter1EndKineticEnergy = energy;}
    void SetEndKineticEnergyInInter2(G4double energy){muInter2EndKineticEnergy = energy;}

    void SetInitialMuonParameters(G4double x,G4double y,G4double z, G4double px,G4double py,G4double pz,G4double time){
         muInitX = x;muInitY = y;muInitZ = z;muInitpX = px;muInitpY = py;muInitpZ = pz; muInitTime = time;}
    void SetInitialEletronParameters(G4double x,G4double y,G4double z, G4double px,G4double py,G4double pz){
         eInitX = x;eInitY = y;eInitZ = z;eInitpX = px;eInitpY = py;eInitpZ = pz;}

    void SetDecayPolGlo(G4ThreeVector pol){muDecayPolX=pol.x(); muDecayPolY=pol.y(); muDecayPolZ=pol.z();}    
    void SetDecayTimeGlo(G4double time){muDecayTime = time;}    

    void SetmuFinalVolume(G4int fpol){Stop_Volume = fpol;}
    void SetnParticleHitVolume(G4int ivolume, G4int ngamma, G4int neletron, G4int nneutron, G4int nother){
                               Ngamma = ngamma; Neletron = neletron; Nneutron = nneutron; Nother = nother;}

    void SetEnergyResolution ();

    TFile* rootFile;
    TTree* rootTree;
    TTree* muonTree;
    TTree* TrackTree;

    TH1F*  h1_process;
    TH1F* h1_StopVol;

  // === variables for tree ===
  private:
    static RootOutput* pointerToRoot;
    const char *var_name[nh1bin] = {"World","Foil-1","Intermediate1","Foil-2","intermediate2","Foil-3","Sample","Chamber","Virtual1","Virtua2","BeWindowTubs", "Holder","None","None","None"};
    const char *pro_name[nh1bin] = {"None","muMinusCaptureAtRest","phot","compt","eBrem",
                                    "neutronInelastic","muIoni","conv","None","None","None","None","None","None","None"};

    Double_t muSamplePolX, muSamplePolY, muSamplePolZ;
    Double_t muWorldPolX, muWorldPolY, muWorldPolZ;
    Double_t muFoil1PolX, muFoil1PolY, muFoil1PolZ;
    Double_t muFoil2PolX, muFoil2PolY, muFoil2PolZ;
    Double_t muFoil3PolX, muFoil3PolY, muFoil3PolZ;
    Double_t muInter1PolX, muInter1PolY, muInter1PolZ;
    Double_t muInter2PolX, muInter2PolY, muInter2PolZ;
    Double_t muTargetPolX, muTargetPolY, muTargetPolZ;
    Double_t muTarget2PolX, muTarget2PolY, muTarget2PolZ;
    Double_t muSampleMomX, muSampleMomY, muSampleMomZ;
    Double_t muWorldMomX, muWorldMomY, muWorldMomZ;
    Double_t muFoil1MomX, muFoil1MomY, muFoil1MomZ;
    Double_t muFoil2MomX, muFoil2MomY, muFoil2MomZ;
    Double_t muFoil3MomX, muFoil3MomY, muFoil3MomZ;
    Double_t muInter1MomX, muInter1MomY, muInter1MomZ;
    Double_t muInter2MomX, muInter2MomY, muInter2MomZ;
    Double_t muTargetMomX, muTargetMomY, muTargetMomZ;
    Double_t muTarget2MomX, muTarget2MomY, muTarget2MomZ;
    Double_t muSampleTime, muWorldTime, muFoil1Time, muFoil2Time, muFoil3Time, muInter1Time, muInter2Time, muTargetTime, muTarget2Time;
    Double_t muSampleKineticEnergy, muWorldKineticEnergy, muFoil1KineticEnergy, muFoil2KineticEnergy, muFoil3KineticEnergy, muInter1KineticEnergy, muInter2KineticEnergy, muTargetKineticEnergy, muTarget2KineticEnergy;

    Double_t muSampleEndPolX, muSampleEndPolY, muSampleEndPolZ;
    Double_t muWorldEndPolX, muWorldEndPolY, muWorldEndPolZ;
    Double_t muInter1EndPolX, muInter1EndPolY, muInter1EndPolZ;
    Double_t muInter2EndPolX, muInter2EndPolY, muInter2EndPolZ;
    Double_t muSampleEndMomX, muSampleEndMomY, muSampleEndMomZ;
    Double_t muWorldEndMomX, muWorldEndMomY, muWorldEndMomZ;
    Double_t muInter1EndMomX, muInter1EndMomY, muInter1EndMomZ;
    Double_t muInter2EndMomX, muInter2EndMomY, muInter2EndMomZ;
    Double_t muSampleEndTime, muWorldEndTime, muInter1EndTime, muInter2EndTime;
    Double_t muSampleEndKineticEnergy, muWorldEndKineticEnergy, muInter1EndKineticEnergy, muInter2EndKineticEnergy;

    Double_t muDecayPolX, muDecayPolY, muDecayPolZ, muDecayTime;
    Double_t muInitX ; Double_t muInitY ;Double_t muInitZ ;Double_t muInitpX ;Double_t muInitpY ;Double_t muInitpZ ; Double_t muInitTime;
    Double_t eInitX ; Double_t eInitY ;Double_t eInitZ ;Double_t eInitpX ;Double_t eInitpY ;Double_t eInitpZ ;  

    Int_t Det_nMax;
    Double_t eDep;
    Double_t eDep_e;
    Double_t eDep_gamma;
    Double_t eDep_other;
    Double_t egamma_hit_time;
    std::string hitparticle;

    Int_t Hit_Volume;
    Int_t Stop_Volume;
    Double_t KineticE;
    Double_t TotalE;
    Double_t DepositeE;
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

    Double_t InitEnergy;
    Int_t nSignals;
    Int_t Det_ID[nhitMax];
    Double_t hit_energy[nhitMax];
    Double_t hit_energy_reso[nhitMax];
    Double_t hit_startx[nhitMax];
    Double_t hit_starty[nhitMax];
    Double_t hit_startz[nhitMax];
    Double_t hit_timestart[nhitMax];
    Double_t hit_timeend[nhitMax];
    Int_t hit_nsteps[nhitMax];
    Double_t hit_length[nhitMax];
    Int_t hit_pdgId[nhitMax];
    Int_t hit_process[nhitMax];

   // === public class to catch info. === 
  public:
    void SetDetectorInfo (G4double edep, G4double edep_e, G4double edep_gamma, G4double edep_other, G4double time);
    void SetRunTime (G4double time) {RunTime = time;}
    void SetnMaxHit (G4int nhits){nSignals = nhits;}
    void SetnInitEnergy (G4double gamma_e){InitEnergy = gamma_e;}
    void SetSignalInfo (G4int det_id, G4int id, G4double energy, G4double start_x, G4double start_y, G4double start_z, G4double time_start, G4double time_end, G4int nsteps, G4double length, G4int pdgId, G4int name_id){
       Det_ID[id]=det_id+1;
       hit_energy[id] = energy;
       hit_startx[id] = start_x;
       hit_starty[id] = start_y;
       hit_startz[id] = start_z;
       hit_timestart[id] = time_start;
       hit_timeend[id] = time_end;
       hit_nsteps[id] = nsteps;
       hit_length[id] = length;
       hit_pdgId[id] = pdgId;
       hit_process[id] = name_id;
    }
};

#endif
