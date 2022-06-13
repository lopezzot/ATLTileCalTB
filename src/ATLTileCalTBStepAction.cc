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

//Includers from Geant4
//
#include "G4RunManager.hh"

//Constructor and de-constructor
//
ATLTileCalTBStepAction::ATLTileCalTBStepAction(ATLTileCalTBEventAction* EventAction)
    : G4UserSteppingAction(),
      fEventAction( EventAction ){}

ATLTileCalTBStepAction::~ATLTileCalTBStepAction() {}

//UserSteppingaction() method
//
void ATLTileCalTBStepAction::UserSteppingAction( const G4Step* aStep ) {
    
    //Collect out of world leakage
    //
    if ( !aStep->GetTrack()->GetNextVolume() ){
        fEventAction->Add( 0, aStep->GetTrack()->GetKineticEnergy() ); 
    }

    //Collect all energy deposition
    //
    fEventAction->Add( 1, aStep->GetTotalEnergyDeposit() );

}

//**************************************************
