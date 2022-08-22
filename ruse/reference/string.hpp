#pragma once

//
// ... ruse header files
//
#include <ruse/reference/hoisted.hpp>
#include <ruse/reference/import.hpp>
#include <ruse/reference/list.hpp>
#include <ruse/reference/tag.hpp>

namespace ruse::reference {

  template<typename T>
  concept Character = same_as<T, char> || same_as<T, signed char> ||
    same_as<T, unsigned char> || same_as<T, wchar_t> || same_as<T, char16_t> ||
    same_as<T, char32_t> || same_as<T, char8_t>;

  constexpr auto is_character_type = []<typename T>(T) {
    if constexpr (TypeProxy<T>) {
      return []<typename U>(Type<U>) { return Character<U>; }(T{});
    } else {
      return false;
    }
  };

  constexpr auto is_character = []<typename T>(T) { return Character<T>; };

  template<typename T>
  concept String = HomogeneousList<T> &&
    is_character_type(flatmap(head_type, type<T>));

  template<typename T>
  concept NonstringList = List<T> and !String<T>;

  template<typename T>
  concept UnitaryString = String<T> and UnitaryList<T>;

  constexpr auto is_string = []<typename T>(T) { return String<T>; };

  template<typename T>
  concept HoistedString =
    HomogeneousHoistedList<T> && String<decltype(T::values)>;

  template<typename T>
  concept NonstringHoistedList = HoistedList<T> && !HoistedString<T>;

  template<Character Char, integer N>
  struct fixed_string
  {
    using character_type = Char;
    static constexpr auto extent = N;

    constexpr fixed_string(const Char* input)
    {
      for (integer i = 0; i < N; ++i) {
        value[i] = input[i];
      }
    }

    Char value[N];
  };

  template<typename Char, auto N>
  fixed_string(const Char (&)[N]) -> fixed_string<Char, N>;

  template<fixed_string str>
  constexpr auto operator""_s()
  {
    return []<auto... Indices>(index_sequence<Indices...>)
    {
      return list(str.value[Indices]...);
    }
    (make_index_sequence<str.extent - 1>());
  }

  template<fixed_string str>
  constexpr auto operator""_hoist()
  {
    return []<auto... Indices>(index_sequence<Indices...>)
    {
      return hoisted_list<str.value[Indices]...>{};
    }
    (make_index_sequence<str.extent - 1>());
  };

  template<fixed_string str>
  constexpr auto operator""_tag()
  {
    return []<auto... Indices>(index_sequence<Indices...>)
    {
      return tag<hoisted_list<str.value[Indices]...>>{};
    }
    (make_index_sequence<str.extent - 1>());
  }

  template<fixed_string s>
  constexpr decltype(operator""_hoist<s>()) str{};

} // end of namespace ruse::reference
