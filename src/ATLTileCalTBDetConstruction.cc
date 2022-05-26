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

//Includers from Geant4
//
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VisAttributes.hh"

//Includers from C++
//
#include <iostream>
#include <vector>

//Constructors and de-constructor
//
ATLTileCalTBDetConstruction::ATLTileCalTBDetConstruction(const G4GDMLParser& parser) 
    : G4VUserDetectorConstruction(),
    fParser(parser)
{}

ATLTileCalTBDetConstruction::~ATLTileCalTBDetConstruction()
{}

G4VPhysicalVolume* ATLTileCalTBDetConstruction::Construct() {

    auto worldPV = fParser.GetWorldVolume();
    
    auto LVStore = G4LogicalVolumeStore::GetInstance();
    for(G4LogicalVolume* volume : *LVStore) {
        G4cout<<"volumeeeeeeee.: "<<volume->GetName()<<G4endl;
    }

    DefineVisAttributes();

    return worldPV;

}

void ATLTileCalTBDetConstruction::DefineVisAttributes() {

    auto CALOVisAttr = new G4VisAttributes();
    CALOVisAttr->SetVisibility( false );
    auto TileTBEnvVisAttr = new G4VisAttributes();
    TileTBEnvVisAttr->SetVisibility( false );
    auto BarrelVisAttr = new G4VisAttributes();
    BarrelVisAttr->SetVisibility( false );
    auto TileBarMod1VisAttr = new G4VisAttributes();
    TileBarMod1VisAttr->SetVisibility( false );
    auto FingerVisAttr = new G4VisAttributes();
    FingerVisAttr->SetForceSolid( true );
    FingerVisAttr->SetColor( G4Color::Grey() );
    FingerVisAttr->SetDaughtersInvisible( true );
    auto ScinVisAttr = new G4VisAttributes();
    ScinVisAttr->SetForceSolid( true );
    ScinVisAttr->SetColor( G4Color::Blue() );
    ScinVisAttr->SetDaughtersInvisible( true );
    auto AbsorberChildVisAttr = new G4VisAttributes();
    AbsorberChildVisAttr->SetForceSolid( true );
    AbsorberChildVisAttr->SetColor( G4Color::Green() );
    AbsorberChildVisAttr->SetDaughtersInvisible( true );

    auto LVStore = G4LogicalVolumeStore::GetInstance();
    for(G4LogicalVolume* volume : *LVStore) {
        if( volume->GetName()=="CALO::CALO" )      volume->SetVisAttributes( CALOVisAttr );
        if( volume->GetName()=="Tile::TileTBEnv" ) volume->SetVisAttributes( TileTBEnvVisAttr );
        if( volume->GetName()=="Barrel" )    { G4cout<<"ciaooooooooo"<<G4endl;      volume->SetVisAttributes( BarrelVisAttr );}
        if( volume->GetName()=="Tile::BarrelModule" ) volume->SetVisAttributes( TileBarMod1VisAttr );
        if( volume->GetName()=="Tile::Finger" ) volume->SetVisAttributes( FingerVisAttr );
        if( volume->GetName()=="Tile::EFinger" ) volume->SetVisAttributes( FingerVisAttr );
        if( volume->GetName()=="Tile::GirderMother" ) volume->SetVisAttributes( FingerVisAttr );
        if( volume->GetName()=="Tile::Scintillator" ) volume->SetVisAttributes( ScinVisAttr );
        if( volume->GetName()=="Tile::Barrel" ) volume->SetVisAttributes( AbsorberChildVisAttr );

    }
}

//**************************************************
