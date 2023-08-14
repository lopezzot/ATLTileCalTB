#include <array>
#include <string>
#include <sstream>
#include <tuple>
#include <numeric>
#include <cmath>
#include <ios>
#include <iostream>
#include <algorithm>

#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <Rtypes.h>
#include <ROOT/RVec.hxx>
#include <ROOT/RDataFrame.hxx>
#include <Math/MinimizerOptions.h>

struct ValErr {
    double value;
    double error;
};
struct GausFitRes {
    ValErr mean;
    ValErr sigma;
    double entries;
};
using RDFI = ROOT::RDF::RInterface<ROOT::Detail::RDF::RNodeBase, void>;

// Constants
constexpr std::size_t N_BEAM_ENERGIES = 4;
const std::array<double, N_BEAM_ENERGIES> BEAM_ENERGIES {16., 18., 20., 30.};
template<typename T> using BEarray = std::array<T, N_BEAM_ENERGIES>;
std::string MERGED_RUN_FILE {"ATLTileCalTBout_RunAll.root"};
const std::string RUN_FILE_TTREE_NAME {"ATLTileCalTBout"};
const int PDG_ID_EL = 11;
const int PDG_ID_PI = 211;
const int PDG_ID_K = 321;
const int PDG_ID_P = 2212;
const double EMSCALE_MUON_ERAW_CUT_GEV = 5.;
const double EMSCALE_ELECTRON_CLONG_CUT = 0.6;
const double EMSCALE_ELECTRON_CTOT_CUT = 0.125;

// ATLAS data
constexpr std::size_t N_ATL_BEAM_ENERGIES = 4;
const std::array<double, N_ATL_BEAM_ENERGIES> ATL_BEAM_ENERGIES {16., 18., 20., 30.};
template<typename T> using ATLBEarray = std::array<T, N_ATL_BEAM_ENERGIES>;
const ATLBEarray<ValErr> ATL_ERESPONSE_PI {
    ValErr({0.7924, 0.0116}),
    ValErr({0.7941, 0.0108}),
    ValErr({0.7948, 0.0101}),
    ValErr({0.8019, 0.0098}),
};
const ATLBEarray<ValErr> ATL_ERESOLUTION_PI {
    ValErr({0.1258, 0.0039}),
    ValErr({0.1188, 0.0023}),
    ValErr({0.1159, 0.0013}),
    ValErr({0.0987, 0.0006}),
};
const ATLBEarray<ValErr> ATL_ERESPONSE_K {
    ValErr({0.7682, 0.0184}),
    ValErr({0.7714, 0.0113}),
    ValErr({0.7723, 0.0095}),
    ValErr({0.7748, 0.0094}),
};
const ATLBEarray<ValErr> ATL_ERESOLUTION_K {
    ValErr({0.1356, 0.0276}),
    ValErr({0.1209, 0.0126}),
    ValErr({0.1131, 0.0019}),
    ValErr({0.0930, 0.0012}),
};
const ATLBEarray<ValErr> ATL_ERESPONSE_P {
    ValErr({0.7195, 0.0086}),
    ValErr({0.7288, 0.0087}),
    ValErr({0.7303, 0.0088}),
    ValErr({0.7549, 0.0091}),
};
const ATLBEarray<ValErr> ATL_ERESOLUTION_P {
    ValErr({0.1122, 0.0004}),
    ValErr({0.1055, 0.0005}),
    ValErr({0.1024, 0.0008}),
    ValErr({0.0877, 0.0004}),
};


// Returns a RResultPtr to sdep histogram that filters after beam energy
inline ROOT::RDF::RResultPtr<TH1D> book_sdep_hist(ROOT::RDF::TH1DModel th1dm_sdep,
                                                  RDFI rdfi,
                                                  const double beam_energy) {
    auto filter_str = "EBeam=="+std::to_string(static_cast<float>(beam_energy * 1e3));
    return rdfi.Filter(filter_str).Histo1D(th1dm_sdep, "SdepSum");
}


