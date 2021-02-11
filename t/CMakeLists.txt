option(CXXUTILS_FAILTESTS "Enable intended-failure compilation tests (a bit slow)" ON)

add_executable(rpav_cxxutils_tests
  any.cpp
  bits.cpp
  channel.cpp
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
  var.cpp
)
target_cxx_std(rpav_cxxutils_tests 20)
target_link_libraries(rpav_cxxutils_tests PRIVATE
  rpav_cxxutils
  CONAN_PKG::GameKernelCore
  CONAN_PKG::Catch2
)

add_test(NAME test_cxxutils COMMAND rpav_cxxutils_tests)

if(CXXUTILS_FAILTESTS)
  add_subdirectory(fail)
endif()
