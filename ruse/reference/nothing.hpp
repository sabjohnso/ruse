#pragma once

//
// ... ruse header files
//
#include <ruse/reference/type.hpp>

namespace ruse::reference {

  /**
   * @brief A type representing the absence of data
   */
  struct nothing_s
  {

    friend constexpr bool
    operator==(nothing_s, nothing_s)
    {
      return true;
    }

    template<typename T>
    friend constexpr bool
    operator==(nothing_s, T)
    {
      return false;
    }

    template<typename T>
    friend constexpr bool
    operator!=(nothing_s x, T y)
    {
      return !(x == y);
    }

    template<typename T>
    friend constexpr bool
    operator!=(T x, nothing_s y)
    {
      return !(x == y);
    }

  } constexpr nothing{};

  /**
   * @brief A concept for nothing
   */
  template<typename T>
  concept Nothing = TypeProxy<type_s<T>, nothing_s>;

  template<typename T>
  concept Something = not Nothing<T>;

  /**
   * @brief Return `true` if the input is `nothing`. Otherwise, return
   * `false`.
   */
  constexpr auto is_nothing = []<typename T>(T) { return Nothing<T>; };

} // end of namespace ruse::reference
