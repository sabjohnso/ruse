//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/experimental/character_range.hpp>
#include <ruse/ruse.hpp>

namespace ruse::experimental::testing {

   TEST(character_value, construction2) {
      STATIC_EXPECT_TRUE(is_character_value(character<'x'>));
   }

   TEST(character_value, char) {
      STATIC_EXPECT_TRUE(is_character_value(character<'x'>));
   }

   TEST(character_value, char8_t) {
      STATIC_EXPECT_TRUE(is_character_value(character<u8'x'>));
   }

   TEST(character_value, wchar_t) {
      STATIC_EXPECT_TRUE(is_character_value(character<L'x'>));
   }

   TEST(character_value, char16_t) {
      STATIC_EXPECT_TRUE(is_character_value(character<u'x'>));
   }

   TEST(character_value, char32_t) {
      STATIC_EXPECT_TRUE(is_character_value(character<U'x'>));
   }

   TEST(charcter_value, adjacent1) {
      STATIC_EXPECT_TRUE(adjacent(character<'e'>, character<'f'>));
   }

   TEST(charcter_value, adjacent2) {
      STATIC_EXPECT_TRUE(adjacent(character<'f'>, character<'e'>));
   }

   TEST(charcter_value, adjacent_fail1) {
      STATIC_EXPECT_FALSE(adjacent(character<'f'>, character<'f'>));
   }

   TEST(charcter_value, adjacent_fail2) {
      STATIC_EXPECT_FALSE(adjacent(character<'f'>, character<'h'>));
   }

   TEST(character_value, overlap) {
      STATIC_EXPECT_TRUE(overlap(character<'f'>, character<'f'>));
   }

   TEST(character_value, overlap_faile) {
      STATIC_EXPECT_FALSE(overlap(character<'f'>, character<'g'>));
   }

   TEST(chacter_range, construction) {
      STATIC_EXPECT_TRUE(is_character_range(range<'a', 'z'>));
   }

   TEST(character_range, adjacent1) {
      STATIC_EXPECT_TRUE(adjacent(range<'a', 'd'>, range<'e', 'f'>));
   }

   TEST(character_range, adjacent2) {
      STATIC_EXPECT_TRUE(adjacent(range<'e', 'f'>, range<'a', 'd'>));
   }

   TEST(character_range, adjacent_fail1) {
      STATIC_EXPECT_FALSE(adjacent(range<'e', 'f'>, range<'a', 'c'>));
   }

   TEST(chacacter_range, overlap) {
      STATIC_EXPECT_TRUE(overlap(range<'e', 'h'>, range<'a', 'f'>));
   }

   TEST(character_range, less_than_pass_lower_bound_is_lower) {
      STATIC_EXPECT_TRUE(range<'a', 'c'> < range<'b', 'c'>);
   }

   TEST(character_range, less_than_pass_upper_bound_is_lower) {
      STATIC_EXPECT_TRUE(range<'b', 'c'> < range<'b', 'd'>);
   }

   TEST(character_range, less_than_fail_lower_bound_is_lower) {
      STATIC_EXPECT_FALSE(range<'b', 'c'> < range<'a', 'c'>);
   }

   TEST(character_range, less_than_fail_upper_bound_is_lower) {
      STATIC_EXPECT_FALSE(range<'a', 'd'> < range<'a', 'c'>);
   }

   TEST(character_range, less_than_fail_equal_ranges) {
      STATIC_EXPECT_FALSE(range<'a', 'c'> < range<'a', 'c'>);
   }

   TEST(character_class_primitive, less_than_pass1) {
      STATIC_EXPECT_FALSE(character<'a'> < range<'a', 'c'>);
   }

} // end of namespace ruse::experimental::testing
