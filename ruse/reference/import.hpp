#pragma once

//
// ... Standard header files
//
#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace ruse::reference {

  // cstdint
  using std::ptrdiff_t;

  // from type_traits
  using std::index_sequence;
  using std::is_aggregate_v;
  using std::is_const_v;
  using std::is_empty_v;
  using std::is_enum_v;
  using std::is_invocable_v;
  using std::is_reference_v;
  using std::is_same_v;
  using std::is_volatile_v;
  using std::make_index_sequence;
  using std::remove_cvref_t;
  using std::result_of_t;

  // from string_view
  using std::string_view;

  // from concepts
  using std::convertible_to;
  using std::same_as;

  // from stdexcept
  using std::logic_error;

  // from functional
  using std::greater;
  using std::invoke;

  // from ???
  using std::max;
} // end of namespace ruse::reference
