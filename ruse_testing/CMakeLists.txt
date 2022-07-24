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
ruse_add_test(utility utility_test utility_test.cpp)
ruse_add_test(pair pair_test pair_test.cpp)
ruse_add_test(type type_test type_test.cpp)
ruse_add_test(list list_test list_test.cpp)
ruse_add_test(string string_test string_test.cpp)
ruse_add_test(monoid monoid_test monoid_test.cpp)
ruse_add_test(tag tag_test tag_test.cpp)
ruse_add_test(parser parser_test parser_test.cpp)
ruse_add_test(hoisted hoisted_test hoisted_test.cpp)
ruse_add_test(grammar grammar_test grammar_test.cpp)
ruse_add_test(character_range character_range_test character_range_test.cpp)
ruse_add_test(character_class character_class_test character_class_test.cpp)
