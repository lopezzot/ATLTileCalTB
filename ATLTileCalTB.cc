//**************************************************
// \file ATLTileCalTB.cc
// \brief: main() of ATLTileCalTB
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 23 May 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBDetConstruction.hh"
#include "ATLTileCalTBActInitialization.hh"

//Includers from Geant4
//
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
//#include "G4RunManagerFactory.hh" //only available from 10.7 on
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UIcommand.hh"
#include "G4GDMLParser.hh"

int main(int argc,char** argv) {
    
    //No arguments => set interactive mode and define UI session
    //
    G4UIExecutive* ui = nullptr;
    if ( argc == 1 ) { //no arguments
        ui = new G4UIExecutive(argc, argv);
    }

    //Construct the run manager
    //
    #ifdef G4MULTITHREADED
    auto runManager = new G4MTRunManager;
    G4int nThreads = 2; //default in MT mode
    if ( argc == 3 ) {  //parser option with nThreads
        nThreads = G4UIcommand::ConvertToInt( argv[2] );
    }
    runManager->SetNumberOfThreads( nThreads ); 
    #else
    auto runManager = new G4RunManager;
    #endif

    //Manadatory Geant4 classes
    //
    auto physicsList = new FTFP_BERT;
    runManager->SetUserInitialization(physicsList);
    G4GDMLParser parser;
    parser.Read("TileTB_2B1EB_nobeamline.gdml", false);
    runManager->SetUserInitialization(new ATLTileCalTBDetConstruction(parser));

    //Classes via ActionInitialization
    //
    runManager->SetUserInitialization(new ATLTileCalTBActInitialization());

    //Visualization manager construction
    //
    auto visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    //
    auto UImanager = G4UImanager::GetUIpointer();

    if ( !ui ) {
        // execute an argument macro file if exist (second parser argument)
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand("/process/em/verbose 0"); //avoid printing em processes
        UImanager->ApplyCommand("/process/had/verbose 0");//avoid printing had processes
        UImanager->ApplyCommand(command+fileName);
    }
    else {
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if (ui->IsGUI()) {
            UImanager->ApplyCommand("/control/execute gui.mac");
        }     
        // start interactive session
        ui->SessionStart();
        delete ui;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted 
    // in the main() program
    //
    delete visManager;
    delete runManager;
}

//**************************************************
