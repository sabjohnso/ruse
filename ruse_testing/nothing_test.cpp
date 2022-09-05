//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::testing {
   TEST(Nothing, nothing_s) { STATIC_EXPECT_TRUE(Nothing<nothing_s>); }

   TEST(Nothing, int) { STATIC_EXPECT_FALSE(Nothing<int>); }

   TEST(is_nothing, nothing) {
      STATIC_EXPECT_TRUE(is_nothing(nothing));
   }

   TEST(is_nothing, something) { STATIC_EXPECT_FALSE(is_nothing(3)); }

   TEST(nothing, equality_affirmative) {
      STATIC_EXPECT_TRUE(nothing == nothing);
   }

   TEST(nothing, equality_negative) {
      STATIC_EXPECT_TRUE(nothing == nothing);
   }

   TEST(nothing, inequality_negative) {
      STATIC_EXPECT_FALSE(nothing != nothing);
   }

   TEST(nothing, inequality_affirmative_left) {
      STATIC_EXPECT_TRUE(nothing != 4);
   }

   TEST(nothing, inequality_affirmative_right) {
      STATIC_EXPECT_TRUE(4 != nothing);
   }

} // end of namespace ruse::testing
