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

    // Sigma of the electronic noise (white noise / gaussian)
    constexpr G4double noise_sigma = (30 * MeV) * photoelectrons_per_energy;

    // Digitization: bin width of early hit frames
    constexpr G4double early_frame_bin_time = 0.5 * ns;

    // Digitization: time window where hit frames are marked as early
    constexpr G4double early_frame_time_window = 150 * ns;

    // Digitization: amount of early time frames
    constexpr std::size_t early_frames = static_cast<std::size_t>(early_frame_time_window / early_frame_bin_time);

    // Digitization: bin width of late hit frames
    constexpr G4double late_frame_bin_time = 5 * ns;

    // Digitization: time window where hit frames are marked as late
    constexpr G4double late_frame_time_window = 200 * ns;

    // Digitization: amount of late time frames
    constexpr std::size_t late_frames = static_cast<std::size_t>(late_frame_time_window / late_frame_bin_time);

    // Digitization: total time window
    constexpr G4double total_time_window = early_frame_time_window + late_frame_time_window;

    // Digitization: total amount of time frames
    constexpr std::size_t total_frames = early_frames + late_frames;

};

#endif //ATLTileCalTBConstants_h

//**************************************************
