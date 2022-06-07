//**************************************************
// \file ATLTileCalTBRunAction.cc
// \brief: implementation of ATLTileCalTBRunAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 6 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBRunAction.hh"

//Includers from Geant4
//
#include "g4root.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//Constructor and de-constructor
//
ATLTileCalTBRunAction::ATLTileCalTBRunAction()
    : G4UserRunAction() { 
    
    //Printing event number per each event
    //
    G4RunManager::GetRunManager()->SetPrintProgress(1);     

    //Get analysis manager
    //
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);
  
    // Creating ntuple
    //
    analysisManager->CreateNtuple("tree", "calo");
    analysisManager->CreateNtupleDColumn("ETile");
    analysisManager->FinishNtuple();

}

ATLTileCalTBRunAction::~ATLTileCalTBRunAction() {
    
    delete G4AnalysisManager::Instance();  

}

//BeginOfRunAction method
//
void ATLTileCalTBRunAction::BeginOfRunAction(const G4Run* run) { 
  
    //Save random number seed
    //
    //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
    auto analysisManager = G4AnalysisManager::Instance();

    std::string runnumber = std::to_string( run->GetRunID() );
    G4String fileName = "ATLTileCalTBout_Run" + runnumber + ".root";
    analysisManager->OpenFile(fileName);
    
}

void ATLTileCalTBRunAction::EndOfRunAction(const G4Run* /*run*/) {

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

}

//**************************************************
