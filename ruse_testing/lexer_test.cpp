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

  TEST(lexer, primitive_item)
  {
    STATIC_EXPECT_EQ(nat<1>, primitive::run(primitive::item<1>, "abc"_s));
    STATIC_EXPECT_EQ(nat<2>, primitive::run(primitive::item<2>, "abc"_s));
    STATIC_EXPECT_EQ(nat<3>, primitive::run(primitive::item<3>, "abc"_s));
    STATIC_EXPECT_EQ(
      nat<0>, primitive::run(primitive::item<4>, "abc"_s)); // <- parse failure
  }

  // TEST(lexer, primitive_guarded)
  // {
  //   EXPECT_EQ(
  //     type_of(nat<1>),
  //     type_of(primitive::run(
  //       primitive::guarded<
  //         [](char x) { return x >= 'a' && x <= 'z'; },
  //         primitive::item<1>>,
  //       "abc"_s)));

  //   EXPECT_EQ(
  //     nat<0>,
  //     primitive::run(
  //       primitive::guarded<
  //         [](char x) { return x >= 'a' && x <= 'z'; },
  //         primitive::item<1>>,
  //       "ABC"_s));
  // }

  // TEST(lexer, primitive_cut)
  // {
  //   constexpr auto letter = primitive::
  //     guarded<[](char c) { return c >= 'a' && c <= 'z'; },
  //     primitive::item<1>>;

  //   constexpr auto digit = primitive::
  //     guarded<[](char c) { return c >= '0' && c <= '9'; },
  //     primitive::item<1>>;

  //   STATIC_EXPECT_EQ(
  //     nat<1>, primitive::run(primitive::cut<letter, digit>, "a"_s));
  //   STATIC_EXPECT_EQ(
  //     nat<1>, primitive::run(primitive::cut<letter, digit>, "1"_s));
  //   STATIC_EXPECT_EQ(
  //     nat<0>, primitive::run(primitive::cut<letter, digit>, "$"_s));
  // }

} // namespace ruse::experimental::lexer::testing
