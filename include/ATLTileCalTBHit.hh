//**************************************************
// \file ATLTileCalTBHit.hh
// \brief: definition of ATLTileCalTBHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 6 June 2022
//**************************************************

#ifndef ATLTileCalTBHit_h
#define ATLTileCalTBHit_h 1

//Includers from project files
//
#include "ATLTileCalTBConstants.hh"

//Includers from Geant4
//
#include "G4VHit.hh"
#include "G4THitsCollection.hh"

//Includers from C++
//
#include <array>

class ATLTileCalTBHit : public G4VHit {
  
    public:
        ATLTileCalTBHit();
        ATLTileCalTBHit( const ATLTileCalTBHit& );
        virtual ~ATLTileCalTBHit();

        //Operators (= and ==)
        //
        const ATLTileCalTBHit& operator=( const ATLTileCalTBHit& );
        G4bool operator==( const ATLTileCalTBHit& ) const;

        //Methods from base class
        //
        virtual void Draw() {}
        virtual void Print(){};

        //Method to get correct vector index from a given time
        static std::size_t GetBinFromTime( G4double time );

        //Methods to handle data
        //
        void AddEdep( G4double dEdep );
        void AddSdep( std::size_t index, G4double dSdepUp, G4double dSdepDown );
        void AddSdep( G4double time, G4double dSdepUp, G4double dSdepDown );

        //Get methods
        //
        G4double GetEdep() const;
        const std::array<G4double, ATLTileCalTBConstants::frames>& GetSdepUp() const;
        const std::array<G4double, ATLTileCalTBConstants::frames>& GetSdepDown() const;

    private:
        // Total energy deposition in the cell
        G4double fEdep;

        //Vectors containing the binned signal
        std::array<G4double, ATLTileCalTBConstants::frames> fSdepUp;
        std::array<G4double, ATLTileCalTBConstants::frames> fSdepDown;

};

using ATLTileCalTBHitsCollection = G4THitsCollection<ATLTileCalTBHit>;

inline void ATLTileCalTBHit::AddEdep(G4double dEdep) { fEdep += dEdep; }

inline void ATLTileCalTBHit::AddSdep(std::size_t index, G4double dSdepUp, G4double dSdepDown) {
    fSdepUp[index] += dSdepUp;
    fSdepDown[index] += dSdepDown;
}

inline void ATLTileCalTBHit::AddSdep(G4double time, G4double dSdepUp, G4double dSdepDown) {
    AddSdep(GetBinFromTime(time), dSdepUp, dSdepDown);
}

inline G4double ATLTileCalTBHit::GetEdep() const { return fEdep; }

inline const std::array<G4double, ATLTileCalTBConstants::frames>& ATLTileCalTBHit::GetSdepUp() const { return fSdepUp; }

inline const std::array<G4double, ATLTileCalTBConstants::frames>& ATLTileCalTBHit::GetSdepDown() const { return fSdepDown; }

#endif //ATLTileCalTBHit_h 1

//**************************************************
