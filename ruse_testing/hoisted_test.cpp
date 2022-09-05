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

  TEST(hoisted, construction)
  {
    constexpr auto x = hoist([] { return "x"_tag(3); });
    STATIC_EXPECT_EQ("x"_tag(3), x.value);
  }

  TEST(hoisted_list, construction)
  {
    STATIC_EXPECT_TRUE(is_hoisted_list("abc"_hoist));
  }

  TEST(hoisted_list, lower) { STATIC_EXPECT_EQ("abc"_s, lower("abc"_hoist)); }

  TEST(hoisted_list, hoist)
  {
    STATIC_EXPECT_TRUE(is_hoisted_list(hoist([] { return "abc"_s; })));
  }

  TEST(hoisted_list, hoist_equality)
  {
    STATIC_EXPECT_TRUE("abc"_hoist == hoist([] { return "abc"_s; }));
  }

  TEST(hoisted_list, hoisted_append)
  {
    STATIC_EXPECT_EQ("abcdef"_hoist, hoisted_append("abc"_hoist, "def"_hoist));
  }

  TEST(hoisted, hoisted_head)
  {
    STATIC_EXPECT_EQ(hoist([] { return 'a'; }), hoisted_head("abc"_hoist));
  }

  TEST(hoisted, hoisted_tail)
  {
    STATIC_EXPECT_EQ("bc"_hoist, hoisted_tail("abc"_hoist));
  }

  TEST(hoisted, hoisted_last)
  {
    STATIC_EXPECT_EQ(hoist([] { return 'c'; }), hoisted_last("abc"_hoist));
  }

  TEST(hoisted, hoisted_butlast)
  {
    STATIC_EXPECT_EQ("ab"_hoist, hoisted_butlast("abc"_hoist));
  }

  TEST(hoisted, hoisted_take)
  {
    STATIC_EXPECT_EQ("abc"_hoist, hoisted_take(nat<3>, "abc123"_hoist));
  }

  TEST(hoisted, hoisted_drop)
  {
    STATIC_EXPECT_EQ("123"_hoist, hoisted_drop(nat<3>, "abc123"_hoist));
  }

  TEST(hoisted_List, fmap)
  {
    EXPECT_EQ(
      "AB"_hoist, fmap([](auto x) -> char { return x - 32; }, "ab"_hoist));
  }

  TEST(hoisted_list, length)
  {
    STATIC_EXPECT_TRUE(HoistedString<decltype(str<"abc">)>);
    STATIC_EXPECT_EQ(nat<3>, hoisted_length(str<"abc">));
  }

} // end of namespace ruse::testing
