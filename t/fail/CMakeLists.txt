include(failtest)

add_failtest(fail_ptr_const1
    CLANG "error: cannot assign to return value"
    GNU "error: assignment of read-only location")
target_cxx_std(fail_ptr_const1 17)
target_link_libraries(fail_ptr_const1 PRIVATE rpav_cxxutils)

add_failtest(fail_ptr_const2
    CLANG "error: cannot assign to return value"
    GNU "error: assignment of read-only location")
target_cxx_std(fail_ptr_const2 17)
target_link_libraries(fail_ptr_const2 PRIVATE rpav_cxxutils)

add_failtest(fail_ptr_const3
    CLANG "error: cannot initialize a member subobject of type"
    GNU "error: invalid conversion")
target_cxx_std(fail_ptr_const3 17)
target_link_libraries(fail_ptr_const3 PRIVATE rpav_cxxutils)
