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

   TEST(property_vector, is_property_vector_affirmative_empty_vector) {
      STATIC_EXPECT_TRUE(is_pvector(empty_vector));
   }

   TEST(
     property_vector,
     is_property_vector_affirmative_nonempty_vector) {
      EXPECT_TRUE(
        is_pvector(vector("x"_tag(1), "y"_tag(2), "z"_tag(3))));
   }

   TEST(property_vector, property_vector_has_tag_affirmative) {
      STATIC_EXPECT_TRUE(property_vector_has_tag(
        "x"_tag,
        vector("x"_tag(1), "y"_tag(2), "z"_tag(3))));
   }

   TEST(property_vector, propety_vector_has_tag_negative) {
      STATIC_EXPECT_FALSE(property_vector_has_tag(
        "a"_tag,
        vector("x"_tag(1), "y"_tag(2), "z"_tag(3))));
   }

   TEST(property_vector, property_vector_reference) {
      STATIC_EXPECT_EQ(
        "x"_tag(1),
        property_vector_ref(
          "x"_tag,
          vector("x"_tag(1), "y"_tag(2), "z"_tag(3))));
   }

   TEST(property_vector, property_vector_reference_failure) {
      STATIC_EXPECT_EQ(
        false,
        property_vector_ref(
          "a"_tag,
          vector("x"_tag(1), "y"_tag(2), "z"_tag(3))));
   }

} // end of namespace ruse::testing
