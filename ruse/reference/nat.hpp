#pragma once

//
// ... ruse header files
//
#include <ruse/reference/base_types.hpp>

namespace ruse::reference {
  template<integer N>
  concept Nonnegative = N >= 0;

  template<integer N>
  requires(Nonnegative<N>) struct Nat
  {
    static_assert(N >= 0);
    static constexpr integer value = N;

    template<integer M>
    friend constexpr bool
    operator<(Nat, Nat<M>)
    {
      return N < M;
    }

    template<integer M>
    friend constexpr bool
    operator<=(Nat, Nat<M>)
    {
      return N <= M;
    }

    template<integer M>
    friend constexpr bool
    operator==(Nat, Nat<M>)
    {
      return N == M;
    }
    template<integer M>
    friend constexpr bool
    operator!=(Nat, Nat<M>)
    {
      return N != M;
    }

    template<integer M>
    friend constexpr bool
    operator>(Nat, Nat<M>)
    {
      return N > M;
    }

    template<integer M>
    friend constexpr bool
    operator>=(Nat, Nat<M>)
    {
      return N >= M;
    }

    template<typename T>
    friend constexpr bool
    operator==(Nat, T)
    {
      return false;
    }
  };

  template<integer N>
  constexpr Nat<N> nat{};

} // end of namespace ruse::reference
