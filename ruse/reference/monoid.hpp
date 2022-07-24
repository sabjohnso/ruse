#pragma once

//
// ... ruse header files
//
#include <ruse/protocol_traits.hpp>

namespace ruse::reference{

  constexpr auto mappend = curry(nat<2>, []<Monoid T>(T mx, Monoid auto my){
      return monoid_traits<T>::op(mx, my);
    });

  template<AdditiveMonoid T, AdditiveMonoid U>
  constexpr auto
  operator+(T x, U y)
  {
    return monoid_traits<T>::op(x, y);
  };

  template<AdditiveMonoid T>
  constexpr auto
  operator+(T x, zero_s)
  {
    return monoid_traits<T>::op(x, monoid_traits<T>::id);
  };

  template<AdditiveMonoid T>
  constexpr auto
  operator+(zero_s, T x)
  {
    return monoid_traits<T>::op(monoid_traits<T>::id, x);
  };


  template<MultiplicativeMonoid T, MultiplicativeMonoid U>
  constexpr auto
  operator *(T x, U y){
    return monoid_traits<T>::op(x, y);
  }

  template<MultiplicativeMonoid T>
  constexpr auto
  operator*(T x, one_s)
  {
    return monoid_traits<T>::op(x, monoid_traits<T>::id);
  };

  template<MultiplicativeMonoid T>
  constexpr auto
  operator*(one_s, T x)
  {
    return monoid_traits<T>::op(monoid_traits<T>::id, x);
  };


} // end of namespace ruse::reference
