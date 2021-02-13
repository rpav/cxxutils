CPMAddPackage(
  NAME CMakeSetup
  GIT_TAG master
  GIT_REPOSITORY ssh://git@git.mephle.net:10022/rpav/CMakeFiles.git
  DOWNLOAD_ONLY
  )
list(APPEND CMAKE_MODULE_PATH "${CMakeSetup_SOURCE_DIR}")
