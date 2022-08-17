#pragma once

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::experimental {

  template<typename T>
  concept StateResult = BinaryList<T>;

  template<typename T, typename S>
  concept BareStateful = requires(T mx, S state)
  {
    {
      mx(state)
      } -> StateResult;
  };

  /**
   * @brief A data structure template denoting a stateful computation
   */
  template<typename F>
  struct stateful
  {
    using function_type = F;
    F run_state;

    constexpr stateful(F input)
      : run_state{input}
    {}
  };

  /**
   * @brief Return `true` if the input is a type proxy for a stateful type.
   * Otherwise, return `false`.
   */
  struct is_stateful_type_s
  {
    constexpr bool operator()(auto) const { return false; }

    template<typename F>
    constexpr bool
    operator()(type_s<stateful<F>>) const
    {
      return true;
    }
  } constexpr is_stateful_type{};

  /**
   * @brief The concept of stateful computations
   */
  template<typename T>
  concept Stateful = is_stateful_type(type<T>);

  /**
   * @brief The concept of a stateful computation that can run  over the
   * specified state type.
   */
  template<typename T, typename S>
  concept StatefulOver =
    Stateful<T> and BareStateful<typename T::function_type, S>;

  /**
   * @brief Return true for stateful input. Otherwise, return false otherwise.
   */
  constexpr auto is_stateful = []<typename T>(auto) { return Stateful<T>; };

  /**
   * @brief Return the result and state from a stateful computation with
   * the input state as the initial state
   */
  constexpr auto run_state =
    curry(nat<2>, []<typename S>(S s, StatefulOver<S> auto mx) {
      return mx.run_state(s);
    });

  /**
   * @brief inject a value into a stateful comutation
   */
  constexpr auto make_stateful = [](auto x) {
    return stateful{[=](auto s) { return list(x, s); }};
  };

  /**
   * @brief Return the state as the return value in stateful computation
   */
  constexpr auto get_state = stateful{[](auto s) { return list(s, s); }};

  /**
   * @brief Return a value selected from the state in a stateful computation
   */
  constexpr auto select_state = [](auto f) { return fmap(f, get_state); };

  /**
   * @brief Set the state in a stateful computation
   */
  constexpr auto put_state = [](auto s) -> Stateful auto
  {
    return stateful{[=](auto) { return list(nothing, s); }};
  };

  /**
   * @begin Modify the state in a stateful computation
   */
  constexpr auto modify_state = [](auto f) -> Stateful auto
  {
    return stateful{[=](auto s) { return list(nothing, f(s)); }};
  };

  /**
   * @begin Run a stateful computation, returning the final value and discarding
   * the final state.
   */
  constexpr auto eval_state =
    curry(nat<2>, []<typename S>(S state, StatefulOver<S> auto mx) {
      return car(run_state(state, mx));
    });

  /**
   * @end Run a stateful computation, returning the final value and discarding
   * the final state.
   */
  constexpr auto exec_state =
    curry(nat<2>, []<typename S>(S state, StatefulOver<S> auto mx) {
      return cadr(run_state(state, mx));
    });

  template<typename F>
  constexpr auto
  get_pure(type_s<stateful<F>>)
  {
    return make_stateful;
  }

  template<typename F>
  constexpr auto
  get_flatmap(type_s<stateful<F>>)
  {
    return [](auto f, auto mx) {
      return stateful{[=](auto s) {
        auto [xnew, snew] = run_state(s, mx);
        return run_state(car(snew), f(xnew));
      }};
    };
  }

} // end of namespace ruse::experimental
