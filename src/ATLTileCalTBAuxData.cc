//**************************************************
// \file ATLTileCalTBAuxData.cc
// \brief: implementation of ATLTileCalTBAuxData
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 7 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBAuxData.hh"

//Includers from Geant4
//
#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//Constructor and de-constructor
//
ATLTileCalTBAuxData::ATLTileCalTBAuxData() 
    : G4Event(),
      fLeakage(0.) {
}

ATLTileCalTBAuxData::~ATLTileCalTBAuxData() {
}

//FillPerEvent() method
//
void ATLTileCalTBAuxData::FillPerEvent() {
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, fLeakage);    
    analysisManager->AddNtupleRow();  

}

//Reset() method
//
void ATLTileCalTBAuxData::Reset() { 
    fLeakage = 0.;
}

//**************************************************
