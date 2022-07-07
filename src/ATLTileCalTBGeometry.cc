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
#include <string>

// ATLTileCalTBGeometry::CellID(G4int, G4int, G4int) constructor
//
ATLTileCalTBGeometry::CellID::CellID( G4int module_no_, G4int row_no_, G4int row_idx_ )
    : module_no(module_no_),
      row_no(row_no_),
      row_idx(row_idx_)
{}

// ATLTileCalTBGeometry::CellID(G4int) constructor
// This is the inverse of the to_cellNo() method.
ATLTileCalTBGeometry::CellID::CellID( G4int cell_no )
{
    // FIXME: to be implemented
    module_no = 0;
    row_no = 0;
    row_idx = 0;
}

// ATLTileCalTBGeometry::CellID::to_cellNo method
// This method converts from the struct to a strictly orderd cell number.
// These cell numbers can be used e.g. for indexing arrays.
std::size_t ATLTileCalTBGeometry::CellID::to_cellNo() const {
    constexpr uint lhb_row_size[] = {10, 9, 0, 4};
    constexpr uint lhb_row_offset[] = {1, 1, 0, 0};
    constexpr uint lhb_total_size = lhb_row_size[0] + lhb_row_size[1] + lhb_row_size[3];

    constexpr uint eb_row_size[] {5, 5, 1, 3};
    constexpr uint eb_row_offset[] = {12, 11, 10, 4};

    std::size_t ret = static_cast<std::size_t>((module_no - 1) * lhb_total_size);

    if (module_no != 5) {
        ret += row_idx - lhb_row_offset[row_no];
        for ( std::size_t n = 0; n < static_cast<std::size_t>(row_no); ++n ) {
            ret += lhb_row_size[n];
        }
    }
    else {
        ret += row_idx - eb_row_offset[row_no];
        for ( std::size_t n = 0; n < static_cast<std::size_t>(row_no); ++n ) {
            ret += eb_row_size[n];
        }
    }

    return ret;
};

// ATLTileCalTBGeometry::CellID::to_string method
//
std::string ATLTileCalTBGeometry::CellID::to_string() const {
    std::string ret;

    ret += "module " + std::to_string(module_no) + " cellID ";

    switch (row_no)
    {
    case 0:
        ret += "A";
        break;
    case 1:
        ret += "B";
        break;
    case 2:
        ret += "C";
        break;
    case 3:
        ret += "D";
        break;
    default:
        break;
    }

    if ( module_no != 5 && row_no == 1 ) {
        ret += "C";
    }

    ret += std::to_string(row_idx);

    ret += " cellNo ";
    ret += std::to_string(to_cellNo());

    return ret;
}
