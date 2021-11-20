//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>


//
// ... ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::testing
{

  TEST(Type, type_s){
    STATIC_EXPECT_TRUE(Type<type_s<int>>);
  }

  TEST(Type, int){
    STATIC_EXPECT_FALSE(Type<int>);
  }

  TEST(TypeProxy, type){
    STATIC_EXPECT_TRUE(TypeProxy<type_s<int>, int>);
  }

  TEST(TypeProxy, wrong_type){
    STATIC_EXPECT_FALSE(TypeProxy<type_s<double>, int>);
  }

  TEST(TypeProxy, something_else){
    STATIC_EXPECT_FALSE(TypeProxy<double, int>);
  }





} // end of namespace ruse::testing
