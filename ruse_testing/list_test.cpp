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

   TEST(List, pair_something_nothing) {
      STATIC_EXPECT_TRUE(List<pair<int, nothing_s>>);
   }

   TEST(List, pair_something_pair_something_nothing) {
      STATIC_EXPECT_TRUE(List<pair<int, pair<double, nothing_s>>>);
   }

   TEST(NotList, int) { STATIC_EXPECT_FALSE(List<int>); }

   TEST(NotList, pair_something_something) {
      STATIC_EXPECT_FALSE(List<pair<int, double>>);
   }

   TEST(is_list, nothing) { STATIC_EXPECT_TRUE(is_list(nothing)); }

   TEST(list, cons_something_nothing) {
      STATIC_EXPECT_TRUE(is_list(cons(1, nothing)));
   }

   TEST(is_list, list0) { STATIC_EXPECT_TRUE(is_list(list())); }

   TEST(is_list, list1) { STATIC_EXPECT_TRUE(is_list(list(1))); }

   TEST(is_list, list2) { STATIC_EXPECT_TRUE(is_list(list(1, 2))); }

   TEST(is_list, list3) { STATIC_EXPECT_TRUE(is_list(list(1, 2, 3))); }

   TEST(is_nonmepty_list, nothing) {
      STATIC_EXPECT_FALSE(is_nonempty_list(nothing));
   }

   TEST(is_nonempty_list, list) {
      STATIC_EXPECT_TRUE(is_nonempty_list(list(1, 2)));
   }

   TEST(is_unitary_list, nothing) {
      STATIC_EXPECT_FALSE(is_unitary_list(nothing));
   }

   TEST(is_uniary_list, list1) {
      STATIC_EXPECT_TRUE(is_unitary_list(list(1)));
   }

   TEST(is_uniary_list, list2) {
      STATIC_EXPECT_FALSE(is_unitary_list(list(1, 2)));
   }

   TEST(is_homogeneous_list, nothing) {
      STATIC_EXPECT_FALSE(is_homogeneous_list(nothing));
   }

   TEST(is_homogeneous_list, list1) {
      STATIC_EXPECT_TRUE(is_homogeneous_list(list(1)));
   }

   TEST(is_homogeneous_list, list2_affirmative) {
      STATIC_EXPECT_TRUE(is_homogeneous_list(list(1, 2)));
   }

   TEST(is_homogeneous_list, list2_negative) {
      STATIC_EXPECT_FALSE(is_homogeneous_list(list('x', 2)));
   }

   TEST(is_vacuous_list, affermative_empty_list) {
      STATIC_EXPECT_TRUE(is_vacuous_list(list()));
   }

   TEST(is_vacuous_list, affermative_one_element_list) {
      STATIC_EXPECT_TRUE(is_vacuous_list(list("x"_tag)));
   }

   TEST(is_vacuous_list, affermative_multiple_element_list) {
      STATIC_EXPECT_TRUE(
        is_vacuous_list(list("x"_tag, "y"_tag, type<int>)));
   }

   TEST(is_vacuous_list, negative_multiple_element_list) {
      STATIC_EXPECT_FALSE(is_vacuous_list(list(3, type<char>, 5)));
   }

   TEST(is_association_list, nothing) {
      STATIC_EXPECT_TRUE(is_association_list(nothing));
   }

   TEST(is_association_list, list1_affirmative) {
      STATIC_EXPECT_TRUE(is_association_list(list(cons('x', 1))));
   }

   TEST(is_association_list, list1_negative) {
      STATIC_EXPECT_FALSE(is_association_list(list('x')));
   }

   TEST(reverse, nothing) {
      STATIC_EXPECT_EQ(nothing, reverse(nothing));
   }

   TEST(reverse, list3) {
      STATIC_EXPECT_EQ(list(3, 2, 1), reverse(list(1, 2, 3)));
   }

   TEST(head, list) { STATIC_EXPECT_EQ(1, head(list(1, 2, 3))); }

   TEST(tail, nothing) { STATIC_EXPECT_EQ(nothing, tail(nothing)); }

   TEST(tail, list) {
      STATIC_EXPECT_EQ(list(2, 3), tail(list(1, 2, 3)));
   }

   TEST(list_ref, list) {
      STATIC_EXPECT_EQ(1, list_ref(nat<0>, list(1, 2, 3)));
      STATIC_EXPECT_EQ(2, list_ref(nat<1>, list(1, 2, 3)));
      STATIC_EXPECT_EQ(3, list_ref(nat<2>, list(1, 2, 3)));
   }

   TEST(last, list) { STATIC_EXPECT_EQ(3, last(list(1, 2, 3))); }

   TEST(butlast, list) {
      STATIC_EXPECT_EQ(list(1, 2), butlast(list(1, 2, 3)));
   }

   TEST(fmap, list_nothing) {
      STATIC_EXPECT_EQ(
        nothing,
        fmap([](auto x) { return x * x; }, nothing));
   }

   TEST(fmap, list) {
      STATIC_EXPECT_EQ(
        list(1, 4, 9),
        fmap([](auto x) { return x * x; }, list(1, 2, 3)));
   }

   TEST(flatmap, list) {
      STATIC_EXPECT_EQ(list(1, 2, 3), flatmap(list, list(1, 2, 3)));
   }

   TEST(flatten, list) {
      STATIC_EXPECT_EQ(
        list(1, 2, 3),
        flatten(list(list(1), list(2), list(3))));
   }

   TEST(letm, list_letm) {
      // clang-format off
    STATIC_EXPECT_EQ(
       list(4, 5, 5, 6),
       letm(list(1, 2), [=](auto x) { return
       letm(list(3, 4), [=](auto y) { return
             list(x + y); });
       }));
      // clang-format on
   }

   TEST(letm, list_letm_pure) {
      // clang-format off
    STATIC_EXPECT_EQ(
       list(4, 5, 5, 6),
       letm(list(1, 2), [=](auto x) { return
       letm(list(3, 4), [=](auto y) { return
             pure(x + y); });
       }));
      // clang-format on
   }

   TEST(fapply, list) {
      STATIC_EXPECT_EQ(
        list(1, 4, 9),
        fapply(list([](auto x) { return x * x; }), list(1, 2, 3)));
   }

   TEST(fapply, pure_list) {
      STATIC_EXPECT_EQ(
        list(1, 4, 9),
        fapply(pure([](auto x) { return x * x; }), list(1, 2, 3)));
   }

   TEST(extract, list) { STATIC_EXPECT_EQ(1, extract(list(1, 2, 3))); }

   TEST(extend, list) {
      STATIC_EXPECT_EQ(list(1, 2, 3), extend(extract, list(1, 2, 3)));
   }

   TEST(duplicate, list) {
      STATIC_EXPECT_EQ(
        list(list(1, 2, 3), list(2, 3), list(3)),
        duplicate(list(1, 2, 3)));
   }

   TEST(zapply, list) {
      constexpr auto add =
        curry(nat<2>, [](auto x, auto y) { return x + y; });
      static_assert(ListLike<
                    decltype(list(1, 2)),
                    decltype(fmap(add, list(1, 2)))>);
      STATIC_EXPECT_EQ(
        list(4, 6),
        zapply(fmap(add, list(1, 2)), list(3, 4)));
   }

   TEST(homogeneous_list, hlist_ref) {
      STATIC_EXPECT_EQ(1, hlist_ref(0, list(1, 2, 3)));
      STATIC_EXPECT_EQ(2, hlist_ref(1, list(1, 2, 3)));
      STATIC_EXPECT_EQ(3, hlist_ref(2, list(1, 2, 3)));
   }

   TEST(homogeneous_list, hlist_sort_1_element) {
      STATIC_EXPECT_EQ(list(2), hlist_sort(std::less{}, list(2)));
   }

   TEST(homogeneous_list, hlist_sort_2_elements_presorted) {
      STATIC_EXPECT_EQ(list(1, 2), hlist_sort(std::less{}, list(1, 2)));
   }

   TEST(homogeneous_list, hlist_sort_2_elements) {
      STATIC_EXPECT_EQ(list(1, 2), hlist_sort(std::less{}, list(2, 1)));
   }

   TEST(homogeneous_list, hlist_sort_3_elements_presorted) {
      STATIC_EXPECT_EQ(
        list(1, 2, 3),
        hlist_sort(std::less{}, list(1, 2, 3)));
   }

   TEST(homogeneous_list, hlist_sort) {
      STATIC_EXPECT_EQ(
        list(1, 2, 3),
        hlist_sort(std::less{}, list(2, 3, 1)));
   }

   TEST(vacuous_list, vlist_sort_1_element) {
      STATIC_EXPECT_EQ(
        list(nat<1>),
        vlist_sort(std::less{}, list(nat<1>)));
   }

   TEST(vacuous_list, vlist_sort_already_sorted) {
      EXPECT_EQ(
        list(nat<1>, nat<2>, nat<3>),
        vlist_sort(std::less{}, list(nat<1>, nat<2>, nat<3>)));
   }

   TEST(vacuous_list, vlist_sort) {
      EXPECT_EQ(
        list(nat<1>, nat<2>, nat<3>),
        vlist_sort(std::less{}, list(nat<2>, nat<3>, nat<1>)));
   }

   TEST(vacuous_list, vlist_filter) {
      STATIC_EXPECT_EQ(
        list(nat<1>, nat<2>),
        vlist_filter(
          [](auto x) { return x < nat<3>; },
          list(nat<1>, nat<2>, nat<3>, nat<4>)));
   }

   TEST(vacuous_list, take_while) {
      STATIC_EXPECT_EQ(
        list(nat<1>, nat<2>),
        vlist_take_while(
          [](auto x) { return x < nat<3>; },
          list(nat<1>, nat<2>, nat<3>, nat<4>)));
   }

   TEST(vacuous_list, take_until) {
      STATIC_EXPECT_EQ(
        list(nat<1>, nat<2>),
        vlist_take_while(
          [](auto x) { return x < nat<3>; },
          list(nat<1>, nat<2>, nat<3>, nat<4>)));
   }

   TEST(property_list, construction) {
      STATIC_EXPECT_TRUE(
        is_property_list(list("a"_tag(1), "b"_tag(2))));
   }

   TEST(property_list, property_list_ref) {
      STATIC_EXPECT_EQ(
        "a"_tag(1),
        property_list_ref("a"_tag, list("a"_tag(1), "b"_tag(2))));
   }

   TEST(list, take) {
      STATIC_EXPECT_EQ(list(1, 2), take(nat<2>, list(1, 2, 3, 4)));
   }

   TEST(list, take_fewer) {
      STATIC_EXPECT_EQ(list(1), take(nat<2>, list(1)));
   }

   TEST(list, drop) {
      STATIC_EXPECT_EQ(list(3, 4), drop(nat<2>, list(1, 2, 3, 4)));
   }

   TEST(list, drop_more) {
      STATIC_EXPECT_EQ(nothing, drop(nat<10>, list(1, 2, 3, 4)));
   }

   TEST(list, addition_operator) {
      static_assert(AdditiveMonoid<decltype(list(1, 2))>);
      STATIC_EXPECT_EQ(list(1, 2, 3, 4), list(1, 2) + list(3, 4));
   }

   TEST(list, addition_operator_associative) {
      STATIC_EXPECT_EQ(
        list(1, 2) + (list(3, 4) + list(5, 6)),
        (list(1, 2) + list(3, 4)) + list(5, 6));
   }

   TEST(list, addition_operator_id) {
      STATIC_EXPECT_EQ(list(1, 2), list(1, 2) + nothing);
      STATIC_EXPECT_EQ(list(1, 2), nothing + list(1, 2));
   }

   TEST(list, addition_operator_zero) {
      STATIC_EXPECT_EQ(list(1, 2), list(1, 2) + zero);
      STATIC_EXPECT_EQ(list(1, 2), zero + list(1, 2));
   }

   TEST(list, multiplication_operator_0_left) {
      STATIC_EXPECT_EQ(list(), nat<0> * list(1, 2));
   }

   TEST(list, multiplication_operator_0_right) {
      STATIC_EXPECT_EQ(list(), list(1, 2) * nat<0>);
   }

   TEST(list, multiplication_operator_1_left) {
      STATIC_EXPECT_EQ(list(1, 2), nat<1> * list(1, 2));
   }

   TEST(list, multiplication_operator_1_right) {
      STATIC_EXPECT_EQ(list(1, 2), list(1, 2) * nat<1>);
   }

   TEST(list, multiplication_operator_2_left) {
      STATIC_EXPECT_EQ(list(1, 2, 1, 2), nat<2> * list(1, 2));
   }

   TEST(list, multiplication_operator_2_right) {
      STATIC_EXPECT_EQ(list(1, 2, 1, 2), list(1, 2) * nat<2>);
   }

   TEST(list, multiplication_operator_3_left) {
      STATIC_EXPECT_EQ(list(1, 2, 1, 2, 1, 2), nat<3> * list(1, 2));
   }

   TEST(list, multiplication_operator_3_right) {
      STATIC_EXPECT_EQ(list(1, 2, 1, 2, 1, 2), list(1, 2) * nat<3>);
   }

   TEST(list, fold) {
      STATIC_EXPECT_EQ(10, foldl(std::plus{}, 0, list(1, 2, 3, 4)));
   }

} // end of namespace ruse::testing
