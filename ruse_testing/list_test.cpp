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

namespace ruse::testing {
  TEST(List, nothing_s) { STATIC_EXPECT_TRUE(List<nothing_s>); }

  TEST(List, pair_something_nothing)
  {
    STATIC_EXPECT_TRUE(List<pair<int, nothing_s>>);
  }

  TEST(List, pair_something_pair_something_nothing)
  {
    STATIC_EXPECT_TRUE(List<pair<int, pair<double, nothing_s>>>);
  }

  TEST(NotList, int) { STATIC_EXPECT_FALSE(List<int>); }

  TEST(NotList, pair_something_something)
  {
    STATIC_EXPECT_FALSE(List<pair<int, double>>);
  }

  TEST(is_list, nothing) { STATIC_EXPECT_TRUE(is_list(nothing)); }

  TEST(list, cons_something_nothing)
  {
    STATIC_EXPECT_TRUE(is_list(cons(1, nothing)));
  }

  TEST(is_list, list0) { STATIC_EXPECT_TRUE(is_list(list())); }

  TEST(is_list, list1) { STATIC_EXPECT_TRUE(is_list(list(1))); }

  TEST(is_list, list2) { STATIC_EXPECT_TRUE(is_list(list(1, 2))); }

  TEST(is_list, list3) { STATIC_EXPECT_TRUE(is_list(list(1, 2, 3))); }

  TEST(is_nonmepty_list, nothing)
  {
    STATIC_EXPECT_FALSE(is_nonempty_list(nothing));
  }

  TEST(is_nonempty_list, list)
  {
    STATIC_EXPECT_TRUE(is_nonempty_list(list(1, 2)));
  }

  TEST(is_unitary_list, nothing)
  {
    STATIC_EXPECT_FALSE(is_unitary_list(nothing));
  }

  TEST(is_uniary_list, list1) { STATIC_EXPECT_TRUE(is_unitary_list(list(1))); }

  TEST(is_uniary_list, list2)
  {
    STATIC_EXPECT_FALSE(is_unitary_list(list(1, 2)));
  }

  TEST(is_homogeneous_list, nothing)
  {
    STATIC_EXPECT_FALSE(is_homogeneous_list(nothing));
  }

  TEST(is_homogeneous_list, list1)
  {
    STATIC_EXPECT_TRUE(is_homogeneous_list(list(1)));
  }

  TEST(is_homogeneous_list, list2_affirmative)
  {
    STATIC_EXPECT_TRUE(is_homogeneous_list(list(1, 2)));
  }

  TEST(is_homogeneous_list, list2_negative)
  {
    STATIC_EXPECT_FALSE(is_homogeneous_list(list('x', 2)));
  }

  TEST(is_association_list, nothing)
  {
    STATIC_EXPECT_TRUE(is_association_list(nothing));
  }

  TEST(is_association_list, list1_affirmative)
  {
    STATIC_EXPECT_TRUE(is_association_list(list(cons('x', 1))));
  }

  TEST(is_association_list, list1_negative)
  {
    STATIC_EXPECT_FALSE(is_association_list(list('x')));
  }

  TEST(reverse, nothing) { STATIC_EXPECT_EQ(nothing, reverse(nothing)); }

  TEST(reverse, list3)
  {
    STATIC_EXPECT_EQ(list(3, 2, 1), reverse(list(1, 2, 3)));
  }

  TEST(head, list) { STATIC_EXPECT_EQ(1, head(list(1, 2, 3))); }

  TEST(tail, nothing) { STATIC_EXPECT_EQ(nothing, tail(nothing)); }

  TEST(tail, list) { STATIC_EXPECT_EQ(list(2, 3), tail(list(1, 2, 3))); }

} // end of namespace ruse::testing
