//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... Ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::testing {
  TEST(Pair, pair){ STATIC_EXPECT_TRUE(Pair<pair<int, char>>); }
  TEST(Pair, int) { STATIC_EXPECT_FALSE(Pair<int>); }
  TEST(is_pair, pair) {STATIC_EXPECT_TRUE(is_pair( pair<int, char>{.first = 1, .second = 'x'})); }
  TEST(is_pair, double) {STATIC_EXPECT_FALSE(is_pair(4.0)); }
  TEST(cons, constructs_a_pair){ STATIC_EXPECT_TRUE(is_pair(cons('x', 2.3))); }
  TEST(car, returns_first_from_pair){
    STATIC_EXPECT_EQ('x', car(cons('x', 2.3)));
  }
  TEST(cdr, returns_second_from_pair){
    STATIC_EXPECT_EQ(2.3, cdr(cons('x', 2.3)));
  }
}
