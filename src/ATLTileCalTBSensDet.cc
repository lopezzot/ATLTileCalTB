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
    G4double sdep = BirkLaw( aStep );
    //G4cout<<aStep->GetTrack()->GetParticleDefinition()->GetParticleName()<<" "<<edep<<" "<<sdep<<G4endl;
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
    hit->SetHitName( "test" );  
    return true;

}

//EndOfEvent base method
//
void ATLTileCalTBSensDet::EndOfEvent(G4HCofThisEvent*) {
    
}

//BrikLaw method
//This method is taken from the ATLAS Athena offline software
//athena/TileCalorimeter/TileG4/TileGeoG4SD/src/TileGeoG4SDCalc.cc
//as on June 2022.
//
G4double ATLTileCalTBSensDet::BirkLaw( const G4Step* aStep ) const {

    /*----------------COMMENT FROM ATHENA---------------*/
    // *** apply BIRK's saturation law to energy deposition ***
    // *** only organic scintillators implemented in this version MODEL=1
    //
    // Note : the material is assumed ideal, which means that impurities
    //        and aging effects are not taken into account
    //
    // algorithm : edep = destep / (1. + RKB*dedx + C*(dedx)**2)
    //
    // the basic units of the coefficient are g/(MeV*cm**2)
    // and de/dx is obtained in MeV/(g/cm**2)
    //
    // exp. values from NIM 80 (1970) 239-244 :
    //
    // RKB = 0.013  g/(MeV*cm**2)  and  C = 9.6e-6  g**2/((MeV**2)(cm**4))
    /*---------------END OF COMMENT FROM ATHENA---------------*/

    const G4double destep = aStep->GetTotalEnergyDeposit() * aStep->GetTrack()->GetWeight();
    const G4Material* material = aStep->GetPreStepPoint()->GetMaterial();
    const G4double charge = aStep->GetPreStepPoint()->GetCharge();

    G4double response = 0.;
    G4double rkb = 0.02002 * CLHEP::g / (CLHEP::MeV * CLHEP::cm2);      //m_birk1 in athena
    G4double m_birk2 = 0.0 * CLHEP::g / (CLHEP::MeV * CLHEP::cm2) * CLHEP::g / (CLHEP::MeV * CLHEP::cm2);

    if ( charge != 0 && aStep->GetStepLength() != 0) {
        //Comment from atlas athena
        // --- correction for particles with more than 1 charge unit ---
        // --- based on alpha particle data (only apply for MODEL=1) ---
        if ( fabs(charge) > 1.0 ) { rkb *= 7.2 / 12.6; }
        const G4double dedx = destep / (aStep->GetStepLength()) / (material->GetDensity());
        response = destep / (1. + rkb * dedx + m_birk2 * dedx * dedx);
    }
    else { response = destep; }

    return response;

}

//**************************************************
