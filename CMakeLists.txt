##**************************************************
## \file CMakeLists.txt
## \brief: CMakeLists of ATLTileCalTB project
## \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
##          @lopezzot
## \start date: 23 May 2022
##**************************************************

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.12...3.18)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(ATLTileCalTB)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Option to enable pulse outputs
#
option(WITH_ATLTileCalTB_PulseOutput "enable pulse output (slow)" OFF)
if(WITH_ATLTileCalTB_PulseOutput)
  add_compile_definitions(ATLTileCalTB_PulseOutput)
endif()

#----------------------------------------------------------------------------
#Option to enable leakage particle spectrum analysis
#
option(WITH_LEAKAGEANALYSIS "enable SpectrumAnalysis on leakage" OFF)
if(WITH_LEAKAGEANALYSIS)
    add_compile_definitions(ATLTileCalTB_LEAKANALYSIS)
    message(STATUS "WITH_LEAKAGEANALYSIS = ON : building with ATLTileCalTB_LEAKANALYSIS compiler definition.")
else()
    message(STATUS "WITH_LEAKAGEANALYSIS = OFF : building without ATLTileCalTB_LEAKANALYSIS compiler definition.")
endif()

#----------------------------------------------------------------------------
# Option to disable noise
#
option(WITH_ATLTileCalTB_NoNoise "disable electronic noise" OFF)
if(WITH_ATLTileCalTB_NoNoise)
  add_compile_definitions(ATLTileCalTB_NoNoise)
endif()

#----------------------------------------------------------------------------
# Output pedantic warnings
#

add_compile_options(-Wall -Wextra -Wpedantic)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find FLUKA interface (optional) and add relative compile definitions.
# (this part is taken from /examples/extended/hadronic/FLUKACern/FlukaInterface/README.md
# as in geant4-11.1.ref05 (May 2023))
# FindFLUKAInterface.cmake is located at your_path_to_geant4/cmake/Modules/FindFLUKAInterface.cmake
# Check that FindFLUKAInterface.cmake can be found from $CMAKE_MODULE_PATH
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
# Otherwise, you can always prepend it to the cmake module search path with:
# set(CMAKE_MODULE_PATH my_path_to_find_fluka ${CMAKE_MODULE_PATH})

# Check whether FLUKA should be used or not
set(G4_USE_FLUKA OFF CACHE BOOL "Using FLUKA")
if(G4_USE_FLUKA)
  message(STATUS "G4_USE_FLUKA=ON : Using FLUKA interface for building ${PROJECT_SOURCE_DIR}")
  add_definitions(-DG4_USE_FLUKA)
  find_package(FLUKAInterface REQUIRED)
  if(FLUKAInterface_FOUND)
    message(STATUS "FLUKA cmake module was found : ${CMAKE_MODULE_PATH}")
  else()
    message(FATAL_ERROR "FLUKA cmake module was NOT found! Please add one.")
  endif()
else()
  message(STATUS "G4_USE_FLUKA=OFF : NOT using FLUKA interface for building ${PROJECT_SOURCE_DIR}. \n \
  If ever you want to use the FLUKA interface, please repeat cmake command with -DG4_USE_FLUKA=1")
endif()
#End of find FLUKAInterface

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
		    ${FLUKAInterface_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(ATLTileCalTB ATLTileCalTB.cc ${sources} ${headers})
target_link_libraries(ATLTileCalTB ${Geant4_LIBRARIES} ${FLUKAInterface_LIBRARIES})
set_target_properties(ATLTileCalTB PROPERTIES CXX_STANDARD 17)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build ATLTileCalTB.
#
set(ATLTileCalTB_SCRIPTS
    TileTB_2B1EB.gdml
    TileTB_2B1EB_nobeamline.gdml
    icons.mac
    gui.mac
    init_vis.mac
    vis.mac
    TBrun.mac
    TBrun_all.mac
    single.mac
    pulse_viewer.py
  )

foreach(_script ${ATLTileCalTB_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(G4ATLTileCalTB DEPENDS ATLTileCalTB)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ATLTileCalTB DESTINATION bin)

#----------------------------------------------------------------------------
# Add analysis
#
option(BUILD_ANALYSIS "build analysis (requires ROOT)" OFF)
if(BUILD_ANALYSIS)
  add_subdirectory(analysis)
endif()

##**************************************************
