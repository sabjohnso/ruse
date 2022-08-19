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
  struct Type
  {
    using type = T;

    template<typename... Us>
    constexpr auto
    operator()(Us... xs) const
    {
      return T{xs...};
    }

    friend constexpr bool
    operator==(Type, Type)
    {
      return true;
    }

    template<typename U>
    friend constexpr bool
    operator==(Type, Type<U>)
    {
      return false;
    }

    template<typename U>
    friend constexpr bool
    operator!=(Type, Type<U>)
    {
      return !(Type{} == Type<U>{});
    }
  };

  /**
   * @brief Type proxy variable
   */
  template<typename T>
  constexpr Type<remove_cvref_t<T>> type{};

  /**
   * @brief Return `true` if the input is a type proxy
   * for a type proxy. Otherwise, return `false`.
   */
  struct is_type_Type
  {

    template<typename T>
    constexpr bool
    operator()(Type<Type<T>>) const
    {
      return true;
    }

    constexpr bool operator()(auto) const { return false; }

  } constexpr is_type_type{};

  /**
   * @brief A concept for type proxies
   */
  template<typename T>
  concept TypeProxy = is_type_type(type<T>);

  /**
   * @brief Return `true` if the input is a type proxy. Otherwise, return
   * `false`.
   */
  constexpr auto is_type = []<typename T>(T) { return TypeProxy<T>; };

  /**
   * @brief A concept for particular type proxies.
   */
  template<typename T, typename U>
  concept TypeProxyOf = is_same_v<remove_cvref_t<T>, Type<remove_cvref_t<U>>>;

  constexpr auto type_of = []<typename T>(T) { return type<T>; };

  template<TypeProxy T>
  constexpr auto
  get_pure(Type<T>)
  {
    return type_of;
  }

  template<TypeProxy T>
  constexpr auto
  get_flatmap(Type<T>)
  {
    return
      []<typename F, typename U>(F, Type<U>) { return result_of_t<F(U)>{}; };
  }

  template<typename T>
  constexpr auto is = []<typename U>(U) { return type<T> == type<U>; };
} // end of namespace ruse::reference
