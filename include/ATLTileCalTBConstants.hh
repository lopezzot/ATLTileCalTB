//**************************************************
// \file ATLTileCalTBConstants.hh
// \brief: definition of ATLTileCalTBConstants
//         namespace
// \author: Stephan Lachnit (CERN EP-SFT-sim) 
//          @stephanlachnit
// \start date: 7 July 2022
//**************************************************


#ifndef ATLTileCalTBConstants_h
#define ATLTileCalTBConstants_h 1

//Includers from Geant4
//
#include "globals.hh"
#include "G4SystemOfUnits.hh"


namespace ATLTileCalTBConstants {

    // Amount of energy deposited in the tiles compared to the total energy in the calorimeter
    constexpr G4double sampling_fraction = 3.5 * perCent;

    // Amount of photoelectrons created per energy (adjusted by the sampling fraction)
    constexpr G4double photoelectrons_per_energy = 70. / GeV / sampling_fraction;

};

#endif //ATLTileCalTBConstants_h

//**************************************************
