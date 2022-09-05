//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/experimental/rough.hpp>
#include <ruse/ruse.hpp>
#include <ruse/show.hpp>

namespace ruse::experimental::testing {
   using namespace ruse::reference;

   constexpr auto make_xs = [] { return list("a"_s, "b"_s, "c"_s); };

   // This test shows that a lambda can be used to lift values
   // into types
   TEST(lambda, to_type) {
      using T = hoisted<make_xs>;

      STATIC_EXPECT_EQ("a"_s, list_ref(nat<0>, T::value()));
      STATIC_EXPECT_EQ("b"_s, list_ref(nat<1>, T::value()));
      STATIC_EXPECT_EQ("c"_s, list_ref(nat<2>, T::value()));

      using Length = hoisted<length(T::value())>;
      using X = hoisted<list_ref(nat<0>, T::value())>;
      using Y = hoisted<list_ref(nat<1>, T::value())>;
      using Z = hoisted<list_ref(nat<2>, T::value())>;

      STATIC_EXPECT_EQ(3, Length::value);
      STATIC_EXPECT_EQ("a"_s, X::value);
      STATIC_EXPECT_EQ("b"_s, Y::value);
      STATIC_EXPECT_EQ("c"_s, Z::value);
   }

   struct MyNode {
      // `State` is a data type that is not literal because
      // it has a data member named value that is a string.
      //
      // However, `State` just defines the type and does not change
      // the literal nature of `MyNode`.
      struct State {
         std::string value;
      };

      // `fn1` is a function that accepts two non-literal values and
      // mutates one of them.  However, the function itself does not
      // capture any values and is constexpr. Is has no impact on the
      // literal nature of `MyNode`
      static constexpr auto fn1 =
        [](const std::string& input, State& state) {
           state.value = input;
        };

      // `fn2` also does not change the literal nature of `MyNode`
      static void
      fn2(const std::string& input, State& state, auto... args) {
         state.value = input;
         emit(82, args...);
      }

      constexpr auto
      operator()() const {
         return list("a"_s, type_of(fn1));
      }
   };

   // How can we get the argument types from the function in the list.
   // We can print it, but that is not the same.
   // Boost DI does it!
   //
   // std::function can be used to get the argument types
   TEST(callable_struct, to_type) {
      using T = hoisted<MyNode{}>;
      STATIC_EXPECT_EQ("a"_s, list_ref(nat<0>, T::value()));
      std::cout
        << type_name<
             decltype(list_ref(nat<0>, T::value()))> << std::endl;
      std::cout
        << type_name<
             decltype(list_ref(nat<1>, T::value()))> << std::endl;

      constexpr auto get_signature = [](auto f) {
         return []<class R, class... Args>(std::function<R(Args...)>) {
            return list(type<R>, list(type<Args>...));
         }(std::function{f});
      };

      std::cout << get_signature([](int x, int y) { return x + y; })
                << std::endl;
      std::cout << get_signature([] {}) << std::endl;
      std::cout << get_signature([](int x) { return x; }) << std::endl;

      // We still can't get out the generic arguments but we could try
      // something like what is done in reflection to get the fields of
      // a struct

      // We could try to have a type called anything and otherone that
      // can't be cast to anything and see what the minimum number of
      // arguments is and what the defined types are. It seems
      // problematic

      // std::cout << get_signature([](auto x) {
      //   return [](int x) { return x; }(x);
      // }) << std::endl;
   }

   // The folling could be technically feasible, but it is not very
   // readablea list(
   //   define_syntax<"begin/m">(
   //     syntax_parser(
   //       pattern((_, "e"_expr)).repl(stx<"e"_expr>)
   //       pattern((_, "e"_expr, +"es"_expr)).repl(
   //         app("with_syntax"_s, list(list("ignored"_s,
   //         generate_temporary())),
   //             stx(app("flatmap"_s, lambda("ignore"_s)(
   //                       app("begin/m"_s, splice<"es"_s>)),
   //                     "e"_s)))))),

   //   define_syntax<"let/m">(
   //     syntax_parser(
   //       rule(patt(ap(_, l(l("x"_id,  "mx"_expr)), "body"_expr)),
   //            repl(ap("flatmap", lambda("x"_s)("body"_s), "mx"_s))),
   //       rule(patt(ap(_, l(l("x"_id, "mx"_expr),
   //       +"more-bidings"_expr), "body"_expr)),
   //            repl(ap("let/m", l(l("x"_id, "mx"_expr)),
   //                    ap("let/m"_s, l(splice("more-bindings"_s)),
   //                       splice("body"_es))))))));

   template<typename T>
   concept Unembelished = std::is_same_v<T, std::remove_cvref_t<T>> and
                          not std::is_pointer_v<T> and
                          not std::is_array_v<T>;

   // If we were collecting information about each distinct type
   // used we would want to know the unembelished types to understand
   // what requirements they have for their constructors.
   //
   // We would also want to know the arrays, because we will need to
   // construct Them from their element type.  Boost DI would produce an
   // array of equal values, because it only makes one of each type.
   TEST(unadorned_type, concept_for) {
      STATIC_EXPECT_TRUE(Unembelished<int>);
      STATIC_EXPECT_FALSE(Unembelished<int*>);
      STATIC_EXPECT_FALSE(Unembelished<int&>);
      STATIC_EXPECT_FALSE(Unembelished<const int>);
      STATIC_EXPECT_FALSE(Unembelished<const int&>);
      STATIC_EXPECT_FALSE(Unembelished<volatile int>);
      STATIC_EXPECT_FALSE(Unembelished<int[ 3 ]>);
   }

   // connecting components with emit
   //
   // In this implementation of emit,
   constexpr auto emit =
     []<class T, class F, class... Fs>(T&& x, F&& f, Fs&&... fs) {
        f(x, fs...);
     };

   // In this implementation, there are multiple
   // receivers
   // constexpr auto emit_multi = [](auto&& x, auto&& fs, auto&&... gss)
   // {
   //   dolist(
   //     fs, [&]<auto F>(auto&& f) {
   //       if constexpr (Invocable<decltype(f), decltype(x),
   //       decltype(gss)...>)
   //       {
   //         f(x, gss...);
   //       }
   //     });
   // };

   TEST(emit, xxx) {
      int x = 3;

      auto increment = [](int input, auto&&... ks) {
         emit(input + 1, ks...);
      };
      auto sqr = [](int input, auto&&... ks) {
         emit(input * input, ks...);
      };
      auto set_result = [ & ](int input, auto&&...) { x = input; };

      increment(x, sqr, set_result);

      EXPECT_EQ(x, 16);

      x = 1;

      // auto increment_multi = [](int input, auto&&... ks) {
      //   emit_multi(input + 1, ks...);
      // };
      // auto sqr_multi = [](int input, auto&&... ks) {
      //   emit_multi(input * input, ks...);
      // };
      // auto set_result_multi = [&](int input, auto&&...) { x = input;
      // };

      // increment_multi(x, list(sqr_multi), list(set_result_multi));
   }

} // namespace ruse::experimental::testing
