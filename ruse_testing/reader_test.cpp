//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/experimental/reader.hpp>

namespace ruse::experimental::testing {

  TEST(reader, make_reader) { STATIC_EXPECT_EQ(3, run('e', make_reader(3))); }

  TEST(reader, fmap)
  {
    STATIC_EXPECT_EQ(9, run('e', fmap(sqr, make_reader(3))));
  }

  TEST(reader, fapply)
  {
    STATIC_EXPECT_EQ(9, run('e', fapply(make_reader(sqr), make_reader(3))));
  }

  TEST(reader, flatmap)
  {
    STATIC_EXPECT_EQ(
      9, run('e', flatmap(compose(make_reader, sqr), make_reader(3))));
  }

  TEST(readeR, letm)
  {
    // clang-format off
    STATIC_EXPECT_EQ(
      7,
      run('e')(
        letm(make_reader(3), [=](auto x){ return
        letm(make_reader(4), [=](auto y){ return
              pure(x + y); }); })));
    // clang-format on
  }

  TEST(reader, ask) { STATIC_EXPECT_EQ('e', run('e', ask)); }

  TEST(reader, asks)
  {
    constexpr auto get_second = [](auto e) { return hlist_ref(1, e); };
    STATIC_EXPECT_EQ('b', run(list('a', 'b', 'c'))(asks(get_second)));
  }

  TEST(reader, local)
  {
    constexpr auto getx = [](auto e) { return plist_ref("x"_tag, e).value; };
    STATIC_EXPECT_EQ(
      9, run(list("x"_tag(3), "y"_tag(4)))(local(getx, asks(sqr))));
  }

  TEST(reader, letm_from_environment)
  {
    // clang-format off
    constexpr auto getx = asks(plist_ref_("x"_tag));
    constexpr auto gety = asks(plist_ref_("y"_tag));
    run(list("x"_tag(3), "y"_tag(4)))(
      letm(getx, [=](auto x){ return
      letm(gety, [=](auto y){ return
            pure(x+y); }); }));
    // clang-format on
  }

} // namespace ruse::experimental::testing
