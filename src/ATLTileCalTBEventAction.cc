//**************************************************
// \file ATLTileCalTBEventAction.cc
// \brief: implementation of ATLTileCalTBEventAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) 
//          @lopezzot
// \start date: 7 June 2022
//**************************************************

//Includers from project files
//
#include "ATLTileCalTBEventAction.hh"
#include "ATLTileCalTBGeometry.hh"
#include "ATLTileCalTBConstants.hh"

//Includers from Geant4
//
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"
#include "Randomize.hh"
#ifdef ATLTileCalTB_PulseOutput
#include "G4RunManager.hh"
#include "G4Run.hh"
#endif

//Includers from C++
//
#include <iomanip>
#include <numeric>
#include <algorithm>
#ifdef ATLTileCalTB_PulseOutput
#include <fstream>
#include <sstream>
#endif

//Constructor and de-constructor
//
ATLTileCalTBEventAction::ATLTileCalTBEventAction()
    : G4UserEventAction(),
      fNoOfCells(ATLTileCalTBGeometry::CellLUT::GetInstance()->GetNumberOfCells()),
      fAux{0., 0.} {
    fEdepVector = std::vector<G4double>(fNoOfCells, 0.);
    fSdepVector = std::vector<G4double>(fNoOfCells, 0.);
}

ATLTileCalTBEventAction::~ATLTileCalTBEventAction() {
}

//BeginOfEvent() method
//
void ATLTileCalTBEventAction::BeginOfEventAction(const G4Event* /*event*/) {  
  
    for ( auto& value : fAux ){ value = 0.; } 
    for ( auto& value : fEdepVector ) { value = 0.; }
    for ( auto& value : fSdepVector ) { value = 0.; }

}

//GetHitsCollection method()
//
ATLTileCalTBHitsCollection* ATLTileCalTBEventAction::GetHitsCollection(G4int hcID,
                                                                       const G4Event* event) const {
    auto hitsCollection = static_cast<ATLTileCalTBHitsCollection*>( event->GetHCofThisEvent()->GetHC(hcID) );
  
    if ( ! hitsCollection ) {
        G4ExceptionDescription msg;
        msg << "Cannot access hitsCollection ID " << hcID; 
        G4Exception("ATLTileCalTBEventAction::GetHitsCollection()",
        "MyCode0003", FatalException, msg);
    } 

    return hitsCollection;

}    

