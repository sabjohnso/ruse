#pragma once

//
// ... Standard header files
//
#include <string>
#include <iostream>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>


namespace ruse
{


  template<List T>
  std::ostream&
  operator <<(std::ostream& os, T xs){
    constexpr auto aux = []<List U>(auto recur, std::ostream& os, U xs) -> std::ostream&{
      if constexpr(Pair<U>){
        return recur(recur, os << "," << head(xs), tail(xs));
      } else {
        return os << ")";
      }
    };
    if constexpr (Nothing<T>){
      return os << "nothing";
    } else {
      return aux(aux, os << "list(" << head(xs), tail(xs));
    }
  }

} // end of namespace ruse