// Fits sdep histogram
GausFitRes fit_sdep_hist(TF1& tf1_gaus, TH1* th1ptr,
                         const double beam_energy,
                         const std::string& name) {
    auto th1_mean = th1ptr->GetMean();
    auto th1_2std = 2 * th1ptr->GetStdDev();
    tf1_gaus.SetParameter(0, 0.95 * th1ptr->GetMaximum());
    tf1_gaus.SetParameter(1, th1_mean);
    tf1_gaus.SetParameter(2, 0.45 * th1_2std);
    // Fit only in two sigma range
    tf1_gaus.SetRange(th1_mean-th1_2std, th1_mean+th1_2std);
    th1ptr->Fit(&tf1_gaus, "RQN");
    // Second fit with adjusted two sigma range
    auto fit1_mean = tf1_gaus.GetParameter(1);
    auto fit1_2std = 2 * tf1_gaus.GetParameter(2);
    tf1_gaus.SetRange(fit1_mean-fit1_2std, fit1_mean+fit1_2std);
    th1ptr->Fit(&tf1_gaus, "RQ");

    std::ostringstream write_name;
    write_name << "Signal " << name << " " << beam_energy << " GeV";
    th1ptr->SetTitle((write_name.str()+";Signal [a.u.];count").c_str());
    th1ptr->Write(write_name.str().c_str());
    return GausFitRes({tf1_gaus.GetParameter(1), tf1_gaus.GetParError(1),
                        tf1_gaus.GetParameter(2), tf1_gaus.GetParError(2),
                        th1ptr->GetEntries()});
}


// Creates Signal per Beam Energy graph
// Returns a tuple containing two arrays with the means and their (fit) errors
std::tuple<BEarray<double>, BEarray<double>> sdeppeb_graph(BEarray<GausFitRes> sdep_res,
                                                           const std::string& name) {
    BEarray<double> sdeppeb_means, sdeppeb_errors;
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        sdeppeb_means[n] = sdep_res[n].mean.value / BEAM_ENERGIES[n];
        sdeppeb_errors[n] = sdep_res[n].mean.error / BEAM_ENERGIES[n];
    }
    auto tge = TGraphErrors(N_BEAM_ENERGIES, BEAM_ENERGIES.data(), sdeppeb_means.data(), nullptr, sdeppeb_errors.data());
    tge.SetTitle(("Signal per Ebeam " + name + ";E_\\text{beam}\\,\\text{[GeV]};\\text{Signal}/E_\\text{beam}\\ \\text{[a.u./GeV]}").c_str());
    tge.Write(("Signal per Ebeam " + name).c_str());
    return std::make_tuple(sdeppeb_means, sdeppeb_errors);
}


