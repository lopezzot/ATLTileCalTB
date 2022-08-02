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
#include "G4ThreadLocalSingleton.hh"

//Includers from C++
//
#include <array>
#include <ostream>


namespace ATLTileCalTBGeometry {

    enum class Module {
        LONG_LOWER,
        LONG_UPPER,
        EXTENDED,
        EXTENDED_C10,
        EXTENDED_D4,
    };
    std::ostream& operator<<(std::ostream& ostream, const Module& module);

    enum class Row {
        A,
        B,
        BC,
        C,
        D,
    };
    std::ostream& operator<<(std::ostream& ostream, const Row& row);

    struct Cell {
        const Module module;
        const Row row;
        const int nCell;
        const std::size_t firstRow;
        const std::size_t lastRow;
        const std::array<const std::size_t, 6> nTilesRow;

        constexpr Cell(Module module_, Row row_, int nCell_,
             std::size_t firstRow_, std::size_t lastRow_,
             std::size_t nTilesRow0, std::size_t nTilesRow1,
             std::size_t nTilesRow2, std::size_t nTilesRow3,
             std::size_t nTilesRow4, std::size_t nTilesRow5)
            : module(module_), row(row_), nCell(nCell_), firstRow(firstRow_), lastRow(lastRow_),
              nTilesRow({nTilesRow0, nTilesRow1, nTilesRow2, nTilesRow3, nTilesRow4, nTilesRow5})
            {};
    };
    std::ostream& operator<<(std::ostream& ostream, const Cell& cell);

    class CellLUT {
        friend class G4ThreadLocalSingleton<CellLUT>;

        public:
            // Returns pointer to Singleton
            static CellLUT* GetInstance() {
                static G4ThreadLocalSingleton<CellLUT> instance {};
                return instance.Instance();
            }

            // Returns the total number of cells
            inline constexpr std::size_t GetNumberOfCells() const { return fNoOfCells; };

            // Finds the cell index given a module, the row index and the cell index
            std::size_t FindCellIndex(Module module, std::size_t rowIdx, std::size_t tileIdx) const;

            // Returns a constant reference of the cell corresponding to the cell index 
            inline constexpr Cell GetCell(std::size_t index) const { return fCellVector[index]; };

        private:
            // Private constructor
            CellLUT() = default;

            // Total numbers of cells
            static constexpr std::size_t fNoOfCells = 104;

