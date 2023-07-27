//**************************************************
// \file ATLTileCalTB.cc
// \brief: main() of ATLTileCalTB
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 23 May 2022
//**************************************************

// Includers from project files
//
#include "ATLTileCalTBActInitialization.hh"
#include "ATLTileCalTBDetConstruction.hh"
#ifdef G4_USE_FLUKA
// include the FTFP_BERT PL custmized with fluka
// hadron inelastic process
#include "G4_CernFLUKAHadronInelastic_FTFP_BERT.hh"
#endif

// Includers from Geant4
//
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#include "G4Threading.hh"
#else
#include "G4RunManager.hh"
#endif
// #include "G4RunManagerFactory.hh" //only available from 10.7 on
#include "G4GDMLParser.hh"
#include "G4PhysListFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UIcommand.hh"
#include "G4UImanager.hh"
#include "G4Version.hh"
#include "G4VisExecutive.hh"
#if G4VERSION_NUMBER >= 1110 // >= Geant4-11.1.0
#include "G4FTFTunings.hh"
#endif

// Includers from FLUKAIntegration
//
#ifdef G4_USE_FLUKA
#include "FLUKAParticleTable.hh"
#endif

// CLI string outputs
namespace CLIOutputs {
void PrintHelp() {
  G4cout << "Usage: ATLTileCalTB [OPTION...]\n\n"
         << "Options:\n"
         << "  -m MACRO        path to macro file to run\n"
         << "  -u UISESSION    string of the Geant4 UI session to use\n"
         << "  -t THREADS      number of threads to use in the simulation\n"
         << "  -p PHYSICSLIST  string of the physics list to use\n"
         << "  -h              print this help and exit\n"
         << G4endl;
}
void PrintError() {
  G4cerr << "Wrong usage, see 'ATLTileCalTB -h' for more information" << G4endl;
}
} // namespace CLIOutputs

// G4err output for FTFTune usage error
//
namespace PrintFTFTuneUsageError {
void FTFTuneUsageError() {
  G4cerr << "Wrong FTF Alternative Tune Name selected. " << G4endl;
  G4cerr << "Geant4-11.1.0 valid indeces/names are: " << G4endl;
  G4cerr << "0(default)\n1(baryon-tune2022-v0)\n2(pion-tune2022-v0)\n3("
            "combined-tune2022-v0)"
         << G4endl;
}
} // namespace PrintFTFTuneUsageError

// G4err output for PhysListFactory usage error
//
namespace PrintPLFactoryUsageError {
void PLFactoryUsageError() {
  G4cerr << "Wrong PLFactory usage: no name for selected PL. " << G4endl;
}
} // namespace PrintPLFactoryUsageError

int main(int argc, char **argv) {

  // CLI variables
  G4String macro;
  G4String session;
  G4String custom_pl = "FTFP_BERT"; // default physics list
#ifdef G4MULTITHREADED
  G4int nThreads = G4Threading::G4GetNumberOfCores();
#endif

  // CLI parsing
  for (G4int i = 1; i < argc; i = i + 2) {
    if (G4String(argv[i]) == "-m")
      macro = argv[i + 1];
    else if (G4String(argv[i]) == "-u")
      session = argv[i + 1];
    else if (G4String(argv[i]) == "-p")
      custom_pl = argv[i + 1];
#ifdef G4MULTITHREADED
    else if (G4String(argv[i]) == "-t") {
      nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
    }
#endif
    else if (G4String(argv[i]) == "-h") {
      CLIOutputs::PrintHelp();
      return 0;
    } else {
      CLIOutputs::PrintError();
      return 1;
    }
  }

#ifndef G4_USE_FLUKA
#if G4VERSION_NUMBER >= 1110 // >= Geant4-11.1.0
  G4bool UseFTFTune = false;
  G4int FTFTuneIndex = 99;
  if (custom_pl.find("tune") != std::string::npos) {
    UseFTFTune = true;
    std::string pltune = custom_pl;
    char tuneidx = pltune.back();
    FTFTuneIndex = tuneidx - '0'; // convert char to int
    custom_pl = custom_pl.substr(0, custom_pl.size() - 6);
    G4cout << "---> Using FTF alternative tune index: " << FTFTuneIndex
           << " and PL: " << custom_pl << " <---" << G4endl;
  }
#endif
#endif

  // Activate interaction mode if no macro card is provided and define UI
  // session
  //
  G4UIExecutive *ui = nullptr;
  if (!macro.size()) { // if macro card is not passed
    ui = new G4UIExecutive(argc, argv, session);
  }

  // Construct the run manager
  //

#ifdef G4MULTITHREADED
  auto runManager = new G4MTRunManager;
  if (nThreads > 0) {
    runManager->SetNumberOfThreads(nThreads);
  }
#else
  auto runManager = new G4RunManager;
#endif

  // Manadatory Geant4 classes
  //

#ifndef G4_USE_FLUKA // build a standard Geant4 PL
  auto physListFactory = new G4PhysListFactory();
  if (!physListFactory->IsReferencePhysList(
          custom_pl)) { // if custom_pl is not a PLname exit
    PrintPLFactoryUsageError::PLFactoryUsageError();
    return 1;
  }
  auto physicsList = physListFactory->GetReferencePhysList(custom_pl);
  runManager->SetUserInitialization(physicsList);
#else // build the customized FTFP_BERT PL with Flula.Cern
  auto physList = new G4_CernFLUKAHadronInelastic_FTFP_BERT;
  runManager->SetUserInitialization(physList);
  // Initialize FLUKA <-> G4 particles conversions tables.
  fluka_particle_table::initialize();
#endif  // #ifndef G4_USE_FLUKA

#ifndef G4_USE_FLUKA 
  // Set FTF tunings (only => Geant4-11.1.0)
  //
#if G4VERSION_NUMBER >= 1110
  if (UseFTFTune) {
    auto FTFTunings = G4FTFTunings::Instance();
    if (FTFTuneIndex == 0)
      FTFTunings->SetTuneApplicabilityState(0, 1);
    else if (FTFTuneIndex == 1)
      FTFTunings->SetTuneApplicabilityState(1, 1);
    else if (FTFTuneIndex == 2)
      FTFTunings->SetTuneApplicabilityState(2, 1);
    else if (FTFTuneIndex == 3)
      FTFTunings->SetTuneApplicabilityState(3, 1);
    else {
      PrintFTFTuneUsageError::FTFTuneUsageError();
      return 1;
    }
  }
#endif // #if G4VERSION_NUMBER >= 1110
#endif // #ifndef G4_USE_FLUKA

  G4GDMLParser parser;
  parser.Read("TileTB_2B1EB_nobeamline.gdml", false);
  runManager->SetUserInitialization(new ATLTileCalTBDetConstruction(parser));

  // Classes via ActionInitialization
  //
  runManager->SetUserInitialization(new ATLTileCalTBActInitialization());

  // Visualization manager construction
  //
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  //
  auto UImanager = G4UImanager::GetUIpointer();

  if (!ui) {
    // execute an argument macro file if exist (second parser argument)
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(
        "/process/em/verbose 0"); // avoid printing em processes
    UImanager->ApplyCommand(
        "/process/had/verbose 0"); // avoid printing had processes
    UImanager->ApplyCommand(command + macro);
  } else {
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