//EndOfEventaction() method
//
void ATLTileCalTBEventAction::EndOfEventAction( const G4Event* event ) {

    auto analysisManager = G4AnalysisManager::Instance();

    G4int counter = 0;
    for ( auto& value : fAux ){ 
        analysisManager->FillNtupleDColumn( counter, value );    
        counter++;
    }

    //Method to create a file containing the pulse
    #ifdef ATLTileCalTB_PulseOutput
    auto CreatePulseOutput = [event]
    (std::array<G4double, ATLTileCalTBConstants::total_frames> sdep_up,
     std::array<G4double, ATLTileCalTBConstants::total_frames> sdep_down,
     std::size_t cell_index) {
        // Add signals
        std::array<G4double, ATLTileCalTBConstants::total_frames> sdep;
        for (std::size_t n = 0; n < sdep.size(); ++n) {
            sdep[n] = sdep_up[n] + sdep_down[n];
        }

        // Check that vector is not empty
        if (std::accumulate(sdep.begin(), sdep.end(), 0) == 0.) return;

        // Generate file name
        auto runNumber = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
        auto eventNumber = event->GetEventID();
        auto cell = ATLTileCalTBGeometry::CellLUT::GetInstance()->GetCell(cell_index);
        std::string dirName1 = "ATLTileCalTBpulse_Run" + std::to_string(runNumber);
        std::string dirName2 = dirName1 + "/Ev" + std::to_string(eventNumber);
        std::ostringstream fileName;
        fileName << dirName2 << "/Mod";
        switch (cell.module) {
            case ATLTileCalTBGeometry::Module::LONG_LOWER:
                fileName << "LL";
                break;
            case ATLTileCalTBGeometry::Module::LONG_UPPER:
                fileName << "LU";
                break;
            case ATLTileCalTBGeometry::Module::EXTENDED:
            case ATLTileCalTBGeometry::Module::EXTENDED_C10:
            case ATLTileCalTBGeometry::Module::EXTENDED_D4:
                fileName << "EX";
                break;
        }
        fileName << "_Cell" << cell.row << cell.nCell << ".dat";

        // Create folder
        mkdir(dirName1.c_str(), 0777);
        mkdir(dirName2.c_str(), 0777);

        // Open file and add cell label
        std::ofstream ofs;
        ofs.open(fileName.str());
        ofs << "# " << cell << "\n";

        // Fill with values and close
        for (auto val : sdep) {
            ofs << val << "\n";
        }
        ofs.close();
    };
    #else
    auto CreatePulseOutput = [](std::array<G4double, ATLTileCalTBConstants::total_frames>, std::array<G4double, ATLTileCalTBConstants::total_frames>, std::size_t) {};
    #endif

    //Method to convolute signal for PMT response
    //From https://gitlab.cern.ch/allpix-squared/allpix-squared/-/blob/86fe21ad37d353e36a509a0827562ab7fadd5104/src/modules/CSADigitizer/CSADigitizerModule.cpp#L271-L283
    auto ConvolutePMT = [](const std::vector<G4double>& sdep) {
        // TODO: check algorithm
        auto outvec = std::array<G4double, ATLTileCalTBConstants::total_frames>();
        for (std::size_t k = 0; k < outvec.size(); ++k) {
            G4double outsum = 0.;
            std::size_t jmin = (k >= sdep.size() - 1) ? k - (sdep.size() - 1) : 0;
            std::size_t jmax = (k >= ATLTileCalTBConstants::pmt_response.size()) ? ATLTileCalTBConstants::pmt_response.size() - 1 : k;
            for (std::size_t j = jmin; j <= jmax; ++j) {
                if ( (k - j) < sdep.size() ) {
                    outsum += sdep.at(k - j) * ATLTileCalTBConstants::pmt_response.at(j);
                }
            }
            outvec.at(k) = outsum;
        }
        return outvec;
    };

    //Method to get sdep from hit
    auto GetSdep = [ConvolutePMT, CreatePulseOutput]
    (const ATLTileCalTBHitsCollection* HC, std::size_t cell_index) -> G4double {
        auto hit = (*HC)[cell_index];

        //PMT response
        auto sdep_up_v = ConvolutePMT(hit->GetSdepUp());
        auto sdep_down_v = ConvolutePMT(hit->GetSdepDown());

        //Create output pulses if requested
        CreatePulseOutput(sdep_up_v, sdep_down_v, cell_index);

        //Use maximum as signal
        G4double sdep_up = *(std::max_element(sdep_up_v.begin(), sdep_up_v.end()));
        G4double sdep_down = *(std::max_element(sdep_down_v.begin(), sdep_down_v.end()));

        //Apply electronic noise
        sdep_up += G4RandGauss::shoot(0., ATLTileCalTBConstants::noise_sigma);
        sdep_down += G4RandGauss::shoot(0., ATLTileCalTBConstants::noise_sigma);

        //Return sum
        return sdep_up + sdep_down;
    };

    //Get hits collections and fill vector
    auto HC = GetHitsCollection(0, event);
    for (std::size_t n = 0; n < fNoOfCells; ++n) {
        fEdepVector[n] = (*HC)[n]->GetEdep();
        fSdepVector[n] = GetSdep(HC, n);
    }

    //Add sums to Ntuple
    analysisManager->FillNtupleDColumn(2, std::accumulate(fEdepVector.begin(), fEdepVector.end(), 0));
    analysisManager->FillNtupleDColumn(3, std::accumulate(fSdepVector.begin(), fSdepVector.end(), 0));

    analysisManager->AddNtupleRow();

} 

//**************************************************
