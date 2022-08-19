//
// ... Standard header files
//
#include <cmath>

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

  TEST(Type, Type) { STATIC_EXPECT_TRUE(TypeProxy<Type<int>>); }

  TEST(Type, int) { STATIC_EXPECT_FALSE(TypeProxy<int>); }

  TEST(TypeProxyOf, type) { STATIC_EXPECT_TRUE(TypeProxyOf<Type<int>, int>); }

  TEST(TypeProxyOf, wrong_type)
  {
    STATIC_EXPECT_FALSE(TypeProxyOf<Type<double>, int>);
  }

  TEST(TypeProxyOf, something_else)
  {
    STATIC_EXPECT_FALSE(TypeProxyOf<double, int>);
  }

  TEST(fmap, type)
  {
    STATIC_EXPECT_EQ(
      type<double>, fmap([](auto x) { return std::sqrt(x); }, type<int>));
  }

  TEST(fapply, type)
  {
    STATIC_EXPECT_EQ(
      type<double>,
      fapply(type_of([](auto x) { return std::sqrt(x); }), type<int>));
  }

  TEST(flatmap, type)
  {
    STATIC_EXPECT_EQ(type<int>, flatmap(type_of, type<int>));
  }

  TEST(flatten, type)
  {
    STATIC_EXPECT_EQ(type<int>, flatten(type_of(type<int>)));
  }

  TEST(letm, type)
  {
    STATIC_EXPECT_EQ(type<double>, letm(type<int>, [](auto x) {
                       return letm(
                         type<double>, [=](auto y) { return type_of(x + y); });
                     }));
  }

  TEST(letf, type)
  {
    STATIC_EXPECT_EQ(
      type<double>, letf(type<int>, [](auto x) { return std::cos(x); }));
  }

} // end of namespace ruse::testing
