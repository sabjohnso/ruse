#pragma once

//
// ... ruse header files
//
#include <ruse/reference/type.hpp>
#include <ruse/reference/nothing.hpp>
#include <ruse/reference/pair.hpp>
#include <ruse/reference/utility.hpp>

namespace ruse::reference
{

  /**
   * @brief Return `true` if the input is a type proxy for a list type.
   * Otherwise, return `false`.
   */
  constexpr auto is_list_type =  []<typename T>(T){
    constexpr auto aux = []<typename U>(auto recur, type_s<U>){
      if constexpr (Pair<U>){
        return recur(recur, type<typename U::second_type>);
      } else if constexpr (Nothing<U>){
        return true;
      } else {
        return false;
      }
    };

    if constexpr (Type<T>){
      return u(aux, T{});
    } else {
      return false;
    }
  };

  /**
   * @brief A concept for lists
   */
  template<typename T>
  concept List = is_list_type(type<T>);

  /**
   * @brief Return `true` if the input is a list.  Otherwise, return `false`.
   */
  constexpr auto is_list = []<typename T>(T){
    return List<T>;
  };

  /**
   * @brief A concept for nonempty lists
   */
  template<typename T>
  concept NonemptyList =  List<T> and Pair<T>;

  /**
   * @brief Return `true` if the input is a nonempty list. Otherwise, return `false`.
   */
  constexpr auto is_nonempty_list = []<typename T>(T){
    return NonemptyList<T>;
  };

  /**
   * @brief Return the number of elements in instances of the list type
   * referenced by the input type proxy.
   */
  constexpr auto length_type = []<List T>(type_s<T>){
    constexpr auto aux = []<typename U>(auto recur, auto accum, type_s<U>){
      if constexpr (Pair<U>){
        return recur(recur, accum+1, type<typename U::second_type>);
      } else {
        return accum;
      }
    };
    return u(aux, 0, type<T>);
  };

  /**
   * @brief A concept for unitary lists
   */
  template<typename T>
  concept UnitaryList = NonemptyList<T> and length_type(type<T>) == 1;

  /**
   * @brief Return `true` if the input is a unitary list.  Otherwise, return `false`.
   */
  constexpr auto is_unitary_list = []<typename T>(T){
    return UnitaryList<T>;
  };

  /**
   * @brief Return a type proxy for the head of the input type proxy.
   */
  constexpr auto head_type = []<NonemptyList T>(type_s<T>){
    return type<typename T::first_type>;
  };

  /**
   * @brief Return a type proxy for the tail of the input type proxy.
   */
  constexpr auto tail_type = []<List T>(type_s<T>){
    if constexpr (Pair<T>){
      return type<typename T::second_type>;
    } else {
      return type<nothing_s>;
    }
  };


  /**
   * @brief Return `true` if the input type proxy is for a homogeneous list. Otherwise, return `false`.
   */
  constexpr auto is_homogeneous_list_type = []<typename T>(T){
    constexpr auto aux = []<NonemptyList U>(auto recur, type_s<U>){
      if constexpr (UnitaryList<U>){
        return true;
      } else if constexpr (Pair<U> && head_type(type<U>) == head_type(tail_type(type<U>))){
        return recur(recur, tail_type(type<U>));
      } else {
        return false;
      }
    };
    if constexpr (Type<T> && NonemptyList<typename T::type>){
      return u(aux, T{});
    } else {
      return false;
    }
  };

  /**
   * @brief A concept for homogeneous lists
   */
  template<typename T>
  concept HomogeneousList = NonemptyList<T> && is_homogeneous_list_type(type<T>);

  /**
   * @brief Return `true` if the input is a homogeneous list. Otherwise, return `false`.
   */
  constexpr auto is_homogeneous_list = []<typename T>(T){
    return HomogeneousList<T>;
  };

  /**
   * @brief Return `true` if the input is an association list type. Otherwise, return `false`.
   */
  constexpr auto is_association_list_type = []<typename T>(T){
    constexpr auto aux = []<List U>(auto recur, type_s<U>){
      if constexpr (Pair<U>){
        if constexpr (Pair<typename U::first_type>) {
          return recur(recur, tail_type(type<U>));
        } else {
          return false;
        }
      } else if constexpr (Nothing<U>){
        return true;
      } else {
        return false;
      }
    };

    if constexpr (Type<T> and List<typename T::type>){
      return u(aux, T{});
    } else {
      return false;
    }
  };

  /**
   * @brief A concept for association lists
   */
  template<typename T>
  concept AssociationList = List<T> && is_association_list_type(type<T>);

  /**
   * @brief Return `true` if the input is an association list. Otherwise, return `false`.
   */
  constexpr auto is_association_list = []<typename T>(T){
    return AssociationList<T>;
  };


  /**
   * @brief A concept for homogeneous association lists
   */
  template<typename T>
  concept HomogeneousAssociationList = HomogeneousList<T> && AssociationList<T>;

  /**
   * @brief Return `true` if the input is a homogeneous association list. Otherwise, return `false`.
   */
  constexpr auto is_homogeneous_association_list = []<typename T>(T){
    return HomogeneousAssociationList<T>;
  };

  /**
   * @brief Return the number of elemements in the input list.
   */
  constexpr auto length = []<List T>(T){
    return length_type(type<T>);
  };

  /**
   * @brief Return the first element of a nonempty list.
   */
  constexpr auto head = curry(nat<1>, [](NonemptyList auto xs){
    return car(xs);
  });

  /**
   * @brief Return a list containing all but the first element of the input list.
   */
  constexpr auto tail = []<List T>(T xs){
      if constexpr (Pair<T>){
        return cdr(xs);
      } else {
        return nothing;
      }
    };

  /**
   * @brief Return the reverse of the first input list appended with the second input list.
   */
  constexpr auto rappend = curry(nat<2>, [](List auto xs, List auto ys){
    constexpr auto aux = []<List T>(auto recur, T xs, List auto ys){
      if constexpr (Pair<T>){
        return recur(recur, tail(xs), cons(head(xs), ys));
      } else {
        return ys;
      }
    };
    return u(aux, xs, ys);
  });


  /**
   * @brief Return the input list reversed.
   */
  constexpr auto reverse = [](List auto xs){
    return rappend(xs, nothing);
  };



  /**
   * @brief Return a list containing the input values.
   */
  constexpr auto list = [](auto ... xs){

    constexpr auto aux = [](auto recur, List auto accum, auto x, auto ... xs){
      if constexpr (sizeof ... (xs) > 0){
        return recur(recur, cons(x, accum), xs ...);
      } else {
        return reverse(cons(x, accum));
      }
    };

    if constexpr (sizeof...(xs) > 0){
      return u(aux, nothing, xs ...);
    } else {
      return nothing;
    }
  };

} // namespace ruse::reference
