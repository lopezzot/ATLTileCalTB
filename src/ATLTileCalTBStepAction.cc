//**************************************************
// \file ATLTileCalTBStepAction.cc
// \brief: implementation of ATLTileCalTBStepAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 8 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBStepAction.hh"
#include "ATLTileCalTBAuxData.hh"
#include "G4RunManager.hh"

//Constructor and de-constructor
//
ATLTileCalTBStepAction::ATLTileCalTBStepAction()
    : G4UserSteppingAction() {}

ATLTileCalTBStepAction::~ATLTileCalTBStepAction() {}

//UserSteppingaction() method
//
void ATLTileCalTBStepAction::UserSteppingAction( const G4Step* aStep ) {
    
    //Collect out of world leakage
    //
    if ( !aStep->GetTrack()->GetNextVolume() ){
        auto auxData = static_cast<ATLTileCalTBAuxData*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
        auxData->AddLeakage( aStep->GetTrack()->GetKineticEnergy() ); 
    }

}

//**************************************************
