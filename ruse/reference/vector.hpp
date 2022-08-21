#pragma once

#include <ruse/reference/functional.hpp>
#include <ruse/reference/pair.hpp>
#include <ruse/reference/singleton.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {

  // FIXME: Move this to a better location
  /**
   * @brief Return `true` if the input is a type proxy for a specialization of a
   * class template.  Otherwise, return `false`.
   */
  constexpr auto is_spec_type = case_lambda(
    []<template<class...> class K, class... Ts>(Type<K<Ts...>>) {
      return true;
    },
    constantly(false));

  /**
   * @brief A concept for template specialization proxies
   */
  template<class T>
  concept Spec = TypeProxy<T> && is_spec_type(type<T>);

  /**
   * @brief
   */
  constexpr auto is_spec = []<class T>(T) { return Spec<T>; };

  struct empty_vector_s : singleton<empty_vector_s>
  {
  } constexpr empty_vector{};

  /**
   * @brief Return `true` if the input is a type proxy for an empty vector type.
   * Otherwise,  return `false`.
   */
  constexpr auto is_empty_vector_type = []<typename T>(T) {
    if constexpr (TypeProxy<T>) {
      return []<typename U>(Type<U>) { return U{} == empty_vector; }(T{});
    } else {
      return false;
    }
  };

  /**
   * @brief A concept for empty vectors.
   */
  template<typename T>
  concept EmptyVector = is_empty_vector_type(type<T>);

  /**
   * @brief Return `true` if the input is an empty vector. Otherwise, return
   * `false`.
   */
  constexpr auto is_empty_vector = []<typename T>(T) { return EmptyVector<T>; };

  /**
   * @brief A value wrapper delimiting the head of a vector
   */
  template<typename T>
  struct vector_head
  {
    using head_type = T;
    head_type head;
  };

  /**
   * @brief Return `true` if the input is a type proxy for a vector head.
   * Otherwise, return `false`.
   */
  constexpr auto is_unitary_vector_type = case_lambda(
    []<class T>(Type<vector_head<T>>) { return true; },
    constantly(false));

  /**
   * @brief A concept for unitary vectors
   */
  template<typename T>
  concept UnitaryVector = is_unitary_vector_type(type<T>);

  /**
   * @brief Return `true` if the input is a unitary vector. Otherwise, return
   * `false`.
   */
  constexpr auto is_unitary_vector = []<typename T>(T) {
    return UnitaryVector<T>;
  };

  /**
   * @brief Return true if the input is a type proxy for a nonempty vector
   */
  constexpr auto is_nonempty_vector_type = [](auto x) {
    return u(
      case_lambda(
        []<class Head, class Tail>(auto recur, Type<pair<Head, Tail>>) {
          return recur(recur, type<Head>);
        },
        []<UnitaryVector T>(auto, Type<T>) { return true; },
        ternary_constantly(false)),
      x);
  };

  /**
   * @brief A concept for nonempty vectors
   */
  template<typename T>
  concept NonemptyVector = UnitaryVector<T> || is_nonempty_vector_type(type<T>);

  /**
   * @brief A concept for vectors
   */
  template<typename T>
  concept Vector = EmptyVector<T> || NonemptyVector<T>;

  /**
   * @brief Return the length of vectors of the input type.
   */
  constexpr auto vector_type_length = []<Vector T>(Type<T>) {
    return u(
      case_lambda(
        []<EmptyVector U, integer N>(auto, Type<U>, Nat<N>) { return N; },
        []<UnitaryVector U, integer N>(auto, Type<U>, Nat<N>) { return N + 1; },
        []<Vector Head, typename Tail, integer N>(
          auto recur, Type<pair<Head, Tail>>, Nat<N>) {
          return recur(recur, type<Head>, nat<N + 1>);
        }),
      type<T>,
      nat<0>);
  };

  /**
   * @brief Return the length of the input vector.
   */
  constexpr auto vector_length = []<Vector T>(T) {
    return vector_type_length(type<T>);
  };

  /**
   * @brief Return a vector like the input vector with an element pushed onto
   * the back
   */
  constexpr auto push_back = []<Vector T>(T xs, auto x) -> Vector auto
  {
    if constexpr (EmptyVector<T>) {
      return vector_head{.head = x};
    } else {
      return cons(xs, x);
    }
  };

  /**
   * @brief Return a vector with the input elements
   */
  constexpr auto vector = [](auto... xs) -> Vector auto
  {
    return u(
      case_lambda(
        [](auto, auto accum) { return accum; },
        [](auto recur, auto accum, auto x, auto... xs) {
          return recur(recur, push_back(accum, x), xs...);
        }),
      empty_vector,
      xs...);
  };

  /**
   * @brief Return true if the input is a type proxy for a homogeneous vector
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
  constexpr auto HomogeneousVector =
    NonemptyVector<T>&& is_homogeneous_vector_type(type<T>);

  /**
   * @brief Return `true` if the input is a homogeneous vector. Otherwise,
   * return `false`.
   */
  constexpr auto is_homogeneous_vector = []<class T>(T) {
    return HomogeneousVector<T>;
  };

  /**
   * @brief Return the last element of a nonempty vector.
   */
  constexpr auto back = curry(nat<1>, []<NonemptyVector T>(T xs) {
    if constexpr (UnitaryVector<T>) {
      return xs.head;
    } else {
      return xs.second;
    }
  });

  /**
   * @brief Return all but the last element of a vector.
   */
  constexpr auto pop_back = []<Vector T>(T xs) {
    if constexpr (EmptyVector<T> || UnitaryVector<T>) {
      return empty_vector;
    } else {
      return xs.first;
    }
  };

  /**
   * @brief Return a type proxy for the last element of the nonempty input
   * vector
   */
  constexpr auto back_type =
    curry(nat<1>, []<NonemptyVector T>(T xs) { return type_of(back(xs)); });

  template<class T>
  concept IntegerComparison = requires(T cmp, integer x, integer y)
  {
    {
      cmp(x, y)
      } -> same_as<bool>;
  };

  template<class T, auto cmp, integer N>
  concept VectorWithLength = Vector<T> && IntegerComparison<decltype(cmp)> &&
    cmp(vector_type_length(type<T>), N);

  constexpr auto vector_rref = curry(
    nat<2>,
    []<integer I, VectorWithLength<greater {}, I> T>(Nat<I>, T xs) {
      return u(
        []<integer J>(auto recur, Nat<J>, auto xs) {
          if constexpr (J == 0) {
            return back(xs);
          } else {
            return recur(recur, nat<J - 1>, pop_back(xs));
          }
        },
        nat<I>,
        xs);
    });

  constexpr auto vector_ref = curry(
    nat<2>,
    []<integer I, VectorWithLength<greater {}, I> T>(Nat<I>, T xs) {
      return vector_rref(nat<vector_type_length(type<T>) - I - 1>, xs);
    });

  template<NonemptyVector T>
  using BackType = typename decltype(flatmap(back_type, type<T>))::type;

  constexpr auto homogeneous_vector_rref =
    curry(nat<2>, []<NonemptyVector T>(auto index, T xs) {
      using R = BackType<T>;
      constexpr auto recur = []<class U>(auto recur, auto index, U xs) -> R {
        if constexpr (UnitaryVector<U>) {
          return back(xs);
        } else {
          return index ? recur(recur, index - 1, pop_back(xs)) : back(xs);
        }
      };
      return (index >= 0 && (index < vector_length(xs)))
               ? recur(recur, index, xs)
               : throw logic_error("invalid homogeneous vector index");
    });

  constexpr auto homogeneous_vector_ref =
    curry(nat<2>, []<NonemptyVector T>(auto index, T xs) {
      return homogeneous_vector_rref(vector_length(xs) - index - 1, xs);
    });

} // end of namespace ruse::reference
