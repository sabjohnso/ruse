#pragma once

//
// ... Standard header files
//
#include <cstdint>
#include <type_traits>
#include <concepts>

namespace ruse::reference {

  // cstdint
  using std::ptrdiff_t;

  // from type_traits
  using std::remove_cvref_t;
  using std::is_same_v;
  using std::is_empty_v;
  using std::index_sequence;
  using std::make_index_sequence;

  // from concepts
  using std::convertible_to;

} // end of namespace ruse::reference
