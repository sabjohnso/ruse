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

  struct example_s : singleton<example_s>
  {
  } constexpr example{};

  TEST(vector, example_equality_affirmative)
  {
    STATIC_EXPECT_TRUE(example == example);
  }

  TEST(vector, example_equality_left_negative)
  {
    STATIC_EXPECT_FALSE(example == 3);
  }

  TEST(vector, example_equality_right_negative)
  {
    STATIC_EXPECT_FALSE(3 == example);
  }

  TEST(vector, example_inequality_negative)
  {
    STATIC_EXPECT_FALSE(example != example);
  }

  TEST(vector, example_inequality_left_affirmative)
  {
    STATIC_EXPECT_TRUE(example != 3);
  }

  TEST(vector, example_inequality_right_affirmative)
  {
    STATIC_EXPECT_TRUE(3 != example);
  }

} // end of namespace ruse::testing
