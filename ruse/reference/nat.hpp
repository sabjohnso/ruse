#pragma once

//
// ... ruse header files
//
#include <ruse/reference/base_types.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {
   template<integer N>
   concept Nonnegative = N >= 0;

   template<integer N>
   requires(Nonnegative<N>) struct Nat {
      static_assert(N >= 0);
      static constexpr integer value = N;

      template<integer M>
      friend constexpr bool
      operator<(Nat, Nat<M>) {
         return N < M;
      }

      template<integer M>
      friend constexpr bool
      operator<=(Nat, Nat<M>) {
         return N <= M;
      }

      template<integer M>
      friend constexpr bool
      operator==(Nat, Nat<M>) {
         return N == M;
      }
      template<integer M>
      friend constexpr bool
      operator!=(Nat, Nat<M>) {
         return N != M;
      }

      template<integer M>
      friend constexpr bool
      operator>(Nat, Nat<M>) {
         return N > M;
      }

      template<integer M>
      friend constexpr bool
      operator>=(Nat, Nat<M>) {
         return N >= M;
      }

      template<typename T>
      friend constexpr bool
      operator==(Nat, T) {
         return false;
      }

      constexpr operator bool() const { return N > 0; }
   };

   template<integer N>
   constexpr Nat<N> nat{};

   constexpr auto add1 = []<integer N>(Nat<N>) { return nat<N + 1>; };
   constexpr auto sub1 = []<integer N>(Nat<N>) {
      if constexpr(N == 0) {
         return nat<0>;
      } else {
         return nat<N - 1>;
      }
   };

} // end of namespace ruse::reference
