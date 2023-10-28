cmake_minimum_required(VERSION 3.25)

if(simplepkg_POPULATED)
  return()
endif()

include(FetchContent)

function(simple_pkg NAME)
  string(TOLOWER "${NAME}" lcname)

  # This lets us switch between file-package and package-name without switching
  # between simple_pkg and include
  string(FIND "${NAME}" "/" is_path)
  if(${is_path} GREATER -1)
    include(${NAME})
    return(PROPAGATE CMAKE_MODULE_PATH)
  elseif(${${lcname}_POPULATED})
    unset(${lcname}_INIT CACHE)
    return()
  endif()

  cmake_parse_arguments(a NO_BUILD "" "" ${ARGN})

  message(STATUS "Simple package: ${NAME}")

  if(${NAME}_OVERRIDE)
    file(REAL_PATH ${${NAME}_OVERRIDE} _overridePath EXPAND_TILDE)

    if(NOT a_NO_BUILD)
      add_subdirectory(${_overridePath} "_override/${NAME}" SYSTEM)
    else()
      message(FATAL_ERROR "Implement non-cmake building?")
    endif()

    # Vaguely like FetchContent
    set(${lcname}_POPULATED True)
    set(${lcname}_SOURCE_DIR ${${NAME}_OVERRIDE})
    set(${lcname}_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/_override/${NAME}")
  else()
    FetchContent_Declare(${NAME} ${ARGN})

    if(a_NO_BUILD)
      FetchContent_Populate(${NAME})
    else()
      FetchContent_MakeAvailable(${NAME})
    endif()
  endif()

  # Let packages add modules
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)

  # Exports
  set(${lcname}_INIT True CACHE BOOL "" FORCE)
  set(${lcname}_POPULATED  "${${lcname}_POPULATED}" CACHE STRING "" FORCE)
  set(${lcname}_SOURCE_DIR "${${lcname}_SOURCE_DIR}" CACHE PATH "" FORCE)
  set(${lcname}_BINARY_DIR "${${lcname}_BINARY_DIR}" CACHE PATH "" FORCE)
endfunction()

function(simple_override NAME PATH)
  if(EXISTS ${PATH})
    set(found "found")
  else()
    set(found "NOT FOUND")
  endif()

  message(STATUS "SimplePkg override ${NAME}: ${PATH}... ${found}")
  set(${NAME}_OVERRIDE "${PATH}" CACHE PATH "Override path for ${NAME}" FORCE)
endfunction()

if(NOT SIMPLEPKG_NO_GLOBAL_OVERRIDES)
  include("$ENV{HOME}/.config/SimplePkg/pkg_overrides.cmake" OPTIONAL)
  include("$ENV{HOME}/Documents/SimplePkg/pkg_overrides.cmake" OPTIONAL)
else()
  message(STATUS "SimplePkg NO global overrides")
endif()

if(NOT SIMPLEPKG_NO_SOURCE_OVERRIDES)
  include("${CMAKE_SOURCE_DIR}/.pkg_overrides.cmake" OPTIONAL)
else()
  message(STATUS "SimplePkg NO source overrides")
endif()

foreach(filename IN LISTS SIMPLEPKG_OVERRIDES)
  message(STATUS "SimplePkg load overrides from: ${filename}")
  include(${filename})
endforeach()

set(simplepkg_POPULATED True CACHE BOOL "" FORCE)
