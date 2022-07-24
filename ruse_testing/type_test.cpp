//
// ... Standard header files
//
#include <cmath>

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

  TEST(fmap, type){
    STATIC_EXPECT_EQ( type<double>, fmap([](auto x){ return std::sqrt(x); }, type<int>));
  }

  TEST(fapply, type){
    STATIC_EXPECT_EQ( type<double>, fapply(type_of([](auto x){ return std::sqrt(x); }), type<int>));
  }

  TEST(flatmap, type){
    STATIC_EXPECT_EQ( type<int>, flatmap(type_of, type<int>));
  }

  TEST(flatten, type){
    STATIC_EXPECT_EQ( type<int>, flatten(type_of(type<int>)));
  }

  TEST(letm, type){
    STATIC_EXPECT_EQ(
      type<double>,
      letm(type<int>,    [ ](auto x){ return
      letm(type<double>, [=](auto y){ return
            type_of(x + y);
      }); }));
  }

  TEST(letf, type){
    STATIC_EXPECT_EQ(
      type<double>,
      letf(type<int>, [](auto x){ return
            std::cos(x);
      }));
  }

} // end of namespace ruse::testing
