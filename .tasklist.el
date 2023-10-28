((common
  (:variables
   ("build"     . "Debug")
   ("toolchain" . "gcc")
   ("target"    . "test"))
  (:cwd "_build/%toolchain-%build"))
 (tasks
  (build
   (:name "Build")
   (:window "Build/Run")
   (:command "mk"))
  (ctrl-shift-build
   (:name "Delete cache and run cmake")
   (:window "Build/Run")
   (:command "rm -f CMakeCache.txt; cmake ../.."))
  (run
   (:name "Run")
   (:window "Build/Run")
   (:command "mk && mk test"))
  (clean
   (:name "Clean build")
   (:window "Build/Run")
   (:command "mk clean"))))
