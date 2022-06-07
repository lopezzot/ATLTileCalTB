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

class ATLTileCalTBEventAction : public G4UserEventAction {
    
    public:
        ATLTileCalTBEventAction();
        virtual ~ATLTileCalTBEventAction();

        virtual void BeginOfEventAction( const G4Event* event );
        virtual void EndOfEventAction( const G4Event* event );
    
};
                     
#endif //ATLTileCalTBEventAction_h

//**************************************************
