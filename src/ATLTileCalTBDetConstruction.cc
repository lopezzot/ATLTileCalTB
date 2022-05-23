//**************************************************
// \file ATLTileCalTBDetConstruction.cc
// \brief: implementation of
//         ATLTileCalTBDetConstruction class
// \author: Original implementation
//          - Alexander Solodkov around 2000 
//          Implementation in ATLTileCalTB
//          - Lorenzo Pezzotti (CERN EP-SFT-sim) 
//            @lopezzot
// \start date: 23 May 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBDetConstruction.hh"
/*#include "TileTBMaterialManager.hh"
#include "TileTBParameterisedModule.hh"
#include "TileTBParameterisedScintillator.hh"
#include "TileTBParameterisedGirder.hh"
#include "TileTBCalorimeterSD.hh"
#include "TileGeometry.hh"*/

//Includers from Geant4
//
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"

#include "G4NistMaanger.hh"

//Includers from C++
//
#include <iostream.h>

//Constructors and de-constructor
//
ATLTileCalTBDetConstruction::ATLTileCalTBDetConstruction() {
    
    experimentalHall_x=1000.*cm;
    experimentalHall_y=1000.*cm;
    experimentalHall_z=2000.*cm;
    initialize_geometry_description();
    m_n_sections=0;

}

ATLTileCalTBDetConstruction::ATLTileCalTBDetConstruction(G4double x, G4double y, G4double z) {
    
    experimentalHall_x=x;
    experimentalHall_y=y;
    experimentalHall_z=z;
    initialize_geometry_description();
    m_n_sections=0;

}

ATLTileCalTBDetConstruction::~ATLTileCalTBDetConstruction()
{;}

G4VPhysicalVolume* ATLTileCalTBDetectorConstruction::Construct() {

    //Colours and vis attributes
    //
    G4VisAttributes* RedColor     = new G4VisAttributes(G4Color(1.,0.,0.));
    G4VisAttributes* GreenColor   = new G4VisAttributes(G4Color(0.,1.,0.));
    G4VisAttributes* BlueColor    = new G4VisAttributes(G4Color(0.,0.,1.));
    G4VisAttributes* CyanColor    = new G4VisAttributes(G4Color(0.,1.,1.));
    G4VisAttributes* MagentaColor = new G4VisAttributes(G4Color(1.,0.,1.));
    G4VisAttributes* YellowColor  = new G4VisAttributes(G4Color(1.,1.,0.));
    G4VisAttributes* XxxColor     = new G4VisAttributes(G4Color(0.,.5,.5));
    G4VisAttributes* PurpleColor  = new G4VisAttributes(G4Color(.5,0.,.5));
    G4VisAttributes* OrangeColor  = new G4VisAttributes(G4Color(.5,.5,0.));

    G4VisAttributes* HallColor       = XxxColor;    HallColor->SetVisibility(false);
    G4VisAttributes* SectionColor    = BlueColor;   SectionColor->SetVisibility(false);
    G4VisAttributes* WedgeColor      = BlueColor;   WedgeColor->SetVisibility(false);
    G4VisAttributes* FrontColor      = MagentaColor;FrontColor->SetVisibility(true);
    G4VisAttributes* EndColor        = PurpleColor; EndColor->SetVisibility(true);
    G4VisAttributes* SensColor       = RedColor;    SensColor->SetVisibility(true);
    G4VisAttributes* PeriodColor     = OrangeColor; PeriodColor->SetVisibility(false);
    G4VisAttributes* WrapColor       = YellowColor; WrapColor->SetVisibility(false);
    G4VisAttributes* ScinColor       = CyanColor;   ScinColor->SetVisibility(false);
    G4VisAttributes* GirderColor     = BlueColor;   GirderColor->SetVisibility(false);
    G4VisAttributes* GirderElemColor = GreenColor;  GirderElemColor->SetVisibility(true);
    
    //Materials
    //
    auto nistManager  = G4NistManager::Instance();
    auto Air          = nistManager->FindOrBuildMaterial("G4_Air");
    auto Iron         = nistManager->FindOrBuildMaterial("G4_Fe");
    auto C            = nistManager->FindOrBuildElement(6);
    auto H            = nistManager->FindOrBuildElement(1);
    auto Scintillator = new G4Material(name="Scintillator", density=1.032*g/cm3, ncomponents=2); 
    Scintillator->addElement("C", natoms=9);
    Scintillator->addElement("H", natoms=10);

    //The Experimental Hall (beam line along Z axis)
    //
    G4cout<<" Experimental Hall dimensions: "<<experimentalHall_x<<","<<
                                               experimentalHall_y<<","<<
                                               experimentalHall_z<<G4endl;

    G4Box* experimentalHall_box
        = new G4Box("expHall_box",experimentalHall_x/2,experimentalHall_y/2,
                                  experimentalHall_z/2);
    G4LogicalVolume* experimentalHall_log
        = new G4LogicalVolume(experimentalHall_box,Air,"expHall_log",0,0,0);

    G4VPhysicalVolume* experimentalHall_phys
        = new G4PVPlacement(0,G4ThreeVector(),"expHall",
                            experimentalHall_log,0,false,0);

    experimentalHall_log->SetVisAttributes(HallColor);

    solidWorld = experimentalHall_box;
    logicWorld = experimentalHall_log;
    physiWorld = experimentalHall_phys;

    return experimentalHall_phys;
}

//**************************************************
