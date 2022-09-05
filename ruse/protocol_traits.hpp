#pragma once

//
// ... ruse header files
//
#include <ruse/reference/type.hpp>

namespace ruse {

   namespace details {

      template<typename T>
      void
      get_fmap(T) {}

      template<typename T>
      void
      get_pure(T) {}

      template<typename T>
      void
      get_fapply(T) {}

      template<typename T>
      void
      get_flatmap(T) {}

      template<typename T>
      void
      get_flatten(T) {}

      template<typename T>
      void
      get_extract(T) {}

      template<typename T>
      void
      get_extend(T) {}

      template<typename T>
      void
      get_duplicate(T) {}

      template<typename T>
      void
      get_zapply(T) {}

      template<typename T>
      constexpr T static_const{};

      struct get_fmap_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_fmap(t);
         }
      };

      struct get_pure_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_pure(t);
         }
      };

      struct get_fapply_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_fapply(t);
         }
      };

      struct get_flatmap_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_flatmap(t);
         }
      };

      struct get_flatten_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_flatten(t);
         }
      };

      struct get_extract_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_extract(t);
         }
      };

      struct get_extend_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_extend(t);
         }
      };

      struct get_duplicate_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_duplicate(t);
         }
      };

      struct get_zapply_fn {
         template<template<typename> class K, typename T>
         constexpr auto
         operator()(K<T> t) const {
            return get_zapply(t);
         }
      };
   } // end of namespace details

   constexpr const auto& get_fmap =
     details::static_const<details::get_fmap_fn>;
   constexpr const auto& get_pure =
     details::static_const<details::get_pure_fn>;
   constexpr const auto& get_fapply =
     details::static_const<details::get_fapply_fn>;
   constexpr const auto& get_flatmap =
     details::static_const<details::get_flatmap_fn>;
   constexpr const auto& get_flatten =
     details::static_const<details::get_flatten_fn>;
   constexpr const auto& get_extract =
     details::static_const<details::get_extract_fn>;
   constexpr const auto& get_extend =
     details::static_const<details::get_extend_fn>;
   constexpr const auto& get_duplicate =
     details::static_const<details::get_duplicate_fn>;
   constexpr const auto& get_zapply =
     details::static_const<details::get_zapply_fn>;

   //
   // ...  monoids
   //
   template<typename T>
   struct monoid_traits;

   template<typename T>
   concept HasOp = requires {
      monoid_traits<T>::op;
   };

   template<typename T>
   concept HasId = requires {
      monoid_traits<T>::id;
   };

   template<typename T>
   concept Monoid = HasOp<T> && HasId<T>;

   struct additive {};

   struct zero_s {
   } constexpr zero{};

   template<typename T>
   concept AdditiveMonoid = Monoid<T> &&
     std::same_as<typename monoid_traits<T>::color, additive>;

   struct multiplicative {};

   struct one_s {
   } constexpr one{};

   template<typename T>
   concept MultiplicativeMonoid = Monoid<T> &&
     std::same_as<typename monoid_traits<T>::color, multiplicative>;

} // end of namespace ruse
