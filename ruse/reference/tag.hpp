#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/template.hpp>
#include <ruse/reference/type.hpp>
#include <ruse/reference/value_wrapper.hpp>

namespace ruse::reference {

  /**
   * @brief A concept for types with a member typedef named `name_type`
   */
  template<typename T>
  concept HasNameType = requires
  {
    typename T::name_type;
  };

  /**
   * @brief A concept for `HasNameType` types where `name_type` is an empty
   * type.
   */
  template<typename T>
  concept HasEmptyNameType = HasNameType<T> && Empty<typename T::name_type>;

  /**
   * @brief A concept for types, which value wappers that have an empty name
   * type.
   */
  template<typename T>
  concept Tagged = ValueWrapper<T> && HasEmptyNameType<T>;

  constexpr auto is_tagged = []<typename T>(T) { return Tagged<T>; };

  template<typename T>
  concept Tag = Empty<T> && HasEmptyNameType<T>;

  constexpr auto is_tag = []<typename T>(T) { return Tag<T>; };

  template<typename T, Empty Name>
  struct tagged
  {
    using value_type = T;
    using name_type = Name;
    static constexpr name_type name{};

    value_type value;

    constexpr auto
    operator<=>(const tagged&) const = default;
  };

  template<Empty Name>
  struct tag
  {
    using name_type = Name;
    static constexpr name_type name{};

    template<typename T>
    constexpr Tagged auto
    operator()(T x) const
    {
      return tagged<T, Name>{.value = x};
    }

    friend constexpr bool
    operator==(tag, tag)
    {
      return true;
    }

    template<typename U>
    friend constexpr bool
    operator==(tag, U)
    {
      return false;
    }

    template<typename U>
    friend constexpr bool
    operator!=(tag, U x)
    {
      return !(tag{} == x);
    }
  };

  constexpr auto name_type = []<TypeProxy T>(T) {
    return type<typename T::type::name_type>;
  };

  constexpr auto is_tagged_type = []<typename T>(T) {
    if constexpr (TypeProxy<T>) {
      return Tagged<typename T::type>;
    } else {
      return false;
    }
  };

  template<typename T, Empty Name>
  struct member
  {
    using value_type = T;
    using name_type = Name;
    T value;

    constexpr operator const value_type&() const& { return value; }
    constexpr
    operator value_type&&() &&
    {
      return value;
    }
    operator value_type&() & { return value; }
  };

} // end of namespace ruse::reference
