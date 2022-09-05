#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {

   /**
    * @brief A concept for types with a member typedef named
    * `value_type`
    */
   template<typename T>
   concept HasValueType = requires {
      typename T::value_type;
   };

   /**
    * @brief A concept tor `HasValueType` types with a data member named
    * `value` with  type `value_type`.
    */
   template<typename T>
   concept HasValue = HasValueType<T> && requires(T x) {
      { x.value } -> convertible_to<typename T::value_type>;
   };

   /**
    * @brief A concept for type that wrap a value.
    *
    * @details A value wrapper has a member typedef named `value_type`,
    * a data member with type `value_type` and has the same layout as
    * `value_type`. Value types are trivial monads and therefore are
    * also comonads.
    */
   template<typename T>
   concept ValueWrapper = HasValue<T> &&
                          sizeof(T) == sizeof(typename T::value_type);

   template<ValueWrapper T>
   constexpr auto
   get_pure(Type<T>) {
      return []<typename U>(U x) {
         return template_of_type(type<T>)(type<U>)(x);
      };
   }

   template<ValueWrapper T>
   constexpr auto
   get_fmap(Type<T>) {
      return [](auto f, ValueWrapper auto x) {
         constexpr auto pure = get_pure(type<T>);
         return pure(f(x.value));
      };
   }

   template<ValueWrapper T>
   constexpr auto
   get_flatten(Type<T>) {
      return [](ValueWrapper auto x) -> ValueWrapper auto{
         return x.value;
      };
   }

   template<ValueWrapper T>
   constexpr auto
   get_extract(Type<T>) {
      return [](ValueWrapper auto x) { return x.value; };
   }

   template<ValueWrapper T>
   constexpr auto
   get_extend(Type<T>) {
      return [](auto f, ValueWrapper auto wx) -> ValueWrapper auto{
         return template_of_type(type<T>)(type<decltype(f(wx))>)(f(wx));
      };
   }

} // end of namespace ruse::reference
