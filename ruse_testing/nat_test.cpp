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

   TEST(nat, less_than_affirmative) {
      STATIC_EXPECT_TRUE(nat<1> < nat<2>);
   }

   TEST(nat, less_than_negative_equal) {
      STATIC_EXPECT_FALSE(nat<1> < nat<1>);
   }

   TEST(nat, less_than_negative_greater) {
      STATIC_EXPECT_FALSE(nat<2> < nat<1>);
   }

   TEST(nat, greater_than_affirmative) {
      STATIC_EXPECT_TRUE(nat<2> > nat<1>);
   }

   TEST(nat, greater_than_negative_equal) {
      STATIC_EXPECT_FALSE(nat<2> > nat<2>);
   }
   TEST(nat, greater_than_negative_greater) {
      STATIC_EXPECT_FALSE(nat<1> > nat<2>);
   }

   TEST(nat, equal_affirmative) {
      STATIC_EXPECT_TRUE(nat<3> == nat<3>);
   }

   TEST(nat, equal_negative_less_than) {
      STATIC_EXPECT_FALSE(nat<3> == nat<2>);
   }

   TEST(nat, equal_negative_greater) {
      STATIC_EXPECT_FALSE(nat<3> == nat<4>);
   }

   TEST(nat, not_equal_affirmative_less) {
      STATIC_EXPECT_TRUE(nat<3> != nat<2>);
   }

   TEST(nat, not_equal_affirmative_greater) {
      STATIC_EXPECT_TRUE(nat<3> != nat<4>);
   }

   TEST(nat, not_equal_negative) {
      STATIC_EXPECT_FALSE(nat<3> != nat<3>);
   }

   TEST(greater_or_equal, affirmative_greater) {
      STATIC_EXPECT_TRUE(nat<4> >= nat<3>);
   }

   TEST(greater_or_equal, affirmative_equal) {
      STATIC_EXPECT_TRUE(nat<3> >= nat<3>);
   }

   TEST(greater_or_equal, negative) {
      STATIC_EXPECT_FALSE(nat<3> >= nat<4>);
   }

   TEST(less_or_equal, affirmative_less) {
      STATIC_EXPECT_TRUE(nat<3> <= nat<4>);
   }

   TEST(less_or_equal, affirmative_equal) {
      STATIC_EXPECT_TRUE(nat<3> <= nat<3>);
   }

   TEST(less_or_equal, negative) {
      STATIC_EXPECT_FALSE(nat<4> <= nat<3>);
   }

} // end of namespace ruse::testing
