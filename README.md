# ATLTileCalTB
A Geant4 simulation of the ATLAS Tile Calorimeter beam tests.

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
      <a href="#how-to">How to</a>
      <ul>
        <li><a href="#build-compile-and-execute-on-maclinux">Build, compile and execute on Mac/Linux</a></li>
        <li><a href="#build-compile-and-execute-on-lxplus">Build, compile and execute on lxplus</a></li>
        <li><a href="#submit-a-job-with-htcondor-on-lxplus">Submit a job with HTCondor on lxplus</a></li>
      </ul>
    </li>
    <li><a href="#geant-val-integration">Geant Val integration</a></li>
    <li><a href="#run-the-analysis"></a>Run the analysis</li>
    <li><a href="#selected-atlas-tilecal-references">Selected ATLAS TileCal references</a></li>
  </ol>
</details>

<!--Project desription-->
## Project description
The project targets a standalone Geant4 simulation of the ATLAS Tile Calorimeter beam tests to perform Geant4 regression testing, physics lists comparison and validation against test-beam data.
- ⏰ Start date: 23 May 2023 
- 📌 Status: under development
  
<!--Authors and contacts-->
## Authors and contacts
- 👨‍🔬 Lorenzo Pezzotti (CERN EP-SFT) - lorenzo.pezzotti@cern.ch 
- 👨‍🔬 Stephan Lachnit (CERN EP-SFT Summer Student) - stephan.lachnit@cern.ch
- 👨‍🔬 Supervisor: Alberto Ribon (CERN EP-SFT)

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
4.  execute (example with TBrun.mac macro card, 2 threads and FTFP_BERT physics list)
    ```sh
    ./ATLTileCalTB -m TBrun.mac -t 2 -p FTFP_BERT
    ```

Parser options
- `-m macro.mac`: pass a Geant4 macro card (example `-m ATLTileCalTB_run.mac` available in source directory and automatically copied in build directory) 
- `-t integer`: pass number of threads for multi-thread execution (example `-t 2`, default is the number of threads on the machine)
- `-p Physics_List`: select Geant4 physics list (example `-p FTFP_BERT`)

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
   cp ATLTileCalTB/TileTB_2B1EB_nobeamline.gdml ATLTileCalTB/analysis/main.cc geant-config-generator/tests/geant4/ATLTileCalTB/files/
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

<!--Run the analysis-->
## Run the analysis
1. For the analysis both electron and pion data is required, which can be obtained by running for example `ATLTileCalTB -m TBrun_all.mac`.
2. After the run, all the ROOT files and their containing TTree need to merged into a single file called `ATLTileCalTBout_All.root`. This can be done with `hadd`:
   ```sh
   hadd -f ATLTileCalTBout_All.root ATLTileCalTBout_Run*.root
   ```
3. To run the analysis, execute the analysis macro in the folder containing the root file:
   ```sh
   root /path/to/ATLTileCalTB/analysis/TBrun_all.C
   ```
   Alternatively, the analysis marco can also be build as executable for slightly faster executation time.
4. The plots created during the analysis are stored in the `analysis.root` file.

<!--Selected ATLAS TileCal references-->
## Selected ATLAS TileCal references
- 📄 <em>Study of energy response and resolution of the ATLAS Tile Calorimeter to hadrons of energies from 16 to 30 GeV</em>, Eur. Phys. J. C (2021) 81:549: [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://link.springer.com/article/10.1140/epjc/s10052-021-09292-5)
