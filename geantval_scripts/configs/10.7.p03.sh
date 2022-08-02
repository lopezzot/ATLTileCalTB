#!/bin/bash
# see scripts/ATLTileCalTB_cvmfs_setup.sh and scripts/ATLTileCalTB_HTCondor_10.7.p03.sh

export G4GCC_VERSION="10"
export G4GCC_PLATFORM="x86_64-centos7-gcc10-opt"
export GEANT4_VERSION="10.7.p03"
export GEANT4_PLATFORM="x86_64-centos7-gcc10-optdeb-MT"
export GCC_VERSION="10"
export GCC_PLATFORM="x86_64-centos7-gcc10-opt"

source /cvmfs/sft.cern.ch/lcg/contrib/gcc/${G4GCC_VERSION}/${G4GCC_PLATFORM}/setup.sh
source /cvmfs/geant4.cern.ch/geant4/${GEANT4_VERSION}/${GEANT4_PLATFORM}/bin/geant4.sh
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/${GCC_VERSION}/${GCC_PLATFORM}/setup.sh

# Test path
GVALPATH="/cvmfs/geant4.cern.ch/opt-geant-val/$GEANT4_VERSION/$GEANT4_PLATFORM/bin:/cvmfs/geant4.cern.ch/opt/$GEANT4_VERSION/$GEANT4_PLATFORM/bin"
YOURPATH="@YOURPATH@"
export PATH="$GVALPATH:$PATH:$YOURPATH"

# Geant Val exports
export VERSION=$GEANT4_VERSION
export PLATFORM=$GEANT4_PLATFORM
