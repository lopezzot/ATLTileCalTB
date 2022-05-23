//**************************************************
// \file ATLTileCalTBPrimaryGenAction.cc
// \brief: implementation of 
//         ATLTileCalTBPrimaryGenAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 23 May 2022
//**************************************************

#include "ATLTileCalTBPrimaryGenAction.hh"

//Includers from Geant4
//
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

ATLTileCalTBPrimaryGenAction::ATLTileCalTBPrimaryGenAction()
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun( nullptr ) {
    
      fParticleGun = new G4ParticleGun(1); //set primary particle(s) to 1

      //default particle gun parameters (can be changed via UI)
      //
      auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle( "e-" );
      fParticleGun->SetParticleDefinition( particleDefinition );
      fParticleGun->SetParticleMomentumDirection( G4ThreeVector( 0.,0.,1. ) );
      fParticleGun->SetParticleEnergy( 1.*GeV );
      fParticleGun->SetParticlePosition( G4ThreeVector( 0.,0.,0. ) );
}

ATLTileCalTBPrimaryGenAction::~ATLTileCalTBPrimaryGenAction() {
    delete fParticleGun;
}

//Define GeneratePrimaries() method
//
void ATLTileCalTBPrimaryGenAction::GeneratePrimaries( G4Event* event){
    fParticleGun->GeneratePrimaryVertex( event );
}

//**************************************************
