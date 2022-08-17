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

namespace ruse::experimental {

  template<typename F>
  struct reader_s
  {
    F run;

    constexpr reader_s(F input)
      : run(input)
    {}
  };

  constexpr auto make_reader = [](auto x) {
    return reader_s{[=](auto) { return x; }};
  };

  template<typename M, typename E>
  concept Reader = requires(M mx, E e)
  {
    mx.run(e);
  };

  constexpr auto run = curry(nat<2>, [](auto e, auto mx) { return mx.run(e); });

  template<typename F>
  constexpr auto
  get_pure(type_s<reader_s<F>>)
  {
    return []<typename T>(T x) { return reader_s{[=](auto) { return x; }}; };
  }

  template<typename F>
  constexpr auto
  get_flatmap(type_s<reader_s<F>>)
  {
    return [](auto f, auto mx) {
      return reader_s{[=](auto e) { return run(e, f(run(e, mx))); }};
    };
  }

  constexpr auto ask = reader_s{identity};

  constexpr auto asks = cute(fmap, ask);

  constexpr auto local = curry(nat<2>, [](auto f, auto mx) {
    return reader_s{[=](auto e) { return run(f(e), mx); }};
  });

} // namespace ruse::experimental
