cmake_minimum_required(VERSION 3.17)
list(APPEND CMAKE_MODULE_PATH "$ENV{HOME}/vcs/CMakeFiles")
include(setup)

project(cxxutils)

enable_testing()

conan_cmake_run(
  CONANFILE ../conanfile.py
  PROFILE ${TOOLCHAIN}-${BUILD_CONFIG}
  SETTINGS compiler.libcxx=${CXX_STDLIB})

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Doxygen)

file(GLOB_RECURSE HEADERS
  include/*)

file(GLOB_RECURSE SRC
  src/*.cpp)

add_library(rpav_cxxutils STATIC ${SRC} ${HEADERS})
target_include_directories(rpav_cxxutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_cxx_std(rpav_cxxutils 20)
target_cxx_features(rpav_cxxutils filesystem)
target_link_libraries(rpav_cxxutils PUBLIC
  CONAN_PKG::GameKernelCore
  CONAN_PKG::utf8proc
  CONAN_PKG::nlohmann_json
  CONAN_PKG::simpleunits
  CONAN_PKG::Catch2
)

if(DOXYGEN_FOUND)
  set(DOXYGEN_LATEX_MAKEINDEX_CMD makeindex) # Bug in config
  doxygen_add_docs(cxxutils_doc ${HEADERS})
else()
  message(STATUS "Doxygen not found; documentation unavailable")
endif()

if(BUILD_TESTING)
  add_subdirectory(t)
endif()

SetupPost()
