#pragma once

//
// ... ruse header files
//
#include <ruse/protocol_traits.hpp>
#include <ruse/reference/nat.hpp>
#include <ruse/reference/nothing.hpp>
#include <ruse/reference/pair.hpp>
#include <ruse/reference/protocol.hpp>
#include <ruse/reference/tag.hpp>
#include <ruse/reference/type.hpp>
#include <ruse/reference/utility.hpp>

namespace ruse::reference {

  /**
   * @brief Return `true` if the input is a type proxy for a list type.
   * Otherwise, return `false`.
   */
  constexpr auto is_list_type = []<typename T>(T) {
    constexpr auto aux = []<typename U>(auto recur, type_s<U>) {
      if constexpr (Pair<U>) {
        return recur(recur, type<typename U::second_type>);
      } else if constexpr (Nothing<U>) {
        return true;
      } else {
        return false;
      }
    };

    if constexpr (Type<T>) {
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
  constexpr auto is_list = []<typename T>(T) { return List<T>; };

  /**
   * @brief A concept for nonempty lists
   */
  template<typename T>
  concept NonemptyList = List<T> and Pair<T>;

  /**
   * @brief Return `true` if the input is a nonempty list. Otherwise, return
   * `false`.
   */
  constexpr auto is_nonempty_list = []<typename T>(T) {
    return NonemptyList<T>;
  };

  /**
   * @brief A concept for empty lists
   */
  template<typename T>
  concept EmptyList = List<T> and not NonemptyList<T>;

  /**
   * @brief Return `true` if the input list is an empty list. Otherwise, return
   * `false`.
   */
  constexpr auto is_empty_list = []<typename T>(T) { return EmptyList<T>; };

  /**
   * @brief Return `true` if the input type proxy is a proxy for an empty list
   * type. Otherwise, return `false`.
   */
  constexpr auto is_empty_list_type = []<typename T>(type_s<T>) {
    return EmptyList<T>;
  };

  /**
   * @brief Return the number of elements in instances of the list type
   * referenced by the input type proxy.
   */
  constexpr auto length_type = []<List T>(type_s<T>) {
    constexpr auto aux = []<typename U>(auto recur, auto accum, type_s<U>) {
      if constexpr (Pair<U>) {
        return recur(recur, accum + 1, type<typename U::second_type>);
      } else {
        return accum;
      }
    };
    return u(aux, 0, type<T>);
  };

  /**
   * @brief A concept for lists of a specified length
   */
  template<typename T, integer N>
  concept ListOfLength = List<T> and length_type(type<T>)
  == N;

  /**
   * @brief A concept for lists of a positive specified length
   */
  template<typename T, integer N>
  concept ListOfPositiveLength =
    (N > 0) && NonemptyList<T>&& ListOfLength<T, N>;

  template<typename T, integer N>
  concept ListOfLengthAtLeast = List<T> &&(length_type(type<T>) >= N);

  template<typename T, integer N>
  concept ListOfLengthGreaterThan = List<T> &&(length_type(type<T>) > N);

  template<typename T, integer N>
  concept ListOfLengthLessThan = List<T> &&(length_type(type<T>) >= N);

  template<typename T, integer N>
  concept ListOfLengthNoMoreThan = List<T> &&(length_type(type<T>) >= N);

  constexpr auto list_type_ref =
    []<integer N, ListOfLengthAtLeast<N> T>(Nat<N>, type_s<T>) {
      constexpr auto recur = []<typename Head, typename Tail, integer M>(
                               auto recur, type_s<pair<Head, Tail>>, Nat<M>) {
        if constexpr (M == N) {
          return type<Head>;
        } else {
          return recur(recur, type<Tail>, nat<M + 1>);
        }
      };

      return recur(recur, type<T>, nat<0>);
    };

  template<typename T, typename E, integer N>
  concept ListWithElement = ListOfLengthAtLeast<T, N> && type<E>
  == list_type_ref(nat<N>, type<T>);

  /**
   * @brief A concept for unitary lists
   */
  template<typename T>
  concept UnitaryList = ListOfPositiveLength<T, 1>;

  /**
   * @brief A concept for two-element lists
   */
  template<typename T>
  concept BinaryList = ListOfPositiveLength<T, 2>;

  /**
   * @brief A concept for three-element lists
   */
  template<typename T>
  concept TernaryList = ListOfPositiveLength<T, 3>;

  /**
   * @brief Return `true` if the input is a unitary list.  Otherwise, return
   * `false`.
   */
  constexpr auto is_unitary_list = []<typename T>(T) { return UnitaryList<T>; };

  /**
   * @brief Return a type proxy for the head of the input type proxy.
   */
  constexpr auto head_type = []<NonemptyList T>(type_s<T>) {
    return type<typename T::first_type>;
  };

  /**
   * @brief Return a type proxy for the tail of the input type proxy.
   */
  constexpr auto tail_type = []<List T>(type_s<T>) {
    if constexpr (Pair<T>) {
      return type<typename T::second_type>;
    } else {
      return type<nothing_s>;
    }
  };

  /**
   * @brief Return `true` if every member of the input list type proxy is a
   * vacuous type. Otherwise, return `false`.
   */
  constexpr auto is_vacuous_list_type = []<List T>(type_s<T>) {
    constexpr auto recur = []<List U>(auto recur, type_s<U>) {
      if constexpr (EmptyList<U>) {
        return true;
      } else if (is_vacuous_type(head_type(type<U>))) {
        return recur(recur, tail_type(type<U>));
      } else {
        return false;
      }
    };
    return recur(recur, type<T>);
  };

  /**
   * @brief The concept of a vacuous list: a list where every member is an
   * instance of a vacuous type.
   */
  template<typename T>
  concept VacuousList = List<T> and is_vacuous_list_type(type<T>);

  /**
   * @brief Return `true` if every member of the input list is a vacuous type.
   */
  constexpr auto is_vacuous_list = []<typename T>(T) { return VacuousList<T>; };

  /**
   * @brief Return `true` if the input type proxy is for a homogeneous list.
   * Otherwise, return `false`.
   */
  constexpr auto is_homogeneous_list_type = []<typename T>(T) {
    constexpr auto aux = []<NonemptyList U>(auto recur, type_s<U>) {
      if constexpr (UnitaryList<U>) {
        return true;
      } else if constexpr (
        Pair<U> && head_type(type<U>) == head_type(tail_type(type<U>))) {
        return recur(recur, tail_type(type<U>));
      } else {
        return false;
      }
    };
    if constexpr (Type<T> && NonemptyList<typename T::type>) {
      return u(aux, T{});
    } else {
      return false;
    }
  };

  /**
   * @brief A concept for homogeneous lists
   */
  template<typename T>
  concept HomogeneousList = NonemptyList<T> &&
    is_homogeneous_list_type(type<T>);

  /**
   * @brief Return `true` if the input is a homogeneous list. Otherwise, return
   * `false`.
   */
  constexpr auto is_homogeneous_list = []<typename T>(T) {
    return HomogeneousList<T>;
  };

  /**
   * @brief Return `true` if the input is an association list type. Otherwise,
   * return `false`.
   */
  constexpr auto is_association_list_type = []<typename T>(T) {
    constexpr auto aux = []<List U>(auto recur, type_s<U>) {
      if constexpr (Pair<U>) {
        if constexpr (Pair<typename U::first_type>) {
          return recur(recur, tail_type(type<U>));
        } else {
          return false;
        }
      } else if constexpr (Nothing<U>) {
        return true;
      } else {
        return false;
      }
    };

    if constexpr (Type<T> and List<typename T::type>) {
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
   * @brief Return `true` if the input is an association list. Otherwise, return
   * `false`.
   */
  constexpr auto is_association_list = []<typename T>(T) {
    return AssociationList<T>;
  };

  /**
   * @brief A concept for homogeneous association lists
   */
  template<typename T>
  concept HomogeneousAssociationList = HomogeneousList<T> && AssociationList<T>;

  /**
   * @brief Return `true` if the input is a homogeneous association list.
   * Otherwise, return `false`.
   */
  constexpr auto is_homogeneous_association_list = []<typename T>(T) {
    return HomogeneousAssociationList<T>;
  };

  /**
   * @brief Return `true` if the input is a type proxy for a property list type.
   * Otherwise, return `false`.
   */
  constexpr auto is_property_list_type = []<typename T>(T) {
    constexpr auto aux = []<List U>(auto recur, type_s<U>) {
      if constexpr (NonemptyList<U>) {
        if constexpr (is_tagged_type(head_type(type<U>))) {
          return recur(recur, tail_type(type<U>));
        } else {
          return false;
        }
      } else {
        return true;
      }
    };

    if constexpr (Type<T>) {
      if constexpr (List<typename T::type>) {
        return u(aux, T{});
      } else {
        return false;
      }
    } else {
      return false;
    }
  };

  /**
   * @brief A concept for property lists
   */
  template<typename T>
  concept PropertyList = is_property_list_type(type<T>);

  /**
   * @brief Return `true` if the input is a property list. Otherwise, return
   * `false`.
   */
  constexpr auto is_property_list = []<typename T>(T) {
    return PropertyList<T>;
  };

  template<typename T, typename U>
  concept ListLike = List<T> && List<U> && length_type(type<T>)
  == length_type(type<U>);

  /**
   * @brief Return the number of elemements in the input list.
   */
  constexpr auto length = []<List T>(T) { return length_type(type<T>); };

  /**
   * @brief Return the first element of a nonempty list.
   */
  constexpr auto head =
    curry(nat<1>, [](NonemptyList auto xs) { return car(xs); });

  /**
   * @brief Return a list containing all but the first element of the input
   * list.
   */
  constexpr auto tail = []<List T>(T xs) -> List auto
  {
    if constexpr (Pair<T>) {
      return cdr(xs);
    } else {
      return nothing;
    }
  };

  /**
   * @brief Return the reverse of the first input list appended with the second
   * input list.
   */
  constexpr auto rappend = curry(
    nat<2>,
    [](List auto xs, List auto ys) -> List auto {
      constexpr auto aux = []<List T>(auto recur, T xs, List auto ys) {
        if constexpr (Pair<T>) {
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
  constexpr auto reverse = [](List auto xs) -> List auto
  {
    return rappend(xs, nothing);
  };

  /**
   * @brief Return a list with the elements from both input lists
   */
  constexpr auto append = curry(nat<2>, [](List auto xs, List auto ys) {
    return rappend(reverse(xs), ys);
  });

  /**
   * @brief Return a list containing the input values.
   */
  constexpr auto list = [](auto... xs) -> List auto
  {

    constexpr auto aux = [](auto recur, List auto accum, auto x, auto... xs) {
      if constexpr (sizeof...(xs) > 0) {
        return recur(recur, cons(x, accum), xs...);
      } else {
        return reverse(cons(x, accum));
      }
    };

    if constexpr (sizeof...(xs) > 0) {
      return u(aux, nothing, xs...);
    } else {
      return nothing;
    }
  };

  constexpr List auto
  make_list_(List auto xs)
  {
    return xs;
  }

  constexpr List auto
  make_list_(auto x, List auto xs)
  {
    return cons(x, xs);
  }

  constexpr List auto
  make_list_(auto x, auto y, auto z, auto... zs)
  {
    return cons(x, make_list_(y, z, zs...));
  }

  constexpr auto list_ = [](auto x, auto... xs) {
    return make_list_(x, xs...);
  };

  /**
   * @brief Return a vacuous list with the elements sorted according to cmp.
   */
  constexpr auto vacuous_list_sort =
    curry(nat<2>, []<VacuousList T>(auto cmp, T) {
      constexpr auto recur = []<VacuousList Left, VacuousList Right>(
                               auto recur, auto cmp, Left, Right) {
        if constexpr (Nothing<Right>) {
          return reverse(Left{});

        } else if constexpr (Nothing<Left>) {
          return recur(recur, cmp, list(car(Right{})), cdr(Right{}));

        } else if constexpr (cmp(car(Left{}), car(Right{}))) {
          return recur(recur, cmp, cons(car(Right{}), Left{}), cdr(Right{}));

        } else {
          return recur(
            recur,
            cmp,
            cdr(Left{}),
            cons(car(Right{}), cons(car(Left{}), cdr(Right{}))));
        }
      };
      return recur(recur, cmp, nothing, T{});
    });

  /**
   * @brief Return the results of filtering the input vacuous list with the
   * input predicate.
   */
  constexpr auto vacuous_list_filter =
    curry(nat<2>, []<VacuousList T>(auto pred, T) {
      constexpr auto recur = []<VacuousList U, VacuousList Accum>(
                               auto recur, auto pred, U, Accum) {
        if constexpr (Nothing<U>) {
          return reverse(Accum{});
        } else if constexpr (pred(car(U{}))) {
          return recur(recur, pred, cdr(U{}), cons(car(U{}), Accum{}));
        } else {
          return recur(recur, pred, cdr(U{}), Accum{});
        }
      };
      return recur(recur, pred, T{}, nothing);
    });

  /**
   * @brief Return the with elements taken from the input list while the input
   * predicate is true.
   */
  constexpr auto vacuous_list_take_while =
    curry(nat<2>, []<VacuousList T>(auto pred, T) {
      constexpr auto recur = []<VacuousList U, VacuousList Accum>(
                               auto recur, auto pred, U, Accum) {
        if constexpr (Nothing<U>) {
          return reverse(Accum{});
        } else if constexpr (not pred(head(U{}))) {
          return reverse(Accum{});
        } else {
          return recur(recur, pred, cdr(U{}), cons(car(U{}), Accum{}));
        }
      };

      return recur(recur, pred, T{}, nothing);
    });

  /**
   * @brief Return a list containing elements from the start of the input list
   * until the first element is reach that satisfies the input predicate.
   */
  constexpr auto vacuous_list_take_until =
    curry(nat<2>, []<VacuousList T>(auto pred, T) {
      return vacuous_list_take_while([=](auto x) { return not pred(x); }, T{});
    });

  /**
   * @brief Return the indicated element of the input list.
   */
  constexpr auto list_ref = curry(nat<2>, []<integer N, List T>(Nat<N>, T xs) {
    static_assert(
      N < length_type(type<T>), "Expected a valid index for the input list");

    constexpr auto aux = []<integer M>(auto recur, Nat<M>, List auto xs) {
      if constexpr (M == 0) {
        return head(xs);
      } else {
        return recur(recur, nat<M - 1>, tail(xs));
      }
    };

    return u(aux, nat<N>, xs);
  });

  /**
   * @brief Return a list containing the first `n` elements of the input
   * list, or return the input list if it has `n` or fewer elements
   */
  constexpr auto take = curry(
    nat<2>,
    []<integer N>(Nat<N>, List auto xs) -> List auto {
      constexpr auto aux = []<integer M, List T>(
                             auto recur, List auto accum, Nat<M>, T xs) {
        if constexpr (M == 0 || Nothing<T>) {
          return reverse(accum);
        } else {
          return recur(recur, cons(head(xs), accum), nat<M - 1>, tail(xs));
        }
      };
      return u(aux, nothing, nat<N>, xs);
    });

  /**
   * @brief Return a list with the first `n` elements of the input list
   * removed, or return `nothing` if the input list has `n` or fewer
   * elements.
   */
  constexpr auto drop = curry(
    nat<2>,
    []<integer N>(Nat<N>, List auto xs) -> List auto {
      constexpr auto aux = []<integer M, List T>(auto recur, Nat<M>, T xs) {
        if constexpr (M == 0 || Nothing<T>) {
          return xs;
        } else {
          return recur(recur, nat<M - 1>, tail(xs));
          ;
        }
      };
      return u(aux, nat<N>, xs);
    });

  /**
   * @brief Return the indicated element from the homogeneous input list.
   */
  constexpr auto homogeneous_list_ref =
    curry(nat<2>, [](integer index, HomogeneousList auto xs) {
      constexpr auto recur = []<NonemptyList T>(
                               auto recur, integer index, T xs) {
        if constexpr (UnitaryList<T>) {
          return head(xs);
        } else {
          return index > 0 ? recur(recur, index - 1, tail(xs)) : head(xs);
        }
      };

      return (index >= 0) && (index < length(xs))
               ? recur(recur, index, xs)
               : throw logic_error("invalid homogeneous list index");
    });

  /**
   * @brief Return the input hlist sorted according to cmp.
   */
  constexpr auto homogeneous_list_sort =
    curry(nat<2>, []<HomogeneousList T>(auto cmp, T xs) {
      constexpr auto recur =
        []<List Left, List Right>(
          auto recur, auto cmp, Left left, Right right) -> T {
        if constexpr (Nothing<Right>) {
          return reverse(left);
        } else if constexpr (Nothing<Left>) {
          return recur(recur, cmp, list(car(right)), cdr(right));
        } else {
          return cmp(car(left), car(right))
                   ? recur(recur, cmp, cons(car(right), left), cdr(right))
                   : recur(
                       recur,
                       cmp,
                       cdr(left),
                       cons(car(right), cons(car(left), cdr(right))));
        }
      };
      return recur(recur, cmp, list(car(xs)), cdr(xs));
    });

  /**
   * @brief Return true if the property list has the indicated tag.
   * Otherwise return false.
   */
  constexpr auto property_list_has_tag =
    curry(nat<2>, [](Tag auto key, PropertyList auto xs) {
      constexpr auto recur = []<Tag K, PropertyList T>(
                               auto recur, K key, T xs) {
        if constexpr (NonemptyList<T>) {
          if constexpr (name_type(type<K>) == name_type(head_type(type<T>))) {
            return true;
          } else {
            return recur(recur, key, tail(xs));
          }
        } else {
          return false;
        }
      };
      return recur(recur, key, xs);
    });

  /**
   * @brief Return the first element of the input property list matching the
   * input tag.
   */
  constexpr auto property_list_ref =
    curry(nat<2>, [](Empty auto tag, PropertyList auto xs) {
      constexpr auto aux = []<Tag K, PropertyList T>(auto recur, K key, T xs) {
        if constexpr (NonemptyList<T>) {
          if constexpr (name_type(type<K>) == name_type(head_type(type<T>))) {
            return head(xs);
          } else {
            return recur(recur, key, tail(xs));
          }
        } else {
          return false;
        }
      };
      return u(aux, tag, xs);
    });

  /**
   * @brief Return the value from the first element of the input property list
   * matching the input tag.
   */
  constexpr auto property_list_ref_ =
    curry(nat<2>, [](Empty auto tag, PropertyList auto xs) {
      return property_list_ref(tag, xs).value;
    });

  /**
   * @brief Return the last element of the nonempty input list.
   */
  constexpr auto last =
    curry(nat<1>, [](NonemptyList auto xs) { return head(reverse(xs)); });

  /**
   * @brief Return a list with all but the last element of the input list.
   */
  constexpr auto butlast = []<List T>(T xs) -> List auto
  {
    return reverse(tail(reverse(xs)));
  };

  /**
   * @brief Exectute the input function over the input list purely for side
   * effects.
   */
  constexpr auto dolist = [](List auto xs, auto&& f) {
    auto aux = [&]<List T>(auto&& recur, T xs) {
      if constexpr (NonemptyList<T>) {
        f(head(xs));
        recur(recur, tail(xs));
      }
    };
    u(aux, xs);
  };

  // /**
  //  * @brief Return the concatenation of the two input lists.
  //  */
  // constexpr auto
  // operator+(List auto xs, List auto ys)
  // {
  //   return append(xs, ys);
  // };

  /**
   * @brief Return the concatenation of multiple copies of the input list
   */
  template<integer N>
  constexpr auto
  operator*(Nat<N>, List auto xs)
  {
    constexpr auto recur =
      []<integer I>(auto recur, Nat<I>, List auto xs, List auto accum) {
        if constexpr (I == 0) {
          return accum;
        } else {
          return recur(recur, nat<I - 1>, xs, xs + accum);
        }
      };
    return recur(recur, nat<N>, xs, list());
  }

  /**
   * @brief Return the concatenation of multiple copies of the input list
   */
  template<integer N>
  constexpr auto
  operator*(List auto xs, Nat<N>)
  {
    return nat<N> * xs;
  }

  /**
   * @brief Return the left fold of the input list with the input function and
   * initial value.
   */
  constexpr auto foldl = curry(nat<3>, [](auto f, auto init, List auto xs) {
    constexpr auto recur = []<List T>(auto recur, auto f, auto init, T xs) {
      if constexpr (Nothing<T>) {
        return init;
      } else {
        return recur(recur, f, f(init, car(xs)), cdr(xs));
      }
    };
    return recur(recur, f, init, xs);
  });

  template<List T>
  constexpr auto
  get_pure(type_s<T>)
  {
    return list;
  }

  template<List T>
  constexpr auto
  get_flatmap(type_s<T>)
  {
    return [](auto f, List auto xs) -> List auto
    {
      constexpr auto aux = []<List U>(
                             auto recur, List auto accum, auto f, U xs) {
        if constexpr (NonemptyList<U>) {
          return recur(recur, rappend(f(head(xs)), accum), f, tail(xs));
        } else {
          return reverse(accum);
        }
      };
      return aux(aux, nothing, f, xs);
    };
  }

  template<NonemptyList T>
  constexpr auto
  get_extract(type_s<T>)
  {
    return head;
  }

  template<NonemptyList T>
  constexpr auto
  get_extend(type_s<T>)
  {
    return [](auto f, NonemptyList auto xs) {
      constexpr auto aux = []<List U>(
                             auto recur, List auto accum, auto f, U xs) {
        if constexpr (NonemptyList<U>) {
          return recur(recur, cons(f(xs), accum), f, tail(xs));
        } else {
          return reverse(accum);
        }
      };
      return u(aux, nothing, f, xs);
    };
  }

  template<NonemptyList T>
  constexpr auto
  get_zapply(type_s<T>)
  {
    return []<NonemptyList F>(F wf, ListLike<F> auto wx) {
      return [=]<auto... Indices>(index_sequence<Indices...>)
      {
        return list(list_ref(nat<Indices>, wf)(list_ref(nat<Indices>, wx))...);
      }
      (make_index_sequence<length_type(type<F>)>());
    };
  }

} // namespace ruse::reference

namespace ruse {

  template<reference::List T>
  struct monoid_traits<T>
  {
    constexpr static auto op = reference::append;
    constexpr static auto id = reference::nothing;
    using color = additive;
  };

} // end of namespace ruse