            // Cell vector
            // https://atlas-geometry-db.web.cern.ch/atlas-geometry-db/node_tag_browser.php
            // TileCal -> TICL -> TICL-00
            static constexpr std::array<const Cell, fNoOfCells> fCellVector {
                // Lower long module
                Cell(Module::LONG_LOWER,   Row::A, -10,  1,  3, 16, 16, 16,  0,  0,  0), //   0
                Cell(Module::LONG_LOWER,   Row::A,  -9,  1,  3, 18, 19, 18,  0,  0,  0), //   1
                Cell(Module::LONG_LOWER,   Row::A,  -8,  1,  3, 18, 17, 18,  0,  0,  0), //   2
                Cell(Module::LONG_LOWER,   Row::A,  -7,  1,  3, 16, 16, 16,  0,  0,  0), //   3
                Cell(Module::LONG_LOWER,   Row::A,  -6,  1,  3, 15, 16, 15,  0,  0,  0), //   4
                Cell(Module::LONG_LOWER,   Row::A,  -5,  1,  3, 15, 15, 15,  0,  0,  0), //   5
                Cell(Module::LONG_LOWER,   Row::A,  -4,  1,  3, 14, 14, 14,  0,  0,  0), //   6
                Cell(Module::LONG_LOWER,   Row::A,  -3,  1,  3, 14, 14, 14,  0,  0,  0), //   7
                Cell(Module::LONG_LOWER,   Row::A,  -2,  1,  3, 14, 13, 14,  0,  0,  0), //   8
                Cell(Module::LONG_LOWER,   Row::A,  -1,  1,  3, 13, 14, 13,  0,  0,  0), //   9
                Cell(Module::LONG_LOWER,   Row::A,   1,  1,  3, 14, 13, 14,  0,  0,  0), //  10
                Cell(Module::LONG_LOWER,   Row::A,   2,  1,  3, 13, 14, 13,  0,  0,  0), //  11
                Cell(Module::LONG_LOWER,   Row::A,   3,  1,  3, 14, 14, 14,  0,  0,  0), //  12
                Cell(Module::LONG_LOWER,   Row::A,   4,  1,  3, 14, 14, 14,  0,  0,  0), //  13
                Cell(Module::LONG_LOWER,   Row::A,   5,  1,  3, 15, 15, 15,  0,  0,  0), //  14
                Cell(Module::LONG_LOWER,   Row::A,   6,  1,  3, 16, 15, 16,  0,  0,  0), //  15
                Cell(Module::LONG_LOWER,   Row::A,   7,  1,  3, 16, 16, 16,  0,  0,  0), //  16
                Cell(Module::LONG_LOWER,   Row::A,   8,  1,  3, 17, 18, 17,  0,  0,  0), //  17
                Cell(Module::LONG_LOWER,   Row::A,   9,  1,  3, 19, 18, 19,  0,  0,  0), //  18
                Cell(Module::LONG_LOWER,   Row::A,  10,  1,  3, 16, 16, 16,  0,  0,  0), //  19
                Cell(Module::LONG_LOWER,   Row::BC, -9,  4,  9, 18, 17, 18,  0,  0,  0), //  20
                Cell(Module::LONG_LOWER,   Row::BC, -8,  4,  9, 20, 20, 20, 20, 20, 20), //  21
                Cell(Module::LONG_LOWER,   Row::BC, -7,  4,  9, 18, 19, 18, 21, 22, 21), //  22
                Cell(Module::LONG_LOWER,   Row::BC, -6,  4,  9, 18, 18, 18, 21, 20, 21), //  23
                Cell(Module::LONG_LOWER,   Row::BC, -5,  4,  9, 17, 16, 17, 19, 19, 19), //  24
                Cell(Module::LONG_LOWER,   Row::BC, -4,  4,  9, 16, 17, 16, 19, 19, 19), //  25
                Cell(Module::LONG_LOWER,   Row::BC, -3,  4,  9, 16, 15, 16, 18, 18, 18), //  26
                Cell(Module::LONG_LOWER,   Row::BC, -2,  4,  9, 15, 16, 15, 18, 18, 18), //  27
                Cell(Module::LONG_LOWER,   Row::BC, -1,  4,  9, 16, 15, 16, 17, 18, 17), //  28
                Cell(Module::LONG_LOWER,   Row::BC,  1,  4,  9, 15, 16, 15, 18, 17, 18), //  29
                Cell(Module::LONG_LOWER,   Row::BC,  2,  4,  9, 16, 15, 16, 18, 18, 18), //  30
                Cell(Module::LONG_LOWER,   Row::BC,  3,  4,  9, 15, 16, 15, 18, 18, 18), //  31
                Cell(Module::LONG_LOWER,   Row::BC,  4,  4,  9, 17, 16, 17, 19, 19, 19), //  32
                Cell(Module::LONG_LOWER,   Row::BC,  5,  4,  9, 16, 17, 16, 19, 19, 19), //  33
                Cell(Module::LONG_LOWER,   Row::BC,  6,  4,  9, 18, 18, 18, 20, 21, 20), //  34
                Cell(Module::LONG_LOWER,   Row::BC,  7,  4,  9, 19, 18, 19, 22, 21, 22), //  35
                Cell(Module::LONG_LOWER,   Row::BC,  8,  4,  9, 20, 20, 20, 20, 20, 20), //  36
                Cell(Module::LONG_LOWER,   Row::BC,  9,  4,  9, 17, 18, 17,  0,  0,  0), //  37
                Cell(Module::LONG_LOWER,   Row::D,  -3, 10, 11, 50, 50,  0,  0,  0,  0), //  38
                Cell(Module::LONG_LOWER,   Row::D,  -2, 10, 11, 43, 43,  0,  0,  0,  0), //  39
                Cell(Module::LONG_LOWER,   Row::D,  -1, 10, 11, 41, 40,  0,  0,  0,  0), //  40
                Cell(Module::LONG_LOWER,   Row::D,   0, 10, 11, 40, 40,  0,  0,  0,  0), //  41
                Cell(Module::LONG_LOWER,   Row::D,   1, 10, 11, 40, 41,  0,  0,  0,  0), //  42
                Cell(Module::LONG_LOWER,   Row::D,   2, 10, 11, 43, 43,  0,  0,  0,  0), //  43
                Cell(Module::LONG_LOWER,   Row::D,   3, 10, 11, 50, 50,  0,  0,  0,  0), //  44
                // Upper long module
                Cell(Module::LONG_UPPER,   Row::A, -10,  1,  3, 16, 16, 16,  0,  0,  0), //  45
                Cell(Module::LONG_UPPER,   Row::A,  -9,  1,  3, 18, 19, 18,  0,  0,  0), //  46
                Cell(Module::LONG_UPPER,   Row::A,  -8,  1,  3, 18, 17, 18,  0,  0,  0), //  47
                Cell(Module::LONG_UPPER,   Row::A,  -7,  1,  3, 16, 16, 16,  0,  0,  0), //  48
                Cell(Module::LONG_UPPER,   Row::A,  -6,  1,  3, 15, 16, 15,  0,  0,  0), //  49
                Cell(Module::LONG_UPPER,   Row::A,  -5,  1,  3, 15, 15, 15,  0,  0,  0), //  50
                Cell(Module::LONG_UPPER,   Row::A,  -4,  1,  3, 14, 14, 14,  0,  0,  0), //  51
                Cell(Module::LONG_UPPER,   Row::A,  -3,  1,  3, 14, 14, 14,  0,  0,  0), //  52
                Cell(Module::LONG_UPPER,   Row::A,  -2,  1,  3, 14, 13, 14,  0,  0,  0), //  53
                Cell(Module::LONG_UPPER,   Row::A,  -1,  1,  3, 13, 14, 13,  0,  0,  0), //  54
                Cell(Module::LONG_UPPER,   Row::A,   1,  1,  3, 14, 13, 14,  0,  0,  0), //  55
                Cell(Module::LONG_UPPER,   Row::A,   2,  1,  3, 13, 14, 13,  0,  0,  0), //  56
                Cell(Module::LONG_UPPER,   Row::A,   3,  1,  3, 14, 14, 14,  0,  0,  0), //  57
                Cell(Module::LONG_UPPER,   Row::A,   4,  1,  3, 14, 14, 14,  0,  0,  0), //  58
                Cell(Module::LONG_UPPER,   Row::A,   5,  1,  3, 15, 15, 15,  0,  0,  0), //  59
                Cell(Module::LONG_UPPER,   Row::A,   6,  1,  3, 16, 15, 16,  0,  0,  0), //  60
                Cell(Module::LONG_UPPER,   Row::A,   7,  1,  3, 16, 16, 16,  0,  0,  0), //  61
                Cell(Module::LONG_UPPER,   Row::A,   8,  1,  3, 17, 18, 17,  0,  0,  0), //  62
                Cell(Module::LONG_UPPER,   Row::A,   9,  1,  3, 19, 18, 19,  0,  0,  0), //  63
                Cell(Module::LONG_UPPER,   Row::A,  10,  1,  3, 16, 16, 16,  0,  0,  0), //  64
                Cell(Module::LONG_UPPER,   Row::BC, -9,  4,  9, 18, 17, 18,  0,  0,  0), //  65
                Cell(Module::LONG_UPPER,   Row::BC, -8,  4,  9, 20, 20, 20, 20, 20, 20), //  66
                Cell(Module::LONG_UPPER,   Row::BC, -7,  4,  9, 18, 19, 18, 21, 22, 21), //  67
                Cell(Module::LONG_UPPER,   Row::BC, -6,  4,  9, 18, 18, 18, 21, 20, 21), //  68
                Cell(Module::LONG_UPPER,   Row::BC, -5,  4,  9, 17, 16, 17, 19, 19, 19), //  69
                Cell(Module::LONG_UPPER,   Row::BC, -4,  4,  9, 16, 17, 16, 19, 19, 19), //  70
                Cell(Module::LONG_UPPER,   Row::BC, -3,  4,  9, 16, 15, 16, 18, 18, 18), //  71
                Cell(Module::LONG_UPPER,   Row::BC, -2,  4,  9, 15, 16, 15, 18, 18, 18), //  72
                Cell(Module::LONG_UPPER,   Row::BC, -1,  4,  9, 16, 15, 16, 17, 18, 17), //  73
                Cell(Module::LONG_UPPER,   Row::BC,  1,  4,  9, 15, 16, 15, 18, 17, 18), //  74
                Cell(Module::LONG_UPPER,   Row::BC,  2,  4,  9, 16, 15, 16, 18, 18, 18), //  75
                Cell(Module::LONG_UPPER,   Row::BC,  3,  4,  9, 15, 16, 15, 18, 18, 18), //  76
                Cell(Module::LONG_UPPER,   Row::BC,  4,  4,  9, 17, 16, 17, 19, 19, 19), //  77
                Cell(Module::LONG_UPPER,   Row::BC,  5,  4,  9, 16, 17, 16, 19, 19, 19), //  78
                Cell(Module::LONG_UPPER,   Row::BC,  6,  4,  9, 18, 18, 18, 20, 21, 20), //  79
                Cell(Module::LONG_UPPER,   Row::BC,  7,  4,  9, 19, 18, 19, 22, 21, 22), //  80
                Cell(Module::LONG_UPPER,   Row::BC,  8,  4,  9, 20, 20, 20, 20, 20, 20), //  81
                Cell(Module::LONG_UPPER,   Row::BC,  9,  4,  9, 17, 18, 17,  0,  0,  0), //  82
                Cell(Module::LONG_UPPER,   Row::D,  -3, 10, 11, 50, 50,  0,  0,  0,  0), //  83
                Cell(Module::LONG_UPPER,   Row::D,  -2, 10, 11, 43, 43,  0,  0,  0,  0), //  84
                Cell(Module::LONG_UPPER,   Row::D,  -1, 10, 11, 41, 40,  0,  0,  0,  0), //  85
                Cell(Module::LONG_UPPER,   Row::D,   0, 10, 11, 40, 40,  0,  0,  0,  0), //  86
                Cell(Module::LONG_UPPER,   Row::D,   1, 10, 11, 40, 41,  0,  0,  0,  0), //  87
                Cell(Module::LONG_UPPER,   Row::D,   2, 10, 11, 43, 43,  0,  0,  0,  0), //  88
                Cell(Module::LONG_UPPER,   Row::D,   3, 10, 11, 50, 50,  0,  0,  0,  0), //  89
                // Extended module
                Cell(Module::EXTENDED,     Row::A,  12,  1,  3,  9,  9,  9,  0,  0,  0), //  90
                Cell(Module::EXTENDED,     Row::A,  13,  1,  3, 25, 25, 25,  0,  0,  0), //  91
                Cell(Module::EXTENDED,     Row::A,  14,  1,  3, 28, 28, 28,  0,  0,  0), //  92
                Cell(Module::EXTENDED,     Row::A,  15,  1,  3, 30, 30, 30,  0,  0,  0), //  93
                Cell(Module::EXTENDED,     Row::A,  16,  1,  3, 48, 48, 48,  0,  0,  0), //  94
                Cell(Module::EXTENDED,     Row::B,  11,  4,  7, 16, 16, 16, 16,  0,  0), //  95
                Cell(Module::EXTENDED,     Row::B,  12,  4,  7, 27, 27, 27, 27,  0,  0), //  96
                Cell(Module::EXTENDED,     Row::B,  13,  4,  7, 30, 30, 30, 30,  0,  0), //  97
                Cell(Module::EXTENDED,     Row::B,  14,  4,  7, 32, 32, 32, 32,  0,  0), //  98
                Cell(Module::EXTENDED,     Row::B,  15,  4,  7, 35, 35, 35, 35,  0,  0), //  99
                Cell(Module::EXTENDED,     Row::D,   5,  8, 11, 65, 65, 65, 65,  0,  0), // 100
                Cell(Module::EXTENDED,     Row::D,   6,  8, 11, 75, 75, 75, 75,  0,  0), // 101
                Cell(Module::EXTENDED_C10, Row::C,  10,  1,  3,  6,  5,  6,  0,  0,  0), // 102
                Cell(Module::EXTENDED_D4,  Row::D,   4,  4,  5, 17, 17,  0,  0,  0,  0), // 103
            };

        public:
            CellLUT(CellLUT const&) = delete;
            void operator=(CellLUT const&) = delete;
    };

}

#endif //ATLTileCalTBGeometry_h

//**************************************************
