//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/experimental/character_class.hpp>

namespace ruse::experimental::testing {
  TEST(character_class, empty_construction)
  {
    STATIC_EXPECT_TRUE(is_character_class(character_class<>));
  }

  TEST(character_class, one_character_construction)
  {
    STATIC_EXPECT_TRUE(is_character_class(character_class<character<'x'>>));
  }

  TEST(character_class, multiple_primitive_construction)
  {
    STATIC_EXPECT_TRUE(
      is_character_class(character_class<range<'a', 'h'>, character<'x'>>));
  }

  TEST(character_class, inclusion_pass)
  {
    STATIC_EXPECT_TRUE(includes(
      character_class<range<'a', 'h'>, range<'o', 'z'>>, character<'g'>));
  }

  TEST(character_class, inclusion_fail)
  {
    STATIC_EXPECT_FALSE(includes(
      character_class<range<'a', 'h'>, range<'o', 'z'>>, character<'i'>));
  }

} // end of namespace ruse::experimental::testing
