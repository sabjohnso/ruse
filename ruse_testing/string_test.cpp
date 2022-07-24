//
// ... Standard header files
//
#include <type_traits>
//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::testing
{
  TEST(hoist, string){
    STATIC_EXPECT_TRUE(std::is_same_v<hoisted_list<'a', 'b', 'c'>, decltype("abc"_hoist)>);
  }

  TEST(string, string){
    STATIC_EXPECT_TRUE(is_string("abc"_s));
  }

} // end of namespace ruse::testing
