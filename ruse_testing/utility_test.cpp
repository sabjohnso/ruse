//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::testing{
  TEST(curry, identity){
    constexpr auto constant = curry(nat<1>, [](auto f){ return f; });
    constexpr auto sqr = [](auto x){ return x*x; };
    STATIC_EXPECT_EQ(4, constant(sqr, 2));
  }
} // end of namespace ruse::testing
