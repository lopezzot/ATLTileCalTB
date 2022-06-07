//**************************************************
// \file ATLTileCalTBAuxData.hh
// \brief: definition of ATLTileCalTBAuxData
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 7 June 2022
//**************************************************

// This class is used to store auxiliary data
// retrieved from the stepping action to be rootified
// in the EndOfEvent() method of EventAction.
// It is used to avoid inter-dependence of Action classes.

#ifndef ATLTileCalTBAuxData_h
#define ATLTileCalTBAuxData_h 1

//Includers from Geant4
//
#include "G4Run.hh"
#include "globals.hh"

class ATLTileCalTBAuxData : public G4Run {
    
    public:
        ATLTileCalTBAuxData();
        virtual ~ATLTileCalTBAuxData();

        void AddLeakage( G4double de );
        void FillPerEvent();
        void Reset();

        //Get methods
        //
        G4double GetLeakage() const;

    private:
        G4double fLeakage;

};

inline void ATLTileCalTBAuxData::AddLeakage( G4double de ) { fLeakage += de; }

inline G4double ATLTileCalTBAuxData::GetLeakage() const { return fLeakage; }   

#endif //ATLTileCalTBAuxData_h 1

//**************************************************
