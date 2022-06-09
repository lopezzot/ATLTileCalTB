//**************************************************
// \file ATLTileCalTBEventAction.cc
// \brief: implementation of ATLTileCalTBEventAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 7 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBEventAction.hh"
#include "ATLTileCalTBAuxData.hh"

//Includers from Geant4
//
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

//Includers from C++
//
#include "Randomize.hh"
#include <iomanip>

//Constructor and de-constructor
//
ATLTileCalTBEventAction::ATLTileCalTBEventAction()
    : G4UserEventAction() {
}

ATLTileCalTBEventAction::~ATLTileCalTBEventAction() {
}

//BeginOfEvent() method
//
void ATLTileCalTBEventAction::BeginOfEventAction(const G4Event* /*event*/) {  
  
    auto AuxData = static_cast<ATLTileCalTBAuxData*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );
    AuxData->Reset();

}

//EndOfEventaction() method
//
void ATLTileCalTBEventAction::EndOfEventAction(const G4Event* /*event*/) {

    auto AuxData = static_cast<ATLTileCalTBAuxData*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );
    G4cout<<AuxData->GetLeakage()<<G4endl;
    AuxData->FillPerEvent();

}  

//**************************************************
