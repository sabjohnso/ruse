find_package(GTest)

macro(ruse_add_test test_name exe_name)
  add_executable(${exe_name} ${ARGN})
  target_link_libraries(${exe_name} PRIVATE
    ruse::header
    GTest::gtest_main)
  set_target_properties(${exe_name}
    PROPERTIES
    CXX_STANDARD ${ruse_CXX_STANDARD})
  add_test(${test_name} ${exe_name})
endmacro()

ruse_add_test(nothing nothing_test nothing_test.cpp)
ruse_add_test(pair pair_test pair_test.cpp)
ruse_add_test(type type_test type_test.cpp)
ruse_add_test(list list_test list_test.cpp)
