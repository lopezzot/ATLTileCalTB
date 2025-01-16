//**************************************************
// \file ATLTileCalTBRunAction.hh
// \brief: definition of ATLTileCalTBRunAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 6 June 2022
//**************************************************

#ifndef ATLTileCalTBRunAction_h
#define ATLTileCalTBRunAction_h 1

//Includers from Geant4
//
#include "G4UserRunAction.hh"
#include "G4Timer.hh"

//Forward declaration from project
//
class ATLTileCalTBEventAction;

//Forward declaration from Geant4
//
class G4Run;

class ATLTileCalTBRunAction : public G4UserRunAction {
  
    public:
        ATLTileCalTBRunAction( ATLTileCalTBEventAction* eventAction );
        virtual ~ATLTileCalTBRunAction();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);

    private:
        ATLTileCalTBEventAction* fEventAction;
        G4Timer fTimer;

};

#endif //ATLTileCalTBRunAction_h 1

//**************************************************
