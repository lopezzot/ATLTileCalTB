//**************************************************
// \file ATLTileCalTBDetConstruction.cc
// \brief: implementation of
//         ATLTileCalTBDetConstruction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 23 May 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBDetConstruction.hh"
#include "ATLTileCalTBSensDet.hh"

//Includers from Geant4
//
#include "G4LogicalVolume.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

//Constructors and de-constructor
//
ATLTileCalTBDetConstruction::ATLTileCalTBDetConstruction(const G4GDMLParser& parser) 
    : G4VUserDetectorConstruction(),
    fParser(parser)
{}

ATLTileCalTBDetConstruction::~ATLTileCalTBDetConstruction()
{}

//Construct() method
//
G4VPhysicalVolume* ATLTileCalTBDetConstruction::Construct() {

    auto worldPV = fParser.GetWorldVolume();
    
    DefineVisAttributes();

    return worldPV;

}

//ConstructSDandField() method
//
void ATLTileCalTBDetConstruction::ConstructSDandField() {

    //Sensitive detectors
    //
    auto caloSD = new ATLTileCalTBSensDet( "caloSD", "caloHitsCollection" );
    G4SDManager::GetSDMpointer()->AddNewDetector( caloSD );

    //Assign to logical volumes
    //
    auto LVStore = G4LogicalVolumeStore::GetInstance();
    for(auto volume : *LVStore) {

        if( volume->GetName()=="Tile::Scintillator" ) volume->SetSensitiveDetector( caloSD );
    
    }

    //No fields involved

}

//DefineVisAttributes() method
//
void ATLTileCalTBDetConstruction::DefineVisAttributes() {

    //Create vis attributes
    //
    auto CALOVisAttr = new G4VisAttributes();      //CALO::CALO invisible
    CALOVisAttr->SetVisibility( false );
    auto TileTBEnvVisAttr = new G4VisAttributes(); //Tile::TileTBEnv invisible
    TileTBEnvVisAttr->SetVisibility( false );
    //auto TileVisAttr = new G4VisAttributes();    //Tile::Scintillator blue
    //TileVisAttr->SetForceSolid( true );
    //TileVisAttr->SetColor( G4Color::Blue() );
    //TileVisAttr->SetDaughtersInvisible( true );
    auto FingerVisAttr = new G4VisAttributes();    //Tile::Finger grey
    FingerVisAttr->SetForceSolid( true );          //Tile::EFinger grey
    FingerVisAttr->SetColor( G4Color::Grey() );    //Tile::GirderMother grey
    FingerVisAttr->SetDaughtersInvisible( true );
    auto AbsorberVisAttr = new G4VisAttributes();  //Tile::Absorber magenta
    AbsorberVisAttr->SetForceSolid( true );
    AbsorberVisAttr->SetColor( G4Color::Cyan() );
    AbsorberVisAttr->SetDaughtersInvisible( true );

    //Assign vis attributes
    //
    auto LVStore = G4LogicalVolumeStore::GetInstance();
    for(auto volume : *LVStore) {
            
        if( volume->GetName()=="CALO::CALO" )           volume->SetVisAttributes( CALOVisAttr );
        if( volume->GetName()=="Tile::TileTBEnv" )      volume->SetVisAttributes( TileTBEnvVisAttr );
        //if( volume->GetName()=="Tile::Scintillator" ) volume->SetVisAttributes( TileVisAttr );
        if( volume->GetName()=="Tile::Finger" )         volume->SetVisAttributes( FingerVisAttr );
        if( volume->GetName()=="Tile::EFinger" )        volume->SetVisAttributes( FingerVisAttr );
        if( volume->GetName()=="Tile::GirderMother" )   volume->SetVisAttributes( FingerVisAttr );
        if( volume->GetName()=="Tile::Absorber" )       volume->SetVisAttributes( AbsorberVisAttr );

    }
}

//**************************************************
