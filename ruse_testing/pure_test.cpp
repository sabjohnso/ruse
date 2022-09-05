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

   TEST(pure, construction) { STATIC_EXPECT_TRUE(is_pure(pure(3))); };

   TEST(pure, fmap) {
      EXPECT_EQ(pure(9), fmap([](auto x) { return x * x; }, pure(3)));
   }

   TEST(pure, fapply) {
      EXPECT_EQ(
        pure(9),
        fapply(pure([](auto x) { return x * x; }), pure(3)));
   }

   TEST(pure, flatmap) { EXPECT_EQ(pure(3), flatmap(pure, pure(3))); }

   TEST(pure, flatten) { EXPECT_EQ(pure(3), flatten(pure(pure(3)))); }

   TEST(pure, letm) {
      // clang-format off
    STATIC_EXPECT_EQ(
      pure(7),
      letm(pure(3), [=](auto x){ return
      letm(pure(4), [=](auto y){ return
            pure(x+y); }); }));
      // clang-format on
   }

   TEST(pure, extract) { STATIC_EXPECT_EQ(3, extract(pure(3))); }

   TEST(pure, duplicate) {
      STATIC_EXPECT_EQ(pure(pure(3)), duplicate(pure(3)));
   }

   TEST(pure, extend) {
      STATIC_EXPECT_EQ(pure(3), extend(extract, pure(3)));
   }

} // end of namespace ruse::testing
