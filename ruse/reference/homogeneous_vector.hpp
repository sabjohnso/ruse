#pragma once

//
// ... ruse header files
//
#include <ruse/reference/vector.hpp>

namespace ruse::reference {

   /**
    * @brief Return true if the input is a type proxy for a homogeneous
    * vector
    */
   constexpr auto is_homogeneous_vector_type = case_lambda(
     []<UnitaryVector T>(Type<T>) { return true; },
     []<Vector Head, class U>(Type<pair<Head, U>>) {
        return u(
          case_lambda(
            [](auto, Type<vector_head<U>>) { return true; },
            []<Vector NewHead>(auto recur, Type<pair<NewHead, U>>) {
               return recur(recur, type<NewHead>);
            },
            ternary_constantly(false)),
          type<Head>);
     },
     constantly(false));

   /**
    * @brief A concept for homogeneous vectors
    */
   template<class T>
   concept HomogeneousVector = NonemptyVector<T> &&
     is_homogeneous_vector_type(type<T>);

   /**
    * @brief Return `true` if the input is a homogeneous vector.
    * Otherwise, return `false`.
    */
   constexpr auto is_homogeneous_vector = []<class T>(T) {
      return HomogeneousVector<T>;
   };

   constexpr auto homogeneous_vector_rref =
     curry(nat<2>, []<NonemptyVector T>(auto index, T xs) {
        using R = BackType<T>;
        constexpr auto recur =
          []<class U>(auto recur, auto index, U xs) -> R {
           if constexpr(UnitaryVector<U>) {
              return back(xs);
           } else {
              return index ? recur(recur, index - 1, pop_back(xs))
                           : back(xs);
           }
        };
        return (index >= 0 && (index < vector_length(xs)))
               ? recur(recur, index, xs)
               : throw logic_error("invalid homogeneous vector index");
     });

   /**
    * @brief Return the requested element from a homogeneous vector.
    */
   constexpr auto homogeneous_vector_ref =
     curry(nat<2>, []<NonemptyVector T>(auto index, T xs) {
        return homogeneous_vector_rref(
          vector_length(xs) - index - 1,
          xs);
     });

} // end of namespace ruse::reference