// Rejection Filter
// Returns a tuple containing three RDataframes: no filter, after muon filter and after electron filter
std::tuple<RDFI, RDFI, RDFI> eraw_rejection_filters(RDFI rdfi, const double r_mean_el) {
    // Definition of Eraw, Clong and Ctot
    auto clong = [](const ROOT::VecOps::RVec<double>& eraw_cell, const float beam_energy) -> double {
        // M0 C  cells : A2,  A3,  A4  : index 11, 12, 13
        // LBC65 cells : A2,  A3,  A4  : index 56, 57, 58
        // EBC65 cells : A12, A13, A14 : index 90, 91, 92
        double erawc_sum = 0.;
        for (std::size_t index: {11, 12, 13, 56, 57, 58, 90, 91, 92}) {
            erawc_sum += eraw_cell[index];
        }
        return erawc_sum / static_cast<double>(beam_energy * 1e-3);
    };
    auto ctot = [](const ROOT::VecOps::RVec<double>& eraw_cell) -> double {
        //FIXME: really ATLAS, which 24 neighbours??? This is just a (similar but wrong) estimate
        constexpr double alpha = 0.6;
        constexpr std::array<std::size_t, 24> contiguous_cells = {
            11, 12, 13, 30, 31, 32, 41, 42, 43,
            56, 57, 58, 75, 76, 77, 86, 87, 88,
            91, 92, 95, 96, 97, 100
        };
        double sum_1 = 0.;
        for (std::size_t index: contiguous_cells) {
            sum_1 += std::pow(eraw_cell[index], alpha);
        }
        double sum_2 = 0.;
        for (std::size_t index: contiguous_cells) {
            sum_2 += std::pow(std::pow(eraw_cell[index], alpha) - sum_1 / contiguous_cells.size(), 2);
        }
        return std::sqrt(sum_2 / contiguous_cells.size()) / sum_1;
    };
    auto rdfi_eraw = rdfi.Define("ErawSum", "SdepSum/"+std::to_string(r_mean_el))
                         .Define("ErawCell", "Sdep/"+std::to_string(r_mean_el))
                         .Define("Clong", clong, {"ErawCell", "EBeam"})
                         .Define("Ctot", ctot, {"ErawCell"});
    // Muon rejection
    auto rdfi_mr = rdfi_eraw.Filter("ErawSum>"+std::to_string(EMSCALE_MUON_ERAW_CUT_GEV));
    // Electron rejection
    auto rdfi_er = rdfi_mr.Filter("Clong<"+std::to_string(EMSCALE_ELECTRON_CLONG_CUT))
                          .Filter("Ctot<="+std::to_string(EMSCALE_ELECTRON_CTOT_CUT));
    rdfi_er = rdfi_mr; // FIXME: include electron rejection once Ctot is fixed
    // Return rdfi_eraw, rdfi_mr and rdfi_er for statistics
    return std::make_tuple(rdfi_eraw, rdfi_mr, rdfi_er);
}


// Prints statistics about the number of rejected events
void print_cut_statistics(std::tuple<RDFI, RDFI, RDFI> rdfis_filters,
                          const std::string& name) {
    auto rdfi_eraw = std::get<0>(rdfis_filters);
    auto rdfi_mr = std::get<1>(rdfis_filters);
    auto rdfi_er = std::get<2>(rdfis_filters);
    std::cout << "Cut statistics for " << name << ":\n"
                << std::setfill('-') << std::setw(11+3*3+3*24) << ""
                << "\n" << std::setfill(' ')
                << std::setw(11) << "EBeam [GeV]"
                << " | " << std::setw(24) << "All events"
                << " | " << std::setw(24) << "After muon rejection"
                << " | " << std::setw(24) << "After electron rejection"
                << "\n";
    // Book counts
    BEarray<ROOT::RDF::RResultPtr<ULong64_t>> count_all, count_mr, count_er;
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        auto filter_str = "EBeam=="+std::to_string(static_cast<float>(BEAM_ENERGIES[n] * 1e3));
        count_all[n] = rdfi_eraw.Filter(filter_str).Count();
        count_mr[n] = rdfi_mr.Filter(filter_str).Count();
        count_er[n] = rdfi_er.Filter(filter_str).Count();
    }
    // Print counts
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        std::cout << std::setw(11) << BEAM_ENERGIES[n]
                    << " | " << std::setw(24) << count_all[n].GetValue()
                    << " | " << std::setw(24) << count_mr[n].GetValue()
                    << " | " << std::setw(24) << count_er[n].GetValue()
                    << "\n";
    }
    std::cout << std::setfill('-') << std::setw(11+3*3+3*24) << "" << std::setfill(' ') << std::endl;
}


