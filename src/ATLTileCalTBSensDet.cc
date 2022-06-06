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
      fNofCells(nofCells) {
  
    collectionName.insert(hitsCollectionName);

}

ATLTileCalTBSensDet::~ATLTileCalTBSensDet() {}

//Intiliaze base method
//
void B4cCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  /*
  // Create hits collection
  fHitsCollection 
    = new B4cCalorHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  auto hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  // fNofCells for cells + one more for total sums 
  for (G4int i=0; i<fNofCells+1; i++ ) {
    fHitsCollection->insert(new B4cCalorHit());
  }
  */
}

//ProcessHits base method
//
G4bool B4cCalorimeterSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
  /*
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }

  if ( edep==0. && stepLength == 0. ) return false;      

  auto touchable = (step->GetPreStepPoint()->GetTouchable());
    
  // Get calorimeter cell id 
  auto layerNumber = touchable->GetReplicaNumber(1);
  
  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber; 
    G4Exception("B4cCalorimeterSD::ProcessHits()",
      "MyCode0004", FatalException, msg);
  }         

  // Get hit for total accounting
  auto hitTotal 
    = (*fHitsCollection)[fHitsCollection->entries()-1];
  
  // Add values
  hit->Add(edep, stepLength);
  hitTotal->Add(edep, stepLength); 
      
  return true;
  */
}

//EndOfEvent base method
//
void B4cCalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
  /*
  if ( verboseLevel>1 ) { 
     auto nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     for ( std::size_t i=0; i<nofHits; ++i ) (*fHitsCollection)[i]->Print();
  }
  */
}

//**************************************************
