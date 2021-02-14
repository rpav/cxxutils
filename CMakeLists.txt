cmake_minimum_required(VERSION 3.17)

option(RPAV_CXXUTILS_BUILD_TESTING "Build testing for rpav_cxxutils" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(pkg/CMakeSetup)

project(cxxutils)

find_package(Doxygen)

file(GLOB_RECURSE HEADERS
  include/*)

file(GLOB_RECURSE SRC
  src/*.cpp)

include(pkg/GameKernelCore)
include(pkg/utf8proc)
include(pkg/nlohmann_json)
include(pkg/simpleunits)
include(pkg/Catch2)

if(RPAV_CXXUTILS_BUILD_TESTING)
  enable_testing()
  add_subdirectory(t)
endif()

add_library(rpav_cxxutils STATIC ${SRC} ${HEADERS})
target_include_directories(rpav_cxxutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_cxx_std(rpav_cxxutils 20)
target_cxx_features(rpav_cxxutils filesystem)
target_link_libraries(rpav_cxxutils PUBLIC
  GameKernelCore
  utf8proc
  nlohmann_json
  simpleunits
  Catch2
)

if(DOXYGEN_FOUND)
  set(DOXYGEN_LATEX_MAKEINDEX_CMD makeindex) # Bug in config
  doxygen_add_docs(cxxutils_doc ${HEADERS})
else()
  message(STATUS "Doxygen not found; documentation unavailable")
endif()

Setup()
