# ATLTileCalTB
A Geant4 simulation of the ATLAS Tile Calorimeter beam tests.

![example workflow](https://github.com/lopezzot/ATLTileCalTB/actions/workflows/geant4-test.yml/badge.svg)
![GitHub commits since latest release (by date)](https://img.shields.io/github/commits-since/lopezzot/ATLTileCalTB/latest/main)

<figure>
<img src="./images/TileCal_mu.gif" alt="Trulli" style="width:100%">
<figcaption align="center"><b>Fig. - 10 GeV muon passing through the ATLAS TileCal.</b></figcaption>
</figure>

<br/><br/>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#project-description">Project description</a></li>
    <li><a href="#authors-and-contacts">Authors and contacts</a></li>
    <li>
      <a href="#results">Results</a>
      <ul>
        <li><a href="#selected-reports-and-presentations">Selected reports and presentations</a></li>
        <li><a href="#available-datasets-and-analyses">Available datasets and analyses</a></li>
      </ul>
   </li>
    <li>
      <a href="#how-to">How to</a>
      <ul>
        <li><a href="#build-compile-and-execute-on-maclinux">Build, compile and execute on Mac/Linux</a></li>
        <li><a href="#build-compile-and-execute-on-lxplus">Build, compile and execute on lxplus</a></li>
        <li><a href="#submit-a-job-with-htcondor-on-lxplus">Submit a job with HTCondor on lxplus</a></li>
        <li><a href="#use-flukacern-hadron-inelastic-process">Use Fluka.Cern hadron inelastic process</a></li>
      </ul>
    </li>
    <li><a href="#geant-val-integration">Geant Val integration</a></li>
    <li><a href="#cmake-options">CMake options</a></li>
    <li><a href="#run-the-analysis">Run the analysis</a></li>
    <li><a href="#selected-atlas-tilecal-references">Selected ATLAS TileCal references</a></li>
  </ol>
</details>

<!--Project desription-->
## Project description
The project targets a standalone Geant4 simulation of the ATLAS Tile Calorimeter beam tests to perform Geant4 regression testing, physics lists comparison and validation against test-beam data.
- ⏰ Start date: 23 May 2022
- 📌 Status: from v1.0 results are deployed on geant-val
  
<!--Authors and contacts-->
## Authors and contacts
- 👨‍🔬 Lorenzo Pezzotti (CERN EP-SFT) - lorenzo.pezzotti@cern.ch 
- 👨‍🔬 Stephan Lachnit (CERN EP-SFT Summer Student) - stephan.lachnit@cern.ch
- 👨‍🔬 Supervisor: Alberto Ribon (CERN EP-SFT)

<!--Results-->
## Results

### Selected reports and presentations
- 🗣️ CALOR2024 22/5/2024, [**Validating Geant4 against calorimeters beam tests**](https://indico.cern.ch/event/1339557/contributions/5898513/attachments/2860143/5004975/lopezzot_CALOR2024.pdf)
- 🗣️ CERN EP-SFT Group Meeting 22/4/2024, [**End of Fellowship Report**](https://indico.cern.ch/event/1395384/contributions/5865083/attachments/2841640/4967534/lopezzot_epsft_22_4_2024.pdf)
- 🗣️ Geant4 Collaboration Meeting 2023 25-29/9/2023, [**Validation of Geant4 physics via calorimeter test-beams in geant-val**](https://indico.cern.ch/event/1307331/contributions/5581219/attachments/2721132/4728358/lopezzot_ValidationCalorimeters_G4CM23.pdf)
- 🗣️ ATLAS Simulation Group Meeting 5/9/2023 (ATLAS restricted), [**Testing ATLAS calorimeters with Geant4 and FLUKA.CERN**](https://indico.cern.ch/event/1321227/contributions/5565265/attachments/2709186/4704111/lopezzot_ATLASsim_5_9_2023.pdf)
- 🗣️ CERN EP-SFT Simulation Meeting 5/9/2023, [**Testing ATLAS calorimeters with Geant4 and FLUKA.CERN**](https://indico.cern.ch/event/1322382/contributions/5565251/attachments/2708985/4703734/lopezzot_5_9_2023.pdf)
- 🗣️ 11th BTTB Workshop 19/4/2023, [**Geant4 Validation on Test-Beam Calorimetry Data**](https://indico.cern.ch/event/1232761/contributions/5320333/attachments/2631283/4551077/lopezzot_11thBTTBWS_19_4_2023.pdf)
- 🗣️ Tile TB at the TileWeek 8/2/2023 (ATLAS restricted), [**Geant4 validation studies using the tile calorimeter beam tests**](https://indico.cern.ch/event/1248628/contributions/5246474/attachments/2589656/4468704/lopezzot_g4_tileweek_8_2_2023.pdf)
- 🗣️ Geant4 Collaboration Meeting 2022 28/9/2022, [**Physics validation of Geant4 via calorimeter test-beams**](https://indico.cern.ch/event/1156193/contributions/5051067/attachments/2517516/4328428/lopezzot_G4Collab2022.pdf)
- 🗣️ ATLAS Simulation Group Meeting 20/9/2022, [**Standalone Geant4 validation on the ATLAS Tile Calorimeter beam test**](https://indico.cern.ch/event/1201254/contributions/5051144/attachments/2512240/4318569/tilecalg4_atlassim_20_9_2022.pdf)
- 📄 CERN summer student report 25/8/2022, [**Standalone Geant4 validation of the ATLAS Tile Calorimeter**](https://cds.cern.ch/record/2825325)
- 🗣️ CERN EP-SFT Simulation Meeting 23/8/2022, [**Standalone Geant4 validation on the ATLAS Tile Calorimeter beam test**](https://indico.cern.ch/event/1192377/contributions/5013459/attachments/2496124/4287061/lopezzot_g4meeting_23_8_2022.pdf)
- 🗣️ CERN EP-SFT Group Meeting 22/8/2022, [**Simulating ATLAS TileCal for Geant4 validation**](https://indico.cern.ch/event/1180970/contributions/4960934/attachments/2495153/4285305/CERN_SS_Presentation.pdf)

### Available datasets and analyses
We provide datasets and ROOT analyses, as well as instructions for their reproducibility.
Ask authors for access to datasets. Results are deployed on Geant Val.
<details><summary>Geant-val-results-table</summary>
<p>

| ATLTileCalTB     | Reproduce data | Reproduce analysis | Comments     |
| -------------    | ----------     | -----------        | -----------  |
| v1.3 <br /> Geant4 11.2.p01 <br /> tag 1.3_1 <br /> FTFP_BERT(+tunes1,2,3), FTFP_BERT_ATL, QGSP_BERT, FTFP_INCLXX <br /> 300k events per run <br /> Added 19/4/2024 | Run with Geant Val, OR <br /> `./ATLTileCalTB -m TBrun_all.mac -p $PHYSLIST` | Run with Geant Val, OR <br /> `root ../analysis/TBrun_all.C` | First results produced on new el9 lxplus machine. |
| v1.1 <br /> Geant4 11.1.p01 <br /> tag 1.1_2 <br /> FTFP_INCLXX Added 2/5/2023 | Run with Geant Val, OR <br /> `./ATLTileCalTB -m TBrun_all.mac -p $PHYSLIST` | Run with Geant Val, OR <br /> `root ../analysis/TBrun_all.C` | FTFP_INCLXX results only because results 1.1_1 do not contain FTFP_INCLXX |
| v1.1 <br /> Geant4 11.1 <br /> tag 1.1_1 <br /> FTFP_BERT(+tunes1,2,3), FTFP_BERT_ATL, QGSP_BERT (tag v1.1_1) <br /> 300k events per run <br /> Added 13/2/2023 | Run with Geant Val, OR <br /> `./ATLTileCalTB -m TBrun_all.mac -p $PHYSLIST` | Run with Geant Val, OR <br /> `root ../analysis/TBrun_all.C` | FTFP_INCLXX results not included due to a crash, to be investigated (problem with merged root files by parser.py) |
| v1.0 <br /> Geant4 10.4.p03, 10.5.p01, 10.6.p03, 10.7.p03, 11.0.p02 <br /> FTFP_BERT, FTFP_BERT_ATL, QGSP_BERT, FTFP_INCLXX <br /> 300k events per run <br /> Added 17/8/2022 | Run with Geant Val, OR <br /> `./ATLTileCalTB -m TBrun_all.mac -p $PHYSLIST` | Run with Geant Val, OR <br /> `root ../analysis/TBrun_all.C` | Adjusted events per run to 300k |
</p>
</details>
<details><summary>Other-results-table</summary>
<p>

| ATLTileCalTB     | Reproduce data | Reproduce analysis | Comments     |
| -------------    | ----------     | -----------        | -----------  |
| v1.2 <br /> Dataset #3 <br /> Geant4.10.6.3 (modified) <br /> tag 1.2_3 <br /> FTFP_BERT <br /> Created on 3/9/2023 | `./ATLTileCalTB -m TBrun_all.mac -p FTFP_BERT` | `root TBrun_all.C` | Results to test the new FTF parameters by A. Ribon to fix the fluctuations disprepancy with respect to ATLAS data. Excitation energy per wounded nucleon set to 50 MeV. |
| v1.2 <br /> Dataset #2 <br /> Geant4.10.6.3 (modified) <br /> tag 1.2_2 <br /> FTFP_BERT <br /> Created on 17/8/2023 | `./ATLTileCalTB -m TBrun_all.mac -p FTFP_BERT` | `root TBrun_all.C` | Results to test the new FTF parameters by A. Ribon to fix the fluctuations disprepancy with respect to ATLAS data. |
| v1.2 <br /> Dataset #1 <br /> Geant4 11.1.ref05 & fluka4-3.3 <br /> tag 1.2_1 <br /> FTFP_BERT and FTFP_BERT+fluka.cern-interface <br /> Created on 5/8/2023 | `./ATLTileCalTB -m TBrun_all.mac -p FTFP_BERT` | `root FLUKA_comparison.C` | Data produced to compare results between Geant4 and FLUKA by using the Fluka.Cern interface. FTFP_BERT was used and compared to its Fluka.Cern customized version available in this repo. The interface was compiled as provided in the Geant4 11.1.ref05 examples. [See how to use it](#use-flukacern-hadron-inelastic-process). |
</p>
</details>

<!--How to-->
## How to

### Build, compile and execute on Mac/Linux
1.  git clone the repo
    ```sh
    git clone https://github.com/lopezzot/ATLTileCalTB.git
    ```
2.  source Geant4 env
    ```sh
    source /relative_path_to/geant4.10.07_p03-install/bin/geant4.sh
    ```
3.  cmake build directory and make (using geant4.10.07_p03)
    ```sh
    mkdir ATLTileCalTB-build; cd ATLTileCalTB-build/
    cmake -DGeant4_DIR=/absolute_path_to/geant4.10.07_p03-install/lib/Geant4-10.7.3/ relative_path_to/ATLTileCalTB/
    make
    ```
    See [CMake options](#cmake-options) for all build options
4.  execute (example with TBrun.mac macro card, 2 threads and FTFP_BERT physics list)
    ```sh
    ./ATLTileCalTB -m TBrun.mac -t 2 -p FTFP_BERT
    ```

Parser options
- `-m macro.mac`: pass a Geant4 macro card (example `-m ATLTileCalTB_run.mac` available in source directory and automatically copied in build directory) 
- `-t integer`: pass number of threads for multi-thread execution (example `-t 2`, default is the number of threads on the machine)
- `-p Physics_List`: select Geant4 physics list (example `-p FTFP_BERT`)
- It is possible to select alternative FTF tunings with PL_tuneID (example -p FTFP_BERT_tune0) [only for Geant4-11.1.0 or higher]

### Build, compile and execute on lxplus
1. git clone the repo
   ```sh
   git clone https://github.com/lopezzot/ATLTileCalTB.git
   ```
2. cmake build directory and make (using geant4.10.07_p03, check for gcc and cmake dependencies for other versions)
   ```sh
   mkdir ATLTileCalTB-build; cd ATLTileCalTB-build/
   cp ../ATLTileCalTB/scripts/ATLTileCalTB_cvmfs_setup.sh ../ATLTileCalTB/scripts/ATLTileCalTB_lxplus_10.7.p03.sh .
   source ./ATLTileCalTB_lxplus_10.7.p03.sh -DBUILD_ANALYSIS=OFF
   ```
3. execute (example with TBrun.mac macro card, 4 threads and FTFP_BERT physics list)
   ```sh
   ./ATLTileCalTB -m TBrun.mac -t 4 -p FTFP_BERT
   ```
   
### Submit a job with HTCondor on lxplus
1. [First follow the build instructions on lxplus](#build-compile-and-execute-on-lxplus)
2. prepare for HTCondor submission (example with Geant4.10.07_p03, TBrun.mac, 4 threads, FTFP_BERT physics list)
    ```sh
    mkdir -p error log output
    cp ../ATLTileCalTB/scripts/ATLTileCalTB_HTCondor.sub ../ATLTileCalTB/scripts/ATLTileCalTB_HTCondor_10.7.p03.sh .
    sed -i "2 i cd $(pwd)" ATLTileCalTB_HTCondor_10.7.p03.sh
    echo ./ATLTileCalTB -m TBrun.mac -t 4 -p FTFP_BERT >> ATLTileCalTB_HTCondor_10.7.p03.sh
    sed -i "1 i executable = ATLTileCalTB_HTCondor_10.7.p03.sh" ATLTileCalTB_HTCondor.sub
    ```
3. submit a job
   ```sh
   condor_submit ATLTileCalTB_HTCondor.sub 
   ```
4. monitor the job
   ```sh
   condor_q
   ```
   or (for persistency)
   ```sh
   condor_wait -status log/*.log
   ```
5. additional info from HTCondor (optional) \
   rm all your jobs
    ```sh
   condor_rm username
   ```
   inspect your accounting group
   ```sh
   condor_q owner $LOGNAME -long | grep '^AccountingGroup' | sort | uniq -c
   ```
   display all accounting groups
   ```sh
   haggis group list
   ```
   display your accounting groups
    ```sh
   haggis rights
   ```
   check what accounting group a job has
   ```sh
   condor_q jobid.0 -af AccountingGroup
   ```
   specify the accounting group for yout job, to be added in the .sub file
   ```sh
   +AccountingGroup = "group_u_*"
   ```
   check job CPU usage
   ```sh
   condor_q -l jobid.0 | grep CPUsUsage
   ```
   ssh to machine where job is running
   ```sh
   condor_ssh_to_job jobid.0
   ```

### Use Fluka.Cern hadron inelastic process
`Geant4-11.1-ref05` introduces a Fluka.Cern interface to use the Fluka.Cern hadron inelastic process in any geant4 application as explained in `examples/extended/hadronic/FlukaCern`. The following are my instructions to use this repo with a customized FTFP_BERT physics list using it. It assumes that cvmfs is mounted (e.g. usage on lxplus).
1. Install Fluka.Cern from source code (example with fluka4-3.3)
   ```sh
   source /cvmfs/sft.cern.ch/lcg/contrib/gcc/10.1.0/x86_64-centos7/setup.sh
   cd fluka4-3.3 && make -j 4
   cd src/ && make cpp_headers
   mkdir /path-to/fluka4-3.3-install && make install DESTDIR=/path-to/fluka4-3.3-install/
   PATH="/absolute-path-to/fluka4-3.3-install/bin/":$PATH
   ```
2. Setup `geant4-11.1.ref05` and compile the fluka interface as in the example
   ```sh
   source /cvmfs/geant4.cern.ch/geant4/11.1.ref05/x86_64-centos7-gcc10-optdeb-MT/CMake-setup.sh 
   source /cvmfs/geant4.cern.ch/geant4/11.1.ref05/x86_64-centos7-gcc10-optdeb-MT/bin/geant4.sh 
   cd FlukaCern/FlukaInterface/
   make interface
   make env
   source env_FLUKA_G4_interface.sh 
   ```
3. Build and execute ATLTileCalTB
   ```sh
   git clone https://github.com/lopezzot/ATLTileCalTB.git
   mkdir ATLTileCalTB-build && cd ATLTileCalTB-build
   /cvmfs/sft.cern.ch/lcg/contrib/CMake/3.23.2/Linux-x86_64/bin/cmake -DG4_USE_FLUKA=1 ../ATLTileCalTB/
   make
   ```
   NOTE: the Fluka.Cern interface can only be used in single-threaded mode.

<!--Geant Val integration-->
## Geant Val integration
[Geant Val](https://geant-val.cern.ch/) is the Geant4 testing and validation suite. It is a project hosted on [gitlab.cern.ch](https://gitlab.cern.ch/GeantValidation) used to facilitate the maintenance and validation of Geant4 applications, referred to as <em>tests</em>.\
The following are instructions to use ATLTileCalTB within Geant Val, from batch submission to website deployment.
1. **On lxplus**, clone ATLTileCalTB and the Geant Val geant-config-generator
   ```sh
   git clone https://github.com/lopezzot/ATLTileCalTB
   git clone https://gitlab.cern.ch/GeantValidation/geant-config-generator.git
   ```
2. [Follow the build instructions on lxplus](#build-compile-and-execute-on-lxplus)
3. Copy the ATLTileCalTB geant val scripts into ```tests/geant4/```
   ```sh
   cp -r ATLTileCalTB/geantval_scripts/ATLTileCalTB/ geant-config-generator/tests/geant4/
   mkdir -p geant-config-generator/tests/geant4/ATLTileCalTB/files
   cp ATLTileCalTB/TileTB_2B1EB_nobeamline.gdml ATLTileCalTB/analysis/TBrun_all.C geant-config-generator/tests/geant4/ATLTileCalTB/files/
   ```
3. We will execute ATLTileCalTB via Geant Val using Geant4.10.7.p03, therefore we must make sure the file ```10.7.p03.sh``` exists in ```configs/geant/```. In the file ```10.7.p03.sh``` we also export the path to the ATLTileCalTB executable (compiled with 10.7.p03). \
   Copy the config file using:
   ```sh
   ./ATLTileCalTB/geantval_scripts/cpconf.sh \
      ATLTileCalTB/geantval_scripts/configs/10.7.p03.sh \
      geant-config-generator/configs/geant4/ \
      $(pwd)/ATLTileCalTB-build
   ```
4. Create macros and metadata for Geant Val execution
   ```sh
   cd geant-config-generator
   ./mc-config-generator.py submit -t ATLTileCalTB -d OUTPUT -v 10.7.p03 -q "testmatch" -r
   ```
   this command creates the Geant Val files for batch submission using HTCondor under the ```OUTPUT``` folder, using ATLTileCalTB, Geant4.10.7.p03 and the ```testmatch``` job flavour.
5. To monitor the jobs use
   ```sh
   ./mc-config-generator.py status -t ATLTileCalTB -d OUTPUT
   ```
   When the job execution ends, the root output files are stored in the corresponding job folder. Each job folder will look like this:
   ```
   ATLTileCalTB-env.log  test_stderr.txt  test_stdout.txt
   ATLTileCalTB.json  ATLTileCalTB.mac  ATLTileCalTBout_Run0.root
   ATLTileCalTB.sh  bsub.sh  config.sh  
   ```
6. Execute the analysis on the root files in the `OUTPUT` folder to create Geant Val JSON output files
   ```sh
   ./mc-config-generator.py parse -t ATLTileCalTB -d OUTPUT
   ```
   Make sure to source ROOT on CVMFS before, for example by sourcing the provided lxplus scripts.\
   Besides the JSON files in the `OUTPUTJSON` folder, the parser script will also create some
   temporary folder that contain the merged ROOT files for each run and the analysis output.
7. The last part is to deploy the results on Geant Val. The ATLTileCalTB layout on the Geant Val
   website is defined in the ATLTileCalTB.xml file on [gitlab.com/thegriglat/geant-val-layouts](https://gitlab.com/thegriglat/geant-val-layouts)
   (additional info are in the tags.json file).
   Deploy JSON files on the Geant Val database
   ```sh
    find . -name '*.json' | while read i; \
      do curl -H "Content-Type: application/json" -H "token: askauthor" --data @$i https://geant-val.cern.ch/upload; \
      echo; done
   ```

<!--CMake options-->
## CMake options
Custom options:
-  `BUILD_ANALYSIS`: if set to `ON` (default), it will be an executable of the analysis, which is
   slightly faster. The analysis can also be run directly with the `root` executable (see
   [Run the analysis](#run-the-analysis)), which is recommended if the compilation fails.
-  `WITH_ATLTileCalTB_PulseOutput`: if set to `ON`, the simulation will output the pulse response
   of the PMTs. These can be viewed by running `./pulse_viewer.py` in the build directory. Since
   this slows the simulation considerably, it is recommended to leave this option disabled except
   for debugging purposes.
-  `WITH_ATLTileCalTB_NoNoise`: if set to `ON`, the simulation will not put electronic noise on the
   signal (per cell) and disable the 2 sigma noise cut. Only relevant for noise calibration.
-  `WITH_GEANT4_UIVIS`: if set to `ON` (default), build with UI and visualization drivers.
-  `G4_USE_FLUKA`: if set to `ON` build against the Fluka.Cern interface (default `OFF`).
-  `WITH_LEAKAGEANALYSIS`: if set to `ON` build with leakage spectrum analyzer (default `OFF`).

Relevant built-in options:
-  `CMAKE_BUILD_TYPE`: set to `Debug` for debugging and to `Release` for production (faster).
-  `CMAKE_EXPORT_COMPILE_COMMANDS`: if `ON`, creates `compile_commands.json` in the build folder.
   Might help some syntax highlighters to properly parse the analysis script if building with
   `BUILD_ANALYSIS=ON`.
-  `Geant4_DIR`: set to the path containing the `Geant4Config.cmake` file of your Geant4 installation.
-  `ROOT_DIR`: set to the path containing th `ROOTConfig.cmake` of your ROOT installation. Only
   used if building with `BUILD_ANALYSIS=ON`.

<!--Run the analysis-->
## Run the analysis
1. For the analysis both electron and pion data is required, which can be obtained by running for example `ATLTileCalTB -m TBrun_all.mac`.
2. After the run, all the ROOT files and their containing TTree need to merged into a single file called `ATLTileCalTBout_All.root`. This can be done with `hadd`:
   ```sh
   hadd -f ATLTileCalTBout_RunAll.root ATLTileCalTBout_Run*.root
   ```
3. To run the analysis, execute the analysis macro in the folder containing the root file:
   ```sh
   root /path/to/ATLTileCalTB/analysis/TBrun_all.C
   ```
   Alternatively, the analysis marco can also be build as executable for slightly faster executation time.
4. The plots created during the analysis are stored in the `analysis.root` file.

<!--Selected ATLAS TileCal references-->
## Selected ATLAS TileCal references
- 📄 <em>Study of energy response and resolution of the ATLAS Tile Calorimeter to hadrons of energies from 16 to 30 GeV</em>, Eur. Phys. J. C (2021) 81:549: [![Website shields.io](https://img.shields.io/website?url=https%3A%2F%2Flink.springer.com%2Farticle%2F10.1140%2Fepjc%2Fs10052-021-09292-5)](https://link.springer.com/article/10.1140/epjc/s10052-021-09292-5)
