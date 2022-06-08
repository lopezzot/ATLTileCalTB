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

class ATLTileCalTBStepAction: public G4UserSteppingAction {

    public:
        ATLTileCalTBStepAction();
        virtual ~ATLTileCalTBStepAction();

        virtual void UserSteppingAction( G4Step* aStep );

}

#endif //ATLTileCalTBStepAction_h 1

//**************************************************
