//**************************************************
// \file ATLTileCalTBGeometry.hh
// \brief: definition of ATLTileCalTBGeometry
//         namespace
// \author: Stephan Lachnit (CERN EP-SFT-sim) 
//          @stephanlachnit
// \start date: 4 July 2022
//**************************************************

#ifndef ATLTileCalTBGeometry_h
#define ATLTileCalTBGeometry_h 1

//Includers from Geant4
//
#include "globals.hh"

//Includers from C++
//
#include <vector>


namespace ATLTileCalTBGeometry {

    // Long half barrel

    // Copy number 0..10 for scintillators ("depth")
    const std::vector<G4int> lhb_row_depths = {2, 5, 8, 10};

    // Copy number 0..152 for periods ("length")
    const std::vector<G4int> lhb_row_A_lengths = {14, 29, 44, 59, 74, 89, 104, 121, 137, 152};
    const std::vector<G4int> lhb_row_B_lengths = {16,  33,  50,  67,  84, 101, 118, 135, 152};
    const std::vector<G4int> lhb_row_C_lengths = {18,  37,  56,  75,  94, 113, 132, 152};
    const std::vector<G4int> lhb_row_D_lengths = {21, 64, 108, 152};
    const std::vector<std::vector<G4int>> lhb_row_lengths = {
        lhb_row_A_lengths,
        lhb_row_B_lengths,
        lhb_row_C_lengths,
        lhb_row_D_lengths
    };

    // Extended barrel

    // Copy number 0..10 for scintillators ("depth")
    const std::vector<G4int> eb_row_depths = {2, 6, 10};

    // Copy number 0..139 for periods ("length")
    const std::vector<G4int> eb_row_A_lengths = {13, 41, 69, 97, 139};
    const std::vector<G4int> eb_row_B_lengths = {15, 46, 77, 108, 139};
    const std::vector<G4int> eb_row_D_lengths = {65, 139};
    const std::vector<std::vector<G4int>> eb_row_lengths = {
        lhb_row_A_lengths,
        lhb_row_B_lengths,
        lhb_row_D_lengths
    };

    // CellID struct
    struct CellID {
        CellID( G4int module_no, G4int row_no, G4int row_idx );
        CellID( G4int cell_no );
        G4int module_no;
        G4int row_no;
        G4int row_idx;
        std::size_t to_cellNo() const;
        std::string to_string() const;
    };

    // Total number of cells, calculated via module 5 cell D6
    const std::size_t cellNoSize = CellID(5, 3, 6).to_cellNo() + 1;
};

#endif //ATLTileCalTBGeometry_h

//**************************************************
