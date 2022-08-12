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

  TEST(apply_to, no_args)
  {
    constexpr auto fun = apply_to();
    STATIC_EXPECT_EQ(3, fun([] { return 3; }));
  }

  TEST(apply_to, one_arg)
  {
    constexpr auto fun = apply_to(3);
    STATIC_EXPECT_EQ(9, fun([](auto x) { return x * x; }));
  }

  TEST(apply_to, two_args)
  {
    constexpr auto fun = apply_to(3, 4);
    STATIC_EXPECT_EQ(7, fun(std::plus{}));
  }

  TEST(apply_to_list, no_args)
  {
    constexpr auto fun = apply_to_list(nothing);
    STATIC_EXPECT_EQ(3, fun([] { return 3; }));
  }

  TEST(apply_to_list, one_arg)
  {
    constexpr auto fun = apply_to_list(list(3));
    STATIC_EXPECT_EQ(9, fun([](auto x) { return x * x; }));
  }

  TEST(apply_to_list, two_args)
  {
    constexpr auto fun = apply_to_list(list(3, 4));
    STATIC_EXPECT_EQ(7, fun(std::plus{}));
  }

} // end of namespace ruse::testing
