#!/usr/bin/env python3
"""Script to analyse data from TBrun_all.mac runs"""

import argparse
from array import array
import pathlib
import sys

import numpy as np
import ROOT

GLOBAL_ROOT_OBJECT_OWNER = []  # contains ROOT objects that should not be deleted when out of scope
BEAM_ENERGIES = [16., 18., 20., 30.]
RUN_FILE_PATH_PREFIX = 'ATLTileCalTBout_Run'
RUN_FILE_PATH_POSTFIX = '.root'
RUN_FILE_TTREE_NAME = 'ATLTileCalTBout'


def open_root_files(paths: list[pathlib.Path]) -> tuple[list[ROOT.RDataFrame], list[ROOT.RDataFrame]]:
    """
    Opens the ROOT files and returns the containing RDataFrames in two lists for e- and pi-.

    Args:
        args: List of paths to ROOT files.
    Returns:
        A tuple with two lists of ROOT.RDataFrames objects: the first for e-, the second for pi-.
    """
    n_energies = len(BEAM_ENERGIES)
    out_el = list[ROOT.TFile]()
    out_pi = list[ROOT.TFile]()
    for count in range(n_energies):
        out_el.append(ROOT.RDataFrame(RUN_FILE_TTREE_NAME, paths[count].as_posix()))
        out_pi.append(ROOT.RDataFrame(RUN_FILE_TTREE_NAME, paths[count+n_energies].as_posix()))
    return tuple((out_el, out_pi))


def create_leakage_plot(rdfs: list[ROOT.RDataFrame], name: str, ybinup: float) -> ROOT.TCanvas:
    ROOT.gStyle.SetOptStat(False)
    canvas = ROOT.TCanvas(f'canvas_leakage_{name}', f'Leakage {name}', -1280, 720)

    xbinlow = BEAM_ENERGIES[0]-1
    xbinup = BEAM_ENERGIES[-1]+1
    nxbins = int((xbinup - xbinlow) / 2)

    th2 = ROOT.TH2D(f'th2_leakage_{name}', f'Leakage {name}', nxbins, xbinlow, xbinup, 50, 0., ybinup)
    for count, beam_energy in enumerate(BEAM_ENERGIES):
        eleak = rdfs[count].AsNumpy(['ELeak'])['ELeak']
        n_eleak = len(eleak)
        th2.FillN(n_eleak, np.full(n_eleak, beam_energy), eleak, np.ones(n_eleak))
    th2.SetTitle(f'Leakage {name};Beam Energy [GeV];Energy Leakage [MeV]')
    th2.Draw('colz')

    canvas.Modified()
    canvas.Update()

    GLOBAL_ROOT_OBJECT_OWNER.append(th2)
    GLOBAL_ROOT_OBJECT_OWNER.append(canvas)

    return canvas


def create_signal_plot(rdfs: list[ROOT.RDataFrame], name: str) -> ROOT.TCanvas:
    ROOT.gStyle.SetOptStat(False)
    canvas = ROOT.TCanvas(f'canvas_signal_{name}', f'Signal {name}', -1280, 720)

    means = []
    sigmas = []
    for count in range(len(BEAM_ENERGIES)):
        th1 = rdfs[count].Histo1D('SdepSum').GetValue()
        fitres = th1.Fit('gaus', 'SQN').Get()
        means.append(fitres.Parameter(1))
        sigmas.append(fitres.Parameter(2) / np.sqrt(th1.GetEntries()))

    tge = ROOT.TGraphErrors(len(BEAM_ENERGIES), array('f', BEAM_ENERGIES), array('f', means), 0, array('f', sigmas))
    tge.SetTitle(f'Signal {name};Beam Energy [GeV];Signal [a.u.]')
    tge.Draw()

    GLOBAL_ROOT_OBJECT_OWNER.append(tge)
    GLOBAL_ROOT_OBJECT_OWNER.append(canvas)

    return canvas


def create_r_plot(rdfs: list[ROOT.RDataFrame], name: str) -> ROOT.TCanvas:
    ROOT.gStyle.SetOptStat(False)
    canvas = ROOT.TCanvas(f'canvas_r_{name}', f'R {name}', -1280, 720)

    means = []
    sigmas = []
    for count, beam_energy in enumerate(BEAM_ENERGIES):
        th1 = rdfs[count].Histo1D('SdepSum').GetValue()
        fitres = th1.Fit('gaus', 'SQN').Get()
        means.append(fitres.Parameter(1) / beam_energy)
        sigmas.append(fitres.Parameter(2)  / np.sqrt(th1.GetEntries()) / beam_energy)

    tge = ROOT.TGraphErrors(len(BEAM_ENERGIES), array('f', BEAM_ENERGIES), array('f', means), 0, array('f', sigmas))
    tge.SetTitle(f'R {name};Beam Energy [GeV];R [a.u. / GeV]')
    tge.Draw()

    GLOBAL_ROOT_OBJECT_OWNER.append(tge)
    GLOBAL_ROOT_OBJECT_OWNER.append(canvas)

    return canvas


