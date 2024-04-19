#!/bin/sh

export ROOT_VERSION="6.30.06"
export ROOT_PLATFORM="x86_64-almalinux9.3-gcc114-opt"
export G4GCC_VERSION="12"
export G4GCC_PLATFORM="x86_64-el9-gcc12-opt"
export GEANT4_VERSION="11.2.p01"
export GEANT4_PLATFORM="x86_64-el9-gcc12-optdeb-MT"
export GCC_VERSION="12"
export GCC_PLATFORM="x86_64-el9-gcc12-opt"

source ./ATLTileCalTB_cvmfs_setup.sh

export CC=$(command -v gcc)
export CXX=$(command -v g++)
cmake3 ../ATLTileCalTB $@
make -j$(nproc)

