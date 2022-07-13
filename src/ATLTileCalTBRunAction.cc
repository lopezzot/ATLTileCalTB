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
#include "ATLTileCalTBEventAction.hh"

//Includers from Geant4
//
#include "g4root.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Version.hh"

//Includers from C++
//
#ifdef ATLTileCalTB_PulseOutput
#include <filesystem>
#endif

//Constructor and de-constructor
//
ATLTileCalTBRunAction::ATLTileCalTBRunAction( ATLTileCalTBEventAction* eventAction )
    : G4UserRunAction(),
      fEventAction(eventAction) { 
    
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
    analysisManager->CreateNtuple("ATLTileCalTBout", "ATLTileCalTBoutput");
    analysisManager->CreateNtupleDColumn("ELeak");
    analysisManager->CreateNtupleDColumn("Ecal");
    analysisManager->CreateNtupleDColumn("EdepSum");
    analysisManager->CreateNtupleDColumn("SdepSum");
    analysisManager->CreateNtupleDColumn("Edep", fEventAction->GetEdepVector());
    analysisManager->CreateNtupleDColumn("Sdep", fEventAction->GetSdepVector());
    analysisManager->FinishNtuple();

}

ATLTileCalTBRunAction::~ATLTileCalTBRunAction() {
    #if G4VERSION_NUMBER < 1100
    delete G4AnalysisManager::Instance();  // not needed for G4 v11 and up
    #endif
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

    #ifdef ATLTileCalTB_PulseOutput
    auto pulse_run_path = std::filesystem::path("ATLTileCalTBpulse_Run" + runnumber);
    std::filesystem::remove_all(pulse_run_path);
    std::filesystem::create_directory(pulse_run_path);
    #endif

}

void ATLTileCalTBRunAction::EndOfRunAction(const G4Run* /*run*/) {

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

}

//**************************************************
