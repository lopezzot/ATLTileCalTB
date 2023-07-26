#!/bin/sh

export ROOT_VERSION="6.24.06"
export ROOT_PLATFORM="x86_64-centos7-gcc48-opt"
export G4GCC_VERSION="10"
export G4GCC_PLATFORM="x86_64-centos7-gcc10-opt"
export GEANT4_VERSION="11.1.ref05"
export GEANT4_PLATFORM="x86_64-centos7-gcc10-optdeb-MT"
export GCC_VERSION="10"
export GCC_PLATFORM="x86_64-centos7-gcc10-opt"

source ./ATLTileCalTB_cvmfs_setup.sh

export CC=$(command -v gcc)
export CXX=$(command -v g++)
cmake3 ../ATLTileCalTB $@
make -j$(nproc)

