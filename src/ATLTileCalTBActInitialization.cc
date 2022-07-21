//**************************************************
// \file ATLTileCalTBActInitialization.cc
// \brief: implementation of
//         ATLTileCalTBActInitialization class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 23 May 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBActInitialization.hh"
#include "ATLTileCalTBPrimaryGenAction.hh"
#include "ATLTileCalTBRunAction.hh"
#include "ATLTileCalTBEventAction.hh"
#include "ATLTileCalTBStepAction.hh"

//Constructor and de-constructor
//
ATLTileCalTBActInitialization::ATLTileCalTBActInitialization()
    : G4VUserActionInitialization() {
}

ATLTileCalTBActInitialization::~ATLTileCalTBActInitialization() {}

//Define Build() and BuildForMaster() methods
//
void ATLTileCalTBActInitialization::BuildForMaster() const {
    auto EventAction = new ATLTileCalTBEventAction(nullptr);
    SetUserAction( new ATLTileCalTBRunAction( EventAction ) );

}

void ATLTileCalTBActInitialization::Build() const {
    auto PrimaryGenAction = new ATLTileCalTBPrimaryGenAction();
    auto EventAction = new ATLTileCalTBEventAction(PrimaryGenAction);

    SetUserAction( PrimaryGenAction );
    SetUserAction( new ATLTileCalTBRunAction( EventAction ) );
    SetUserAction( EventAction );
    SetUserAction( new ATLTileCalTBStepAction( EventAction ) );

}

//**************************************************
