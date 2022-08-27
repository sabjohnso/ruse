//
// ... standard header files
//
#include <any>
#include <tuple>
#include <utility>

//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... external header files
//
#include <pfr.hpp>

//
// ... ruse header files
//
#include <ruse/reflection_macros.hpp>
#include <ruse/ruse.hpp>
#include <ruse/show.hpp>

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

  TEST(reflection, named_members)
  {
    struct point
    {
      RUSE_FIELD(double, x){};
      RUSE_FIELD(double, y){};
      RUSE_FIELD(double, z){};
    };

    STATIC_EXPECT_TRUE(NamedAggregate<point>);
  }

  RUSE_ENUM(things, int, (a), (b), (c));
  TEST(reflection, enum_names)
  {
    STATIC_EXPECT_TRUE(type_of(things::a) == type<things>);
    STATIC_EXPECT_TRUE(type_of(things::b) == type<things>);
    STATIC_EXPECT_TRUE(type_of(things::c) == type<things>);
    STATIC_EXPECT_TRUE(length(get_enum_values(type<things>)) == 3);
    std::cout << enum_values<things> << std::endl;
    std::cout << enum_names<things> << std::endl;
    STATIC_EXPECT_TRUE(is_list(enum_values<things>));
    STATIC_EXPECT_TRUE(is_list(enum_names<things>));
    STATIC_EXPECT_TRUE(is_list(enum_names_short<things>));
    STATIC_EXPECT_TRUE(is_list(enum_names_unqualified<things>));
    STATIC_EXPECT_EQ(
      std::string_view("ruse::testing::things::a"),
      lookup_enum_name(things::a));
    STATIC_EXPECT_EQ(
      std::string_view("things::a"), lookup_enum_name_short(things::a));
    STATIC_EXPECT_EQ(
      std::string_view("a"), lookup_enum_name_unqualified(things::a));
  }

  RUSE_ENUM(more_things, char, (a, 'x'), (bc, 'y'), (def, 'z'));
  TEST(reflection, enum_names_with_explicit_values)
  {
    STATIC_EXPECT_TRUE(char(more_things::a) == 'x');
    STATIC_EXPECT_TRUE(char(more_things::bc) == 'y');
    STATIC_EXPECT_TRUE(char(more_things::def) == 'z');
  }

  TEST(reflection, long_enum_names)
  {
    STATIC_EXPECT_EQ(
      get_enum_name(hoisted<more_things::a>{}),
      std::string_view("ruse::testing::more_things::a"));

    EXPECT_EQ(
      get_short_enum_name(hoisted<more_things::a>{}),
      std::string_view("more_things::a"));

    STATIC_EXPECT_EQ(
      get_enum_name(hoisted<more_things::bc>{}),
      std::string_view("ruse::testing::more_things::bc"));

    EXPECT_EQ(
      get_short_enum_name(hoisted<more_things::bc>{}),
      std::string_view("more_things::bc"));

    STATIC_EXPECT_EQ(
      get_enum_name(hoisted<more_things::def>{}),
      std::string_view("ruse::testing::more_things::def"));

    EXPECT_EQ(
      get_short_enum_name(hoisted<more_things::def>{}),
      std::string_view("more_things::def"));
  }

  TEST(reflection, short_enum_names)
  {

    STATIC_EXPECT_EQ(
      get_short_enum_name(hoisted<more_things::a>{}),
      std::string_view("more_things::a"));

    STATIC_EXPECT_EQ(
      get_short_enum_name(hoisted<more_things::bc>{}),
      std::string_view("more_things::bc"));

    STATIC_EXPECT_EQ(
      get_short_enum_name(hoisted<more_things::def>{}),
      std::string_view("more_things::def"));
  }

  namespace ns1::ns2 {
    RUSE_ENUM(more_things, char, (a, 'x'), (bc, 'y'), (def, 'z'));
  } // end of namespace ns1::ns2

  TEST(reflection, short_enum_namesin_deeper_namespace)
  {

    STATIC_EXPECT_EQ(
      get_short_enum_name(hoisted<ns1::ns2::more_things::a>{}),
      std::string_view("more_things::a"));

    STATIC_EXPECT_EQ(
      get_short_enum_name(hoisted<ns1::ns2::more_things::bc>{}),
      std::string_view("more_things::bc"));

    STATIC_EXPECT_EQ(
      get_short_enum_name(hoisted<ns1::ns2::more_things::def>{}),
      std::string_view("more_things::def"));
  }

  TEST(relfection, unqualified_enum_name)
  {
    EXPECT_EQ(
      std::string_view("a"),
      get_unqualified_enum_name(hoisted<more_things::a>{}));

    EXPECT_EQ(
      std::string_view("bc"),
      get_unqualified_enum_name(hoisted<more_things::bc>{}));

    EXPECT_EQ(
      std::string_view("def"),
      get_unqualified_enum_name(hoisted<more_things::def>{}));
  }
} // end of namespace ruse::testing
