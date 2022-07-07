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
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

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

        //Methods to handle data
        //
        void AddE( G4double de );
        void AddS( G4double ds );

        //Get methods
        G4double GetEdep() const;
        G4double GetSdep() const;

    private:
        G4double fEdep; 
        G4double fSdep;

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

inline void ATLTileCalTBHit::AddE( G4double de ) { fEdep += de; }

inline void ATLTileCalTBHit::AddS( G4double ds ) { fSdep += ds; }

inline G4double ATLTileCalTBHit::GetEdep() const { return fEdep; }

inline G4double ATLTileCalTBHit::GetSdep() const { return fSdep; }

#endif //ATLTileCalTBHit_h 1

//**************************************************
