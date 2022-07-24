//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>
#include <ruse/show.hpp>

namespace ruse::testing
{
  TEST(parser, result){
    constexpr auto output = result(hoisted<'a'>{})( "bc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_TRUE(is_plist(head(output)));
    STATIC_EXPECT_EQ(hoisted<'a'>{}, plist_ref("value"_tag, head(output)).value);
    STATIC_EXPECT_EQ("bc"_hoist, plist_ref("sequence"_tag, head(output)).value);
  }

  TEST(parse, fail){
    constexpr auto output = fail("bc"_hoist);
    STATIC_EXPECT_TRUE(is_nothing(output));
  }

  TEST(parse, item){
    constexpr auto output = item("abc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_TRUE(is_plist(head(output)));
    STATIC_EXPECT_EQ(hoisted<'a'>{}, plist_ref("value"_tag, head(output)).value);
    STATIC_EXPECT_EQ("bc"_hoist, plist_ref("sequence"_tag, head(output)).value);
  }

  TEST(parse, item_fail){
    constexpr auto output = item(""_hoist);
    STATIC_EXPECT_TRUE(is_nothing(output));
  }

  TEST(parse, empty){
    constexpr auto output = empty(hoisted<nothing_s{}>{}, ""_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_TRUE(is_plist(head(output)));
    STATIC_EXPECT_EQ(hoisted<nothing_s{}>{}, plist_ref("value"_tag, head(output)).value);
    STATIC_EXPECT_EQ(""_hoist, plist_ref("sequence"_tag, head(output)).value);
  }

  TEST(parse, empty_fail){
    constexpr auto output = empty(hoisted_list<>{}, "abc"_hoist);
    STATIC_EXPECT_EQ(nothing, output);
  }

  TEST(parse, conj){
    constexpr auto output =
      conj(item, [ ](auto x){ return result(x); }, "abc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
  }

  TEST(parse, disj){
    constexpr auto output =
      disj(item, item, "abc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(2, length(output));
  }

  TEST(parse, disj_fail_first){
    constexpr auto output =
      disj(fail, item, "abc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
  }

  TEST(parse, disj_fail_second){
    constexpr auto output =
      disj(item, fail, "abc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
  }

  TEST(parse, disj_fail_both){
    constexpr auto output =
      disj(fail, fail, "abc"_hoist);
    STATIC_EXPECT_TRUE(is_nothing(output));
  }

  TEST(parse, alt){
    constexpr auto output =
      alt(result(hoisted<'a'>{}), item, "bc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
  }

  TEST(parse, fail_second){
    constexpr auto output =
      alt(result(hoisted<'a'>{}), fail, "bc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
  }

  TEST(parse, alt_fail_first){
    constexpr auto output =
      alt(fail, item, "bc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
  }

  TEST(parse, alt_fail_both){
    constexpr auto output =
      alt(fail, fail, "bc"_hoist);
    STATIC_EXPECT_TRUE(is_nothing(output));
  }

  TEST(parse, sat){
    constexpr auto output =
      sat([]<auto Value>(hoisted<Value>){ return Value == 'a'; },
          "abc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
  }

  TEST(parse, sat_fail){
    constexpr auto output =
      sat([]<auto Value>(hoisted<Value>){ return Value == 'b'; },
          "abc"_hoist);
    STATIC_EXPECT_TRUE(is_nothing(output));
  }

  TEST(parse, opt){
    constexpr auto opt = curry(nat<1>, [](auto parser){
      return alt(parser, result(nothing));
      });
    constexpr auto output = opt(sat([]<auto Value>(hoisted<Value>){ return Value == 'a'; }), "abc"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_EQ('a', parse_result_value(head(output)).value);
    STATIC_EXPECT_EQ("bc"_hoist, parse_result_sequence(head(output)));
  }

  TEST(parse, equal){
    constexpr auto parser =
      conj(equal<'a'>, [](auto){ return
      conj(equal<'b'>, [](auto){ return
      conj(equal<'c'>, [](auto){ return
            result(hoisted_list<'a', 'b', 'c'>{});
      }); }); });

    constexpr auto output = parser("abc"_hoist);

    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_EQ("abc"_hoist, parse_result_value(head(output)));
  }

  TEST(parse, seq){

    constexpr auto parser = seq<
      equal<'a'>,
      equal<'b'>,
      equal<'c'>>;

    constexpr auto output = parser("abc123"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_EQ("abc"_hoist, parse_result_value(head(output)));
  }


  TEST(parse, word){
    constexpr auto parser = word("abc"_hoist);
    constexpr auto output = parser("abc123"_hoist);
    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_EQ("abc"_hoist, parse_result_value(head(output)));
    STATIC_EXPECT_EQ("123"_hoist, parse_result_sequence(head(output)));
  }

  TEST(pare, opt){
    constexpr auto parser = opt(equal<'a'>);
    constexpr auto output = parser("abc123"_hoist);

    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_EQ(hoisted<'a'>{}, parse_result_value(head(output)));
  }

  TEST(parse, opt_not_present){
    constexpr auto parser = opt(equal<'q'>);
    constexpr auto output = parser("abc123"_hoist);

    STATIC_EXPECT_TRUE(is_nonempty_list(output));
    STATIC_EXPECT_EQ(1, length(output));
    STATIC_EXPECT_EQ(nothing, parse_result_value(head(output)));
  }

  // TEST(parse, many_match_3){
  //   constexpr auto parser = many(equal<'a'>);
  //   constexpr auto output = parser("aaabc"_hoist);
  //   STATIC_EXPECT_TRUE(is_nonempty_list(output));
  //   STATIC_EXPECT_EQ(1, length(output));
  //   STATIC_EXPECT_EQ("aaa"_hoist, parse_result_value(head(output)));
  //   STATIC_EXPECT_EQ("bc"_hoist, parse_result_sequence(head(output)));
  // }

  // TEST(parse, many_match_1){
  //   constexpr auto parser = many(equal<'a'>);
  //   constexpr auto output = parser("abc"_hoist);
  //   STATIC_EXPECT_TRUE(is_nonempty_list(output));
  //   STATIC_EXPECT_EQ(1, length(output));
  //   STATIC_EXPECT_EQ("a"_hoist, parse_result_value(head(output)));
  //   STATIC_EXPECT_EQ("bc"_hoist, parse_result_sequence(head(output)));
  // }

  // TEST(parse, many_match_0){
  //   constexpr auto parser = many(equal<'a'>);
  //   constexpr auto output = parser("bbc"_hoist);
  //   STATIC_EXPECT_TRUE(is_nonempty_list(output));
  //   STATIC_EXPECT_EQ(1, length(output));
  //   STATIC_EXPECT_EQ(hoisted_nothing, parse_result_value(head(output)));
  //   STATIC_EXPECT_EQ("bbc"_hoist, parse_result_sequence(head(output)));
  // }

  // TEST(parse, man1_match_3){
  //   constexpr auto parser = many1(equal<'a'>);
  //   constexpr auto output = parser("aaabc"_hoist);
  //   STATIC_EXPECT_TRUE(is_nonempty_list(output));
  //   STATIC_EXPECT_EQ(1, length(output));
  //   STATIC_EXPECT_EQ("aaa"_hoist, parse_result_value(head(output)));
  //   STATIC_EXPECT_EQ("bc"_hoist, parse_result_sequence(head(output)));
  // }


  // TEST(parse, many1_match_1){
  //   constexpr auto parser = many1(equal<'a'>);
  //   constexpr auto output = parser("abc"_hoist);
  //   STATIC_EXPECT_TRUE(is_nonempty_list(output));
  //   STATIC_EXPECT_EQ(1, length(output));
  //   STATIC_EXPECT_EQ("a"_hoist, parse_result_value(head(output)));
  //   STATIC_EXPECT_EQ("bc"_hoist, parse_result_sequence(head(output)));
  // }

  // TEST(parse, many1_match_0){
  //   constexpr auto parser = many1(equal<'a'>);
  //   constexpr auto output = parser("bbc"_hoist);
  //   STATIC_EXPECT_TRUE(is_nothing(output));
  // }

} // end of namespace ruse::testing
