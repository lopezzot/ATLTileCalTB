#!/bin/sh

CENTOS_VERSION="7"
GCC_VERSION="10"
CMVFS_SETUP="/cvmfs/sft.cern.ch/lcg/contrib/gcc/${GCC_VERSION}/x86_64-centos${CENTOS_VERSION}/setup.sh"
GEANT4_BASE="/cvmfs/geant4.cern.ch/geant4/10.7.p03/x86_64-centos${CENTOS_VERSION}-gcc${GCC_VERSION}-optdeb-MT"
GEANT4_CMAKE="${GEANT4_BASE}/lib64/Geant4-10.7.3"
GEANT4_SETUP="${GEANT4_BASE}/bin/geant4.sh"

source ${CMVFS_SETUP}
source ${GEANT4_SETUP}
