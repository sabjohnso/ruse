#pragma once

//
// ... ruse header files
//
#include <ruse/experimental/character_class.hpp>
#include <ruse/experimental/character_range.hpp>
#include <ruse/reference/list.hpp>
#include <ruse/reference/utility.hpp>

namespace ruse::experimental {

  using namespace ruse::reference;

  constexpr auto ordered_primitives =
    []<CharacterClassPrimitive... Primitives>(Primitives...)
  {
    constexpr auto recur =
      [
      ]<CharacterClassPrimitive P1,
        CharacterClassPrimitive P2,
        CharacterClassPrimitive... Ps>(auto recur, P1, P2, Ps...)
    {

      if constexpr (P1{} < P2{}) {
        if constexpr (sizeof...(Ps) > 0) {
          return recur(recur, P2{}, Ps{}...);
        } else {
          return true;
        }
      } else {
        return false;
      }
    };

    if constexpr (sizeof...(Primitives) > 1) {
      return u(recur, Primitives{}...);
    } else {
      return true;
    }
  };

  template<CharacterClassPrimitive auto... Primitives>
  requires(ordered_primitives(Primitives...)) struct CharacterClass
  {
    static constexpr auto primitives = list(Primitives...);
  };

  template<CharacterClassPrimitive auto... Primitives>
  constexpr CharacterClass<Primitives...> character_class{};

  constexpr auto is_character_class =
    []<CharacterClassPrimitive auto... Ps>(CharacterClass<Ps...>)
  {
    return true;
  };

  constexpr auto includes = curry(
    nat<2>,
    []<CharacterClassPrimitive auto... Ps, Character auto c>(
      CharacterClass<Ps...>,
      character_value<c>) {
      constexpr auto recur = [](auto recur, auto p, auto... ps) {
        if constexpr (overlap(p, character<c>)) {
          return true;
        } else {
          if constexpr (sizeof...(ps) == 0) {
            return false;
          } else {
            return recur(recur, ps...);
          }
        }
      };

      if constexpr (sizeof...(Ps) > 0) {
        return recur(recur, Ps...);
      } else {
        return false;
      }
    });

} // end of namespace ruse::experimental