def create_emscale_plot(rdf_pi: ROOT.RDataFrame, rdf_el: ROOT.RDataFrame, beam_energy: float) -> tuple[ROOT.TH1, ROOT.TCanvas]:
    ROOT.gStyle.SetOptStat(False)
    beam_energy_str = str(int(beam_energy)) + ' GeV'
    canvas = ROOT.TCanvas(f'canvas_emscale_{beam_energy_str}', f'EM-Scale {beam_energy_str} poins', -1280, 720)

    th1 = rdf_el.Histo1D('SdepSum').GetValue()
    fitres = th1.Fit('gaus', 'SQN').Get()
    mean = fitres.Parameter(1) / beam_energy

    th1_pi = rdf_pi.Define('SdepSumEMScale', f'SdepSum/{mean}').Histo1D('SdepSumEMScale').GetValue()
    th1_pi.SetTitle(f'Signal at EM-Scale {beam_energy_str} pions;Reconstructed Energy [GeV];')
    th1_pi.Draw()

    GLOBAL_ROOT_OBJECT_OWNER.append(th1_pi)
    GLOBAL_ROOT_OBJECT_OWNER.append(canvas)

    return tuple((th1_pi, canvas))


def create_pie_plot(th1s: list[ROOT.TH1]) -> ROOT.TCanvas:
    ROOT.gStyle.SetOptStat(False)
    canvas = ROOT.TCanvas(f'canvas_pie', f'pi/e ratio', -1280, 720)

    means = []
    sigmas = []
    for count, th1 in enumerate(th1s):
        fitres = th1.Fit('gaus', 'SQN').Get()
        means.append(fitres.Parameter(1) / BEAM_ENERGIES[count])
        sigmas.append(fitres.Parameter(2) / BEAM_ENERGIES[count] / np.sqrt(th1.GetEntries()))

    tge = ROOT.TGraphErrors(len(BEAM_ENERGIES), array('f', BEAM_ENERGIES), array('f', means), 0, array('f', sigmas))
    tge.SetTitle(f'pi/e ratio;Beam Energy [GeV];R')
    tge.Draw()

    GLOBAL_ROOT_OBJECT_OWNER.append(tge)
    GLOBAL_ROOT_OBJECT_OWNER.append(canvas)

    return canvas


def analysis(rdfs_el: list[ROOT.RDataFrame], rdfs_pi: list[ROOT.RDataFrame], path: pathlib.Path):
    c_lp_el = create_leakage_plot(rdfs_el, 'electrons', 100.)
    c_lp_el.SaveAs(f'{path}/lp_el.pdf')
    c_lp_pi = create_leakage_plot(rdfs_pi, 'pions', 1000.)
    c_lp_pi.SaveAs(f'{path}/lp_pi.pdf')

    c_sp_el = create_signal_plot(rdfs_el, 'electrons')
    c_sp_el.SaveAs(f'{path}/sp_el.pdf')
    c_sp_pi = create_signal_plot(rdfs_pi, 'pions')
    c_sp_pi.SaveAs(f'{path}/sp_pi.pdf')

    c_rp_el = create_r_plot(rdfs_el, 'electrons')
    c_rp_el.SaveAs(f'{path}/rp_el.pdf')
    c_rp_pi = create_r_plot(rdfs_pi, 'pions')
    c_rp_pi.SaveAs(f'{path}/rp_pi.pdf')

    emscale_th1s = list[ROOT.TH1]()
    for count, beam_energy in enumerate(BEAM_ENERGIES):
        th1_ep, c_ep = create_emscale_plot(rdfs_pi[count], rdfs_el[count], beam_energy)
        c_ep.SaveAs(f'{path}/ep_{str(int(beam_energy))}.pdf')
        emscale_th1s.append(th1_ep)

    c_rpiep = create_pie_plot(emscale_th1s)
    c_rpiep.SaveAs(f'{path}/rpiep.pdf')


def parse_args(args: list[str]) -> argparse.Namespace:
    """
    Parses the command-line arguments.

    Args:
        args: List of strings to parse as command-line arguments.
    Returns:
        A namespace with the parsed arguments.
    """
    parser = argparse.ArgumentParser()

    parser.add_argument('path', type=pathlib.Path, help='directory with root files from TBrun.mac')

    return parser.parse_args(args=args)


def main(args: list[str] = None) -> None:
    """
    Runs the command-line interace and runs the analysis.

    Args:
        args: List of strings to parse as command-line arguments. Defaults to sys.argv if set to None.
    """
    if args is None:
        args = sys.argv[1:]

    cli_options = parse_args(args)
    path = cli_options.path

    if not path.is_dir():  # also checks if exists
        raise Exception(f'Path {path} is not a directory')

    run_file_paths = []
    for count in range(8):
        run_file_path = path.joinpath(f'{RUN_FILE_PATH_PREFIX}{count}{RUN_FILE_PATH_POSTFIX}')
        run_file_paths.append(run_file_path)

    for run_file_path in run_file_paths:
        if not run_file_path.is_file():  # also checks if exists
            raise Exception(f'Path {run_file_path} is not a file')

    rdfs_el, rdfs_pi = open_root_files(run_file_paths)
    analysis(rdfs_el, rdfs_pi, path)


if __name__ == '__main__':
    main()
    input()
