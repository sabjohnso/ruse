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

  TEST(vector, is_empty_vector_type_affirmative)
  {
    STATIC_EXPECT_TRUE(is_empty_vector_type(type_of(empty_vector)));
  }

  TEST(vector, is_empty_vector_type_negative_empty_vector)
  {
    STATIC_EXPECT_FALSE(is_empty_vector_type(empty_vector));
  }

  TEST(vector, is_empty_vector_type_negative_other_type_proxy)
  {
    STATIC_EXPECT_FALSE(is_empty_vector_type(type<int>));
  }

  TEST(vector, is_empty_vector_type_negative_some_other_value)
  {
    STATIC_EXPECT_FALSE(is_empty_vector_type(3));
  }

  TEST(vector, is_empty_vector_affirmative)
  {
    STATIC_EXPECT_TRUE(is_empty_vector(empty_vector));
  }

  TEST(vector, is_empty_vector_negative_some_other_value)
  {
    STATIC_EXPECT_FALSE(is_empty_vector(5));
  }

  TEST(vector, EmptyVector_affirmative)
  {
    STATIC_EXPECT_TRUE(EmptyVector<decltype(empty_vector)>);
  }

  TEST(vector, push_back_1)
  {
    constexpr auto xs = push_back(empty_vector, 3);
    STATIC_EXPECT_TRUE(Vector<decltype(xs)>);
    STATIC_EXPECT_TRUE(NonemptyVector<decltype(xs)>);
    STATIC_EXPECT_TRUE(UnitaryVector<decltype(xs)>);
    STATIC_EXPECT_FALSE(EmptyVector<decltype(xs)>);
  }

  TEST(vector, push_back_2)
  {
    constexpr auto xs = push_back(push_back(empty_vector, 1), 2);
    STATIC_EXPECT_TRUE(Vector<decltype(xs)>);
    STATIC_EXPECT_TRUE(NonemptyVector<decltype(xs)>);
    STATIC_EXPECT_FALSE(UnitaryVector<decltype(xs)>);
    STATIC_EXPECT_FALSE(EmptyVector<decltype(xs)>);
  }

  TEST(vector, EmptyVector_negative_nonempty_vector)
  {
    STATIC_EXPECT_FALSE(EmptyVector<decltype(vector(1, 2, 3))>);
  }

  TEST(vector, EmptyVector_negative_some_other_type)
  {
    STATIC_EXPECT_FALSE(EmptyVector<int>);
  }

  TEST(vector, is_empty_vector_negative_nonempty_vector)
  {
    STATIC_EXPECT_FALSE(is_empty_vector(vector(1, 2, 3)));
  }

  TEST(vector, Vector_affirmative_empty_vector)
  {
    STATIC_EXPECT_TRUE(Vector<decltype(empty_vector)>);
  }

  TEST(vector, length_empty)
  {
    STATIC_EXPECT_EQ(vector_length(empty_vector), 0);
  }

  TEST(vector, length_1) { STATIC_EXPECT_EQ(vector_length(vector(1)), 1); }

  TEST(vector, length_2) { STATIC_EXPECT_EQ(vector_length(vector(1, 2)), 2); }

  TEST(vector, length_3)
  {
    STATIC_EXPECT_EQ(vector_length(vector(1, 2, 3)), 3);
  }

  TEST(vector, vector_ref)
  {
    STATIC_EXPECT_EQ(vector_ref(nat<0>, vector(1, 2, 3)), 1);
    STATIC_EXPECT_EQ(vector_ref(nat<1>, vector(1, 2, 3)), 2);
    STATIC_EXPECT_EQ(vector_ref(nat<2>, vector(1, 2, 3)), 3);
  }

  TEST(vector, back_type)
  {
    STATIC_EXPECT_EQ(type<int>, back_type(vector('a', 3.4, 1)));
  }

  TEST(homogeneous_vector, is_hvector_affirmative)
  {
    STATIC_EXPECT_TRUE(is_hvector(vector(1, 2, 3)));
  }

  TEST(homogeneous_vector, is_hvector_negative_vector)
  {
    STATIC_EXPECT_FALSE(is_hvector(vector(1, "banana", 3)));
  }

  TEST(homogeneous_vector, is_hvector_negative_something_else)
  {
    STATIC_EXPECT_FALSE(is_hvector(8));
  }

  TEST(homogeneous_vector, homogeneous_vector_ref)
  {
    EXPECT_EQ(hvector_ref(0, vector(1, 2, 3)), 1);
    EXPECT_EQ(hvector_ref(1, vector(1, 2, 3)), 2);
    EXPECT_EQ(hvector_ref(2, vector(1, 2, 3)), 3);
  }

} // end of namespace ruse::testing
