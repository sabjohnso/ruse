#pragma once

//
// ... ruse header files
//
#include "ruse/reference/import.hpp"
#include <ruse/reference/list.hpp>
#include <ruse/reference/string.hpp>

namespace ruse::experimental {

   using namespace ruse::reference;

   template<typename T>
   struct field_decl_list {
      static constexpr auto values = T{};
   };

   template<HoistedString T>
   struct field_name {
      template<typename U>
      constexpr auto
      operator()(Type<U>) const {
         return list(field_name{}, type<U>);
      }

      template<typename U>
    constexpr auto operator,(Type<U>) const
    {
         return list(field_name{}, type<U>);
      }

      friend constexpr bool
      operator==(field_name, field_name) {
         return true;
      }

      template<typename U>
      friend constexpr bool
      operator==(field_name, U) {
         return false;
      }

      template<typename U>
      friend constexpr bool
      operator!=(field_name, U x) {
         return ! (field_name{}, x);
      }
   };

   template<fixed_string str>
   constexpr auto operator""_field() {
      return []<auto... Indices>(index_sequence<Indices...>) {
         return field_name<hoisted_list<str.value[ Indices ]...>>{};
      }
      (make_index_sequence<str.extent - 1>());
   };

   struct is_field_name_Type {
      template<typename T>
      constexpr bool
      operator()(Type<field_name<T>>) const {
         return true;
      }

      constexpr bool
      operator()(auto) const {
         return false;
      }

   } constexpr is_field_name_type{};

   template<typename T>
   concept FieldName = is_field_name_type(type<T>);
   constexpr auto is_field_name = []<typename T>(T) {
      return FieldName<T>;
   };

   /**
    * @brief A concept for typed field declarations
    */
   template<typename T>
   concept TypedFieldDecl = BinaryList<T> &&
     is_field_name_type(list_type_ref(nat<0>, type<T>)) &&
     is_type_type(list_type_ref(nat<1>, type<T>));

   /**
    * @brief Return `true` if the input is a typed field declaration.
    * Otherwise, return `false`.
    *
    * @details Typed field declarations are two-elements lists where the
    * first element is the field name and the second element is the
    * field type.
    */
   constexpr auto is_typed_field_decl = []<typename T>(T) {
      return TypedFieldDecl<T>;
   };

   /**
    * @brief A concept for field declarations.
    *
    * @details There are two forms of field declarations. The first form
    * is just a field name and the field type will be parameteric.  The
    * second form is a typed field declaration which specifies field's
    * type.
    */
   template<typename T>
   concept FieldDecl = FieldName<T> || TypedFieldDecl<T>;

   /**
    * @brief Return `true` if the input is a field declaration.
    * Otherwise, return `false`.
    */
   constexpr auto is_field_decl = []<typename T>(T) {
      return FieldDecl<T>;
   };

   template<HoistedString T>
   struct record_name {
      constexpr auto
      operator()(FieldDecl auto... decls) const {
         return list(record_name{}, list(decls...));
      }

      friend constexpr bool
      operator==(record_name, record_name) {
         return true;
      }

      template<typename U>
      friend constexpr bool
      operator==(record_name, U) {
         return false;
      }

      template<typename U>
      friend constexpr bool
      operator!=(record_name, U x) {
         return ! (record_name{} == x);
      }
   };

   struct is_record_name_type_s {
      template<HoistedString T>
      constexpr bool
      operator()(Type<record_name<T>>) const {
         return true;
      }
      constexpr bool
      operator()(auto) const {
         return false;
      }
   } constexpr is_record_name_type{};

   template<typename T>
   concept RecordName = is_record_name_type(type<T>);

   constexpr auto is_record_name = []<typename T>(T) {
      return RecordName<T>;
   };

   template<fixed_string str>
   constexpr auto operator""_record_name() {
      return []<auto... Indices>(index_sequence<Indices...>) {
         return record_name<hoisted_list<str.value[ Indices ]...>>{};
      }
      (make_index_sequence<str.extent - 1>());
   };

   /**
    * @brief Return `true` if the input is a field declaration list.
    * Otherwise return `false`.
    */
   struct is_field_decl_list_Type {

      constexpr bool
      operator()(Type<nothing_s>) const {
         return true;
      }

      template<typename Head, typename Tail>
      constexpr bool
      operator()(Type<pair<Head, Tail>>) const {

         if constexpr(FieldDecl<Head>) {
            return (*this)(type<Tail>);
         } else {
            return false;
         }
      }
      constexpr bool
      operator()(auto) const {
         return false;
      }
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
   concept Pair_ = requires(T x) {
      []<typename U, typename V>(pair<U, V>) {}(x);
   };

   template<template<typename...> class K>
   struct conceptify_template {
      template<typename... Ts>
      constexpr auto
      operator()(Type<K<Ts...>>) {
         return true;
      }
      constexpr auto
      operator()(auto) const {
         return false;
      }
   };

   constexpr auto definition_record_name =
     [](RecordDefinition auto def) { return list_ref(nat<0>, def); };

   constexpr auto definition_field_decl_list =
     [](RecordDefinition auto def) { return list_ref(nat<1>, def); };

   // return the type tag for a record definition
   constexpr auto record_definition_type_tag =
     [](RecordDefinition auto def) {
        return template_swap(tmplt<tag>, definition_record_name(def));
     };

   constexpr auto field_decl_tag = []<FieldDecl T>(T decl) {
      if constexpr(TypedFieldDecl<T>) {
         return template_swap(tmplt<tag>, list_ref(nat<0>, decl));
      } else {
         return template_swap(tmplt<tag>, decl);
      }
   };

   constexpr auto make_record =
     [](RecordDefinition auto definition, auto... field_values) {
        constexpr auto name = record_definition_type_tag(definition);
        return [=]<auto... I>(index_sequence<I...>) {
           return name(list(field_decl_tag(
             list_ref(nat<I>, definition_field_decl_list(definition)))(
             field_values)...));
        }
        (make_index_sequence<sizeof...(field_values)>());
     };

   template<typename T>
   std::ostream&
   operator<<(std::ostream& os, const record_name<T>&) {
      return os << "\"" << T{} << "\"_record";
   }

   template<typename T>
   std::ostream&
   operator<<(std::ostream& os, const field_name<T>&) {
      return os << "\"" << T{} << "\"_field";
   }

} // end of namespace ruse::experimental
