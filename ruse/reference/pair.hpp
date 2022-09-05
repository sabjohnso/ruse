#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/nat.hpp>
#include <ruse/reference/utility.hpp>

namespace ruse::reference {

   /**
    * @brief A concept for pairs
    */
   template<typename T>
   concept Pair = requires(T x) {
      typename T::first_type;
      typename T::second_type;
      { x.first } -> convertible_to<typename T::first_type>;
      { x.second } -> convertible_to<typename T::second_type>;
   };

   /**
    * @brief A type representing an ordered pair of values.
    */
   template<typename First, typename Second>
   struct pair {
      using first_type = First;
      using second_type = Second;

      first_type first;
      second_type second;

      friend constexpr bool
      operator==(pair xs, Pair auto ys) {
         return (xs.first == ys.first) && (xs.second == ys.second);
      }

      friend constexpr bool
      operator==(pair, auto) {
         return false;
      }

      friend constexpr bool
      operator!=(pair xs, auto ys) {
         return ! (xs == ys);
      }
   };

   /**
     @brief Return `true` if the input is a pair. Otherwise, return
     `false`.
    */
   constexpr auto is_pair = []<typename T>(T) { return Pair<T>; };

   /**
    * @brief Return a pair holding the two input values.
    */
   constexpr auto cons =
     curry(nat<2>, []<typename T, typename U>(T x, U y) {
        return pair<T, U>{.first = x, .second = y};
     });

   /**
    * @brief Return the first member from a pair.
    */
   constexpr auto car =
     curry(nat<1>, [](Pair auto xs) { return xs.first; });

   /**
    * @brief Return the first member from a pair.
    */
   constexpr auto cdr =
     curry(nat<1>, [](Pair auto xs) { return xs.second; });

} // end of namespace ruse::reference
