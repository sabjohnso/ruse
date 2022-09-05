#pragma once

//
// ... ruse header files
//
#include <ruse/reference/list.hpp>
#include <ruse/reference/string.hpp>

namespace ruse::experimental {
   using namespace ruse::reference;

   template<typename T>
   struct equality {
      friend constexpr bool
      operator==(T, T) {
         return true;
      }

      template<typename U>
      friend constexpr bool
      operator==(T, U) {
         return false;
      }

      template<typename U>
      friend constexpr bool
      operator!=(T x, U y) {
         return ! (x == y);
      }
   };

   template<typename... Rules>
   struct grammar : equality<grammar<Rules...>> {
      constexpr grammar() = default;
      constexpr grammar(Rules...) {}

      static constexpr auto rules = list(Rules{}...);
   };

   template<typename... Rules>
   grammar(Rules...) -> grammar<Rules...>;

   template<typename Name, typename... Alternatives>
   struct production_rule
      : equality<production_rule<Name, Alternatives...>> {
      static constexpr Name name{};
      static constexpr auto alternatives = list(Alternatives{}...);

      template<typename T>
      friend constexpr production_rule<Name, Alternatives..., T>
      operator|(production_rule, T) {
         return {};
      }
   };

   template<typename... Forms>
   struct sequence : equality<sequence<Forms...>> {
      static constexpr auto forms = list(Forms{}...);

      template<typename... OtherForms>
      friend constexpr sequence<Forms..., OtherForms...>
      operator,(sequence, sequence<OtherForms...>) {
         return {};
      }

      template<typename T>
      friend constexpr sequence<Forms..., T>
      operator,(sequence, T) {
         return {};
      }
   };

   template<typename Form>
   struct modifier {
      static constexpr Form form{};
   };

   template<typename Form>
   struct zero_or_more
      : modifier<Form>
      , equality<zero_or_more<Form>> {
      template<typename T>
    friend constexpr auto
    operator,(zero_or_more, T)
    {
         return (sequence<zero_or_more>{}, T{});
      }
   };

   template<typename Form>
   struct one_or_more
      : modifier<Form>
      , equality<one_or_more<Form>> {
      template<typename T>
    friend constexpr auto
    operator,(one_or_more, T)
    {
         return (sequence<one_or_more>{}, T{});
      }
   };

   template<typename Form>
   struct zero_or_one
      : modifier<Form>
      , equality<zero_or_one<Form>> {
      template<typename T>
    friend constexpr auto
    operator,(zero_or_one, T)
    {
         return (sequence<zero_or_one>{}, T{});
      }
   };

   template<typename T>
   struct modifiable {
      friend constexpr zero_or_more<T>
      operator*(T) {
         return {};
      }

      friend constexpr one_or_more<T>
      operator+(T) {
         return {};
      }

      friend constexpr zero_or_one<T>
      operator~(T) {
         return {};
      }
   };

   template<fixed_string Str>
   struct raised_string {
      static constexpr auto value = Str;
   };

   template<fixed_string... Lines>
   struct comment_s : equality<comment_s<Lines...>> {
      static constexpr auto lines = list(Lines...);
   };

   template<fixed_string Str>
   struct terminal
      : raised_string<Str>
      , equality<terminal<Str>>
      , modifiable<terminal<Str>> {
      template<typename T>
    friend constexpr auto
    operator,(terminal, T)
    {
         return (sequence<terminal>{}, T{});
      }
   };

   template<fixed_string Str>
   struct semiterminal
      : raised_string<Str>
      , equality<semiterminal<Str>>
      , modifiable<semiterminal<Str>> {

      template<typename T>
    friend constexpr auto
    operator,(semiterminal, T)
    {
         return (sequence<semiterminal>{}, T{});
      }
   };

   template<fixed_string Str>
   struct nonterminal
      : raised_string<Str>
      , equality<nonterminal<Str>>
      , modifiable<nonterminal<Str>> {
      template<typename T>
      constexpr production_rule<nonterminal, T>
      operator<=>(T) const {
         return {};
      }

      template<typename T>
    friend constexpr auto
    operator,(nonterminal, T)
    {
         return (sequence<nonterminal>{}, T{});
      }
   };

