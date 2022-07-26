#include <array>
#include <string>
#include <sstream>
#include <tuple>
#include <numeric>

#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TFitResult.h>
#include <TGraphErrors.h>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RResultPtr.hxx>
#include <ROOT/RDF/RInterface.hxx>

struct ValErr {
    double value;
    double error;
};

const std::array<double, 4> BEAM_ENERGIES {16., 18., 20., 30.};
const std::string RUN_FILE_PATH_PREFIX {"ATLTileCalTBout_Run"};
const std::string RUN_FILE_PATH_POSTFIX {".root"};
const std::string RUN_FILE_TTREE_NAME {"ATLTileCalTBout"};
const int PDG_ID_EL = 11;
const int PDG_ID_PI = -211;

const std::array<double, 4> ATL_BEAM_ENERGIES {16., 18., 20., 30.};
const std::array<ValErr, ATL_BEAM_ENERGIES.size()> ATL_RATIO_PIE {
    ValErr({0.7924, 0.0116}),
    ValErr({0.7941, 0.0108}),
    ValErr({0.7948, 0.0101}),
    ValErr({0.8019, 0.0098}),
};


void TBrun_all() {
    ROOT::EnableImplicitMT();

    // Create TChain with input files    
    TChain chain {RUN_FILE_TTREE_NAME.c_str()};
    for(std::size_t n = 0; n < 2 * BEAM_ENERGIES.size(); ++n) {
        auto file_name = RUN_FILE_PATH_PREFIX + std::to_string(n) + RUN_FILE_PATH_POSTFIX;
        chain.Add(file_name.c_str());
    }

    // Create output file and RDataFrame
    TFile output {"analysis.root", "RECREATE"};
    ROOT::RDataFrame rdf {chain};

    // Create default canvas
    TCanvas canvas {"canvas", "canvas", -1280, 720};

    // Book electron / pion filters
    auto rdf_el = rdf.Filter("PDGID=="+std::to_string(PDG_ID_EL));
    auto rdf_pi = rdf.Filter("PDGID=="+std::to_string(PDG_ID_PI));

    // Book Sdep histograms
    auto book_sdep_hist = [](auto rdfi, const double beam_energy) {
        auto beam_energy_f = static_cast<float>(beam_energy * 1e3);
        return rdfi.Filter("EBeam=="+std::to_string(beam_energy_f)).Histo1D("SdepSum");
    };
    std::array<ROOT::RDF::RResultPtr<TH1D>, BEAM_ENERGIES.size()> th1s_sdep_el, th1s_sdep_pi;
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        th1s_sdep_el[n] = book_sdep_hist(rdf_el, BEAM_ENERGIES[n]);
        th1s_sdep_pi[n] = book_sdep_hist(rdf_pi, BEAM_ENERGIES[n]);
    }

    // Fit Sdep histograms
    auto fit_sdep_hist = [](TH1* th1ptr, const double beam_energy, const std::string& name) {
        auto fitres = th1ptr->Fit("gaus", "SQ").Get();
        auto mean = fitres->Parameter(1);
        auto error = fitres->Parameter(2) / sqrt(th1ptr->GetEntries());
        std::ostringstream write_name;
        write_name << "Signal " << name << " " << beam_energy << " GeV";
        th1ptr->SetTitle((write_name.str()+";Signal [a.u.];count").c_str());
        th1ptr->Write(write_name.str().c_str());
        return ValErr({mean, error});
    };
    std::array<ValErr, BEAM_ENERGIES.size()> sdep_res_el, sdep_res_pi;
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        sdep_res_el[n] = fit_sdep_hist(th1s_sdep_el[n].GetPtr(), BEAM_ENERGIES[n], "electrons");
        sdep_res_pi[n] = fit_sdep_hist(th1s_sdep_pi[n].GetPtr(), BEAM_ENERGIES[n], "pions");
    }

    // Create R graphs
    auto r_graph = [](std::array<ValErr, BEAM_ENERGIES.size()> sdep_res, const std::string& name) {
        std::array<double, BEAM_ENERGIES.size()> r_means, r_errors;
        for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
           r_means[n] = sdep_res[n].value / BEAM_ENERGIES[n];
           r_errors[n] = sdep_res[n].error / BEAM_ENERGIES[n];
        }
        auto tge = TGraphErrors(BEAM_ENERGIES.size(), BEAM_ENERGIES.data(), r_means.data(), nullptr, r_errors.data());
        tge.SetTitle(("R " + name + ";Beam Energy [GeV];R [a.u./GeV]").c_str());
        tge.Write(("R " + name).c_str());
        return std::make_tuple(r_means, r_errors);
    };
    auto r_res_el = r_graph(sdep_res_el, "electrons");
    auto r_res_pi = r_graph(sdep_res_pi, "pions");

    // Book EM-Scale histograms
    auto book_ems_hist = [](auto rdfi, const double beam_energy) {
        auto beam_energy_f = static_cast<float>(beam_energy * 1e3);
        return rdfi.Filter("EBeam=="+std::to_string(beam_energy_f)).Histo1D("SdepSumEMScale");
    };
    auto r_means_el = std::get<0>(r_res_el);
    double r_mean_el = std::reduce(r_means_el.begin(), r_means_el.end()) / r_means_el.size();  // TODO: error of r_mean_el?
    auto rdf_pi_ems = rdf_pi.Define("SdepSumEMScale", "SdepSum/"+std::to_string(r_mean_el));
    std::array<ROOT::RDF::RResultPtr<TH1D>, BEAM_ENERGIES.size()> th1s_ems;
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        th1s_ems[n] = book_ems_hist(rdf_pi_ems, BEAM_ENERGIES[n]);
    }

    // Fit EM-Scale histograms
    auto fit_ems_hist = [](TH1* th1ptr, const double beam_energy, const std::string& name) {
        auto fitres = th1ptr->Fit("gaus", "SQ").Get();
        auto mean = fitres->Parameter(1) / beam_energy;
        auto error = fitres->Parameter(2)  / beam_energy / sqrt(th1ptr->GetEntries());
        std::ostringstream write_name;
        write_name << "Signal EM-Scale " << name << " " << beam_energy << " GeV";
        th1ptr->SetTitle((write_name.str()+";Energy at EM-Scale [GeV];count").c_str());
        th1ptr->Write(write_name.str().c_str());
        return ValErr({mean, error});
    };
    std::array<ValErr, BEAM_ENERGIES.size()> ems_res_pi;
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        ems_res_pi[n] = fit_ems_hist(th1s_ems[n].GetPtr(), BEAM_ENERGIES[n], "pions");
    }

    // Create x/e ratio graph
    auto xer_graph = [](std::array<ValErr, BEAM_ENERGIES.size()> ems_res, const std::string& name) {
        std::array<double, BEAM_ENERGIES.size()> ems_means, ems_errors;
        for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
           ems_means[n] = ems_res[n].value;
           ems_errors[n] = ems_res[n].error;
        }
        auto tge = TGraphErrors(BEAM_ENERGIES.size(), BEAM_ENERGIES.data(), ems_means.data(), nullptr, ems_errors.data());
        tge.SetTitle((name + " Ratio;Beam Energy [GeV];Ratio").c_str());
        tge.Write((name + " Ratio").c_str());
    };
    xer_graph(ems_res_pi, "pi/e");

    // Create x/e ratio graph (ATLAS data)
    auto atl_xer_graph = [](std::array<ValErr, ATL_BEAM_ENERGIES.size()> ratios, const std::string& name) {
        std::array<double, ATL_BEAM_ENERGIES.size()> ratio_means, ratio_errors;
        for (std::size_t n = 0; n < ATL_BEAM_ENERGIES.size(); ++n) {
           ratio_means[n] = ratios[n].value;
           ratio_errors[n] = ratios[n].error;
        }
        auto tge = TGraphErrors(BEAM_ENERGIES.size(), BEAM_ENERGIES.data(), ratio_means.data(), nullptr, ratio_errors.data());
        tge.SetTitle(("ATLAS " + name + " Ratio;Beam Energy [GeV];Ratio").c_str());
        tge.Write(("ATLAS " + name + " Ratio").c_str());
    };
    atl_xer_graph(ATL_RATIO_PIE, "pi/e");

    // Write and close
    output.Write();
    output.Close();
}
