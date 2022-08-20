#pragma once

//
// ... ruse header files
//
#include <ruse/reference/pair.hpp>

namespace ruse::reference {

  constexpr auto is_tuple_type = []<typename T>(T) {
    return Pair<T>;
  };

  template<typename T>
  concept Tuple =

} // end of namespace ruse::reference
