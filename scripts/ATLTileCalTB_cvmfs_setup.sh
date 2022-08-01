#!/bin/sh

# GCC version used to compile Geant4
G4GCC_SETUP="/cvmfs/sft.cern.ch/lcg/contrib/gcc/${G4GCC_VERSION}/${G4GCC_PLATFORM}/setup.sh"

# Geant4 version
GEANT4_SETUP="/cvmfs/geant4.cern.ch/geant4/${GEANT4_VERSION}/${GEANT4_PLATFORM}/CMake-setup.sh"

# GCC version to compile code (use GCC10 or later)
GCC_SETUP="/cvmfs/sft.cern.ch/lcg/contrib/gcc/${GCC_VERSION}/${GCC_PLATFORM}/setup.sh"

source ${G4GCC_SETUP}
source ${GEANT4_SETUP}
source ${GCC_SETUP}
