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
#include "ATLTileCalTBPrimaryGenAction.hh"
#ifdef ATLTileCalTB_LEAKANALYSIS
#include "SpectrumAnalyzer.hh"
#endif

//Includers from Geant4
//
#include "G4Event.hh"
#include "Randomize.hh"
#include "G4ParticleGun.hh"
#include "G4Version.hh"
#if G4VERSION_NUMBER < 1100
#include "g4root.hh"  // replaced by G4AnalysisManager.h  in G4 v11 and up
#else
#include "G4AnalysisManager.hh"
#endif
#ifdef ATLTileCalTB_PulseOutput
#include "G4RunManager.hh"
#include "G4Run.hh"
#endif

//Includers from C++
//
#include <numeric>
#include <algorithm>
#ifdef ATLTileCalTB_PulseOutput
#include <fstream>
#include <sstream>
#include <filesystem>
#endif

//Constructor and de-constructor
//
ATLTileCalTBEventAction::ATLTileCalTBEventAction(ATLTileCalTBPrimaryGenAction* pga)
    : G4UserEventAction(),
      fPrimaryGenAction(pga),
      fNoOfCells(ATLTileCalTBGeometry::CellLUT::GetInstance()->GetNumberOfCells()),
      fAux{0., 0.} {
    fEdepVector = std::vector<G4double>(fNoOfCells, 0.);
    fSdepVector = std::vector<G4double>(fNoOfCells, 0.);
}

ATLTileCalTBEventAction::~ATLTileCalTBEventAction() {
}

//BeginOfEvent() method
//
void ATLTileCalTBEventAction::BeginOfEventAction([[maybe_unused]] const G4Event* event) {
    for ( auto& value : fAux ){ value = 0.; } 
    for ( auto& value : fEdepVector ) { value = 0.; }
    for ( auto& value : fSdepVector ) { value = 0.; }

    #ifdef ATLTileCalTB_PulseOutput
    auto runNumber = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
    auto eventNumber = event->GetEventID();
    pulse_event_path = std::filesystem::path("ATLTileCalTBpulse_Run" + std::to_string(runNumber) + "/Ev" + std::to_string(eventNumber));
    std::filesystem::create_directory(pulse_event_path);
    #endif
    
    #ifdef ATLTileCalTB_LEAKANALYSIS
    SpectrumAnalyzer::GetInstance()->ClearEventFields();
    #endif
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

    //Method to convolute signal for PMT response
    //From https://gitlab.cern.ch/allpix-squared/allpix-squared/-/blob/86fe21ad37d353e36a509a0827562ab7fadd5104/src/modules/CSADigitizer/CSADigitizerModule.cpp#L271-L283
    auto ConvolutePMT = [](const std::array<G4double, ATLTileCalTBConstants::frames>& sdep) {
        constexpr auto pmt_response_size = ATLTileCalTBConstants::pmt_response.size();
        auto outvec = std::array<G4double, ATLTileCalTBConstants::frames>();
        for (std::size_t k = 0; k < outvec.size(); ++k) {
            G4double outsum = 0.;
            auto jmax = (k >= pmt_response_size) ? pmt_response_size - 1 : k;
            for (std::size_t j = 0; j <= jmax; ++j) {
                outsum += sdep[k - j] * ATLTileCalTBConstants::pmt_response[j];
            }
            outvec[k] = outsum;
        }
        return outvec;
    };

    //Method to get sdep from hit
    auto GetSdep = [ConvolutePMT, this]
    (const ATLTileCalTBHitsCollection* HC, std::size_t cell_index) -> G4double {
        auto hit = (*HC)[cell_index];

        //PMT response
        auto sdep_up_v = ConvolutePMT(hit->GetSdepUp());
        auto sdep_down_v = ConvolutePMT(hit->GetSdepDown());

        //Create output pulses if requested
        #ifdef ATLTileCalTB_PulseOutput
        {
            // Add signals
            std::array<G4double, ATLTileCalTBConstants::frames> sdep_sum_v;
            for (std::size_t n = 0; n < sdep_sum_v.size(); ++n) {
                sdep_sum_v[n] = sdep_up_v[n] + sdep_down_v[n];
            }

            // Check that vector is not empty
            if (std::accumulate(sdep_sum_v.begin(), sdep_sum_v.end(), 0) != 0.) {
                // Generate file name
                auto cell = ATLTileCalTBGeometry::CellLUT::GetInstance()->GetCell(cell_index);
                std::ostringstream fileName;
                fileName << pulse_event_path.string() << "/Mod";
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

                // Open file and add cell label
                std::ofstream ofs;
                ofs.open(fileName.str());
                ofs << "# " << cell << "\n";

                // Fill with values and close
                for (auto val : sdep_sum_v) {
                    ofs << val << "\n";
                }
                ofs.close();
            }
        };
        #endif

        //Use maximum as signal
        G4double sdep_up = *(std::max_element(sdep_up_v.begin(), sdep_up_v.end()));
        G4double sdep_down = *(std::max_element(sdep_down_v.begin(), sdep_down_v.end()));

        #ifdef ATLTileCalTB_NoNoise
        return sdep_up + sdep_down;
        #else
        //Apply electronic noise
        sdep_up += G4RandGauss::shoot(0., ATLTileCalTBConstants::signal_noise_sigma);
        sdep_down += G4RandGauss::shoot(0., ATLTileCalTBConstants::signal_noise_sigma);

        //Return sum if signal is larger than 2 * noise
        auto sdep_sum = sdep_up + sdep_down;
        return (sdep_sum > 2 * ATLTileCalTBConstants::signal_noise_sigma) ? sdep_sum : 0.;
        #endif
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

    analysisManager->FillNtupleIColumn(6, fPrimaryGenAction->GetParticlenGun()->GetParticleDefinition()->GetPDGEncoding());
    analysisManager->FillNtupleFColumn(7, fPrimaryGenAction->GetParticlenGun()->GetParticleEnergy());

    analysisManager->AddNtupleRow();
    
    #ifdef ATLTileCalTB_LEAKANALYSIS
    SpectrumAnalyzer::GetInstance()->FillEventFields();
    #endif
} 

//**************************************************
