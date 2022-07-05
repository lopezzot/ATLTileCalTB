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
ATLTileCalTBSensDet::ATLTileCalTBSensDet( const G4String& name, const G4String& hitsCollectionName )
    : G4VSensitiveDetector(name),
      fHitsCollection(nullptr) {
  
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
    for ( G4int i=0; i<ATLTileCalTBGeometry::cellNoSize; i++ ) {
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

    auto cellID = GetCellID( aStep );

    auto edep = aStep->GetTotalEnergyDeposit();
    if ( edep==0. ) return false; 
    G4double sdep = BirkLaw( aStep );
    //G4cout<<aStep->GetTrack()->GetParticleDefinition()->GetParticleName()<<" "<<edep<<" "<<sdep<<G4endl;
    //Get hit (just one for the moment)
    //
    auto hit = (*fHitsCollection)[cellID.to_cellNo()];
    if ( ! hit ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hit "; 
        G4Exception("ATLTileCalTBSensDet::ProcessHits()",
        "MyCode0004", FatalException, msg);
    }         

    //Add hit energy 
    //
    hit->AddE( sdep );
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

// GetCellID method
//
ATLTileCalTBGeometry::CellID ATLTileCalTBSensDet::GetCellID( const G4Step* aStep ) const {
    auto handle = aStep->GetPreStepPoint()->GetTouchableHandle();

    // Get scintillator and period copy number, identical everywhere
    G4int scintillator_copy_no = handle->GetVolume(0)->GetCopyNo();
    G4int period_copy_no = handle->GetVolume(2)->GetCopyNo();

    // Get module number via string, depends on module layout
    G4int module_no = -1;
    auto module_name = handle->GetVolume(5)->GetName();
    if ( module_name == "Tile::BarrelModule" ) {
        module_no = handle->GetVolume(5)->GetCopyNo();
    }
    else if ( module_name == "EBarrelPos" ) {
        module_no = 5;
    }
    else if ( module_name == "Tile::ITCModule" ) {
        auto plug_name = handle->GetVolume(4)->GetName();
        if ( plug_name == "Tile::Plug1Module" ) {
            module_no = 6;
        }
        else if ( plug_name == "Tile::Plug2Module" ) {
            module_no = 7;
        }
    }

    // lambda to get row and column number
    auto get_row_column = [](const G4int row_depth_no, const G4int row_length_no,
                             const std::vector<G4int>& row_depths,
                             const std::vector<std::vector<G4int>>& row_lengths) {
        // Get row
        std::size_t row_depth_idx = 0;
        for (; row_depth_idx < row_depths.size(); ++row_depth_idx) {
            if ( row_depth_no <= row_depths[row_depth_idx] ) {
                break;
            }
        }

        // Get column
        std::size_t row_length_idx = 0;
        for (; row_length_idx < row_lengths[row_depth_idx].size(); ++row_length_idx) {
            if ( row_length_no <= row_lengths[row_depth_idx][row_length_idx] ) {
                break;
            }
        }

        return std::pair<std::size_t, std::size_t>(row_depth_idx, row_length_idx);
    };

    G4int row_depth_idx = -1;
    G4int row_length_idx = -1;
    switch (module_no)
    {
    case 1: case 2: { // Long Barrel Module

        // Contains two half barrel, thus split period_copy_no and module_no
        if ( period_copy_no > 152 ) {
            // Right module, adjust period_copy_no and module_no
            period_copy_no = period_copy_no - 152;
            module_no += 2;
        }
        else {
            // Left module, adjust period_copy_no and flip orientation
            period_copy_no = 152 - period_copy_no;
        }

        // Get row and column
        auto idx = get_row_column(scintillator_copy_no, period_copy_no,
                                  ATLTileCalTBGeometry::lhb_row_depths,
                                  ATLTileCalTBGeometry::lhb_row_lengths);
        row_depth_idx = static_cast<G4int>(idx.first);
        row_length_idx = static_cast<G4int>(idx.second);

        // Row B and C are connected to form row BC
        if ( row_depth_idx == 2 ) row_depth_idx = 1;

        // Row A and BC start with index 1, only row D with index 0
        if ( row_depth_idx != 3) row_length_idx += 1;

        break;
    }
    case 5: { // Extended Barrel Module

        // Get row and column
        auto idx = get_row_column(scintillator_copy_no, period_copy_no,
                                  ATLTileCalTBGeometry::lhb_row_depths,
                                  ATLTileCalTBGeometry::lhb_row_lengths);
        row_depth_idx = static_cast<G4int>(idx.first);
        row_length_idx = static_cast<G4int>(idx.second);

        switch (row_depth_idx)
        {
        case 0: {  // Row A
            // Cells start with A12
            row_length_idx += 12;
            break;
        }
        case 1: { // Row B
            // Cells start with B11
            row_length_idx += 11;
            break;
        }
        case 2: { // Row D
            // Cells start with D5
            row_length_idx += 5;
            // Adjust to be interpreted as row D
            row_depth_idx = 3;
            break;
        }
        default:
            break;
        }

        break;
    }
    case 6: { // ITCModule Plug 1
        // Adjust to be interpreted as cell D4 from extended barrel
        module_no = 5;
        row_depth_idx = 3;
        row_length_idx = 4;

        break;
    }
    case 7: { // ITCModule Plug 2
        // Adjust to be interpreted as cell C10 from extended barrel
        module_no = 5;
        row_depth_idx = 2;
        row_length_idx = 10;

        break;
    }
    default:
        break;
    }

    ATLTileCalTBGeometry::CellID cellID = {module_no, row_depth_idx, row_length_idx};

    //G4cout << cellID.to_string() << " no " << std::to_string(cellID.to_cellNo()) << G4endl;

    return cellID;

}

//**************************************************
