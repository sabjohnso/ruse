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

      struct Item {
         integer count;
      };

      template<auto... elements>
      struct Alt {
         static constexpr auto values = list(elements...);
      };

      template<auto... elements>
      constexpr Alt<elements...> alt{};

      template<auto... elements>
      struct Cut {
         static constexpr auto values = list(elements...);
      };

      template<auto... elements>
      constexpr Cut<elements...> cut{};

      template<auto... elements>
      struct Seq {
         static constexpr auto values = list(elements...);
      };

      template<auto... elements>
      constexpr Seq<elements...> seq{};

      template<auto Guard, auto Lex>
      struct Guarded {
         static constexpr auto guard = Guard;
         static constexpr auto lexeme = Lex;
      };

      template<auto Guard, auto Lex>
      constexpr Guarded<Guard, Lex> guarded{};

      template<auto Lex, auto Proc>
      struct Bind {};

      template<auto Lex, auto Proc>
      constexpr Bind<Lex, Proc> bind{};

      template<auto Lex, auto Proc>
      struct BindValue {};

      template<auto Lex, auto Proc>
      constexpr BindValue<Lex, Proc> bind_value{};

      constexpr auto run = curry(nat<2>, [](auto lexeme, auto xs) {
         constexpr auto recur = case_lambda(

           // Guarded
           []<auto Guard, auto Lex>(
             auto recur,
             Guarded<Guard, Lex>,
             auto xs) {
              return Guard(xs[ 0 ]) ? recur(recur, Lex, xs) : nat<0>;
           },

           // Item
           [](auto /*recur*/, Item item, auto xs) {
              return ssize(xs) >= item.count ? item.count : 0;
           },

           // Cut
           []<auto Lex, auto... Lexs>(
             auto recur,
             Cut<Lex, Lexs...>,
             auto xs) {
              const auto m = recur(recur, Lex, xs);
              return m ? m : recur(recur, cut<Lexs...>, xs);
           },

           // Seq
           []<auto Lex>(auto recur, Seq<Lex>, auto xs) {
              return recur(recur, Lex, xs);
           },
           []<auto Lex, auto... Lexs>(
             auto recur,
             Seq<Lex, Lexs...>,
             auto xs) {
              const auto m = recur(recur, Lex, xs);
              const auto n = m ? recur(
                                   recur,
                                   seq<Lexs...>,
                                   xs.substr(1, string_view::npos))
                               : 0;
              return m && n ? m + n : 0;
           },

           // Alt
           []<auto Lex>(auto recur, Alt<Lex>, auto xs) {
              return recur(recur, Lex, xs);
           },
           []<auto Lex, auto... Lexs>(
             auto recur,
             Alt<Lex, Lexs...>,
             auto xs) {
              const auto m = recur(recur, Lex, xs);
              const auto n = recur(recur, alt<Lexs...>, xs);
              return m > n ? m : n;
           },

           // Bind
           []<auto Lex, auto Proc>(
             auto recur,
             Bind<Lex, Proc>,
             auto xs) {
              const auto n = recur(recur, Lex, xs);
              return n ? recur(recur, Proc(n), xs) : n;
           },

           // BindValue
           []<auto Lex, auto Proc>(
             auto recur,
             BindValue<Lex, Proc>,
             auto xs) {
              const auto n = recur(recur, Lex, xs);
              return n ? recur(recur, Proc(xs.substr(0, n)), xs) : n;
           },

           // Fail
           [](auto...) { return 0; });
         return recur(recur, lexeme, xs);
      });

   } // namespace primitive

} // end of namespace ruse::experimental::lexer
