//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

#include <ruse/show.hpp>

#include <any>
#include <tuple>
#include <utility>

namespace example {
  struct A
  {};
}; // namespace example

namespace ruse::testing {

  TEST(reflection, type_name_fundamental)
  {
    STATIC_EXPECT_EQ(type_name<int>, std::string_view("int"));
  }

  TEST(reflection, type_name_with_namespace)
  {
    STATIC_EXPECT_EQ(type_name<example::A>, std::string_view{"example::A"});
  }

  TEST(reflection, member_count_1)
  {
    struct A
    {
      int x;
    };
    STATIC_EXPECT_EQ(aggregate_member_count<A>, 1);
  }

  TEST(reflection, member_count_2)
  {
    struct A
    {
      int x;
      int y;
    };
    STATIC_EXPECT_EQ(aggregate_member_count<A>, 2);
  }

  TEST(reflection, member_count_3)
  {
    struct A
    {
      int x;
      int y;
      int z;
    };
    STATIC_EXPECT_EQ(aggregate_member_count<A>, 3);
  }

  TEST(reflection, member_count_complex)
  {
    struct A
    {
      int x;
      int y;
      int z;
    };
    struct B
    {
      A x;
      int y;
    };
    STATIC_EXPECT_EQ(aggregate_member_count<B>, 2);
  }

  TEST(reflection, member_count_empty)
  {
    struct A
    {};
    STATIC_EXPECT_EQ(aggregate_member_count<A>, 0);
  }

  TEST(reflection, member_count_complex_empty)
  {
    struct A
    {};

    struct B
    {
      A x;
      int y;
    };
    STATIC_EXPECT_EQ(aggregate_member_count<B>, 2);
  };

  TEST(reflection, member_types)
  {
    struct point
    {
      double x{};
      double y{};
      double z{};
      bool feature{};
    };

    STATIC_EXPECT_EQ(
      list(type<double>, type<double>, type<double>, type<bool>),
      aggregate_member_types<point>);
  }

} // end of namespace ruse::testing
