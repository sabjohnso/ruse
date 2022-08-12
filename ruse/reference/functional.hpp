#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/list.hpp>

namespace ruse::reference {

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

} // namespace ruse::reference