// Creates plots about Ctot and Clong
#if ATLTileCalTBana_CtotClongHists
void clong_ctot_hist(auto rdfi, const double beam_energy, const std::string& name) {
    auto filter_str = "EBeam=="+std::to_string(static_cast<float>(beam_energy * 1e3));
    auto rdfi_be = rdfi.Filter(filter_str);
    auto h_clong = rdfi_be.Histo1D("Clong");
    auto h_ctot = rdfi_be.Histo1D("Ctot");
    ROOT::RDF::TH2DModel th2dm {"th2dm_clong_ctot", "th2dm_clong_ctot", 200, 0., 0.2, 160, 0., 1.6};
    auto h_clong_ctot = rdfi_be.Histo2D(th2dm, "Ctot", "Clong");
    std::ostringstream name_wbe;
    name_wbe << name << " " << beam_energy << " GeV";
    h_clong->SetTitle(("Clong " + name_wbe.str() + ";Clong;count").c_str());
    h_ctot->SetTitle(("Ctot " + name_wbe.str() + ";Ctot;count").c_str());
    h_clong_ctot->SetTitle(("Clong:Ctot " + name_wbe.str() + ";Ctot;Clong").c_str());
    h_clong_ctot->SetOption("colz");
    h_clong_ctot->SetStats(kFALSE);
    h_clong->Write(("Clong " + name_wbe.str()).c_str());
    h_ctot->Write(("Ctot " + name_wbe.str()).c_str());
    h_clong_ctot->Write(("Clong:Ctot " + name_wbe.str()).c_str());
}
#endif


// Returns a RResultPtr to Eraw histogram that filters after beam energy
inline ROOT::RDF::RResultPtr<TH1D> book_eraw_hist(ROOT::RDF::TH1DModel th1dm_eraw,
                                                  RDFI rdfi,
                                                  const double beam_energy) {
    auto filter_str = "EBeam=="+std::to_string(static_cast<float>(beam_energy * 1e3));
    return rdfi.Filter(filter_str).Histo1D(th1dm_eraw, "ErawSum");
}


// Fits Eraw histogram
GausFitRes fit_eraw_hist(TF1& tf1_gaus, TH1* th1ptr,
                        const double beam_energy,
                        const std::string& name,
                        const bool IsFluka = false) {
    auto th1_mean = th1ptr->GetMean();
    //Results with fluka show a large tail on the left side
    //of energy distributions, due to heavy leakage.
    //As of now, I will only fit the right part of 
    //the distributions for fluka data.
    if (IsFluka){
        //first fir aroung maximum-bin
        int binmax = th1ptr->GetMaximumBin();
        th1_mean = th1ptr->GetXaxis()->GetBinCenter(binmax);
    }
    auto th1_2std = 2 * th1ptr->GetStdDev();
    tf1_gaus.SetParameter(0, 0.95 * th1ptr->GetMaximum());
    tf1_gaus.SetParameter(1, th1_mean);
    tf1_gaus.SetParameter(2, 0.45 * th1_2std);
    // Fit only in two sigma range
    tf1_gaus.SetRange(th1_mean-th1_2std, th1_mean+th1_2std);
    th1ptr->Fit(&tf1_gaus, "RQN");
    // Second fit with adjusted two sigma range
    auto fit1_mean = tf1_gaus.GetParameter(1);
    auto fit1_2std = 2 * tf1_gaus.GetParameter(2);
    if (!IsFluka) tf1_gaus.SetRange(fit1_mean-fit1_2std, fit1_mean+fit1_2std);
    //for fluka only fir the "right" side
    else tf1_gaus.SetRange(fit1_mean-0.1*fit1_2std, fit1_mean+2.*fit1_2std);
    th1ptr->Fit(&tf1_gaus, "RQ");

    std::ostringstream write_name;
    write_name << "Signal EM-Scale " << name << " " << beam_energy << " GeV";
    th1ptr->SetTitle((write_name.str()+";E^\\text{raw}\\,\\text{[GeV]};count").c_str());
    th1ptr->Write(write_name.str().c_str());
    return GausFitRes({tf1_gaus.GetParameter(1), tf1_gaus.GetParError(1),
                        tf1_gaus.GetParameter(2), tf1_gaus.GetParError(2),
                        th1ptr->GetEntries()});
}


