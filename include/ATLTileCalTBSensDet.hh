//**************************************************
// \file ATLTileCalTBSensDet.hh
// \brief: definition of ATLTileCalTBSensDet class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 6 June 2022
//**************************************************

#ifndef ATLTileCalTBSensDet_h
#define ATLTileCalTBSensDet_h 1

//Includers from Geant4
//
#include "G4VSensitiveDetector.hh"

//Includers form project files
//
#include "ATLTileCalTBHit.hh"
#include "ATLTileCalTBGeometry.hh"

//Forward declaration from Geant4
//
class G4Step;
class G4HCofThisEvent;

class ATLTileCalTBSensDet : public G4VSensitiveDetector {
  
    public:
        ATLTileCalTBSensDet( const G4String& name, const G4String& hitsCollectionName );
        virtual ~ATLTileCalTBSensDet();
  
        //Methods from base class
        //
        virtual void Initialize( G4HCofThisEvent* hitCollection );
        virtual G4bool ProcessHits( G4Step* aStep, G4TouchableHistory* history );
        virtual void   EndOfEvent( G4HCofThisEvent* hitCollection );

    private:
        ATLTileCalTBHitsCollection* fHitsCollection;
        G4double BirkLaw( const G4Step* aStep) const;
        std::size_t FindCellIndexFromG4( const G4Step* aStep ) const;
        G4double Tile_1D_profileRescaled( G4int row, G4double x, G4double y, G4int PMT, ATLTileCalTBGeometry::Cell cell/*, G4int nSide*/);

};

#endif //ATLTileCalTBSensDet_h 1

//**************************************************
