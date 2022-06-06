//**************************************************
// \file ATLTileCalTBSensDet.cc
// \brief: implementation of ATLTileCalTBSensDet
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 6 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBSensDet.hh"

//Includers from Geant4
//
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//Constructor and de-constructor
//
ATLTileCalTBSensDet::ATLTileCalTBSensDet( const G4String& name, const G4String& hitsCollectionName, G4int nCells )
    : G4VSensitiveDetector(name),
      fHitsCollection(nullptr),
      fNCells(nCells) {
  
    collectionName.insert(hitsCollectionName);

}

ATLTileCalTBSensDet::~ATLTileCalTBSensDet() {}

//Intiliaze base method
//
void ATLTileCalTBSensDet::Initialize(G4HCofThisEvent* hce) {
  
    //Create hits collection
    //
    fHitsCollection = new ATLTileCalTBHitsCollection( SensitiveDetectorName, collectionName[0] ); 

    //Add this collection in the hits collection of this event
    //
    auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection ); 

    //Allocate hits in hce (currently just one hit)
    //
    for ( G4int i=0; i<fNCells; i++ ) {
        fHitsCollection->insert(new ATLTileCalTBHit());
    }

}

//ProcessHits base method
//
G4bool ATLTileCalTBSensDet::ProcessHits( G4Step* aStep, G4TouchableHistory* ) {
  
    //Print out some info step-by-step in sensitive elements
    //
    //G4cout<<"Track #: "<< aStep->GetTrack()->GetTrackID()<< " " <<
    //        "Step #: " << aStep->GetTrack()->GetCurrentStepNumber()<< " "<<
    //        "Volume: " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()<< " " << G4endl;
    //G4cout<<"x: "<< aStep->GetPreStepPoint()->GetPosition().x() <<
    //        "y: "<< aStep->GetPreStepPoint()->GetPosition().y() <<
    //        "z: "<< aStep->GetPreStepPoint()->GetPosition().z() << G4endl;
    //G4cout<<"Particle "<< aStep->GetTrack()->GetParticleDefinition()->GetParticleName()<< " " <<
    //        "Dep(MeV) "<< aStep->GetTotalEnergyDeposit() << " " <<
    //        "Mat "     << aStep->GetPreStepPoint()->GetMaterial()->GetName() << " " << 
    //        "Vol "     << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << G4endl; 
    
    auto edep = aStep->GetTotalEnergyDeposit();
    if ( edep==0. ) return false; 

    //Get hit (just one for the moment)
    //
    auto hit = (*fHitsCollection)[0];
    if ( ! hit ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hit "; 
        G4Exception("ATLTileCalTBSensDet::ProcessHits()",
        "MyCode0004", FatalException, msg);
    }         

    //Add hit energy 
    //
    hit->AddE( edep );
      
    return true;

}

//EndOfEvent base method
//
void ATLTileCalTBSensDet::EndOfEvent(G4HCofThisEvent*) {
    
    auto hit = (*fHitsCollection)[0];
    G4cout<<"Energy in scintillator (all) " << hit->GetEdep() << G4endl; 

}

//**************************************************
