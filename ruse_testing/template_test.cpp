//
// ... Standard header files
//
#include <vector>

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

  TEST(template_proxy, concept_affirmative)
  {
    STATIC_EXPECT_TRUE(TemplateProxy<Template<std::vector>>);
  }

  TEST(template_proxy, concept_negative)
  {
    STATIC_EXPECT_FALSE(TemplateProxy<Type<std::vector<int>>>);
  }

} // end of namespace ruse::testing
