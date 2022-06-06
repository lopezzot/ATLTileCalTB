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
//#include "B4cCalorHit.hh"

//Includers from C++
//
#include <vector>

//Forward declaration from Geant4
//
class G4Step;
class G4HCofThisEvent;

class ATLTileCalTBSensDet : public G4VSensitiveDetector {
  
    public:
        ATLTileCalTBSensDet( const G4String& name, const G4String& hitsCollectionName, G4int nCells );
        virtual ~ATLTileCalTBSensDet();
  
        //Methods from base class
        //
        virtual void Initialize( G4HCofThisEvent* hitCollection );
        virtual G4bool ProcessHits( G4Step* step, G4TouchableHistory* history );
        virtual void   EndOfEvent( G4HCofThisEvent* hitCollection );

    private:
        //ATLTileCalTBHitsCollection* fHitsCollection;
        G4int  fNCells;

};

#endif //ATLTileCalTBSensDet_h 1

//**************************************************
