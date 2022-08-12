#pragma once

//
// ... Standard header files
//
#include <concepts>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace ruse::reference {

  // cstdint
  using std::ptrdiff_t;

  // from type_traits
  using std::index_sequence;
  using std::is_empty_v;
  using std::is_invocable_v;
  using std::is_same_v;
  using std::make_index_sequence;
  using std::remove_cvref_t;
  using std::result_of_t;

  // from concepts
  using std::convertible_to;
  using std::same_as;

  // from stdexcept
  using std::logic_error;

  // from functional
  using std::invoke;

} // end of namespace ruse::reference
