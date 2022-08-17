//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <ruse_testing/test_macros.hpp>

//
// ... ruse header files
//
#include <ruse/experimental/grammar.hpp>

namespace ruse::experimental::testing {
  TEST(comment, construction)
  {
    STATIC_EXPECT_EQ(
      R"(
Definitions include variable and syntax definitions, begin forms containing zero or
more definitions, let-syntax and letrec-syntax forms expanding into zero or more
definitions, and derived definitions. Derived definitions are syntactic extensions
that expand into some form of definition. A transformer expression is a syntax-rules
form or some other expression that produces a transformer.
)"_comment,
      comment<R"(
Definitions include variable and syntax definitions, begin forms containing zero or
more definitions, let-syntax and letrec-syntax forms expanding into zero or more
definitions, and derived definitions. Derived definitions are syntactic extensions
that expand into some form of definition. A transformer expression is a syntax-rules
form or some other expression that produces a transformer.
)">);
  }

  TEST(nonterminal, construction)
  {
    STATIC_EXPECT_EQ("program"_n, n<"program">);
  }

  TEST(semiterminal, construction) { STATIC_EXPECT_EQ("[A-Z]"_s, s<"[A-Z]">); }

  TEST(terminal, construction) { STATIC_EXPECT_EQ("begin"_t, t<"begin">); }

  TEST(zero_or_more, terminal)
  {
    EXPECT_EQ(zero_or_more<terminal<"begin">>{}, *t<"begin">);
  }

  TEST(zero_or_more, semiterminal)
  {
    EXPECT_EQ(zero_or_more<semiterminal<"[A-Z]">>{}, *s<"[A-Z]">);
  }

  TEST(zero_or_more, nonterminal)
  {
    STATIC_EXPECT_EQ(zero_or_more<nonterminal<"form">>{}, *n<"form">);
  }

  TEST(one_or_more, terminal)
  {
    EXPECT_EQ(one_or_more<terminal<"begin">>{}, +t<"begin">);
  }

  TEST(one_or_more, semiterminal)
  {
    EXPECT_EQ(one_or_more<semiterminal<"[A-Z]">>{}, +s<"[A-Z]">);
  }

  TEST(one_or_more, nonterminal)
  {
    STATIC_EXPECT_EQ(one_or_more<nonterminal<"form">>{}, +n<"form">);
  }

  TEST(zero_or_one, terminal)
  {
    EXPECT_EQ(zero_or_one<terminal<"begin">>{}, ~t<"begin">);
  }

  TEST(zero_or_one, semiterminal)
  {
    EXPECT_EQ(zero_or_one<semiterminal<"[A-Z]">>{}, ~s<"[A-Z]">);
  }

  TEST(zero_or_one, nonterminal)
  {
    STATIC_EXPECT_EQ(zero_or_one<nonterminal<"form">>{}, ~n<"form">);
  }

  TEST(sequence, terminal_terminal)
  {
    STATIC_EXPECT_EQ(
      (sequence<terminal<"(">, terminal<"begin">>{}), (t<"(">, t<"begin">));
  }

  TEST(sequence, multiple_tokens)
  {
    constexpr auto parens = []<typename... Forms>(Forms...)
    {
      return (t<"(">, sequence<Forms...>{}, t<")">);
    };
    STATIC_EXPECT_EQ(
      (sequence<
        terminal<"(">,
        nonterminal<"define-syntax">,
        nonterminal<"keyword">,
        nonterminal<"transformer expression">,
        terminal<")">>{}),
      parens(n<"define-syntax">, n<"keyword">, n<"transformer expression">));
  }

  TEST(production_rule, construction)
  {
    STATIC_EXPECT_EQ(
      (production_rule<
        nonterminal<"expression">,
        nonterminal<"constant">,
        nonterminal<"variable">,
        sequence<
          terminal<"(">,
          terminal<"quote">,
          nonterminal<"datum">,
          terminal<")">>>{}),
      n<"expression"> <=> n<"constant"> | n<"variable"> |
        (t<"(">, t<"quote">, n<"datum">, t<")">));
  }

  TEST(grammar, construction)
  {

    EXPECT_EQ(
      (grammar<
        production_rule<
          nonterminal<"program">,
          zero_or_more<nonterminal<"form">>>,
        production_rule<
          nonterminal<"form">,
          nonterminal<"definition">,
          nonterminal<"expression">>>{}),
      (grammar{
        n<"program"> <=> *n<"form">,
        n<"form"> <=> n<"definition"> | n<"expression">}));
  }

  TEST(grammar, scheme)
  {
    constexpr auto define = "define"_t;
    constexpr auto definesyntax = "define-syntax"_t;
    constexpr auto lambda = "lambda"_t;
    constexpr auto begin = "begin"_t;
    constexpr auto letsyntax = "let-syntax"_t;
    constexpr auto letrecsyntax = "letrec-syntax"_t;
    constexpr auto quote = "quote"_t;
    constexpr auto plus = "+"_t;
    constexpr auto minus = "-"_t;

    constexpr auto dot = "."_t;
    // clang-format off
    constexpr auto scheme = grammar{

      rule<"program">(*"form"_n),

      rule<"form">("definition"_n, "expression"_n),

      comment<
        "*Definitions*.  Definitions include variable and syntax definitions, begin forms containing",
        "zero or more definitions, let-syntax and letrec-syntax forms expanding into zero or more",
        "definitions, and derived definitions. Derived definitions are syntactic extensions that",
        "expand into some form of definition. A transformer expression is a syntax-rules form or",
        "some other expression that produces a transformer.">,

      rule<"definition">(
        "definition"_n,
        "variable defintion"_n,
        "syntax defintion"_n,
        parens(begin, *"definition"_n),
        parens(letsyntax, parens(*"syntax binding"_n), *"definition"_n),
        parens(letrecsyntax, parens(*"syntax binding"_n), *"definition"_n),
        "derived_definition"_n),

      rule<"variable definition">(
        parens(define, "variable"_n, "expression"_n),
        parens(define, parens(+"variable"_n), "body"_n),
        parens(define, parens(+"variable"_n, dot, "variable"_n), "body"_n)),

      rule<"variable">("identifier"_n),

      rule<"body">((*"definition"_n, +"expression"_n)),

      rule<"syntax definition">(parens(definesyntax, "keyword"_n, "transformer expression"_n)),

      rule<"keyword">("identifier"_n),

      rule<"syntax binding">(parens("keyword"_n, "transformer expression"_n)),

      comment<
        "*Expressions*.  Expressions include core expressions, let-syntax or letrec-syntax forms expanding",
        "into a sequence of one or more expressions, and derived expressions. The core expressions are",
        "self-evaluating constants, variable references, applications, and quote, lambda, if, and set! expressions.",
        "Derived expressions include and, begin, case, cond, delay, do, let, let*, letrec, or, and quasiquote",
        "expressions plus syntactic extensions that expand into some form of expression.">,

      rule<"expression">(
        "constant"_n,
        "variable"_n,
        parens(quote, "datum"_n),
        ("'"_t, "datum"_n),
        parens(lambda, "formals"_n, "body"_n),
        parens("if"_t, "expression"_n, "expression"_n, "expression"_n),
        parens("if"_t, "expression"_n, "expression"_n),
        parens("set!"_t, "variable", "expression"_n),
        "application"_n,
        parens(letsyntax, parens(*"syntax binding"_n), +"expression"_n),
        parens(letrecsyntax, parens(*"syntax binding"_n), +"expression"_n),
        "derived expression"_n),

      rule<"constant">("boolean"_n, "number"_n, "character"_n, "string"_n),

      rule<"formals">(
        "variable"_n,
        parens(*"variable"_n),
        parens(+"variable"_n, dot, "variable"_n)),

      rule<"application">(parens("expression"_n)),

      comment<
        "*Identifiers*.  Identifiers may denote variables, keywords, or symbols, depending upon context.",
        "They are formed from sequences of letters, digits, and special characters. With three exceptions",
        "identifiers cannot begin with a character that can also begin a number, i.e., they cannot begin",
        "with ., +, -, or a digit. The three exceptions are the identifiers ..., +, and -. Case is insignificant",
        "in symbols so that, for example, newspaper, NewsPaper, and NEWSPAPER all represent the same identifier.">,

      comment<
        "*Data*.  Data include booleans, numbers, characters, strings, symbols, lists, and vectors. Case is",
        "insignificant in the syntax for booleans, numbers, and character names, but it is significant in",
        "other character constants and in strings. For example, #T is equivalent to #t, #E1E3 is equivalent",
        "to #e1e3, #X2aBc is equivalent to #x2abc, and #\\NewLine is equivalent to #\\newline; but #\\A is distinct",
        "from #\\a and \"String\" is distinct from \"string\".">,

      comment<
        "Numbers.  Numbers can appear in one of four radixes: 2, 8, 10, and 16, with 10 the default. The first",
        "several of productions below are parameterized by the radix, r, and each represents four productions,",
        "one for each of the four possible radixes. Numbers that contain radix points or exponents are constrained",
        "to appear in radix 10, so <decimal r> is valid only when r is 10.">,

      rule<"num(r)">(
        "real(r)"_n,
        ("real(r)"_n, "@"_t, "real(r)"_n),
        ("real(r)"_n, plus, "imag(r)"_n),
        ("real(r)"_n, minus, "imag(r)"_n),
        (plus, "imag(r)"_n),
        (minus, "imag(r)"_n)),

      rule<"imag(r)">("i"_t, "ureal(r)"_n),

      rule<"real(r)">(("sign"_n, "ureal(r)"_n)),

      rule<"ureal(r)">("integer(r)"_n, ("integer(r)"_n, "/"_t, "integer(r)"_n), "decimal(r)"_n),

      rule<"uinteger(r)">((+"digit(r)"_n, *"#"_t)),

      rule<"prefix(r::rad)">(("radix(r::rad)"_n, "exactness"_n), ("exactness"_n, "radix(r::rad)"_n)),

      rule<"suffix">("exponent"_n, empty),

      rule<"exponent">(("exponent marker"_n, "sign"_n, +"digit(10)"_n)),

      rule<"exponent marker">("e"_t, "s"_t, "f"_t, "d"_t),

      rule<"sign">("+"_t, "-"_t, empty),

      rule<"exactness">("#i"_t, "#e"_t, empty),

      rule<"radix(2)">("#b"_t),

      rule<"radix(8)">("#o"_t),

      rule<"radix(10)">("#d"_t, empty),

      rule<"radix(16)">("#x"_t),

      rule<"digit(2)">("[01]"_s),

      rule<"digit(8)">("[0-7]"_s),

      rule<"digit(10)">("[0-9]"_s),

      rule<"digit(16)">("[0-9a-f]"_s)};
    EXPECT_EQ(37, length(scheme.rules));
  }
} // end of namespace ruse::experimental::testing
