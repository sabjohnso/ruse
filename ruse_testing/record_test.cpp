//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
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

} // end of namespace ruse::experimental::testing
