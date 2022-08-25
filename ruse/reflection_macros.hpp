#pragma once

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

#define RUSE_FORCE_SEMICOLON() static_assert(true)
#define RUSE_QUOTE_AUX(...) #__VA_ARGS__
#define RUSE_QUOTE(...) RUSE_QUOTE_AUX(__VA_ARGS__)
#define RUSE_FIELD(type, name) ::ruse::named<type, RUSE_QUOTE(name)> name

#define RUSE_ENUM_VALUES(name, ...)                                            \
  constexpr auto get_enum_values(::ruse::Type<name>)                           \
  {                                                                            \
    using enum name;                                                           \
    return std::array{__VA_ARGS__};                                            \
  }

#define RUSE_ENUM(name, type, ...)                                             \
  enum name : type                                                             \
  {                                                                            \
    __VA_ARGS__                                                                \
  };                                                                           \
  RUSE_ENUM_VALUES(name, __VA_ARGS__)                                          \
  RUSE_FORCE_SEMICOLON()
