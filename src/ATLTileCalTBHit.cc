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

//Includers from Geant4
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator<ATLTileCalTBHit>* ATLTileCalTBHitAllocator = 0;

//Constructor and de-constructor
//
ATLTileCalTBHit::ATLTileCalTBHit()
    : G4VHit(),
      fEdep(0.),
      fSdep(0.) {
}

ATLTileCalTBHit::~ATLTileCalTBHit() {}

ATLTileCalTBHit::ATLTileCalTBHit(const ATLTileCalTBHit& right)
    : G4VHit() {
  
    fEdep = right.fEdep;
    fSdep = right.fSdep;

}

//Operator = definition
//
const ATLTileCalTBHit& ATLTileCalTBHit::operator=(const ATLTileCalTBHit& right) {
  
    fEdep = right.fEdep;
    fSdep = right.fSdep;

    return *this;

}

//Operator == definition
//
G4bool ATLTileCalTBHit::operator==(const ATLTileCalTBHit& right) const {
  
    return ( this == &right ) ? true : false;

}

//**************************************************
