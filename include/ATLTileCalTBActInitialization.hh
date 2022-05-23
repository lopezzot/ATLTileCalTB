//**************************************************
// \file ATLTileCalTBActInitialization.hh
// \brief: definition of ATLTileCalTBActInitialization
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 23 May 2022
//**************************************************

#ifndef ATLTileCalTBActInitialization_h
#define ATLTileCalTBActInitialization_h 1

//Includers from Geant4
//
#include "G4VUserActionInitialization.hh"

class ATLTileCalTBActInitialization : public G4VUserActionInitialization {
    
    public:
        ATLTileCalTBActInitialization();
        virtual ~ATLTileCalTBActInitialization();
        
        virtual void BuildForMaster() const;
        virtual void Build() const;

};

#endif //ATLTileCalTBActInitialization_h

//**************************************************
