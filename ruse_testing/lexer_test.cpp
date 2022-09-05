//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/experimental/character_class.hpp>
#include <ruse/experimental/lexer.hpp>
#include <ruse/ruse.hpp>
#include <ruse/show.hpp>

namespace ruse::experimental::lexer::testing {

   constexpr auto letter = primitive::guarded<
     [](char c) { return c >= 'a' && c <= 'z'; },
     primitive::Item{.count = 1}>;

   constexpr auto digit = primitive::guarded<
     [](char c) { return c >= '0' && c <= '9'; },
     primitive::Item{.count = 1}>;

   TEST(lexer, primitive_item) {
      EXPECT_EQ(
        1,
        primitive::run(
          primitive::Item{.count = 1},
          string_view("abc")));
      EXPECT_EQ(
        2,
        primitive::run(
          primitive::Item{.count = 2},
          string_view("abc")));
      EXPECT_EQ(
        3,
        primitive::run(
          primitive::Item{.count = 3},
          string_view("abc")));
      EXPECT_EQ(
        0,
        primitive::run(
          primitive::Item{.count = 4},
          string_view("abc")));
   }

   TEST(lexer, primitive_guarded) {
      EXPECT_EQ(
        1,
        primitive::run(
          primitive::guarded<
            [](char x) { return x >= 'a' && x <= 'z'; },
            primitive::Item{.count = 1}>,
          string_view("abc")));

      EXPECT_EQ(
        0,
        primitive::run(
          primitive::guarded<
            [](char x) { return x >= 'a' && x <= 'z'; },
            primitive::Item{.count = 1}>,
          string_view("ABC")));
   }

   TEST(lexer, primitive_cut) {

      EXPECT_EQ(
        1,
        primitive::run(
          primitive::cut<letter, digit>,
          string_view("a")));
      EXPECT_EQ(
        1,
        primitive::run(
          primitive::cut<letter, digit>,
          string_view("1")));
      EXPECT_EQ(
        0,
        primitive::run(
          primitive::cut<letter, digit>,
          string_view("$")));
   }

   TEST(lexer, primitive_sequence) {
      EXPECT_EQ(
        2,
        primitive::run(
          primitive::seq<letter, digit>,
          string_view("a1b2")));

      EXPECT_EQ(
        0,
        primitive::run(
          primitive::seq<letter, digit>,
          string_view("1a2b")));
   }

   TEST(lexer, primitive_alt) {
      EXPECT_EQ(
        2,
        primitive::run(
          primitive::alt<primitive::seq<letter, digit>, letter>,
          string_view("a1")));
   }

   TEST(lexer, primitve_bind) {
      EXPECT_EQ(
        2,
        primitive::run(
          primitive::bind<
            primitive::Item{.count = 1},
            [](auto n) { return primitive::Item{.count = n + 1}; }>,
          string_view("ab")));

      EXPECT_EQ(
        0,
        primitive::run(
          primitive::bind<
            primitive::Item{.count = 1},
            [](auto n) { return primitive::Item{.count = n + 1}; }>,
          string_view("a")));
   }

   TEST(lexer, bind_value) { EXPECT_TRUE(false); }

} // namespace ruse::experimental::lexer::testing
