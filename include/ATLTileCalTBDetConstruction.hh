//**************************************************
// \file ATLTileCalTBDetConstruction.hh
// \brief: definition of ATLTileCalTBDetConstruction
//         class
// \author: Original implementation
//          - Alexander Solodkov around 2000 
//          Implementation in ATLTileCalTB
//          - Lorenzo Pezzotti (CERN EP-SFT-sim) 
//            @lopezzot
// \start date: 23 May 2022
//**************************************************

#ifndef ATLTileCalTBDetConstruction_h
#define ATLTileCalTBDetConstruction_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4VUserDetectorConstruction.hh"

#include "TileGeometry.hh"
//#include "TileTBCalorimeterSD.hh"

class G4VPhysicalVolume;

class ATLTileCalTBDetConstruction : public G4VUserDetectorConstruction {
    
    public:
        ATLTileCalTBDetConstruction  ();
        ATLTileCalTBDetConstruction  (G4double x, G4double y, G4double z);
        ~ATLTileCalTBDetConstruction ();

        void set_Hall_x (G4double x) {experimentalHall_x=x;}
        void set_Hall_y (G4double y) {experimentalHall_y=y;}
        void set_Hall_z (G4double z) {experimentalHall_z=z;}

        virtual G4VPhysicalVolume* Construct    ();
        const G4LogicalVolume*     GetSection   (G4int n)       {return logicSection[n];}
        const G4LogicalVolume*     GetModule    (G4int n)       {return logicModule[n];}
        const G4LogicalVolume*     GetGirder    (G4int n)       {return logicGirder[n];}
        const G4LogicalVolume*     GetInstrum   (G4int n)       {return logicInstrum[n];}
        const G4LogicalVolume*     GetAbsorber  (G4int n)       {return logicAbsorber[n];}
        const TileCell *           GetCell      (int sec)       {return tilegeo->section(sec)->tilecell();}
        const G4int                n_sections   ()              {return m_n_sections;}
#ifdef ARRAYS_2D
	const G4LogicalVolume*     GetWrapping  (G4int n, G4int s) {return logicWrapping[n,s];}
        const G4LogicalVolume*     GetScin      (G4int n, G4int s) {return logicScin[n,s];}
        const G4LogicalVolume*     GetSensitive (G4int n, G4int s) {return logicSens[n,s];}
        const G4int                n_sensvols   (G4int n)          {return m_n_sensvols[n];}
#endif

private:
        void initialize_geometry_description ();

        TileGeometry* tilegeo;
        G4double      experimentalHall_x;
        G4double      experimentalHall_y;
        G4double      experimentalHall_z;

#define NSECTION 10
#define NSCINTILLATOR 12
#define NSENSVOLUME   25

        G4Box *                 solidWorld;             //pointer to the solid World
        G4LogicalVolume *       logicWorld;             //pointer to the logical World
        G4VPhysicalVolume *     physiWorld;             //pointer to the physical World

        G4Tubs *                solidSection[NSECTION]; //pointer to the solid World
        G4LogicalVolume *       logicSection[NSECTION]; //pointer to the logical Calorimeter Section
        G4VPhysicalVolume *     physiSection[NSECTION]; //pointer to the physical Calorimeter Section
     
        G4Trd *                 solidModule[NSECTION];  //pointer to the solid Module
        G4LogicalVolume *       logicModule[NSECTION];  //pointer to the logical Module
        G4VPhysicalVolume *     physiModule[NSECTION];  //pointer to the physical Module
     
        G4Trd *                 solidGirder[NSECTION];  //pointer to the solid Girder
        G4LogicalVolume *       logicGirder[NSECTION];  //pointer to the logical Girder
        G4VPhysicalVolume *     physiGirder[NSECTION];  //pointer to the physical Girder

        G4Trd *                 solidInstrum[NSECTION]; //pointer to the solid instrumented part
        G4LogicalVolume *       logicInstrum[NSECTION]; //pointer to the logical instrumented part
        G4VPhysicalVolume *     physiInstrum[NSECTION]; //pointer to the physical instrumented part

        G4Trd *                 solidAbsorber[NSECTION];//pointer to the solid Absorber (1 period)
        G4LogicalVolume *       logicAbsorber[NSECTION];//pointer to the logical Absorber (1 period)
        G4VPhysicalVolume *     physiAbsorber[NSECTION];//pointer to the physical Absnorber (1 period)
     
#ifdef ARRAYS_2D
#ifdef __sun
        G4Trd *                 solidWrapping[NSECTION*NSCINTILLATOR];//pointer to the solid Wrapping
        G4LogicalVolume *       logicWrapping[NSECTION*NSCINTILLATOR];//pointer to the logical Wrapping
	G4VPhysicalVolume *     physiWrapping[NSECTION*NSCINTILLATOR];//pointer to the physical Wrapping

        G4Trd *                 solidScin[NSECTION*NSCINTILLATOR];//pointer to the solid Scintillator
        G4LogicalVolume *       logicScin[NSECTION*NSCINTILLATOR];//pointer to the logical Scintillator
        G4VPhysicalVolume *     physiScin[NSECTION*NSCINTILLATOR];//pointer to the physical Scintillator

        G4LogicalVolume *       logicSens[NSECTION*NSENSVOLUME];//pointer to the sensitive logical volumes
#else
        G4Trd *                 solidWrapping[NSECTION, NSCINTILLATOR];//pointer to the solid Wrapping
        G4LogicalVolume *       logicWrapping[NSECTION, NSCINTILLATOR];//pointer to the logical Wrapping
	G4VPhysicalVolume *     physiWrapping[NSECTION, NSCINTILLATOR];//pointer to the physical Wrapping

        G4Trd *                 solidScin[NSECTION, NSCINTILLATOR];//pointer to the solid Scintillator
        G4LogicalVolume *       logicScin[NSECTION, NSCINTILLATOR];//pointer to the logical Scintillator
        G4VPhysicalVolume *     physiScin[NSECTION, NSCINTILLATOR];//pointer to the physical Scintillator

        G4LogicalVolume *       logicSens[NSECTION, NSENSVOLUME];//pointer to the sensitive logical volumes
#endif
        G4int                   m_n_sensvols[NSECTION];
#endif
        G4int                   m_n_sections;
    
        // TileTBDetectorMessenger * detectorMessenger;    //pointer to the Messenger
        //TileTBCalorimeterSD *     calorimeterSD;        //pointer to the sensitive detector
};

#endif //ATLTileCalTBDetConstruction_h

//**************************************************
