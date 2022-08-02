find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

option(WITH_ATLTileCalTBana_CtotClongHists "enable Ctot and Clong histograms (slow)" OFF)
if(WITH_ATLTileCalTBana_CtotClongHists)
  add_compile_definitions(ATLTileCalTBana_CtotClongHists)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(ATLTileCalTBana main.cc TBrun_all.C)
target_link_libraries(ATLTileCalTBana ${ROOT_LIBRARIES})
set_target_properties(ATLTileCalTBana PROPERTIES CXX_STANDARD 17)

install(TARGETS ATLTileCalTBana DESTINATION bin)