   struct empty_s {
   } constexpr empty{};

   //
   // ... literals
   //
   template<fixed_string Line>
   constexpr comment_s<Line> operator""_comment() {
      return {};
   }

   template<fixed_string Str>
   constexpr nonterminal<Str> operator""_n() {
      return {};
   }

   template<fixed_string Str>
   constexpr terminal<Str> operator""_t() {
      return {};
   }

   template<fixed_string Str>
   constexpr semiterminal<Str> operator""_s() {
      return {};
   }

   constexpr auto seq = []<typename... Forms>(Forms...) {
      return sequence<Forms...>{};
   };

   //
   // ... terse constant names
   //

   template<fixed_string... Lines>
   constexpr comment_s<Lines...> comment{};

   template<fixed_string Str>
   constexpr nonterminal<Str> n{};

   template<fixed_string Str>
   constexpr terminal<Str> t{};

   template<fixed_string Str>
   constexpr semiterminal<Str> s{};

   template<fixed_string Str>
   constexpr auto rule = []<typename... Alternatives>(Alternatives...) {
      return production_rule<nonterminal<Str>, Alternatives...>{};
   };

   constexpr auto parens = []<typename... Forms>(Forms...) {
      return ("("_t, sequence<Forms...>{}, ")"_t);
   };

   constexpr auto brackets = []<typename... Forms>(Forms...) {
      return ("["_t, sequence<Forms...>{}, "]"_t);
   };

   constexpr auto braces = []<typename... Forms>(Forms...) {
      return ("{"_t, sequence<Forms...>{}, "}"_t);
   };

   constexpr auto double_brackets = []<typename... Forms>(Forms...) {
      return ("[["_t, sequence<Forms...>{}, "]]"_t);
   };

   constexpr auto oxford_brackets = []<typename... Forms>(Forms...) {
      return ("[|"_t, sequence<Forms...>{}, "|]"_t);
   };

   template<typename... Forms>
   struct union_s {
      static constexpr auto forms = list(Forms{}...);
   };

   template<typename... Forms>
   struct intersection_s {
      static constexpr auto forms = list(Forms{}...);
   };

   template<typename... Forms>
   struct difference_s {
      static constexpr auto forms = list(Forms{}...);
   };

   template<typename... Forms>
   struct symmetric_difference_s {
      static constexpr auto forms = list(Forms{}...);
   };

   template<typename Name, typename... Alternatives>
   std::ostream&
   operator<<(
     std::ostream& os,
     production_rule<Name, Alternatives...>) {
      os << Name{};
      if constexpr(sizeof...(Alternatives) > 0) {
         constexpr auto xs = list(Alternatives{}...);
         os << " <=> " << head(xs);
         dolist(tail(xs), [ & ](auto x) { os << " | " << x; });
      }
      return os;
   }

   template<typename Form>
   std::ostream&
   operator<<(std::ostream& os, zero_or_more<Form>) {
      return os << "*" << Form{};
   }

   template<typename Form>
   std::ostream&
   operator<<(std::ostream& os, one_or_more<Form>) {
      return os << "+" << Form{};
   }

   template<typename Form>
   std::ostream&
   operator<<(std::ostream& os, zero_or_one<Form>) {
      return os << "~" << Form{};
   }

   template<typename... Forms>
   std::ostream&
   operator<<(std::ostream& os, sequence<Forms...>) {
      constexpr auto xs = list(Forms{}...);
      os << "(";
      os << head(xs);
      dolist(tail(xs), [ & ](auto x) { os << ", " << x; });
      os << ")";
      return os;
   }

   template<fixed_string Str>
   std::ostream&
   operator<<(std::ostream& os, nonterminal<Str> n) {
      os << "n<\"" << n.value.value << "\">";
      return os;
   }

   template<fixed_string Str>
   std::ostream&
   operator<<(std::ostream& os, terminal<Str> t) {
      os << "t<\"" << t.value.value << "\">";
      return os;
   }

   template<fixed_string Str>
   std::ostream&
   operator<<(std::ostream& os, semiterminal<Str> s) {
      os << "s<\"" << s.value.value << "\">";
      return os;
   }

} // end of namespace ruse::experimental
