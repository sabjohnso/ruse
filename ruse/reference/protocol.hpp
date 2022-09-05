#pragma once

//
// ... ruse header files
//
#include <ruse/reference/pure.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {

   template<typename T>
   concept HasFmap = ! same_as<void, decltype(ruse::get_fmap(type<T>))>;

   template<typename T>
   concept HasFapply =
     ! same_as<void, decltype(ruse::get_fapply(type<T>))>;

   template<typename T>
   concept HasPure = ! same_as<void, decltype(ruse::get_pure(type<T>))>;

   template<typename T>
   concept HasFlatmap =
     ! same_as<void, decltype(ruse::get_flatmap(type<T>))>;

   template<typename T>
   concept HasFlatten =
     ! same_as<void, decltype(ruse::get_flatten(type<T>))>;

   template<typename T>
   concept HasExtract =
     ! same_as<void, decltype(ruse::get_extract(type<T>))>;

   template<typename T>
   concept HasExtend =
     ! same_as<void, decltype(ruse::get_extend(type<T>))>;

   template<typename T>
   concept HasDuplicate =
     ! same_as<void, decltype(ruse::get_duplicate(type<T>))>;

   template<typename T>
   concept HasZapply =
     ! same_as<void, decltype(ruse::get_zapply(type<T>))>;

   //
   // ... Monads
   //

   template<typename T>
   concept Monad = HasPure<T> &&
     (HasFlatmap<T> || (HasFlatten<T> && HasFmap<T>));

   constexpr auto flatmap = curry(
     nat<2>,
     []<Monad T>(auto f, T mx) -> Monad auto{
        if constexpr(HasFlatmap<T>) {
           constexpr auto flatmap = get_flatmap(type<T>);
           return flatmap(
             [ = ](auto x) {
                if constexpr(Pure<decltype(f(x))>) {
                   return get_pure(type_of(mx))(f(x).value);
                } else {
                   return f(x);
                }
             },
             mx);
        } else {
           constexpr auto flatten = get_flatten(type<T>);
           constexpr auto fmap = get_fmap(type<T>);
           return flatten(fmap(f, mx));
        }
     });

   constexpr auto letm = []<Monad T>(T mx, auto f) -> Monad auto{
      return flatmap(f, mx);
   };

   constexpr auto beginm = [](auto expr, auto... exprs) {
      constexpr auto recur = [](auto recur, auto expr, auto... exprs) {
         if constexpr(sizeof...(exprs) == 0) {
            return expr;
         } else {
            return letm(expr, [ = ](auto) {
               return recur(recur, exprs...);
            });
         }
      };
      return recur(recur, expr, exprs...);
   };

   constexpr auto flatten = curry(
     nat<1>,
     []<Monad T>(T mmx) -> Monad auto{
        if constexpr(HasFlatten<T>) {
           using ruse::get_flatten;
           return get_flatten(type<T>)(mmx);
        } else {
           return flatmap(identity, mmx);
        }
     });

   //
   // ... Applicative Functors
   //

   template<typename T>
   concept Applicative = Monad<T> ||(HasPure<T>&& HasFapply<T>);

   constexpr auto fapply = curry(
     nat<2>,
     []<Applicative G, Applicative T>(G mg, T mx) -> Applicative auto{
        constexpr auto aux =
          []<Applicative F>(F mf, Applicative auto mx) {
             if constexpr(HasFapply<F>) {
                using ruse::get_fapply;
                return get_fapply(type<F>)(mf, mx);
             } else {
                using ruse::get_pure;
                constexpr auto pure = get_pure(type<F>);
                // clang-format off
          return
            letm(mf, [=](auto f) { return
            letm(mx, [=](auto x) { return
                  pure(f(x)); });});
                // clang-format on
             }
          };
        if constexpr(
          (Pure<G> && Pure<T>) || ((! Pure<G>)&&(! Pure<T>))) {
           return aux(mg, mx);
        } else if constexpr(Pure<G>) {
           return aux(get_pure(type_of(mx))(mg.value), mx);
        } else {
           return aux(mg, get_pure(type_of(mg))(extract(mx)));
        }
     });

   //
   // ... Functors
   //
   template<typename T>
   concept Functor = Applicative<T> || HasFmap<T>;

   constexpr auto fmap = curry(
     nat<2>,
     []<Functor T>(auto f, T mx) -> Functor auto{
        if constexpr(HasFmap<T>) {
           using ruse::get_fmap;
           return get_fmap(type<T>)(f, mx);
        } else {
           static_assert(HasPure<T>);
           constexpr auto pure = ruse::get_pure(type<T>);
           return fapply(pure(f), mx);
        }
     });

   constexpr auto letf = []<Functor T>(T mx, auto f) -> Functor auto{
      return fmap(f, mx);
   };

   //
   // ... Comonads
   //
   template<typename T>
   concept Comonad = HasExtract<T> &&
     (HasExtend<T> || (HasDuplicate<T> && HasFmap<T>));

   template<typename T>
   concept ComonadApply = Comonad<T> && HasZapply<T>;

   constexpr auto extract = curry(nat<1>, []<Comonad T>(T wx) {
      constexpr auto extract = ruse::get_extract(type<T>);
      return extract(wx);
   });

   constexpr auto duplicate = curry(nat<1>, []<Comonad T>(T wx) {
      if constexpr(HasDuplicate<T>) {
         constexpr auto duplicate = ruse::get_duplicate(type<T>);
         return duplicate(wx);
      } else {
         constexpr auto extend = ruse::get_extend(type<T>);
         return extend(identity, wx);
      }
   });

   constexpr auto extend = curry(nat<2>, []<Comonad T>(auto f, T wx) {
      constexpr auto extend = ruse::get_extend(type<T>);
      return extend(f, wx);
   });

   constexpr auto zapply =
     curry(nat<2>, []<ComonadApply F>(F wf, ComonadApply auto wx) {
        constexpr auto zapply = ruse::get_zapply(type<F>);
        return zapply(wf, wx);
     });

} // end of namespace ruse::reference
