#pragma once

#include <ruse/reference/pair.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {

  struct empty_vector_s
  {
    constexpr bool
    operator<=>(const empty_vector_s&) const noexcept = default;
  } constexpr empty_vector{};

  /**
   * @brief Return `true` if the input is a type proxy for an empty vector type.
   * Otherwise,  return `false`.
   */
  constexpr auto is_empty_vector_type = []<typename T>(T) {
    if constexpr (TypeProxy<T>) {
      return
        []<typename U>(Type<U>) { return same_as<U, empty_vector_s>; }(T{});
    } else {
      return false;
    }
  };

  /**
   * @brief A concept for empty vectors.
   */
  template<typename T>
  concept EmptyVector = is_empty_vector(type<T>);

  /**
   * @brief Return `true` if the input is an empty vector. Otherwise, return
   * `false`.
   */
  constexpr auto is_empty_vector = []<typename T>(T) { return EmptyVector<T>; };

  /**
   * @brief A value wrapper delimiting the head of a vector
   */
  template<typename T>
  struct vector_head_s
  {
    using value_type = T;
    value_type value;
  };

  /**
   * @brief Return `true` if the input is a type proxy for a vector head.
   * Otherwise, return `false`.
   */
  constexpr auto is_unitary_vector_type = []<typename T>(T) {};

  template<typename T>
  concept Vector = Pair<T>;

  constexpr auto vector_length_type = []<Vector T>(Type<T>) {
    constexpr auto recur = []<typename Head, typename Tail, integer N>(
                             auto recur, Type<pair<Head, Tail>>, Nat<N>) {
      if constexpr (Vector<Head>) {
        return recur(recur, type<Head>, nat<N + 1>);
      }
    } return recur(type<T>, nat<2>);
  };

  constexpr auto vector_length = []<Vector T>(T xs) {
    constexpr auto recur = []<typename Head, typename Tail, integer N>(
                             auto recur, pair<Head, Tail> xs, Nat<N>) {
      if constexpr (Vector<Head>) {
        return recur(recur, xs.first, nat<N + 1>);
      } else {
        return N;
      }
    };
    return recur(xs, nat<2>);
  };

  template<typename T>
  concept VectorOf =

} // end of namespace ruse::reference
