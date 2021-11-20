#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>

namespace ruse::reference {

  /**
   * @brief A type whose instances act as proxies for the
   * type specified with the template parameter
   */
  template<typename T>
  struct type_s
  {
    using type = T;

    friend constexpr bool
    operator ==(type_s, type_s){ return true; }

    template<typename U>
    friend constexpr bool
    operator ==(type_s, type_s<U>){ return false; }


    template<typename U>
    friend constexpr bool
    operator !=(type_s, type_s<U>){ return !(type_s{} == type_s<U>{}); }
  };

  /**
   * @brief Type proxy variable
   */
  template<typename T>
  constexpr type_s<remove_cvref_t<T>> type{};

  /**
   * @brief Return `true` if the input is a type proxy
   * for a type proxy. Otherwise, return `false`.
   */
  struct is_type_type_s
  {

    template<typename T>
    constexpr bool operator()(type_s<type_s<T>>) const
    {
      return true;
    }

    constexpr bool operator()(auto) const { return false; }

  } constexpr is_type_type{};


  /**
   * @brief A concept for type proxies
   */
  template<typename T>
  concept Type = is_type_type(type<T>);


  /**
   * @brief Return `true` if the input is a type proxy. Otherwise, return `false`.
   */
  constexpr auto is_type = []<typename T>(T) { return Type<T>; };

  /**
   * @brief A concept for particular type proxies.
   */
  template<typename T, typename U>
  concept TypeProxy = is_same_v<remove_cvref_t<T>, type_s<remove_cvref_t<U>>>;

} // end of namespace ruse::reference
