//**************************************************
// \file ATLTileCalTBDetConstruction.hh
// \brief: definition of ATLTileCalTBDetConstruction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 23 May 2022
//**************************************************

#ifndef ATLTileCalTBDetConstruction_h
#define ATLTileCalTBDetConstruction_h 1

//Includers from Geant4
//
#include "G4VUserDetectorConstruction.hh"
class G4GDMLParser;
class G4VPhysicalVolume;

class ATLTileCalTBDetConstruction : public G4VUserDetectorConstruction {
    
    public:
        ATLTileCalTBDetConstruction  (const G4GDMLParser& parser);
        ~ATLTileCalTBDetConstruction ();

    private:
        const G4GDMLParser& fParser;
        G4VPhysicalVolume* Construct();
        void DefineVisAttributes();

};

#endif //ATLTileCalTBDetConstruction_h

//**************************************************
