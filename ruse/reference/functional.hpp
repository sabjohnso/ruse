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

} // namespace ruse::reference
