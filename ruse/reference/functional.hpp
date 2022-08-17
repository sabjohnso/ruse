#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/list.hpp>

namespace ruse::reference {

  /**
   * @brief Return a function that will return the result of the first of the
   * input functions that is invocable with the arguments applied to the
   * arguments.
   */
  constexpr auto case_lambda = [](auto... gs) {
    return [](auto fs) {
      return [=]<typename... Ts>(Ts... args)
      {
        return u(
          [=]<typename Fs>(auto recur, Fs fs) {
            if constexpr (Something<Fs>) {
              return [=]<typename F>(F f, auto fs) {
                if constexpr (is_invocable_v<F, Ts...>) {
                  return invoke(f, args...);
                } else {
                  return recur(recur, fs);
                }
              }(car(fs), cdr(fs));
            } else {
              static_assert(Something<Fs>, "Bad arguments");
            }
          },
          fs);
      };
    }(list(gs...));
  };

  template<List ArgList>
  constexpr auto
  apply_aux(auto f, ArgList arg_list, auto... unpacked_args)
  {
    if constexpr (Nothing<ArgList>) {
      return f(unpacked_args...);
    } else {
      return apply_aux(f, cdr(arg_list), unpacked_args..., car(arg_list));
    }
  }

  /**
   * @brief Apply a function to input arguments where the last of the input
   * arguments is a list that will be spliced into the other arguments.
   */
  constexpr auto apply = [](auto f, auto arg, auto... args) {
    return apply_aux(f, list_(arg, args...));
  };

  /**
   * @brief Return a function that will accept a function that is invocable over
   * the input arguments and return the result of that application.
   */
  constexpr auto apply_to = []<typename... Ts>(Ts... args)
  {
    return [=]<Invocable<Ts...> F>(F f) { return f(args...); };
  };

  /**
   * @brief Return a function that will accept a function that is invocable over
   * the input list of arguments and return the result of that application.
   */
  constexpr auto apply_to_list = [](List auto arg_list) {
    return apply(apply_to, arg_list);
  };

  constexpr auto cut = [](auto f, auto... xs) {
    return [=](auto... ys) { return f(xs..., ys...); };
  };

  constexpr auto cute = [](auto f, auto... ys) {
    return [=](auto... xs) { return f(xs..., ys...); };
  };

} // namespace ruse::reference
