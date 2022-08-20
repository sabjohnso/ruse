//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/show.hpp>

#include <ruse/experimental/record.hpp>

namespace ruse::experimental::testing {

  TEST(record, record_name)
  {
    constexpr auto name = "point"_record_name;
    STATIC_EXPECT_TRUE(is_record_name(name));
  }

  TEST(record, field_name)
  {
    constexpr auto x = "x"_field;
    STATIC_EXPECT_TRUE(is_field_name(x));
    STATIC_EXPECT_TRUE(is_field_decl(x));
    STATIC_EXPECT_TRUE(FieldName<decltype(x)>);
    STATIC_EXPECT_TRUE(FieldDecl<decltype(x)>);
  }

  TEST(record, typed_field_decl)
  {
    constexpr auto x = ("x"_field, type<double>);
    STATIC_EXPECT_TRUE(is_list(x));
    STATIC_EXPECT_TRUE(length(x) == 2);
    STATIC_EXPECT_TRUE(is_field_decl(x));
    STATIC_EXPECT_TRUE(is_typed_field_decl(x));

    STATIC_EXPECT_TRUE(FieldName<decltype(car(x))>);
    STATIC_EXPECT_TRUE(TypeProxy<decltype(list_ref(nat<1>, x))>);
    STATIC_EXPECT_TRUE(FieldDecl<decltype(x)>);
    STATIC_EXPECT_TRUE(TypedFieldDecl<decltype(x)>);
  }

  TEST(record, record_definition)
  {
    constexpr auto point = "point"_record_name(
      "x"_field(type<double>),
      "y"_field(type<double>),
      "z"_field(type<double>));

    STATIC_EXPECT_TRUE(RecordDefinition<decltype(point)>);
    STATIC_EXPECT_EQ(
      list(
        "point"_record_name,
        list(
          list("x"_field, type<double>),
          list("y"_field, type<double>),
          list("z"_field, type<double>))),
      point);

    EXPECT_EQ(
      make_record(point, 1.0, 2.0, 3.0),
      "point"_tag(list("x"_tag(1.0), "y"_tag(2.0), "z"_tag(3.0))));
  }

} // end of namespace ruse::experimental::testing
