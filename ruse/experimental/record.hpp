#pragma once

//
// ... ruse header files
//
#include "ruse/reference/import.hpp"
#include <ruse/reference/list.hpp>
#include <ruse/reference/string.hpp>

namespace ruse::experimental {

  using namespace ruse::reference;

  template<HoistedString T>
  struct record_name
  {};

  struct is_record_name_Type
  {
    template<HoistedString T>
    constexpr bool
    operator()(Type<record_name<T>>) const
    {
      return true;
    }
    constexpr bool operator()(auto) const { return false; }
  } constexpr is_record_name_type{};

  template<typename T>
  concept RecordName = is_record_name_type(type<T>);

  constexpr auto is_record_name = []<typename T>(T) { return RecordName<T>; };

  template<fixed_string str>
  constexpr auto operator""_record_name()
  {
    return []<auto... Indices>(index_sequence<Indices...>)
    {
      return record_name<hoisted_list<str.value[Indices]...>>{};
    }
    (make_index_sequence<str.extent - 1>());
  };

  template<typename T>
  struct field_decl_list
  {
    static constexpr auto values = T{};
  };

  template<HoistedString T>
  struct field_name
  {
    template<typename U>
    constexpr auto operator,(Type<U>) const
    {
      return list(field_name{}, type<U>);
    }

    template<HoistedString U>
    constexpr auto operator,(field_name<U>)
    {
      return list(field_name{}, field_name<U>{});
    }
  };

  template<fixed_string str>
  constexpr auto operator""_field()
  {
    return []<auto... Indices>(index_sequence<Indices...>)
    {
      return field_name<hoisted_list<str.value[Indices]...>>{};
    }
    (make_index_sequence<str.extent>());
  };

  struct is_field_name_Type
  {
    template<typename T>
    constexpr bool
    operator()(Type<field_name<T>>) const
    {
      return true;
    }

    constexpr bool operator()(auto) const { return false; }

  } constexpr is_field_name_type{};

  template<typename T>
  concept FieldName = is_field_name_type(type<T>);
  constexpr auto is_field_name = []<typename T>(T) { return FieldName<T>; };

  /**
   * @brief A concept for typed field declarations
   */
  template<typename T>
  concept TypedFieldDecl = BinaryList<T> &&
    is_field_name_type(list_type_ref(nat<0>, type<T>)) &&
    is_type_type(list_type_ref(nat<1>, type<T>));

  /**
   * @brief Return `true` if the input is a typed field declaration.  Otherwise,
   * return `false`.
   *
   * @details Typed field declarations are two-elements lists where the first
   * element is the field name and the second element is the field type.
   */
  constexpr auto is_typed_field_decl = []<typename T>(T) {
    return TypedFieldDecl<T>;
  };

  /**
   * @brief A concept for field declarations.
   *
   * @details There are two forms of field declarations. The first form is just
   * a field name and the field type will be parameteric.  The second form is a
   * typed field declaration which specifies field's type.
   */
  template<typename T>
  concept FieldDecl = FieldName<T> || TypedFieldDecl<T>;

  /**
   * @brief Return `true` if the input is a field declaration. Otherwise, return
   * `false`.
   */
  constexpr auto is_field_decl = []<typename T>(T) { return FieldDecl<T>; };

  /**
   * @brief Return `true` if the input is a field declaration list.  Otherwise
   * return `false`.
   */
  struct is_field_decl_list_Type
  {

    constexpr bool
    operator()(Type<nothing_s>) const
    {
      return true;
    }

    template<typename Head, typename Tail>
    constexpr bool
    operator()(Type<pair<Head, Tail>>) const
    {

      if constexpr (FieldDecl<Head>) {
        return (*this)(type<Tail>);
      } else {
        return false;
      }
    }
    constexpr bool operator()(auto) const { return false; }
  } constexpr is_field_decl_list_type{};

  /**
   * @brief A concept for field declaration lists.
   */
  template<typename T>
  concept FieldDeclList = is_field_decl_list_type(type<T>);

  /**
   * @brief A concept for record definitions
   */
  template<typename T>
  concept RecordDefinition = BinaryList<T> &&
    is_record_name_type(list_type_ref(nat<0>, type<T>));

  template<typename T>
  concept Pair_ = requires(T x)
  {
    []<typename U, typename V>(pair<U, V>) {}(x);
  };

  template<template<typename...> class K>
  struct conceptify_template
  {
    template<typename... Ts>
    constexpr auto
    operator()(Type<K<Ts...>>)
    {
      return true;
    }
    constexpr auto operator()(auto) const { return false; }
  };

} // end of namespace ruse::experimental
