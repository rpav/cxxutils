((tasks
  (build
   (:name "Test")
   (:window "Build/Run %p")
   (:command "grunt" "cmake_build:all"))
  (run
   (:name "Run")
   (:window "Build/Run %p")
   (:command "grunt" "cmake_build_run:test"))
  (clean
   (:name "Clean %p Build")
   (:window "Build/Run %p")
   (:command "grunt" "cmake_build:clean"))
  (cmake
   (:name "Configure CMake")
   (:window "Build/Run %p")
   (:command "grunt" "cmake_config"))
  (cmake-clear-cache
   (:name "Clear Cache and Configure CMake")
   (:window "Build/Run %p")
   (:command "grunt" "cmake_config --clear-cache"))))
