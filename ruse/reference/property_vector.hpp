#pragma once

//
// ... ruse header files
//
#include <ruse/reference/tag.hpp>
#include <ruse/reference/vector.hpp>

namespace ruse::reference {

   /**
    * @brief Return true if the input is a type proxy for a type that is
    * a property vector.
    */
   constexpr auto is_property_vector_type = []<typename T>(T) {
      if constexpr(TypeProxy<T>) {
         return u(
           []<class U>(auto recur, Type<U>) {
              if constexpr(NonemptyVector<U>) {
                 if constexpr(is_tagged_type(
                                flatmap(back_type, type<U>))) {
                    return recur(
                      recur,
                      fmap(vector_head_type, type<U>));
                 } else {
                    return false;
                 }
              } else {
                 return true;
              }
           },
           T{});
      } else {
         return false;
      }
   };

   /**
    * @brief A concept for property vectors.
    */
   template<typename T>
   concept PropertyVector = is_property_vector_type(type<T>);

   /**
    * @brief Return `true` if the input is a property vector. Otherwise,
    * return `false`.
    */
   constexpr auto is_property_vector = []<class T>(T) {
      return PropertyVector<T>;
   };

   /**
    * @brief Return `true` if the property vector has a value with the
    * indicated tag. Otherwise, return `false`.
    */
   constexpr auto property_vector_has_tag =
     curry(nat<2>, []<Tag K, PropertyVector T>(K, T) {
        constexpr auto key_name_type = flatmap(name_type, type<K>);
        return u(
          [ = ]<PropertyVector U>(auto recur, Type<U>) {
             if constexpr(NonemptyVector<U>) {
                if constexpr(
                  key_name_type ==
                  flatmap(name_type, flatmap(back_type, type<U>))) {
                   return true;
                } else {
                   return recur(recur, fmap(pop_back, type<U>));
                }
             } else {
                return false;
             }
          },
          type<T>);
     });

   /**
    * @brief Return the tagged value from the property vector if an
    * element with the tag is present.  Otherwise, return `false`.
    */
   constexpr auto property_vector_ref =
     curry(nat<2>, []<Tag K>(K, PropertyVector auto xs) {
        constexpr auto key_name_type = flatmap(name_type, type<K>);
        return u(
          [ = ]<PropertyVector T>(auto recur, T xs) {
             if constexpr(NonemptyVector<T>) {
                if constexpr(
                  key_name_type ==
                  flatmap(name_type, flatmap(back_type, type<T>))) {
                   return back(xs);
                } else {
                   return recur(recur, pop_back(xs));
                }
             } else {
                return false;
             }
          },
          xs);
     });

} // end of namespace ruse::reference
