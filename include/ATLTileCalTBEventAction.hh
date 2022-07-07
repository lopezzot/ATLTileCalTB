//**************************************************
// \file ATLTileCalTBEventAction.hh
// \brief: definition of ATLTileCalTBEventAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 7 June 2022
//**************************************************

#ifndef ATLTileCalTBEventAction_h
#define ATLTileCalTBEventAction_h 1

//Includers from Geant4
//
#include "G4UserEventAction.hh"
#include "globals.hh"

//Includers from project files
//
#include "ATLTileCalTBHit.hh"

//Includers from C++
//
#include <array>

const G4int nAuxData = 2; //0->Leakage, 1->Energy Deposited in Calo

class ATLTileCalTBEventAction : public G4UserEventAction {
    
    public:
        ATLTileCalTBEventAction();
        virtual ~ATLTileCalTBEventAction();

        virtual void BeginOfEventAction( const G4Event* event );
        virtual void EndOfEventAction( const G4Event* event );

        void Add( G4int index, G4double de );
        void FillPerEvent();
        void Reset();

        std::vector<G4double>& GetEdepVector() { return fEdepVector; };
        std::vector<G4double>& GetSdepVector() { return fSdepVector; };

    private:
        ATLTileCalTBHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
        std::array<G4double, nAuxData> fAux;
        std::vector<G4double> fEdepVector;
        std::vector<G4double> fSdepVector;
    
};
                     
inline void ATLTileCalTBEventAction::Add( G4int index, G4double de ) { fAux[index] += de; }

inline void ATLTileCalTBEventAction::Reset() { 

    for ( auto& value : fAux ) { value = 0.; } 

}

#endif //ATLTileCalTBEventAction_h

//**************************************************
