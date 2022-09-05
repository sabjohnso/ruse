//
// ... Standard header files
//
#include <sstream>

//
// ... Testing header files
//
#include <gtest/gtest.h>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>
#include <ruse/show.hpp>

namespace ruse::testing {

   TEST(show, nothing) {
      std::stringstream ss;
      ss << nothing;
      EXPECT_EQ(ss.str(), "nothing");
   };

   TEST(show, hlist) {
      std::stringstream ss;
      ss << list(1, 2, 3);
      EXPECT_EQ(ss.str(), "list(1,2,3)");
   }

   TEST(show, string) {
      std::stringstream ss;
      ss << "abc"_s;
      EXPECT_EQ(ss.str(), "abc");
   }

   TEST(show, tag) {
      std::stringstream ss;
      ss << "abc"_tag;
      EXPECT_EQ(ss.str(), "\"abc\"_tag");
   }

   // TEST(show, plist)
   // {
   //   std::stringstream ss;
   //   ss << list("x"_tag(1), "y"_tag(2));
   //   EXPECT_EQ(ss.str(), "");
   // }

} // end of namespace ruse::testing
