#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/string.hpp>

namespace ruse::experimental {

   using namespace ruse::reference;

   template<Character auto Value>
   struct character_value {
      using character_type = decltype(Value);
      static constexpr character_type value = Value;

      static constexpr bool
      contains(character_value) {
         return true;
      }

      template<character_type OtherValue>
      static constexpr bool
      contains(character_value<OtherValue>) {
         return false;
      }

      static constexpr bool
      contains(character_type input) {
         return input == value;
      }
   };

   struct {
      template<Character auto Value>
      constexpr bool
      operator()(Type<character_value<Value>>) const {
         return true;
      }

      constexpr bool
      operator()(auto) const {
         return false;
      }
   } constexpr is_character_value_type{};

   template<typename T>
   concept CharacterValue = is_character_value_type(type<T>);

   constexpr auto is_character_value = []<typename T>(T) {
      return CharacterValue<T>;
   };

   template<Character auto Value>
   constexpr character_value<Value> character{};

   template<Character auto Lower, Character auto Upper>
   requires(
     Lower < Upper && is_same_v<
                        decltype(Lower),
                        decltype(Upper)>) struct character_range {
      using character_type = decltype(Lower);
      static constexpr character_type lower = Lower;
      static constexpr character_type upper = Upper;

      template<character_type Value>
      static constexpr bool
      contains(character_value<Value>) {
         return Value >= Lower && Value <= Upper;
      }

      static constexpr bool
      contains(character_type value) {
         return value >= Lower && value <= Upper;
      }
   };

   struct {

      template<Character auto Lower, Character auto Upper>
      constexpr bool
      operator()(Type<character_range<Lower, Upper>>) const {
         return true;
      }

      constexpr bool
      operator()(auto) const {
         return false;
      }

   } constexpr is_character_range_type{};

   template<typename T>
   concept CharacterRange = is_character_range_type(type<T>);

   constexpr auto is_character_range = []<typename T>(T) {
      return CharacterRange<T>;
   };

   template<Character auto Lower, Character auto Upper>
   requires(
     Lower < Upper &&
     is_same_v<
       decltype(Lower),
       decltype(Upper)>) constexpr character_range<Lower, Upper> range{};

   template<typename T>
   concept CharacterClassPrimitive =
     CharacterValue<T> || CharacterRange<T>;

   template<CharacterClassPrimitive C1, CharacterClassPrimitive C2>
   constexpr bool
   operator<(C1, C2) {
      if constexpr(CharacterValue<C1> && CharacterValue<C2>) {
         return C1::value < C2::value;

      } else if constexpr(CharacterValue<C1> && CharacterRange<C2>) {
         return C1::value < C2::lower;

      } else if constexpr(CharacterRange<C1> && CharacterValue<C2>) {
         return C1::lower < C2::value;

      } else {
         return (C1::lower < C2::lower) ||
                ((C1::lower == C2::lower) && (C1::upper < C2::upper));
      }
   }

   template<CharacterClassPrimitive C1, CharacterClassPrimitive C2>
   constexpr bool
   operator>(C1 x, C2 y) {
      return y < x;
   }

   template<CharacterClassPrimitive C1, CharacterClassPrimitive C2>
   constexpr bool
   operator==(C1 x, C2 y) {
      return ! (x < y || y < x);
   }

   template<CharacterClassPrimitive C1, CharacterClassPrimitive C2>
   constexpr bool
   operator<=(C1 x, C2 y) {
      return ! (y < x);
   }

   template<CharacterClassPrimitive C1, CharacterClassPrimitive C2>
   constexpr bool
   operator>=(C1 x, C2 y) {
      return ! (x < y);
   }

   constexpr auto left_adjacent = curry(
     nat<2>,
     []<CharacterClassPrimitive C1, CharacterClassPrimitive C2>(
       C1,
       C2) {
        if constexpr(CharacterValue<C1> && CharacterValue<C2>) {
           return C1::value + 1 == C2::value;

        } else if constexpr(CharacterValue<C1> && CharacterRange<C2>) {
           return C1::value + 1 == C2::lower;

        } else if constexpr(CharacterRange<C1> && CharacterValue<C2>) {
           return C1::upper + 1 == C2::value;

        } else {
           return C1::upper + 1 == C2::lower;
        }
     });

   constexpr auto right_adjacent = curry(
     nat<2>,
     []<CharacterClassPrimitive C1, CharacterClassPrimitive C2>(
       C1 x,
       C2 y) { return left_adjacent(y, x); });

   constexpr auto adjacent = curry(
     nat<2>,
     []<CharacterClassPrimitive C1, CharacterClassPrimitive C2>(
       C1 x,
       C2 y) { return left_adjacent(x, y) || right_adjacent(x, y); });

   constexpr auto overlap = curry(
     nat<2>,
     []<CharacterClassPrimitive C1, CharacterClassPrimitive C2>(
       C1,
       C2) {
        if constexpr(CharacterValue<C1> && CharacterValue<C2>) {
           return C2::contains(C1{});

        } else if constexpr(CharacterValue<C1> && CharacterRange<C2>) {
           return C2::contains(C1{});

        } else if constexpr(CharacterRange<C1> && CharacterValue<C2>) {
           return C1::contains(C2{});

        } else {
           return C1::contains(C2::lower) || C1::contains(C2::upper) ||
                  C2::contains(C1::lower) || C2::contains(C1::upper);
        }
     });

} // end of namespace ruse::experimental
