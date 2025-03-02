option(CXXUTILS_FAILTESTS "Enable intended-failure compilation tests (a bit slow)" ON)

add_executable(rpav_cxxutils_tests
  any.cpp
  bits.cpp
  ctti.cpp
  dynarray.cpp
  easing.cpp
  gk.cpp
  iterator.cpp
  json.cpp
  math.cpp
  main.cpp
  modify_track.cpp
  observer.cpp
  ptr.cpp
  ringbuffer.cpp
  span.cpp
  str_std.cpp
  typeutil.cpp
  ustring.cpp
)
target_cxx_std(rpav_cxxutils_tests 20)
target_include_directories(rpav_cxxutils_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rpav_cxxutils_tests PRIVATE
  rpav_cxxutils
  GameKernelCore
  Catch2
)

add_test(NAME test_cxxutils COMMAND rpav_cxxutils_tests)

if(CXXUTILS_FAILTESTS)
  add_subdirectory(fail)
endif()
