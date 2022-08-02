##**************************************************
## \file parser.py
## \brief: Implementation of ATLTileCalTB parser
##         for geant-val usage
## \author: Stephan Lachnit (CERN EP-SFT-sim)
##          @stephanlachnit
## \start date: 02 August 2022
##**************************************************

#!/usr/bin/env python3

import os.path
import subprocess

from gts.BaseParser import BaseParser, mergeROOT, mktemp
from gts.utils import getJSON
import ROOT


class Test(BaseParser):
    TEST = "ATLTileCalTB"
    IGNOREKEYS = ["PARTICLE", "ENERGY"]  # we need all particles and energies in the analysis

    def parse(self, jobs):
        physlist = jobs[0]['PHYSLIST']
        print('start parsing for ' + physlist)

        # merge ROOT files
        root_files = [os.path.join(job["path"],"ATLTileCalTBout_Run0.root") for job in jobs]
        tempdir = mktemp(isDir=True)
        mergeROOT(root_files, os.path.join(tempdir, 'ATLTileCalTBout_RunAll.root'))
        print('merged ROOT files for ' + physlist + ' in ' + tempdir)

        # run analysis
        cmd = ['root', '-b', '-l', '-q', '/afs/cern.ch/user/s/slachnit/ATLTileCalTB/analysis/TBrun_all.C']  # FIXME: don't hardcode
        proc = subprocess.Popen(cmd, cwd=tempdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        proc.wait()
        proc_out = 'ROOT output for ' + physlist + ':\n' + proc.stdout.read().strip() + '\n' + proc.stderr.read().strip()
        print(proc_out)

        # get data from analysis
        tf = ROOT.TFile(os.path.join(tempdir, 'analysis.root'), 'READ')
        tge_eresp_pi = tf.Get('Energy response Pions')
        tge_eres_pi = tf.Get('Energy resolution Pions')

        # return results
        print('end parsing for ' + physlist)
        # Energy response pions
        yield getJSON(jobs[0], 'chart',
            testName='ATLTileCalTB',
            mctool_name='GEANT4',
            mctool_model=physlist,
            observableName=tge_eresp_pi.GetTitle(),
            xAxisName=tge_eresp_pi.GetXaxis().GetTitle(),
            yAxisName=tge_eresp_pi.GetYaxis().GetTitle(),
            xValues=list(tge_eresp_pi.GetX()),
            yValues=list(tge_eresp_pi.GetY()),
            yStatErrorsMinus=list(tge_eresp_pi.GetEY()),
            yStatErrorsPlus=list(tge_eresp_pi.GetEY()),
        )
        # Energy resolution pions
        yield getJSON(jobs[0], 'chart',
            testName='ATLTileCalTB',
            mctool_name='GEANT4',
            mctool_model=physlist,
            observableName=tge_eres_pi.GetTitle(),
            xAxisName=tge_eres_pi.GetXaxis().GetTitle(),
            yAxisName=tge_eres_pi.GetYaxis().GetTitle(),
            xValues=list(tge_eres_pi.GetX()),
            yValues=list(tge_eres_pi.GetY()),
            yStatErrorsMinus=list(tge_eres_pi.GetEY()),
            yStatErrorsPlus=list(tge_eres_pi.GetEY()),
        )
