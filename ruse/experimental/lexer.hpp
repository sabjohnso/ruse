#pragma once

//
// ... ruse header files
//
#include <ruse/reference/functional.hpp>
#include <ruse/reference/string.hpp>
#include <ruse/reference/utility.hpp>

namespace ruse::experimental::lexer {
  using namespace ruse::reference;

  template<integer X>
  concept Positive = (X > 0);

  namespace primitive {
    template<integer Count>
    struct Item
    {};

    template<integer Count>
    constexpr Item<Count> item{};

    template<auto... elements>
    struct Alt
    {
      static constexpr auto values = list(elements...);
    };

    template<auto... elements>
    constexpr Alt<elements...> alt{};

    template<auto... elements>
    struct Cut
    {
      static constexpr auto values = list(elements...);
    };

    template<auto... elements>
    constexpr Cut<elements...> cut{};

    template<auto... elements>
    struct Seq
    {
      static constexpr auto values = list(elements...);
    };

    template<auto... elements>
    constexpr Seq<elements...> seq{};

    template<auto Guard, auto Lexeme>
    struct Guarded
    {
      static constexpr auto guard = Guard;
      static constexpr auto lexeme = Lexeme;
    };

    template<auto Guard, auto Lexeme>
    constexpr Guarded<Guard, Lexeme> guarded{};

    constexpr auto run = curry(nat<2>, [](auto lexeme, auto xs) {
      constexpr auto recur = case_lambda(

        // Guarded
        []<auto Guard, auto Lexeme, String Sequence>(
          auto recur, Guarded<Guard, Lexeme>, Sequence xs) {
          return Guard(car(xs)) ? recur(recur, Lexeme, xs) : nat<0>;
        },

        // Item
        []<integer N, String Sequence>(auto, Item<N>, Sequence) {
          if constexpr (length_type(type<Sequence>) >= N) {
            return nat<N>;
          } else {
            return nat<0>;
          }
        },

        // Cut
        // []<auto Lexeme, auto... Lexemes, String Sequence>(
        //   auto recur, Cut<Lexeme, Lexemes...>, Sequence sequence) {
        //   constexpr auto N = decltype(recur(recur, Lexeme, sequence);
        //   const auto n = recur(recur, Lexeme, sequence);
        //   if constexpr (same_as<remove_cvref_t<decltype(n)>, Nat<0>>) {
        //   return recur(recur, cut<Lexemes...>, sequence);
        //   } else {
        //   return n;
        //   }
        // },

        // // Seq
        // []<auto Lexeme, auto... Lexemes, String Sequence>(
        //   auto recur, Seq<Lexeme, Lexemes...>, Sequence sequence) {
        //   const auto m = recur(recur, Lexeme, sequence);
        //   if constexpr (sizeof...(Lexemes) == 0) {
        //     return m
        //   } else {
        //     const auto n = recur(recur, seq<Lexemes...>, drop(m, sequence));
        //     if constexpr
        //   }
        // },

        // Fail
        [](auto...) -> Natural auto { return nat<0>; });
      return recur(recur, lexeme, xs);
    });

  } // namespace primitive

} // end of namespace ruse::experimental::lexer
