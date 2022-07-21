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
#include "G4Types.hh"

//Includers from project files
//
#include "ATLTileCalTBHit.hh"

//Includers from C++
//
#include <array>
#include <vector>
#ifdef ATLTileCalTB_PulseOutput
#include <filesystem>
#endif

//Forward declaration from project
//
class ATLTileCalTBPrimaryGenAction;

constexpr std::size_t nAuxData = 2; //0->Leakage, 1->Energy Deposited in Calo

class ATLTileCalTBEventAction : public G4UserEventAction {
    
    public:
        ATLTileCalTBEventAction(ATLTileCalTBPrimaryGenAction* pga);
        virtual ~ATLTileCalTBEventAction();

        virtual void BeginOfEventAction( const G4Event* event );
        virtual void EndOfEventAction( const G4Event* event );

        void Add( std::size_t index, G4double de );

        std::vector<G4double>& GetEdepVector() { return fEdepVector; };
        std::vector<G4double>& GetSdepVector() { return fSdepVector; };

    private:
        ATLTileCalTBHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
        ATLTileCalTBPrimaryGenAction* fPrimaryGenAction;
        std::size_t fNoOfCells;
        std::array<G4double, nAuxData> fAux;
        std::vector<G4double> fEdepVector;
        std::vector<G4double> fSdepVector;
        #ifdef ATLTileCalTB_PulseOutput
        std::filesystem::path pulse_event_path;
        #endif
};
                     
inline void ATLTileCalTBEventAction::Add( std::size_t index, G4double de ) { fAux[index] += de; }

#endif //ATLTileCalTBEventAction_h

//**************************************************
