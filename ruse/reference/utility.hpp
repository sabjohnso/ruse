#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/nat.hpp>


namespace ruse::reference
{
  /**
   * @brief U combinator introducing recursion.
   */
  constexpr auto u = [](auto f, auto ... xs){
    return f(f, xs ...);
  };

  template<typename T, auto M>
  concept Select = true;


  /**
   * @brief Curried function application
   */
  constexpr auto curry = []<integer N>(Nat<N>, auto f, auto ... xs){

    constexpr auto aux = [](auto recur, auto f, auto ... xs){
      return [=](auto ... ys){

        constexpr integer M  = sizeof...(xs) + sizeof... (ys);

        if constexpr (M == N){
          return f(xs ..., ys ...);

        } else if constexpr (M > N){
          return  []<std::size_t ... Indices>(index_sequence<Indices ...>){
            return [](auto f, Select<Indices> auto ... xs, auto ... ys){
              return f(xs ...)(ys ...);
            };
          }(make_index_sequence<N>())(f, xs ..., ys ...);

        } else {
          return recur(recur, f, xs..., ys...);

        }
      };
    };
    return u(aux, f, xs ...);
  };

} // end of namespace ruse::reference
