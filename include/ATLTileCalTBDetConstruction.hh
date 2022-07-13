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

//Forward declaration from Geant4
//
class G4GDMLParser;
class G4VPhysicalVolume;

class ATLTileCalTBDetConstruction : public G4VUserDetectorConstruction {
    
    public:
        ATLTileCalTBDetConstruction  (const G4GDMLParser& parser);
        ~ATLTileCalTBDetConstruction ();
        virtual G4VPhysicalVolume* Construct();
        virtual void ConstructSDandField();

    private:
        const G4GDMLParser& fParser;
        void DefineVisAttributes();

};

#endif //ATLTileCalTBDetConstruction_h

//**************************************************
