#pragma once

//
// ... ruse header files
//
#include <ruse/reference/type.hpp>

namespace ruse::reference {

  template<typename T>
  struct pure_s
  {
    using value_type = T;
    value_type value;

    template<typename U>
    friend constexpr bool
    operator==(pure_s x, pure_s<U> y)
    {
      return x.value == y.value;
    }

    template<typename U>
    friend constexpr bool
    operator!=(pure_s x, pure_s<U> y)
    {
      return not(x == y);
    }
  };

  struct is_pure_type_s
  {
    template<typename T>
    constexpr auto
    operator()(type_s<pure_s<T>>) const
    {
      return true;
    }
    constexpr auto operator()(auto) const { return false; }
  } constexpr is_pure_type{};

  template<typename T>
  concept Pure = is_pure_type(type<T>);

  constexpr auto is_pure = []<typename T>(T) { return Pure<T>; };

  constexpr auto pure = []<typename T>(T x) { return pure_s<T>{.value = x}; };

} // end of namespace ruse::reference
