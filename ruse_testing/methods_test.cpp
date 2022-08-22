//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/experimental/methods.hpp>
#include <ruse/ruse.hpp>

namespace myspace {

  using ruse::car;
  using ruse::cdr;
  using ruse::cons;
  using ruse::List;
  using ruse::list;
  using ruse::NonemptyList;
  using ruse::nothing;
  using ruse::Nothing;
  using ruse::Type;

  using ruse::experimental::method;

  template<typename T>
  struct stack
  {};

  template<typename T>
  constexpr auto
  methods(Type<stack<T>>)
  {
    return list(

      method<"make-new"> = [](stack<T>) { return nothing; },

      method<"empty?"> =
        []<List U>(stack<T>, U) {
          if constexpr (Nothing<U>) {
            return false;
          } else {
            return true;
          }
        },

      method<"push!"> = [](stack<T>, List auto xs, T x) { return cons(x, xs); },

      method<"pop!"> = [](stack<T>, List auto xs) -> List auto {
        if constexpr (NonemptyList<decltype(xs)>) {
          return cdr(xs);
        } else {
          return nothing;
        }
      },

      method<"top"> = [](stack<T>, NonemptyList auto xs) { return car(xs); });
  }

} // namespace myspace

namespace ruse::experimental::testing {

  TEST(methods, method)
  {
    constexpr auto int_stack = myspace::stack<int>{};
    STATIC_EXPECT_EQ(nothing, send<"make-new">(int_stack));
    STATIC_EXPECT_EQ(
      cons(3, nothing),
      send<"push!">(int_stack, send<"make-new">(int_stack), 3));
  }
} // namespace ruse::experimental::testing
