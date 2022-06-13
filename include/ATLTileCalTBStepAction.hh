//**************************************************
// \file ATLTileCalTBStepAction.hh
// \brief: definition of ATLTileCalTBStepAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 8 June 2022
//**************************************************

#ifndef ATLTileCalTBStepAction_h
#define ATLTileCalTBStepAction_h 1

//Includers from Geant4
//
#include "G4Step.hh"
#include "G4UserSteppingAction.hh"

//Includers from project files
//
#include "ATLTileCalTBEventAction.hh"

class ATLTileCalTBStepAction: public G4UserSteppingAction {

    public:
        ATLTileCalTBStepAction( ATLTileCalTBEventAction* EvtAction );
        virtual ~ATLTileCalTBStepAction();

        virtual void UserSteppingAction( const G4Step* aStep );
    
    private:
        ATLTileCalTBEventAction* fEventAction;

};

#endif //ATLTileCalTBStepAction_h 1

//**************************************************
