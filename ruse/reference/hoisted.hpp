#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/list.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {

  template<auto Value>
  struct hoisted
  {
    static constexpr auto value = Value;

    friend constexpr bool
    operator==(hoisted, hoisted)
    {
      return true;
    }

    template<typename T>
    friend constexpr bool
    operator==(hoisted, T)
    {
      return false;
    }

    template<typename T>
    friend constexpr bool
    operator!=(hoisted x, T y)
    {
      return !(x == y);
    }
  };

  /**
   * @brief Return `true` if the input is a type proxy for a hoisted value type.
   * Otherwise, return `false`.
   */
  struct is_hoisted_Type
  {
    template<auto Value>
    constexpr bool
    operator()(Type<hoisted<Value>>) const
    {
      return true;
    }

    constexpr bool operator()(auto) const { return false; }
  } constexpr is_hoisted_type{};

  /**
   * @brief A concept for hoisted values
   */
  template<typename T>
  concept Hoisted = is_hoisted_type(type<T>);

  /**
   * @brief Return `true` if the input is a hoisted value. Otherwise, return
   * `false`.
   */
  constexpr auto is_hoisted = []<typename T>(T) { return Hoisted<T>; };

  template<auto... Values>
  struct hoisted_list
  {
    static constexpr auto values = list(Values...);

    friend constexpr bool
    operator==(hoisted_list, hoisted_list)
    {
      return true;
    }

    template<typename T>
    friend constexpr bool
    operator==(hoisted_list, T)
    {
      return false;
    }

    template<typename T>
    friend constexpr bool
    operator!=(hoisted_list x, T y)
    {
      return !(x == y);
    }
  };

  /**
   * @brief Return true if the input is a type proxy for a hoisted list type.
   * Otherwise, return `false`.
   */
  struct is_hoisted_list_Type
  {
    template<auto... Values>
    constexpr bool
    operator()(Type<hoisted_list<Values...>>) const
    {
      return true;
    }

    constexpr bool operator()(auto) const { return false; }

  } constexpr is_hoisted_list_type{};

  /**
   * @brief A concept for hoisted lists
   */
  template<typename T>
  concept HoistedList = is_hoisted_list_type(type<T>);

  /**
   * @brief A concept for homogeneous hoisted lists
   */
  template<typename T>
  concept HomogeneousHoistedList =
    HoistedList<T> && HomogeneousList<decltype(T::values)>;

  /**
   * @brief Return `true` if the input is a hoisted list. Otherwise, return
   * `false`.
   */
  constexpr auto is_hoisted_list = []<typename T>(T) { return HoistedList<T>; };

  template<typename T>
  concept NonemptyHoistedList =
    HoistedList<T> && NonemptyList<decltype(T::values)>;

  constexpr auto lower = []<HoistedList T>(T) { return T::values; };

  /**
   * @brief A concept for thunks.
   */
  template<typename F>
  concept Thunk = requires(F f)
  {
    f();
  };

  /**
   * @brief A concept for default constructible thunks.
   */
  template<typename F>
  concept CompileTimeThunk = Thunk<F> && requires
  {
    F{}();
  };

  constexpr auto hoist = []<CompileTimeThunk F>(F) {
    constexpr auto result = F{}();
    if constexpr (List<decltype(result)>) {
      return [=]<auto... Indices>(index_sequence<Indices...>)
      {
        return hoisted_list<list_ref(nat<Indices>, result)...>{};
      }
      (make_index_sequence<length_type(type_of(result))>());
    } else {
      return hoisted<result>{};
    }
  };

  constexpr auto hoist_default = []<Thunk F>(F) {
    using result = result_of_t<F()>;
    static constexpr result result_value{};
    if constexpr (List<result>) {
      return []<auto... Indices>(index_sequence<Indices...>)
      {
        return hoisted_list<list_ref(nat<Indices>, result_value)...>{};
      }
      (make_index_sequence<length_type(type<result>)>());
    } else {
      return hoisted_list<result_value>{};
    }
  };

  constexpr auto hoisted_nothing = hoisted_list<>{};

  constexpr auto hoisted_cons = curry(
    nat<2>,
    []<auto x, auto... xs>(hoisted<x>, hoisted_list<xs...>) {
      return hoisted_list<x, xs...>{};
    });

  constexpr auto hoisted_head = []<NonemptyHoistedList T>(T) {
    return hoist([] { return head(T::values); });
  };

  constexpr auto hoisted_tail = []<HoistedList T>(T) {
    return hoist([] { return tail(T::values); });
  };

  constexpr auto hoisted_last = []<NonemptyHoistedList T>(T) {
    return hoist([] { return last(T::values); });
  };

  constexpr auto hoisted_butlast = []<HoistedList T>(T) {
    return hoist([] { return butlast(T::values); });
  };

  constexpr auto hoisted_reverse = []<HoistedList T>(T) {
    return hoist([] { return reverse(T::values); });
  };

  constexpr auto hoisted_append =
    curry(nat<2>, []<HoistedList T, HoistedList U>(T, U) {
      return hoist([] { return append(T::values, U::values); });
    });

  constexpr auto hoisted_take =
    curry(nat<2>, []<integer N, HoistedList T>(Nat<N>, T) {
      return hoist([] { return take(nat<N>, T::values); });
    });

  constexpr auto hoisted_drop =
    curry(nat<2>, []<integer N, HoistedList T>(Nat<N>, T) {
      return hoist([] { return drop(nat<N>, T::values); });
    });

  constexpr auto hoisted_length = []<HoistedList T>(T) {
    return nat<length(T::values)>;
  };

  template<HoistedList T>
  constexpr auto
  get_fmap(Type<T>)
  {
    return []<typename F, HoistedList U>(F, U) {
      return hoist([] { return fmap(F{}, U::values); });
    };
  }

  template<HoistedList T>
  constexpr auto
  get_pure(Type<T>)
  {
    return []<Hoisted U>(U) { return hoist([] { return list(U::value); }); };
  }

  template<HoistedList T>
  constexpr auto
  get_flatmap(Type<T>)
  {
    return []<typename F, HoistedList U>(F, U) {
      return hoist([] { return flatmap(F{}, U::values); });
    };
  }

} // end of namespace ruse::reference
