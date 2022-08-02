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
const std::array<double, 4> BEAM_ENERGIES {16., 18., 20., 30.};
template<typename T> using BEarray = std::array<T, BEAM_ENERGIES.size()>;
const std::string MERGED_RUN_FILE {"ATLTileCalTBout_RunAll.root"};
const std::string RUN_FILE_TTREE_NAME {"ATLTileCalTBout"};
const int PDG_ID_EL = 11;
const int PDG_ID_PI = -211;
const double EMSCALE_MUON_ERAW_CUT_GEV = 5.;
const double EMSCALE_ELECTRON_CLONG_CUT = 0.6;
const double EMSCALE_ELECTRON_CTOT_CUT = 0.125;

// ATLAS data
const std::array<double, 4> ATL_BEAM_ENERGIES {16., 18., 20., 30.};
template<typename T> using ATLBEarray = std::array<T, ATL_BEAM_ENERGIES.size()>;
const ATLBEarray<ValErr> ATL_ERESPONSE_PIE {
    ValErr({0.7924, 0.0116}),
    ValErr({0.7941, 0.0108}),
    ValErr({0.7948, 0.0101}),
    ValErr({0.8019, 0.0098}),
};
const ATLBEarray<ValErr> ATL_ERESOLUTION_PIE {
    ValErr({0.1258, 0.0039}),
    ValErr({0.1188, 0.0023}),
    ValErr({0.1159, 0.0013}),
    ValErr({0.0987, 0.0006}),
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
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        sdeppeb_means[n] = sdep_res[n].mean.value / BEAM_ENERGIES[n];
        sdeppeb_errors[n] = sdep_res[n].mean.error / BEAM_ENERGIES[n];
    }
    auto tge = TGraphErrors(BEAM_ENERGIES.size(), BEAM_ENERGIES.data(), sdeppeb_means.data(), nullptr, sdeppeb_errors.data());
    tge.SetTitle(("Signal per EBeam " + name + ";Beam Energy [GeV];Signal/EBeam [a.u./GeV]").c_str());
    tge.Write(("Signal per EBeam " + name).c_str());
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
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        auto filter_str = "EBeam=="+std::to_string(static_cast<float>(BEAM_ENERGIES[n] * 1e3));
        count_all[n] = rdfi_eraw.Filter(filter_str).Count();
        count_mr[n] = rdfi_mr.Filter(filter_str).Count();
        count_er[n] = rdfi_er.Filter(filter_str).Count();
    }
    // Print counts
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
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
                        const std::string& name) {
    auto th1_mean = th1ptr->GetMean();
    auto th1_2std = 2 * th1ptr->GetStdDev();
    tf1_gaus.SetParameter(0, 0.95 * th1ptr->GetMaximum());
    tf1_gaus.SetParameter(1, th1_mean);
    tf1_gaus.SetParameter(2, 0.45 * th1_2std);
    // Fit only in two sigma range
    tf1_gaus.SetRange(th1_mean-th1_2std, th1_mean+th1_2std);
    th1ptr->Fit(&tf1_gaus, "RQ");
    std::ostringstream write_name;
    write_name << "Signal EM-Scale " << name << " " << beam_energy << " GeV";
    th1ptr->SetTitle((write_name.str()+";E^{raw} [GeV];count").c_str());
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
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        eresp_vals[n] = eraw_res[n].mean.value / BEAM_ENERGIES[n];
        eresp_errors[n] = eraw_res[n].mean.error / BEAM_ENERGIES[n];
        eres_vals[n] = eraw_res[n].sigma.value / BEAM_ENERGIES[n];
        eres_errors[n] = eraw_res[n].sigma.error / BEAM_ENERGIES[n];
        invsqrtbe[n] = 1 / sqrt(BEAM_ENERGIES[n]);
    }
    auto tge = TGraphErrors(BEAM_ENERGIES.size(), BEAM_ENERGIES.data(), eresp_vals.data(), nullptr, eresp_errors.data());
    tge.SetTitle(("Energy response " + name + ";Beam Energy [GeV];R^{E^{raw}}").c_str());
    tge.Write(("Energy response " + name).c_str());
    auto tge2 = TGraphErrors(BEAM_ENERGIES.size(), reverse_copy_vector(invsqrtbe).data(), reverse_copy_vector(eres_vals).data(), nullptr, reverse_copy_vector(eres_errors).data());
    tge2.SetTitle(("Energy resolution " + name + ";1/\\sqrt{E_{Beam} [GeV]};R^{\\sigma^{raw}}").c_str());
    tge2.Write(("Energy resolution " + name).c_str());
    return std::make_tuple(tge, tge2);
}


