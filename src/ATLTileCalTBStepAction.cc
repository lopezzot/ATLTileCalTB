//**************************************************
// \file ATLTileCalTBStepAction.cc
// \brief: implementation of ATLTileCalTBStepAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 8 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBStepAction.hh"
#ifdef ATLTileCalTB_LEAKANALYSIS
#include "SpectrumAnalyzer.hh"
#endif

//Constructor and de-constructor
//
ATLTileCalTBStepAction::ATLTileCalTBStepAction(ATLTileCalTBEventAction* EventAction)
    : G4UserSteppingAction(),
      fEventAction( EventAction ){}

ATLTileCalTBStepAction::~ATLTileCalTBStepAction() {}

//UserSteppingaction() method
//
void ATLTileCalTBStepAction::UserSteppingAction( const G4Step* aStep ) {
    
    //Collect out of world leakage
    //
    if ( !aStep->GetTrack()->GetNextVolume() ){
        fEventAction->Add( 0, aStep->GetTrack()->GetKineticEnergy() ); 
        #ifdef ATLTileCalTB_LEAKANALYSIS
        SpectrumAnalyzer::GetInstance()->Analyze(aStep);
        #endif
    }

    if ( aStep->GetTrack()->GetTouchableHandle()->GetVolume()->GetName() != "CALO::CALO" ||
         aStep->GetTrack()->GetTouchableHandle()->GetVolume()->GetName() != "Barrel" ) {
 
        //Collect calo energy deposition (everything but what goes into CALO::CALO and Barrel) 
        //Warning: not exact measurement
        //
        fEventAction->Add( 1, aStep->GetTotalEnergyDeposit() );
    
    }

}

//**************************************************
