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
  TEST(tag, construction) { STATIC_EXPECT_TRUE(is_tag("abc"_tag)); }

  TEST(tagged, construction) { STATIC_EXPECT_TRUE(is_tagged("abc"_tag(4))); }

  TEST(tagged, fmap)
  {
    STATIC_EXPECT_EQ(
      "abc"_tag(4), fmap([](auto x) { return x + 1; }, "abc"_tag(3)));
  }

  TEST(tagged, fapply)
  {
    STATIC_EXPECT_EQ(
      "abc"_tag(4),
      fapply("abc"_tag([](auto x) { return x + 1; }), "abc"_tag(3)));
  }

  TEST(tagged, flatmap)
  {
    STATIC_EXPECT_EQ("abc"_tag(1), flatmap("abc"_tag, "abc"_tag(1)));
  }

  TEST(tagged, flatten)
  {
    STATIC_EXPECT_EQ("abc"_tag(1), flatten("abc"_tag("abc"_tag(1))));
  }

  TEST(tagged, letm){
    STATIC_EXPECT_EQ(
      "c"_tag(4),
      letm("a"_tag(1), [ ](auto x){ return
      letm("b"_tag(3), [=](auto y){ return
            "c"_tag(x+y);
      }); }));
  }

} // end of namespace ruse::testing