// Create x/e ratio graphs for ATLAS data
std::tuple<TGraphErrors, TGraphErrors> atl_xer_graphs(ATLBEarray<ValErr> eresp,
                                                      ATLBEarray<ValErr> eres,
                                                      const std::string& name) {
    BEarray<double> eresp_vals, eresp_errors, eres_vals, eres_errors, invsqrtbe;
    for (std::size_t n = 0; n < ATL_BEAM_ENERGIES.size(); ++n) {
        eresp_vals[n] = eresp[n].value;
        eresp_errors[n] = eresp[n].error;
        eres_vals[n] = eres[n].value;
        eres_errors[n] = eres[n].error;
        invsqrtbe[n] = 1 / sqrt(BEAM_ENERGIES[n]);
    }
    auto tge = TGraphErrors(BEAM_ENERGIES.size(), BEAM_ENERGIES.data(), eresp_vals.data(), nullptr, eresp_errors.data());
    tge.SetTitle(("ATLAS Energy response " + name + ";Beam Energy [GeV];R^{E^{raw}}").c_str());
    tge.Write(("ATLAS Energy response " + name).c_str());
    auto tge2 = TGraphErrors(BEAM_ENERGIES.size(), reverse_copy_vector(invsqrtbe).data(), reverse_copy_vector(eres_vals).data(), nullptr, reverse_copy_vector(eres_errors).data());
    tge2.SetTitle(("ATLAS Energy resolution " + name + ";1/\\sqrt{E_{Beam} [GeV]};R^{\\sigma^{raw}}").c_str());
    tge2.Write(("ATLAS Energy resolution " + name).c_str());
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
    tmg_eresp.GetXaxis()->SetTitle("E_\\text{beam} \\text{[GeV]}");
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
    tmg_eres.GetXaxis()->SetTitle("E_\\text{beam} \\text{[GeV]}");
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
void TBrun_all() {
    ROOT::EnableImplicitMT(); // TODO: Buggy?
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2", "Minimize");
    gROOT->SetStyle("Modern");

    // Create output file and RDataFrame
    TFile output {"analysis.root", "RECREATE"};
    ROOT::RDataFrame rdf {RUN_FILE_TTREE_NAME, MERGED_RUN_FILE};

    // Create default canvas and default gaus
    TCanvas canvas {"canvas", "canvas", -1280, 720};
    auto tf1_gaus = TF1("tf1_gaus", "gaus");

    // Book electron / pion filters
    auto rdf_el = rdf.Filter("PDGID=="+std::to_string(PDG_ID_EL));
    auto rdf_pi = rdf.Filter("PDGID=="+std::to_string(PDG_ID_PI));

    // Book Sdep histograms
    ROOT::RDF::TH1DModel th1dm_sdep {"th1dm_sdep", "th1dm_sdep", 300, 0., 3000.};
    BEarray<ROOT::RDF::RResultPtr<TH1D>> th1s_sdep_el, th1s_sdep_pi;
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        th1s_sdep_el[n] = book_sdep_hist(th1dm_sdep, rdf_el, BEAM_ENERGIES[n]);
        th1s_sdep_pi[n] = book_sdep_hist(th1dm_sdep, rdf_pi, BEAM_ENERGIES[n]);
    }

    // Fit Sdep histograms
    BEarray<GausFitRes> sdep_res_el, sdep_res_pi;
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        sdep_res_el[n] = fit_sdep_hist(tf1_gaus, th1s_sdep_el[n].GetPtr(), BEAM_ENERGIES[n], "Electrons");
        sdep_res_pi[n] = fit_sdep_hist(tf1_gaus, th1s_sdep_pi[n].GetPtr(), BEAM_ENERGIES[n], "Pions");
    }

    // Create Signal per EBeam graphs
    auto sdeppeb_res_el = sdeppeb_graph(sdep_res_el, "Electrons");
    sdeppeb_graph(sdep_res_pi, "Pions");

    // Apply rejection filter
    auto r_means_el = std::get<0>(sdeppeb_res_el);
    double r_mean_el = std::accumulate(r_means_el.begin(), r_means_el.end(), 0.) / r_means_el.size();  // TODO: error of r_mean_el?
    auto rdfs_pi_filters = eraw_rejection_filters(rdf_pi, r_mean_el);

    // Cut statistics
    print_cut_statistics(rdfs_pi_filters, "Pions");

    // Clong and Ctot histograms
    #if ATLTileCalTBana_CtotClongHists
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        clong_ctot_hist(std::get<1>(rdfs_pi_filters), BEAM_ENERGIES[n], "Pions");
    }
    #endif

    // Book EM-Scale histograms
    ROOT::RDF::TH1DModel th1dm_eraw {"th1dm_eraw", "th1dm_eraw", 200, 5., 45.};
    BEarray<ROOT::RDF::RResultPtr<TH1D>> th1s_eraw;
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        th1s_eraw[n] = book_eraw_hist(th1dm_eraw, std::get<2>(rdfs_pi_filters), BEAM_ENERGIES[n]);
    }

    // Fit EM-Scale histograms
    BEarray<GausFitRes> eraw_res_pi;
    for (std::size_t n = 0; n < BEAM_ENERGIES.size(); ++n) {
        eraw_res_pi[n] = fit_eraw_hist(tf1_gaus, th1s_eraw[n].GetPtr(), BEAM_ENERGIES[n], "Pions");
    }

    // Create x/e ratio graphs
    auto sim_pier_graphs = xer_graphs(eraw_res_pi, "Pions");

    // Create x/e ratio graph (ATLAS data)
    auto atl_pier_graphs = atl_xer_graphs(ATL_ERESPONSE_PIE, ATL_ERESOLUTION_PIE, "Pions");

    // Create comparision plots
    gROOT->SetStyle("ATLAS");
    xer_graphs_comp_canvas(sim_pier_graphs, atl_pier_graphs, 0.77, 0.85, 0.086, 0.135, "Pions");

    // Uncomment to see comparison plots directly when executing script
    //std::string tmp; std::cin >> tmp;

    // Write and close
    output.Write();
    output.Close();
}
