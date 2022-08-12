//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

//
// ... Standard header files
//
#include <string>

namespace ruse::testing {

  TEST(curry, identity)
  {
    constexpr auto constant = curry(nat<1>, [](auto f) { return f; });
    constexpr auto sqr = [](auto x) { return x * x; };
    STATIC_EXPECT_EQ(4, constant(sqr, 2));
  }

  TEST(case_lambda, addition)
  {
    using namespace std::literals;
    constexpr auto sum = case_lambda(
      [] { return 0; },
      [](int x) { return x; },
      [](int x, int y) { return x + y; },
      [](auto... xs) { return (xs + ...); });

    STATIC_EXPECT_EQ(0, sum());
    STATIC_EXPECT_EQ(1, sum(1));
    STATIC_EXPECT_EQ(3, sum(1, 2));
    STATIC_EXPECT_EQ(10, sum(1, 2, 3, 4));
    EXPECT_EQ("abc123"s, sum("abc"s, "123"s));
  }

  TEST(apply, sum)
  {
    constexpr auto sum = [](auto... args) { return (0 + ... + args); };
    STATIC_EXPECT_EQ(10, apply(sum, list(1, 2, 3, 4)));
    STATIC_EXPECT_EQ(3, apply(sum, 1, 2, nothing));
  }

} // end of namespace ruse::testing