// Function that reverses a vector
template<class vectype> inline vectype reverse_copy_vector(const vectype& vec) {
    vectype outvec;
    std::reverse_copy(vec.begin(), vec.end(), outvec.begin());
    return outvec;
}


// Create x/e ratio graphs
std::tuple<TGraphErrors, TGraphErrors> xer_graphs(BEarray<GausFitRes> eraw_res,
                                                  const std::string& name) {
    BEarray<double> eresp_vals, eresp_errors, eres_vals, eres_errors, invsqrtbe;
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        eresp_vals[n] = eraw_res[n].mean.value / BEAM_ENERGIES[n];
        eresp_errors[n] = eraw_res[n].mean.error / BEAM_ENERGIES[n];
        eres_vals[n] = eraw_res[n].sigma.value / BEAM_ENERGIES[n];
        eres_errors[n] = eraw_res[n].sigma.error / BEAM_ENERGIES[n];
        invsqrtbe[n] = 1 / sqrt(BEAM_ENERGIES[n]);
    }
    auto tge = TGraphErrors(N_BEAM_ENERGIES, BEAM_ENERGIES.data(), eresp_vals.data(), nullptr, eresp_errors.data());
    tge.SetTitle(("Energy Response " + name + ";E_{beam} [GeV];R^{E^{raw}}").c_str());
    tge.Write(("Energy_Response_" + name).c_str());
    auto tge2 = TGraphErrors(N_BEAM_ENERGIES, reverse_copy_vector(invsqrtbe).data(), reverse_copy_vector(eres_vals).data(), nullptr, reverse_copy_vector(eres_errors).data());
    tge2.SetTitle(("Energy Resolution " + name + ";1/#sqrt{E_{beam} [GeV]};R^{#sigma^{raw}}").c_str());
    tge2.Write(("Energy_Resolution_" + name).c_str());
    return std::make_tuple(tge, tge2);
}


// Create x/e ratio graphs for ATLAS data
std::tuple<TGraphErrors, TGraphErrors> atl_xer_graphs(ATLBEarray<ValErr> eresp,
                                                      ATLBEarray<ValErr> eres,
                                                      const std::string& name) {
    BEarray<double> eresp_vals, eresp_errors, eres_vals, eres_errors, invsqrtbe;
    for (std::size_t n = 0; n < N_ATL_BEAM_ENERGIES; ++n) {
        eresp_vals[n] = eresp[n].value;
        eresp_errors[n] = eresp[n].error;
        eres_vals[n] = eres[n].value;
        eres_errors[n] = eres[n].error;
        invsqrtbe[n] = 1 / sqrt(BEAM_ENERGIES[n]);
    }
    auto tge = TGraphErrors(N_BEAM_ENERGIES, BEAM_ENERGIES.data(), eresp_vals.data(), nullptr, eresp_errors.data());
    tge.SetTitle(("ATLAS Energy Response " + name + ";E_{beam} [GeV];R^{E^{raw}}").c_str());
    tge.Write(("ATLAS_Energy_Response_" + name).c_str());
    auto tge2 = TGraphErrors(N_BEAM_ENERGIES, reverse_copy_vector(invsqrtbe).data(), reverse_copy_vector(eres_vals).data(), nullptr, reverse_copy_vector(eres_errors).data());
    tge2.SetTitle(("ATLAS Energy Resolution " + name + ";1/#sqrt{E_{beam} [GeV]};R^{#sigma^{raw}}").c_str());
    tge2.Write(("ATLAS_Energy_Resolution_" + name).c_str());
    return std::make_tuple(tge, tge2);
}


