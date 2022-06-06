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

ATLTileCalTBActInitialization::ATLTileCalTBActInitialization()
    : G4VUserActionInitialization()
{}

ATLTileCalTBActInitialization::~ATLTileCalTBActInitialization() {}

//Define Build() and BuildForMaster() methods
//
void ATLTileCalTBActInitialization::BuildForMaster() const {
    
    SetUserAction( new ATLTileCalTBRunAction() );

}

void ATLTileCalTBActInitialization::Build() const {
    
    SetUserAction( new ATLTileCalTBPrimaryGenAction );
    SetUserAction( new ATLTileCalTBRunAction() );

}

//**************************************************
