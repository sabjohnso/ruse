//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::testing {

   TEST(reader, Reader) { STATIC_EXPECT_TRUE(Reader<decltype(ask)>); }

   TEST(reader, ReaderOf) {
      STATIC_EXPECT_TRUE(ReaderOf<decltype(ask), int>);
   }

   TEST(reader, make_reader) {
      STATIC_EXPECT_EQ(3, run_reader('e', make_reader(3)));
   }

   TEST(reader, fmap) {
      STATIC_EXPECT_EQ(9, run_reader('e', fmap(sqr, make_reader(3))));
   }

   TEST(reader, fapply) {
      STATIC_EXPECT_EQ(
        9,
        run_reader('e', fapply(make_reader(sqr), make_reader(3))));
   }

   TEST(reader, flatmap) {
      STATIC_EXPECT_EQ(
        9,
        run_reader(
          'e',
          flatmap(compose(make_reader, sqr), make_reader(3))));
   }

   TEST(readeR, letm) {
      // clang-format off
    STATIC_EXPECT_EQ(
      7,
      run_reader('e')(
        letm(make_reader(3), [=](auto x){ return
        letm(make_reader(4), [=](auto y){ return
              pure(x + y); }); })));
      // clang-format on
   }

   TEST(reader, ask) { STATIC_EXPECT_EQ('e', run_reader('e', ask)); }

   TEST(reader, asks) {
      constexpr auto get_second = [](auto e) {
         return hlist_ref(1, e);
      };
      STATIC_EXPECT_EQ(
        'b',
        run_reader(list('a', 'b', 'c'))(asks(get_second)));
   }

   TEST(reader, local) {
      constexpr auto getx = [](auto e) {
         return plist_ref("x"_tag, e).value;
      };
      STATIC_EXPECT_EQ(
        9,
        run_reader(list("x"_tag(3), "y"_tag(4)))(
          local(getx, asks(sqr))));
   }

   TEST(reader, letm_from_environment) {
      // clang-format off
    constexpr auto getx = asks(plist_ref_("x"_tag));
    constexpr auto gety = asks(plist_ref_("y"_tag));
    run_reader(list("x"_tag(3), "y"_tag(4)))(
      letm(getx, [=](auto x){ return
      letm(gety, [=](auto y){ return
            pure(x+y); }); }));
      // clang-format on
   }

} // namespace ruse::testing
