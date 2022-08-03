#pragma once

//
// ... ruse header files
//
#include <ruse/reference/list.hpp>
#include <ruse/reference/string.hpp>
#include <ruse/reference/tag.hpp>
#include <ruse/reference/utility.hpp>

namespace ruse::reference {

  constexpr auto parse_result_value = [](PropertyList auto parse_result) {
    return property_list_ref("value"_tag, parse_result).value;
  };

  constexpr auto parse_result_sequence = [](PropertyList auto parse_result) {
    return property_list_ref("sequence"_tag, parse_result).value;
  };

  template<Vacuous T>
  struct result_parser
  {
    template<HoistedList Sequence>
    constexpr auto
    operator()(Sequence) const
    {
      return list(list("value"_tag(T{}), "sequence"_tag(Sequence{})));
    }
  };

  constexpr auto result =
    curry(nat<1>, []<Vacuous T>(T) { return result_parser<T>{}; });

  struct fail_parser_s
  {
    constexpr List auto
    operator()(HoistedList auto) const
    {
      return nothing;
    }
  } constexpr fail{};

  struct item_parser
  {
    template<HoistedList Sequence>
    constexpr List auto
    operator()(Sequence xs) const
    {
      if constexpr (NonemptyHoistedList<Sequence>) {
        return result(hoisted_head(xs))(hoisted_tail(xs));
      } else {
        return fail(xs);
      }
    }
  } constexpr item{};

  template<Vacuous T>
  struct empty_parser
  {
    template<HoistedList Sequence>
    constexpr List auto
    operator()(Sequence) const
    {
      if constexpr (NonemptyHoistedList<Sequence>) {
        return fail(Sequence{});
      } else {
        return result(T{})(Sequence{});
      }
    }
  };

  constexpr auto empty =
    curry(nat<1>, []<Vacuous T>(T) { return empty_parser<T>{}; });

  template<Vacuous Parser, Vacuous ParserConstructor>
  struct conj_parser_s
  {

    static constexpr Parser parser{};
    static constexpr ParserConstructor parser_constructor{};

    template<HoistedList Sequence>
    constexpr List auto
    operator()(Sequence) const
    {
      using Results = result_of_t<Parser(Sequence)>;
      if constexpr (NonemptyList<Results>) {
        return letm(Results{}, [](auto result) {
          return parser_constructor(parse_result_value(result))(
            parse_result_sequence(result));
        });
      } else {
        return fail(Sequence{});
      }
    }
  };

  constexpr auto conj = curry(
    nat<2>,
    []<Vacuous Parser, Vacuous ParserConstructor>(Parser, ParserConstructor) {
      return conj_parser_s<Parser, ParserConstructor>{};
    });

  template<Vacuous Parser1, Vacuous Parser2>
  struct disj_parser_s
  {
    static constexpr Parser1 p1{};
    static constexpr Parser2 p2{};

    template<HoistedList Sequence>
    constexpr List auto
    operator()(Sequence xs) const
    {
      return append(p1(xs), p2(xs));
    }
  };

  constexpr auto disj =
    curry(nat<2>, []<Vacuous Parser1, Vacuous Parser2>(Parser1, Parser2) {
      return disj_parser_s<Parser1, Parser2>{};
    });

  template<Vacuous Parser1, Vacuous Parser2>
  struct alt_parser_s
  {
    static constexpr Parser1 p1{};
    static constexpr Parser2 p2{};

    template<HoistedList Sequence>
    constexpr List auto
    operator()(Sequence xs) const
    {
      using Results1 = decltype(p1(xs));
      if constexpr (NonemptyList<Results1>) {
        return Results1{};
      } else {
        return p2(xs);
      }
    }
  };

  constexpr auto alt =
    curry(nat<2>, []<Vacuous Parser1, Vacuous Parser2>(Parser1, Parser2) {
      return alt_parser_s<Parser1, Parser2>{};
    });

  template<auto... Parsers>
  struct sequence_parser
  {

    template<HoistedList Sequence>
    constexpr auto
    operator()(Sequence) const
    {
      if constexpr (0 == sizeof...(Parsers)) {
        return result(hoisted_nothing, Sequence{});
      } else {
        return []<auto P, auto... Ps>(hoisted_list<P, Ps...>)
        {
          return conj(
            P,
            []<auto Value>(hoisted<Value>) {
              return conj(
                sequence_parser<Ps...>{},
                []<auto... Values>(hoisted_list<Values...>) {
                  return result(hoisted_list<Value, Values...>{});
                });
            },
            Sequence{});
        }
        (hoisted_list<Parsers...>{});
      }
    }
  };

  template<auto Parser, auto... Parsers>
  constexpr sequence_parser<Parser, Parsers...> seq{};

  constexpr auto sat = curry(nat<1>, []<Vacuous Predicate>(Predicate) {
    return conj(item, []<Vacuous T>(T) {
      if constexpr (Predicate{}(T{})) {
        return result(T{});
      } else {
        return fail;
      }
    });
  });

  template<auto X, auto Y>
  concept EqualityComparableValues = requires
  {
    X == Y;
  };

  template<auto Value>
  constexpr auto equal = sat([]<auto Input>(hoisted<Input>) {
    if constexpr (EqualityComparableValues<Input, Value>) {
      return Input == Value;
    } else {
      return false;
    }
  });

  constexpr auto word =
    []<auto Value, auto... Values>(hoisted_list<Value, Values...>)
  {
    return seq<equal<Value>, equal<Values>...>;
  };

  constexpr auto opt =
    curry(nat<1>, [](Vacuous auto p) { return alt(p, result(nothing)); });

  // constexpr auto many = curry(nat<1>, []<Vacuous Parser>(Parser) {
  //   return u([]<typename Recur>(Recur) {
  //     return conj(opt(Parser{}), []<typename T>(T) {
  //       if constexpr (Nothing<T>) {
  //         return result(hoisted_nothing);
  //       } else {
  //         return conj(Recur{}(Recur{}), []<Vacuous Ts>(Ts) {
  //           return result(hoisted_cons(T{}, Ts{}));
  //         });
  //       }
  //     });
  //   });
  // });

  // constexpr auto many1 = curry(nat<1>, []<Vacuous Parser>(Parser) {
  //   return conj(Parser{}, []<typename T>(T) {
  //     return conj(many(Parser{}), []<typename Ts>(Ts) {
  //       return result(hoisted_cons(T{}, Ts{}));
  //     });
  //   });
  // });

} // end of namespace ruse::reference