// Create x/e ratio comparison graphs
void xer_graphs_comp_canvas(std::tuple<TGraphErrors, TGraphErrors> sim_xer_graphs_t,
                            std::tuple<TGraphErrors, TGraphErrors> atl_xer_graphs_t,
                            double eresp_ymin, double eresp_ymax,
                            double eres_ymin, double eres_ymax,
                            const std::string& name) {
    // Get selected canvas since we stored canvases
    auto* canvas = gROOT->GetSelectedPad();
    // Energy response
    TCanvas canvas_eresp {"canvas_eresp", "canvas_eresp", -1280, 720};
    gROOT->SetSelectedPad(&canvas_eresp);
    TMultiGraph tmg_eresp {};
    auto tge_sim_eresp = std::get<0>(sim_xer_graphs_t);
    auto tge_atl_eresp = std::get<0>(atl_xer_graphs_t);
    tge_sim_eresp.SetMarkerStyle(kCircle);
    tge_atl_eresp.SetLineColor(kBlue);
    tge_atl_eresp.SetMarkerColor(kBlue);
    tge_atl_eresp.SetMarkerStyle(kFullCircle);
    tmg_eresp.Add(&tge_sim_eresp, "AP");
    tmg_eresp.Add(&tge_atl_eresp, "AP");
    tmg_eresp.GetYaxis()->SetRangeUser(eresp_ymin, eresp_ymax);
    tmg_eresp.GetYaxis()->SetTitle("R^{E^\\text{raw}}");
    tmg_eresp.GetXaxis()->SetTitle("E_\\text{beam}\\,\\text{[GeV]}");
    tmg_eresp.Draw("a");
    TLegend leg_eresp {0.6, 0.75, 0.9, 0.9};
    leg_eresp.AddEntry((TObject*)nullptr, name.c_str(), "");
    leg_eresp.AddEntry(&tge_atl_eresp, "Experimental data", "P");
    leg_eresp.AddEntry(&tge_sim_eresp, "Simulated data", "P");
    leg_eresp.SetBorderSize(0);
    leg_eresp.Draw();
    canvas_eresp.Modified();
    canvas_eresp.Update();
    canvas_eresp.Write(("Comparision Energy Response " + name).c_str());
    // Energy resolution
    TCanvas canvas_eres {"canvas_eres", "canvas_eres", -1280, 720};
    gROOT->SetSelectedPad(&canvas_eres);
    TMultiGraph tmg_eres {};
    auto tge_sim_eres = std::get<1>(sim_xer_graphs_t);
    auto tge_atl_eres = std::get<1>(atl_xer_graphs_t);
    tge_sim_eres.SetMarkerStyle(kCircle);
    tge_atl_eres.SetLineColor(kBlue);
    tge_atl_eres.SetMarkerColor(kBlue);
    tge_atl_eres.SetMarkerStyle(kFullCircle);
    tmg_eres.Add(&tge_sim_eres, "AP");
    tmg_eres.Add(&tge_atl_eres, "AP");
    tmg_eres.GetYaxis()->SetRangeUser(eres_ymin, eres_ymax);
    tmg_eres.GetYaxis()->SetTitle("R^{\\sigma^\\text{raw}}");
    tmg_eres.GetXaxis()->SetTitle("1/\\sqrt{E_\\text{beam}\\,\\text{[GeV]}}");
    tmg_eres.Draw("a");
    TLegend leg_eres {0.5, 0.75, 0.8, 0.9};
    leg_eres.AddEntry((TObject*)nullptr, name.c_str(), "");
    leg_eres.AddEntry(&tge_atl_eres, "Experimental data", "P");
    leg_eres.AddEntry(&tge_sim_eres, "Simulated data", "P");
    leg_eres.SetBorderSize(0);
    leg_eres.Draw();
    canvas_eres.Modified();
    canvas_eres.Update();
    canvas_eres.Write(("Comparision Energy Resolution " + name).c_str());
    // Set originally selected canvas as selected again
    gROOT->SetSelectedPad(canvas);
}


