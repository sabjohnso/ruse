#pragma once

//
// ... external header files
//
#include <boost/preprocessor.hpp>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

#define RUSE_FORCE_SEMICOLON() static_assert(true)
#define RUSE_QUOTE_AUX(...) #__VA_ARGS__
#define RUSE_QUOTE(...) RUSE_QUOTE_AUX(__VA_ARGS__)
#define RUSE_FIELD(type, name)                                         \
   ::ruse::named<type, RUSE_QUOTE(name)> name

#define RUSE_APPLY_AUX(f, x) f(x)
#define RUSE_APPLY(f, x) f(x)

#define _xRUSE_POP_AUX(...) __VA_ARGS__
#define _xRUSE_POP(tuple) _xRUSE_POP_AUX tuple

#define _xRUSE_EQUAL(decl)                                             \
   BOOST_PP_TUPLE_ELEM(0, decl) = BOOST_PP_TUPLE_ELEM(1, decl)

#define _xRUSE_ENUM_VALUE_DECL_OP(ignored1, ignored2, decl)            \
   BOOST_PP_IF(                                                        \
     BOOST_PP_DEC(BOOST_PP_TUPLE_SIZE(decl)),                          \
     _xRUSE_EQUAL(decl),                                               \
     BOOST_PP_TUPLE_ELEM(0, decl))

#define _xRUSE_ENUM_VALUE_NAME_OP(ignored1, ignored2, decl)            \
   BOOST_PP_TUPLE_ELEM(0, decl)

// clang-format off
#define RUSE_ENUM_VALUES(name, ...)                                     \
  constexpr auto get_enum_values(::ruse::Type<name>)                    \
  {                                                                     \
    using enum name;                                                    \
    return ::ruse::list(                                                \
      _xRUSE_POP(BOOST_PP_SEQ_TO_TUPLE(                                 \
        BOOST_PP_SEQ_TRANSFORM(                                         \
          _xRUSE_ENUM_VALUE_NAME_OP,                                    \
          BOOST_PP_NIL,                                                 \
          BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))));                    \
  }
// clang-format on
#define RUSE_ENUM(name, type, ...)                                     \
   enum class name : type {                                            \
      _xRUSE_POP(BOOST_PP_SEQ_TO_TUPLE(BOOST_PP_SEQ_TRANSFORM(         \
        _xRUSE_ENUM_VALUE_DECL_OP,                                     \
        BOOST_PP_NIL,                                                  \
        BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))))                       \
   };                                                                  \
   RUSE_ENUM_VALUES(name, __VA_ARGS__)                                 \
   RUSE_FORCE_SEMICOLON()
