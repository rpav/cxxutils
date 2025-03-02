cmake_minimum_required(VERSION 3.17)
project(cxxutils)

option(CXXUTILS_BUILD_TESTING "Build testing for rpav_cxxutils" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(SimplePkg)

find_package(Doxygen)

file(GLOB_RECURSE HEADERS
  include/*)

file(GLOB_RECURSE SRC
  src/*.cpp)

simple_pkg(SimplePkg-Repo)
simple_pkg(CMakeUtils)

simple_pkg(simpleunits)

simple_pkg(pkg/nlohmann_json/3.9.1)
simple_pkg(pkg/GameKernelCore/master)
simple_pkg(pkg/Catch2/2.13.8)

include(setup)

if(CXXUTILS_BUILD_TESTING)
  enable_testing()
  add_subdirectory(t)
endif()

add_library(rpav_cxxutils STATIC ${SRC} ${HEADERS})
target_include_directories(rpav_cxxutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_cxx_std(rpav_cxxutils 20)
target_cxx_features(rpav_cxxutils filesystem)
target_link_libraries(rpav_cxxutils PUBLIC
  GameKernelCore
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
