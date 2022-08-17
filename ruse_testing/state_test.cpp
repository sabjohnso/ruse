//
// ... Standard header files
//
#include <string>

//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/experimental/state.hpp>
#include <ruse/ruse.hpp>
#include <unistd.h>

using namespace std::literals;
namespace ruse::experimental::testing {

  TEST(state, make_state)
  {
    STATIC_EXPECT_EQ(list('x', 's'), run_state('s')(make_stateful('x')));
  }

  TEST(state, fmap)
  {
    STATIC_EXPECT_EQ(list(2, 's'), run_state('s', fmap(twc, make_stateful(1))));
  }

  TEST(state, fapply)
  {
    STATIC_EXPECT_EQ(
      list(2, 's'),
      run_state('s')(fapply(make_stateful(twc), make_stateful(1))));
  }

  TEST(state, flatmap)
  {
    STATIC_EXPECT_EQ(
      list('x', 's'),
      run_state('s')(flatmap(make_stateful, make_stateful('x'))));
  }

  TEST(state, flatten)
  {
    STATIC_EXPECT_EQ(
      list('x', 's'),
      run_state('s')(flatten(make_stateful(make_stateful('x')))));
  }

  TEST(stat, get)
  {
    STATIC_EXPECT_EQ(list('s', 's'), run_state('s')(get_state));
  }

  TEST(state, select)
  {
    STATIC_EXPECT_EQ(
      list(3, list("x"_tag(3), "y"_tag(4))),
      run_state(list("x"_tag(3), "y"_tag(4)))(
        select_state(plist_ref_("x"_tag))));
  }

  TEST(state, put)
  {
    STATIC_EXPECT_EQ(list(nothing, 's'), run_state('t')(put_state('s')));
  }

  TEST(state, modify)
  {
    STATIC_EXPECT_EQ(list(nothing, 2), run_state(1)(modify_state(twc)));
  }

  TEST(state, letm)
  {
    // clang-format off
    STATIC_EXPECT_EQ(
      list(7, list("x"_tag(3), "y"_tag(4))),
      run_state(list("x"_tag(3), "y"_tag(4)))(
        letm(select_state(plist_ref_("x"_tag)), [=](auto x) { return
        letm(select_state(plist_ref_("y"_tag)), [=](auto y) { return
              pure(x+y);
        }); })));
    // clang-format on
  }

  TEST(state, game)
  {
    enum struct input
    {
      off,
      on,
      up,
      down,
      invalid_input,
    };

    enum struct status
    {
      off,
      on
    };

    static constexpr auto onoff = [](BinaryList auto state) {
      return list_ref(nat<0>, state);
    };

    STATIC_EXPECT_EQ(status::on, onoff(list(status::on, 0)));

    static constexpr auto is_on = [](BinaryList auto state) {
      return onoff(state) == status::on;
    };
    STATIC_EXPECT_TRUE(is_on(list(status::on, 0)));
    STATIC_EXPECT_FALSE(is_on(list(status::off, 0)));

    static constexpr auto score = [=](BinaryList auto state) {
      return list_ref(nat<1>, state);
    };

    STATIC_EXPECT_EQ(3, score(list(status::on, 3)));

    static constexpr auto update_state = curry(nat<2>, [](auto in, auto state) {
      return is_on(state)
               ? (in == input::off
                    ? list(status::off, score(state))
                    : (in == input::up ? list(status::on, score(state) + 1)
                                       : (in == input::down
                                            ? list(status::on, score(state) - 1)
                                            : state)))
               : (in == input::on ? list(status::on, score(state)) : state);
    });

    STATIC_EXPECT_EQ(
      list(status::on, 0),
      update_state(input::invalid_input, list(status::on, 0)));

    STATIC_EXPECT_EQ(
      list(status::on, 0), update_state(input::on, list(status::off, 0)));

    STATIC_EXPECT_EQ(
      list(status::on, 1), update_state(input::up, list(status::on, 0)));

    STATIC_EXPECT_EQ(
      list(status::on, -1), update_state(input::down, list(status::on, 0)));

    static constexpr auto game = [](List auto inputs) {
      constexpr auto recur = []<List T>(auto recur, T inputs) {
        if constexpr (Nothing<T>) {
          return get_state;
        } else {
          return beginm(
            modify_state(update_state(car(inputs))), recur(recur, cdr(inputs)));
        }
      };
      return recur(recur, inputs);
    };

    STATIC_EXPECT_EQ(
      list(status::off, 0), eval_state(list(status::off, 0))(game(list())));

    STATIC_EXPECT_EQ(
      list(status::on, 0),
      eval_state(list(status::off, 0))(game(list(input::on))));

    STATIC_EXPECT_EQ(
      list(status::off, 1),
      eval_state(list(status::off, 0))(
        game(list(input::on, input::up, input::off))));

    STATIC_EXPECT_EQ(
      list(status::off, 2),
      eval_state(list(status::off, 0))(game(list(
        input::on,
        input::up,
        input::down,
        input::invalid_input,
        input::up,
        input::up,
        input::off))));
  }
} // namespace ruse::experimental::testing
