//**************************************************
// \file ATLTileCalTBHit.cc
// \brief: implementation of ATLTileCalTBHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 6 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBHit.hh"
#include "ATLTileCalTBConstants.hh"

//Includers from Geant4
#include "G4UnitsTable.hh"

//Constructor and de-constructor
//
ATLTileCalTBHit::ATLTileCalTBHit()
    : G4VHit(),
      fEdep(0.),
      fSdepUp(),
      fSdepDown() {
    fSdepUp.fill(0.);
    fSdepDown.fill(0.);

}

ATLTileCalTBHit::~ATLTileCalTBHit() {}

ATLTileCalTBHit::ATLTileCalTBHit(const ATLTileCalTBHit& right)
    : G4VHit() {
    fEdep = right.fEdep;
    fSdepUp = std::array<G4double, ATLTileCalTBConstants::frames>(right.fSdepUp);
    fSdepDown = std::array<G4double, ATLTileCalTBConstants::frames>(right.fSdepDown);

}

//Operator = definition
//
const ATLTileCalTBHit& ATLTileCalTBHit::operator=(const ATLTileCalTBHit& right) {
  
    fEdep = right.fEdep;
    fSdepUp = std::array<G4double, ATLTileCalTBConstants::frames>(right.fSdepUp);
    fSdepDown = std::array<G4double, ATLTileCalTBConstants::frames>(right.fSdepDown);

    return *this;

}

//Operator == definition
//
G4bool ATLTileCalTBHit::operator==(const ATLTileCalTBHit& right) const {
  
    return ( this == &right ) ? true : false;

}

//ATLTileCalTBHit::GetBinFromTime method
//
std::size_t ATLTileCalTBHit::GetBinFromTime( G4double time ) {
    using namespace ATLTileCalTBConstants;
    if ( time < frame_time_window ) {
        return static_cast<std::size_t>(std::ceil(time / frame_bin_time));
    }
    else {
        G4ExceptionDescription msg;
        msg << "Time " << G4BestUnit(time, "Time") << " is above the total time window." << G4endl; 
        G4Exception("ATLTileCalTBHit::GetBinFromTime()",
        "MyCode0008", FatalException, msg);
        return SIZE_MAX; // Return impossible size
    }
}

//**************************************************
