#pragma once

//
// ... ruse header files
//
#include <ruse/reference/type.hpp>

namespace ruse::reference
{

  template<template<typename ... > class K, typename ... Ts>
  struct template_s
  {
    template<typename T>
    constexpr auto
    operator()(type_s<T>) const {
      return type<K<T, Ts ...>>;
    }
  };

  template< template<typename ...> class K, typename ... Ts>
  constexpr template_s<K,Ts...> tmplt{};


  constexpr auto template_of = []<template<typename ...> class K, typename T, typename ... Ts>
    (K<T,Ts ...>){
    return tmplt<K, Ts ...>;
  };

  constexpr auto template_of_type = []<template<typename ...> class K, typename T, typename ... Ts>
    (type_s<K<T,Ts ...>>){
    return tmplt<K, Ts ...>;
  };



} // end of namespace ruse::reference
