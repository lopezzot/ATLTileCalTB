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
        tge_eresp_pi = tf.Get('Energy Response Pions')
        tge_eres_pi = tf.Get('Energy Resolution Pions')
        tge_eresp_k = tf.Get('Energy Response Kaons')
        tge_eres_k = tf.Get('Energy Resolution Kaons')
        tge_eresp_p = tf.Get('Energy Response Protons')
        tge_eres_p = tf.Get('Energy Resolution Protons')

        # Return simulation results
        # Energy response pions
        yield getJSON(jobs[0], 'chart',
            mctool_name='GEANT4',
            mctool_model=physlist,
            targetName='ATLAS-TileCal',
            beamParticle='pi+',
            beamEnergies=list(tge_eresp_pi.GetX()),
            observableName='Energy Response',
            xAxisName=tge_eresp_pi.GetXaxis().GetTitle(),
            yAxisName=tge_eresp_pi.GetYaxis().GetTitle(),
            xValues=list(tge_eresp_pi.GetX()),
            yValues=list(tge_eresp_pi.GetY()),
            yStatErrorsMinus=list(tge_eresp_pi.GetEY()),
            yStatErrorsPlus=list(tge_eresp_pi.GetEY()),
        )
        # Energy resolution pions
        yield getJSON(jobs[0], 'chart',
            mctool_name='GEANT4',
            mctool_model=physlist,
            targetName='ATLAS-TileCal',
            beamParticle='pi+',
            beamEnergies=list(tge_eresp_pi.GetX()),
            observableName='Energy Resolution',
            xAxisName=tge_eres_pi.GetXaxis().GetTitle(),
            yAxisName=tge_eres_pi.GetYaxis().GetTitle(),
            xValues=list(tge_eres_pi.GetX()),
            yValues=list(tge_eres_pi.GetY()),
            yStatErrorsMinus=list(tge_eres_pi.GetEY()),
            yStatErrorsPlus=list(tge_eres_pi.GetEY()),
        )
        # Energy response kaons
        yield getJSON(jobs[0], 'chart',
            mctool_name='GEANT4',
            mctool_model=physlist,
            targetName='ATLAS-TileCal',
            beamParticle='kaon+',
            beamEnergies=list(tge_eresp_k.GetX()),
            observableName='Energy Response',
            xAxisName=tge_eresp_k.GetXaxis().GetTitle(),
            yAxisName=tge_eresp_k.GetYaxis().GetTitle(),
            xValues=list(tge_eresp_k.GetX()),
            yValues=list(tge_eresp_k.GetY()),
            yStatErrorsMinus=list(tge_eresp_k.GetEY()),
            yStatErrorsPlus=list(tge_eresp_k.GetEY()),
        )
        # Energy resolution kaons
        yield getJSON(jobs[0], 'chart',
            mctool_name='GEANT4',
            mctool_model=physlist,
            targetName='ATLAS-TileCal',
            beamParticle='kaon+',
            beamEnergies=list(tge_eresp_k.GetX()),
            observableName='Energy Resolution',
            xAxisName=tge_eres_k.GetXaxis().GetTitle(),
            yAxisName=tge_eres_k.GetYaxis().GetTitle(),
            xValues=list(tge_eres_k.GetX()),
            yValues=list(tge_eres_k.GetY()),
            yStatErrorsMinus=list(tge_eres_k.GetEY()),
            yStatErrorsPlus=list(tge_eres_k.GetEY()),
        )
        # Energy response protons
        yield getJSON(jobs[0], 'chart',
            mctool_name='GEANT4',
            mctool_model=physlist,
            targetName='ATLAS-TileCal',
            beamParticle='proton',
            beamEnergies=list(tge_eresp_p.GetX()),
            observableName='Energy Response',
            xAxisName=tge_eresp_p.GetXaxis().GetTitle(),
            yAxisName=tge_eresp_p.GetYaxis().GetTitle(),
            xValues=list(tge_eresp_p.GetX()),
            yValues=list(tge_eresp_p.GetY()),
            yStatErrorsMinus=list(tge_eresp_p.GetEY()),
            yStatErrorsPlus=list(tge_eresp_p.GetEY()),
        )
        # Energy resolution protons
        yield getJSON(jobs[0], 'chart',
            mctool_name='GEANT4',
            mctool_model=physlist,
            targetName='ATLAS-TileCal',
            beamParticle='proton',
            beamEnergies=list(tge_eresp_p.GetX()),
            observableName='Energy Resolution',
            xAxisName=tge_eres_p.GetXaxis().GetTitle(),
            yAxisName=tge_eres_p.GetYaxis().GetTitle(),
            xValues=list(tge_eres_p.GetX()),
            yValues=list(tge_eres_p.GetY()),
            yStatErrorsMinus=list(tge_eres_p.GetEY()),
            yStatErrorsPlus=list(tge_eres_p.GetEY()),
        )

        #-------------------------------------------------------------
	    # Create JSON files for experimental data
        # uncomment this part only if you want to recreate these files
        # usually this is done in one upload and the never again
        #-------------------------------------------------------------
        """
        if physlist=='FTFP_BERT':  # only do once
            print('Creating JSONs for experimental data')
            tge_atl_eresp_pi = tf.Get('ATLAS Energy Response Pions')
            tge_atl_eres_pi = tf.Get('ATLAS Energy Resolution Pions')
            tge_atl_eresp_k = tf.Get('ATLAS Energy Response Kaons')
            tge_atl_eres_k = tf.Get('ATLAS Energy Resolution Kaons')
            tge_atl_eresp_p = tf.Get('ATLAS Energy Response Protons')
            tge_atl_eres_p = tf.Get('ATLAS Energy Resolution Protons')
            # Energy response pions
            yield getJSON(jobs[0], 'chart',
                mctool_name="experiment",
                mctool_version="experiment",
                mctool_model="experiment",
                targetName='ATLAS-TileCal',
                beamParticle='pi+',
                beamEnergies=list(tge_atl_eresp_pi.GetX()),
                observableName='Energy Response',
                xAxisName=tge_atl_eresp_pi.GetXaxis().GetTitle(),
                yAxisName=tge_atl_eresp_pi.GetYaxis().GetTitle(),
                xValues=list(tge_atl_eresp_pi.GetX()),
                yValues=list(tge_atl_eresp_pi.GetY()),
                yStatErrorsMinus=list(tge_atl_eresp_pi.GetEY()),
                yStatErrorsPlus=list(tge_atl_eresp_pi.GetEY()),
            )
            # Energy resolution pions
            yield getJSON(jobs[0], 'chart',
                mctool_name="experiment",
                mctool_version="experiment",
                mctool_model="experiment",
                targetName='ATLAS-TileCal',
                beamParticle='pi+',
                beamEnergies=list(tge_atl_eresp_pi.GetX()),
                observableName='Energy Resolution',
                xAxisName=tge_atl_eres_pi.GetXaxis().GetTitle(),
                yAxisName=tge_atl_eres_pi.GetYaxis().GetTitle(),
                xValues=list(tge_atl_eres_pi.GetX()),
                yValues=list(tge_atl_eres_pi.GetY()),
                yStatErrorsMinus=list(tge_atl_eres_pi.GetEY()),
                yStatErrorsPlus=list(tge_atl_eres_pi.GetEY()),
            )
            # Energy response kaons
            yield getJSON(jobs[0], 'chart',
                mctool_name="experiment",
                mctool_version="experiment",
                mctool_model="experiment",
                targetName='ATLAS-TileCal',
                beamParticle='kaon+',
                beamEnergies=list(tge_atl_eresp_k.GetX()),
                observableName='Energy Response',
                xAxisName=tge_atl_eresp_k.GetXaxis().GetTitle(),
                yAxisName=tge_atl_eresp_k.GetYaxis().GetTitle(),
                xValues=list(tge_atl_eresp_k.GetX()),
                yValues=list(tge_atl_eresp_k.GetY()),
                yStatErrorsMinus=list(tge_atl_eresp_k.GetEY()),
                yStatErrorsPlus=list(tge_atl_eresp_k.GetEY()),
            )
            # Energy resolution kaons
            yield getJSON(jobs[0], 'chart',
                mctool_name="experiment",
                mctool_version="experiment",
                mctool_model="experiment",
                targetName='ATLAS-TileCal',
                beamParticle='kaon+',
                beamEnergies=list(tge_atl_eresp_k.GetX()),
                observableName='Energy Resolution',
                xAxisName=tge_atl_eres_k.GetXaxis().GetTitle(),
                yAxisName=tge_atl_eres_k.GetYaxis().GetTitle(),
                xValues=list(tge_atl_eres_k.GetX()),
                yValues=list(tge_atl_eres_k.GetY()),
                yStatErrorsMinus=list(tge_atl_eres_k.GetEY()),
                yStatErrorsPlus=list(tge_atl_eres_k.GetEY()),
            )
            # Energy response protons
            yield getJSON(jobs[0], 'chart',
                mctool_name="experiment",
                mctool_version="experiment",
                mctool_model="experiment",
                targetName='ATLAS-TileCal',
                beamParticle='proton',
                beamEnergies=list(tge_atl_eresp_p.GetX()),
                observableName='Energy Response',
                xAxisName=tge_atl_eresp_p.GetXaxis().GetTitle(),
                yAxisName=tge_atl_eresp_p.GetYaxis().GetTitle(),
                xValues=list(tge_atl_eresp_p.GetX()),
                yValues=list(tge_atl_eresp_p.GetY()),
                yStatErrorsMinus=list(tge_atl_eresp_p.GetEY()),
                yStatErrorsPlus=list(tge_atl_eresp_p.GetEY()),
            )
            # Energy resolution protons
            yield getJSON(jobs[0], 'chart',
                mctool_name="experiment",
                mctool_version="experiment",
                mctool_model="experiment",
                targetName='ATLAS-TileCal',
                beamParticle='proton',
                beamEnergies=list(tge_atl_eresp_p.GetX()),
                observableName='Energy Resolution',
                xAxisName=tge_atl_eres_p.GetXaxis().GetTitle(),
                yAxisName=tge_atl_eres_p.GetYaxis().GetTitle(),
                xValues=list(tge_atl_eres_p.GetX()),
                yValues=list(tge_atl_eres_p.GetY()),
                yStatErrorsMinus=list(tge_atl_eres_p.GetEY()),
                yStatErrorsPlus=list(tge_atl_eres_p.GetEY()),
            )
        """

        print('end parsing for ' + physlist)
