//**************************************************
// \file ATLTileCalTBHit.hh
// \brief: definition of ATLTileCalTBHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 6 June 2022
//**************************************************

#ifndef ATLTileCalTBHit_h
#define ATLTileCalTBHit_h 1

//Includers from Geant4
//
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Threading.hh"

//Includers from C++
//
#include <vector>

class ATLTileCalTBHit : public G4VHit {
  
    public:
        ATLTileCalTBHit();
        ATLTileCalTBHit( const ATLTileCalTBHit& );
        virtual ~ATLTileCalTBHit();

        //Operators (= and ==)
        //
        const ATLTileCalTBHit& operator=( const ATLTileCalTBHit& );
        G4bool operator==( const ATLTileCalTBHit& ) const;

        inline void* operator new(size_t);
        inline void  operator delete(void*);

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
        const std::vector<G4double>& GetSdepUp() const;
        const std::vector<G4double>& GetSdepDown() const;

    private:
        // Total energy deposition in the cell
        G4double fEdep;

        //Vectors containing the binned signal
        std::vector<G4double> fSdepUp;
        std::vector<G4double> fSdepDown;

};

using ATLTileCalTBHitsCollection = G4THitsCollection<ATLTileCalTBHit>;

extern G4ThreadLocal G4Allocator<ATLTileCalTBHit>* ATLTileCalTBHitAllocator;

inline void* ATLTileCalTBHit::operator new(size_t) {
  
    if (!ATLTileCalTBHitAllocator) {
        ATLTileCalTBHitAllocator = new G4Allocator<ATLTileCalTBHit>;
    }
    void *hit;
    hit = (void *) ATLTileCalTBHitAllocator->MallocSingle();
    return hit;

}

inline void ATLTileCalTBHit::operator delete(void *hit) {
  
    if (!ATLTileCalTBHitAllocator) {
        ATLTileCalTBHitAllocator = new G4Allocator<ATLTileCalTBHit>;
    }
    ATLTileCalTBHitAllocator->FreeSingle((ATLTileCalTBHit*) hit);

}

inline void ATLTileCalTBHit::AddEdep(G4double dEdep) { fEdep += dEdep; };

inline void ATLTileCalTBHit::AddSdep(std::size_t index, G4double dSdepUp, G4double dSdepDown) {
    fSdepUp[index] += dSdepUp;
    fSdepDown[index] += dSdepDown;
};

inline void ATLTileCalTBHit::AddSdep(G4double time, G4double dSdepUp, G4double dSdepDown) {
    AddSdep(GetBinFromTime(time), dSdepUp, dSdepDown);
}

inline G4double ATLTileCalTBHit::GetEdep() const { return fEdep; };

inline const std::vector<G4double>& ATLTileCalTBHit::GetSdepUp() const { return fSdepUp; };

inline const std::vector<G4double>& ATLTileCalTBHit::GetSdepDown() const { return fSdepDown; };

#endif //ATLTileCalTBHit_h 1

//**************************************************
