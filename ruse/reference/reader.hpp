#pragma once

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

//
// ... Standard header files
//
#include <concepts>
#include <utility>

namespace ruse::reference {

   /**
    * @bief A data structure template denoting a computation in an
    * environment
    */
   template<typename F>
   struct reader_s {
      constexpr reader_s(F input) : run(input) {}
      F run;
   };

   /**
    * @brief A concept for a reader
    */
   template<typename T>
   concept Reader = TemplateSpec<T, Template<reader_s>>;

   /**
    * @brief Return `true` if the input is a reader. Otherwise, return
    * `false`.
    */
   constexpr auto is_reader = []<typename T>(T) { return Reader<T>; };

   /**
    * @brief A concept for readers accepting the environment
    */
   template<typename T, typename E>
   concept ReaderOf = Reader<T> and requires(T mx, E e) {
      mx.run(e);
   };

   /**
    * @brief Run a computation in the specified environment
    */
   constexpr auto run_reader = curry(
     nat<2>,
     []<typename E>(E e, ReaderOf<E> auto mx) { return mx.run(e); });

   /**
    * @brief Inject a value into a computation in an environment
    */
   constexpr auto make_reader = [](auto x) -> Reader auto{
      return reader_s{[ = ](auto) { return x; }};
   };

   /**
    * @brief Retreive the environment
    */
   constexpr auto ask = reader_s{identity};

   /**
    * @brief Retrieve a selection from the environment
    */
   constexpr auto asks = cute(fmap, ask);

   /**
    * @brief Run a computation in a local environment.
    */
   constexpr auto local = curry(nat<2>, [](auto f, auto mx) {
      return reader_s{[ = ](auto e) { return run_reader(f(e), mx); }};
   });

   template<typename F>
   constexpr auto
   get_pure(Type<reader_s<F>>) {
      return []<typename T>(T x) {
         return reader_s{[ = ](auto) { return x; }};
      };
   }

   template<typename F>
   constexpr auto
   get_flatmap(Type<reader_s<F>>) {
      return [](auto f, auto mx) {
         return reader_s{[ = ](auto e) {
            return run_reader(e, f(run_reader(e, mx)));
         }};
      };
   }

} // namespace ruse::reference
