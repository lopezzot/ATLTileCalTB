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
#include <algorithm>

//Constructor and de-constructor
//
ATLTileCalTBEventAction::ATLTileCalTBEventAction()
    : G4UserEventAction(),
      fNoOfCells(ATLTileCalTBGeometry::CellLUT::GetInstance()->GetNumberOfCells()),
      fAux{0., 0.} {
    fEdepVector = std::vector<G4double>(fNoOfCells, 0.);
    fSdepVector = std::vector<G4double>(fNoOfCells, 0.);
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

    //Method to convolute signal for PMT response
    auto ConvolutePMT = [](const std::vector<G4double>& sdep) {
        //TODO: implement
        return sdep;
    };

    //Method to get sdep from hit
    auto GetSdep = [ConvolutePMT](const ATLTileCalTBHit* hit) -> G4double {
        //PMT response
        auto sdep_up_v = ConvolutePMT(hit->GetSdepUp());
        auto sdep_down_v = ConvolutePMT(hit->GetSdepDown());

        //Use maximum as signal
        G4double sdep_up = *(std::max_element(sdep_up_v.begin(), sdep_up_v.end()));
        G4double sdep_down = *(std::max_element(sdep_down_v.begin(), sdep_down_v.end()));

        //Apply electronic noise
        sdep_up += G4RandGauss::shoot(0., ATLTileCalTBConstants::noise_sigma);
        sdep_down += G4RandGauss::shoot(0., ATLTileCalTBConstants::noise_sigma);

        //Return sum
        return sdep_up + sdep_down;
    };

    //Get hits collections and fill vector
    auto HC = GetHitsCollection(0, event);
    for (std::size_t n = 0; n < fNoOfCells; ++n) {
        fEdepVector[n] = (*HC)[n]->GetEdep();
        fSdepVector[n] = GetSdep((*HC)[n]);
    }

    //Add sums to Ntuple
    analysisManager->FillNtupleDColumn(2, std::accumulate(fEdepVector.begin(), fEdepVector.end(), 0));
    analysisManager->FillNtupleDColumn(3, std::accumulate(fSdepVector.begin(), fSdepVector.end(), 0));

    analysisManager->AddNtupleRow();

} 

//**************************************************
