#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/nat.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {
  /**
   * @brief U combinator introducing recursion.
   */
  constexpr auto u = [](auto f, auto... xs) { return f(f, xs...); };

  /**
   * @brief A concept for default constructible types
   */
  template<typename T>
  concept DefaultConstructible = requires
  {
    T{};
  };

  /**
   * @brief A concept for empty types
   */
  template<typename T>
  concept Empty = is_empty_v<T>;

  /**
   * @brief A concept for vacuous types
   */
  template<typename T>
  concept Vacuous = Empty<T> && DefaultConstructible<T>;

  constexpr auto is_vacuous_type = []<typename T>(type_s<T>) {
    return Vacuous<T>;
  };

  /**
   * @brief A concept for a selector
   */
  template<typename T, auto M>
  concept Select = true;

  /**
   * @brief Curried function application
   */
  constexpr auto curry = []<integer N>(Nat<N>, auto f, auto... xs) {
    constexpr auto aux = [](auto recur, auto f, auto... xs) {
      return [=](auto... ys) {
        constexpr integer M = sizeof...(xs) + sizeof...(ys);

        if constexpr (M == N) {
          return f(xs..., ys...);

        } else if constexpr (M > N) {

          // gcc fails with the compact expression
#ifdef __clang__
          return [=]<std::size_t... Indices>(index_sequence<Indices...>)
          {
            return [=](Select<Indices> auto... xs, auto... ys) {
              return f(xs...)(ys...);
            };
          }
          (make_index_sequence<N>())(xs..., ys...);
#else
          constexpr auto apply_taking =
            []<std::size_t... Indices>(index_sequence<Indices...>)
          {
            return [](auto g, auto zs) { return g(std::get<Indices>(zs)...); };
          }
          (make_index_sequence<N>());

          constexpr auto apply_dropping =
            []<std::size_t... Indices>(index_sequence<Indices...>)
          {
            return
              [](auto g, auto zs) { return g(std::get<Indices + N>(zs)...); };
          }
          (make_index_sequence<M - N>());
          return [=](auto zs) {
            return apply_dropping(apply_taking(f, zs), zs);
          }(std::tuple{xs..., ys...});
#endif
        } else {
          return recur(recur, f, xs..., ys...);
        }
      };
    };
    return u(aux, f, xs...);
  };

  /**
   * @brief Lazy function application
   */
  constexpr auto Zzz = [](auto f, auto... xs) {
    return curry(nat<sizeof...(xs)>, f, xs...);
  };

  /**
   * @brief Return the input value;
   */
  constexpr auto identity = curry(nat<1>, [](auto x) { return x; });

  /**
   * @brief Return the right-to-left composition of the two input functions
   */
  constexpr auto compose = curry(nat<2>, [](auto f, auto g) {
    return [=](auto... xs) { return f(g(xs...)); };
  });

  /**
   * @brief Return the result of applying the input function to the input
   * result.
   */
  constexpr auto let = [](auto x, auto f) { return f(x); };

  template<typename F, typename... Ts>
  concept Invocable = is_invocable_v<F, Ts...>;

} // end of namespace ruse::reference