// Macro entry
// Usage: root TBrun_all.C for standard G4-only data analysis
//        root 'TBrun_all.C(true)' for analysis of data using FLUKA.CERN interface
//
void TBrun_all(const bool IsFluka = false) {
    ROOT::EnableImplicitMT();
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("TMinuit", "Minimize");
    gROOT->SetStyle("Modern");

    // Create output file and RDataFrame
    std::string output_name{};
    if(!IsFluka) output_name = "analysis.root";
    else output_name = "analysis_fluka.root";
    TFile output {output_name.c_str(), "RECREATE"};
    if (IsFluka) MERGED_RUN_FILE = "ATLTileCalTBout_RunAll_Fluka.root";
    ROOT::RDataFrame rdf {RUN_FILE_TTREE_NAME, MERGED_RUN_FILE};

    // Create default canvas and default gaus
    TCanvas canvas {"canvas", "canvas", -1280, 720};
    auto tf1_gaus = TF1("tf1_gaus", "gaus");

    // Book electron, pion, kaon and proton filters
    auto rdf_el = rdf.Filter("PDGID=="+std::to_string(PDG_ID_EL));
    auto rdf_pi = rdf.Filter("PDGID=="+std::to_string(PDG_ID_PI));
    auto rdf_k  = rdf.Filter("PDGID=="+std::to_string(PDG_ID_K));
    auto rdf_p  = rdf.Filter("PDGID=="+std::to_string(PDG_ID_P));

    // Book Sdep histograms
    ROOT::RDF::TH1DModel th1dm_sdep {"th1dm_sdep", "th1dm_sdep", 300, 0., 3000.};
    BEarray<ROOT::RDF::RResultPtr<TH1D>> th1s_sdep_el, th1s_sdep_pi, th1s_sdep_k, th1s_sdep_p;
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        th1s_sdep_el[n] = book_sdep_hist(th1dm_sdep, rdf_el, BEAM_ENERGIES[n]);
        th1s_sdep_pi[n] = book_sdep_hist(th1dm_sdep, rdf_pi, BEAM_ENERGIES[n]);
        th1s_sdep_k[n]  = book_sdep_hist(th1dm_sdep, rdf_k,  BEAM_ENERGIES[n]);
        th1s_sdep_p[n]  = book_sdep_hist(th1dm_sdep, rdf_p,  BEAM_ENERGIES[n]);
    }

    // Fit Sdep histograms
    BEarray<GausFitRes> sdep_res_el, sdep_res_pi, sdep_res_k, sdep_res_p;
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        sdep_res_el[n] = fit_sdep_hist(tf1_gaus, th1s_sdep_el[n].GetPtr(), BEAM_ENERGIES[n], "Electrons");
        sdep_res_pi[n] = fit_sdep_hist(tf1_gaus, th1s_sdep_pi[n].GetPtr(), BEAM_ENERGIES[n], "Pions");
        sdep_res_k[n] =  fit_sdep_hist(tf1_gaus, th1s_sdep_k[n].GetPtr(),  BEAM_ENERGIES[n], "Kaons");
        sdep_res_p[n] =  fit_sdep_hist(tf1_gaus, th1s_sdep_p[n].GetPtr(),  BEAM_ENERGIES[n], "Protons");
    }

    // Create Signal per EBeam graphs
    auto sdeppeb_res_el = sdeppeb_graph(sdep_res_el, "Electrons");
    sdeppeb_graph(sdep_res_pi, "Pions");
    sdeppeb_graph(sdep_res_k,  "Kaons");
    sdeppeb_graph(sdep_res_p,  "Protons");

    // Apply rejection filter
    auto r_means_el = std::get<0>(sdeppeb_res_el);
    double r_mean_el = std::accumulate(r_means_el.begin(), r_means_el.end(), 0.) / r_means_el.size();  // TODO: error of r_mean_el?
    auto rdfs_pi_filters = eraw_rejection_filters(rdf_pi, r_mean_el);
    auto rdfs_k_filters =  eraw_rejection_filters(rdf_k,  r_mean_el);
    auto rdfs_p_filters =  eraw_rejection_filters(rdf_p,  r_mean_el);

    // Cut statistics
    print_cut_statistics(rdfs_pi_filters, "Pions");
    print_cut_statistics(rdfs_k_filters,  "Kaons");
    print_cut_statistics(rdfs_p_filters,  "Protons");

    // Clong and Ctot histograms
    #if ATLTileCalTBana_CtotClongHists
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        clong_ctot_hist(std::get<1>(rdfs_pi_filters), BEAM_ENERGIES[n], "Pions");
        clong_ctot_hist(std::get<1>(rdfs_k_filters),  BEAM_ENERGIES[n], "Kaons");
        clong_ctot_hist(std::get<1>(rdfs_p_filters),  BEAM_ENERGIES[n], "Protons");
    }
    #endif

    // Book EM-Scale histograms
    ROOT::RDF::TH1DModel th1dm_eraw {"th1dm_eraw", "th1dm_eraw", th1dm_sdep.fNbinsX,
                                     th1dm_sdep.fXLow / r_mean_el, th1dm_sdep.fXUp  / r_mean_el};
    BEarray<ROOT::RDF::RResultPtr<TH1D>> th1s_eraw_pi, th1s_eraw_k, th1s_eraw_p;
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        th1s_eraw_pi[n] = book_eraw_hist(th1dm_eraw, std::get<2>(rdfs_pi_filters), BEAM_ENERGIES[n]);
        th1s_eraw_k[n]  = book_eraw_hist(th1dm_eraw, std::get<2>(rdfs_k_filters),  BEAM_ENERGIES[n]);
        th1s_eraw_p[n]  = book_eraw_hist(th1dm_eraw, std::get<2>(rdfs_p_filters),  BEAM_ENERGIES[n]);
    }

    // Fit EM-Scale histograms
    BEarray<GausFitRes> eraw_res_pi, eraw_res_k, eraw_res_p;
    for (std::size_t n = 0; n < N_BEAM_ENERGIES; ++n) {
        eraw_res_pi[n] = fit_eraw_hist(tf1_gaus, th1s_eraw_pi[n].GetPtr(), BEAM_ENERGIES[n], "Pions", IsFluka);
        eraw_res_k[n]  = fit_eraw_hist(tf1_gaus, th1s_eraw_k[n].GetPtr(),  BEAM_ENERGIES[n], "Kaons", IsFluka);
        eraw_res_p[n]  = fit_eraw_hist(tf1_gaus, th1s_eraw_p[n].GetPtr(),  BEAM_ENERGIES[n], "Protons", IsFluka);
    }

    // Create x/e ratio graphs
    auto sim_pier_graphs = xer_graphs(eraw_res_pi, "Pions");
    auto sim_ker_graphs  = xer_graphs(eraw_res_k,  "Kaons");
    auto sim_per_graphs  = xer_graphs(eraw_res_p,  "Protons");

    // Create x/e ratio graph (ATLAS data)
    auto atl_pier_graphs = atl_xer_graphs(ATL_ERESPONSE_PI, ATL_ERESOLUTION_PI, "Pions");
    auto atl_ker_graphs  = atl_xer_graphs(ATL_ERESPONSE_K,  ATL_ERESOLUTION_K,  "Kaons");
    auto atl_per_graphs  = atl_xer_graphs(ATL_ERESPONSE_P,  ATL_ERESOLUTION_P,  "Protons");

    // Create comparision plots
    gROOT->SetStyle("ATLAS");
    xer_graphs_comp_canvas(sim_pier_graphs, atl_pier_graphs, 0.77, 0.85, 0.076, 0.135, "Pions");
    xer_graphs_comp_canvas(sim_ker_graphs,  atl_ker_graphs,  0.74, 0.82, 0.072, 0.160, "Kaons");
    xer_graphs_comp_canvas(sim_per_graphs,  atl_per_graphs,  0.68, 0.80, 0.076, 0.130, "Protons");

    // Uncomment to see comparison plots directly when executing script
    //std::string tmp; std::cin >> tmp;

    // Write and close
    output.Write();
    output.Close();

}
