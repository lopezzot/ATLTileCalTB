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
#include "ATLTileCalTBGeometry.hh"
#include "ATLTileCalTBConstants.hh"

//Includers from Geant4
//
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"
#include "Randomize.hh"

//Includers from C++
//
#include <iomanip>
#include <numeric>

//Constructor and de-constructor
//
ATLTileCalTBEventAction::ATLTileCalTBEventAction()
    : G4UserEventAction(),
      fAux{0., 0.} {
    fEdepVector = std::vector<G4double>(ATLTileCalTBGeometry::cellNoSize, 0.);
    fSdepVector = std::vector<G4double>(ATLTileCalTBGeometry::cellNoSize, 0.);
}

ATLTileCalTBEventAction::~ATLTileCalTBEventAction() {
}

//BeginOfEvent() method
//
void ATLTileCalTBEventAction::BeginOfEventAction(const G4Event* /*event*/) {  
  
    for ( auto& value : fAux ){ value = 0.; } 
    for ( auto& value : fEdepVector ) { value = 0.; }
    for ( auto& value : fSdepVector ) { value = 0.; }

}

//GetHitsCollection method()
//
ATLTileCalTBHitsCollection* ATLTileCalTBEventAction::GetHitsCollection(G4int hcID,
                                                                       const G4Event* event) const {
    auto hitsCollection = static_cast<ATLTileCalTBHitsCollection*>( event->GetHCofThisEvent()->GetHC(hcID) );
  
    if ( ! hitsCollection ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hitsCollection ID " << hcID; 
        G4Exception("ATLTileCalTBEventAction::GetHitsCollection()",
        "MyCode0003", FatalException, msg);
    } 

    return hitsCollection;

}    

//EndOfEventaction() method
//
void ATLTileCalTBEventAction::EndOfEventAction( const G4Event* event ) {

    auto analysisManager = G4AnalysisManager::Instance();

    G4int counter = 0;
    for ( auto& value : fAux ){ 
        analysisManager->FillNtupleDColumn( counter, value );    
        counter++;
    }

    //Get hits collections and fill vector
    auto HC = GetHitsCollection(0, event);
    for (std::size_t n = 0; n < ATLTileCalTBGeometry::cellNoSize; ++n) {
        fEdepVector[n] = (*HC)[n]->GetEdep();
        fSdepVector[n] = (*HC)[n]->GetSdep() + G4RandGauss::shoot(0., ATLTileCalTBConstants::noise_sigma); // Add electronic noise
    }

    //Add sums to Ntuple
    analysisManager->FillNtupleDColumn(2, std::accumulate(fEdepVector.begin(), fEdepVector.end(), 0));
    analysisManager->FillNtupleDColumn(3, std::accumulate(fSdepVector.begin(), fSdepVector.end(), 0));

    analysisManager->AddNtupleRow();

} 

//**************************************************
