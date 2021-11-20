#pragma once

//
// ... Testing header files
//
#include <gtest/gtest.h>

#define RUSE_QUOTE_AUX(...) #__VA_ARGS__
#define RUSE_QUOTE(...) RUSE_QUOTE_AUX(__VA_ARGS__)

#define RUSE_FORCE_SEMICOLON() static_assert(true, "")

#define STATIC_EXPECT_TRUE(...)                                                \
  do {                                                                         \
    static_assert(                                                             \
      (__VA_ARGS__),                                                           \
      "Expected " RUSE_QUOTE(__VA_ARGS__) " to evaluate to true.");            \
    EXPECT_TRUE((__VA_ARGS__));                                                \
  } while (false)

#define STATIC_EXPECT_FALSE(...)                                               \
  do {                                                                         \
    static_assert(                                                             \
      !(__VA_ARGS__),                                                          \
      "Expected " RUSE_QUOTE(__VA_ARGS__) " to evaluate to false.");           \
    EXPECT_FALSE((__VA_ARGS__));                                               \
  } while (false)

#define STATIC_EXPECT_EQ(a, b)                                                 \
  do {                                                                         \
    static_assert(                                                             \
      (a) == (b),                                                              \
      "Expected " RUSE_QUOTE(a) " to evaluate equal to " RUSE_QUOTE(b) ".");   \
    EXPECT_EQ((a), (b));                                                       \
  } while (false)
