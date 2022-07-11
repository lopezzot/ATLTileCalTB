//**************************************************
// \file ATLTileCalTBGeometry.cc
// \brief: implementation of ATLTileCalTBGeometry
//         namespace
// \author: Stephan Lachnit (CERN EP-SFT-sim) 
//          @stephanlachnit
// \start date: 4 July 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBGeometry.hh"

//Includers from C++
//
#include <sstream>

using namespace ATLTileCalTBGeometry;

std::ostream& ATLTileCalTBGeometry::operator<<(std::ostream& ostream, const Module& module) {
    switch (module) {
        case Module::LONG_LOWER:
            ostream << "lower long";
            break;
        case Module::LONG_UPPER:
            ostream << "upper long";
            break;
        case Module::EXTENDED:
        case Module::EXTENDED_C10:
        case Module::EXTENDED_D4:
            ostream << "extended";
            break;
    }
    ostream << " module";
    return ostream;
}

std::ostream& ATLTileCalTBGeometry::operator<<(std::ostream& ostream, const Row& row) {
    switch (row) {
        case Row::A:
            ostream << "A";
            break;
        case Row::B:
            ostream << "B";
            break;
        case Row::BC:
            ostream << "BC";
            break;
        case Row::C:
            ostream << "C";
            break;
        case Row::D:
            ostream << "D";
            break;
    }
    return ostream;
}

std::ostream& ATLTileCalTBGeometry::operator<<(std::ostream& ostream, const Cell& cell) {
    ostream << cell.module << " cell " << cell.row << cell.nCell;
    return ostream;
}

std::size_t CellLUT::FindCellIndex(Module module, std::size_t rowIdx, std::size_t tileIdx) const {
    constexpr std::size_t long_module_no_cells = 45;
    constexpr std::size_t long_module_row_A_lastrow = 3;
    constexpr std::size_t long_module_row_A_no_cells = 20;
    constexpr std::size_t long_module_row_BC_lastrow = 9;
    constexpr std::size_t long_module_row_BC_no_cells = 18;
    constexpr std::size_t extended_module_row_A_lastrow = 3;
    constexpr std::size_t extended_module_A_no_cells = 5;
    constexpr std::size_t extended_module_row_B_lastrow = 7;
    constexpr std::size_t extended_module_B_no_cells = 5;

    std::size_t index = 0;

    // Fast forward module
    switch (module) {
        case Module::LONG_LOWER:
            break;
        case Module::LONG_UPPER:
            index += long_module_no_cells;
            break;
        case Module::EXTENDED:
            index += 2 * long_module_no_cells;
            break;
        case Module::EXTENDED_C10:
            // not parsable with rowIdx and tileIdx, return directly
            return fNoOfCells - 2;
        case Module::EXTENDED_D4:
            // not parsable with rowIdx and tileIdx, return directly
            return fNoOfCells - 1;
    }

    // Row indecies start with 1 in ATLAS convention
    rowIdx += 1;

    // Fast forward row
    if ( module != Module::EXTENDED ) {
        if ( rowIdx > long_module_row_A_lastrow) {
            index += long_module_row_A_no_cells;
        }
        if ( rowIdx > long_module_row_BC_lastrow ) {
            index += long_module_row_BC_no_cells;
        }
    }
    else {
        // Note: ignore ITC module as handled before
        if ( rowIdx > extended_module_row_A_lastrow ) {
            index += extended_module_A_no_cells;
        }
        if ( rowIdx > extended_module_row_B_lastrow ) {
            index += extended_module_B_no_cells;
        }
    }

    // Get index for nTilesRow, stays constant in a row
    const std::size_t n_tiles_row_index = rowIdx - GetCell(index).firstRow;

    //G4cout << "row " << rowIdx << " fast-forwarded index " << index << " " << GetCell(index).to_string() << G4endl;

    // Count trough cells and comapre to periods
    --index;
    std::size_t counter = 0;
    std::size_t next_cell_count = 0;
    do {
        ++index;
        counter += next_cell_count;
        next_cell_count = GetCell(index).nTilesRow[n_tiles_row_index];
        //G4cout << "index " << index << " counter " << counter << " next_cell_count " << next_cell_count << " sum " << counter + next_cell_count << G4endl;
    }
    while ( tileIdx >= counter + next_cell_count );

    // Sanity check
    if (index >= fNoOfCells) {
        G4ExceptionDescription msg;
        msg << "Cell with index " << index << " does not exist, row index " << rowIdx << " and tile index " << tileIdx
            << " are probably out of range." << G4endl;
        G4Exception("ATLTileCalTBGeometry::CellLUT::FindCellIndex",
        "MyCode0007", FatalException, msg);
        return SIZE_MAX; // Return impossible size
    }

    //G4cout << GetCell(index).to_string() << G4endl;

    return index;
}
