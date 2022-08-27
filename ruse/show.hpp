#pragma once

//
// ... Standard header files
//
#include <iostream>
#include <regex>
#include <string>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::reference {

  template<Enum T>
  std::ostream&
  operator<<(std::ostream& os, T const)
  {
    return os << "enum_value";
  }

  template<typename T, typename Name>
  std::ostream&
  operator<<(std::ostream& os, tagged<T, Name> x)
  {
    return os << tag<Name>{} << "(" << x.value << ")";
  }

  template<String T>
  std::ostream&
  operator<<(std::ostream& os, const T& str)
  {
    if constexpr (length_type(type<T>) == 1) {
      return os << car(str);
    } else {
      return os << car(str) << cdr(str);
    }
  }

  template<char... cs>
  std::ostream&
  operator<<(std::ostream& os, const hoisted_list<cs...>& x)
  {
    return os << x.values;
  }

  template<HoistedString T>
  std::ostream&
  operator<<(std::ostream& os, tag<T>)
  {
    return os << "\"" << T::values << "\"_tag";
  }

  template<Vacuous T>
  std::ostream&
  operator<<(std::ostream& os, T x)
  {
    std::string str = [](auto) { return __PRETTY_FUNCTION__; }(x);
    std::smatch sm;
    if (std::regex_match(str, sm, std::regex(".*::(.*)\\]"))) {
      return os << sm[1];
    } else {
      throw std::logic_error("Yikes");
    }
    return os << str;
  }

  template<auto... Values, template<auto...> class K>
  void
  print_delimited(std::ostream& os, K<Values...>, auto delimiter)
  {
    constexpr auto values = list(Values...);
    if constexpr (is_nonempty_list(values)) {
      os << head(values);
      dolist(tail(values), [&](auto value) { os << delimiter << value; });
    }
  }

  template<auto Parser, auto... Parsers>
  std::ostream&
  operator<<(std::ostream& os, sequence_parser<Parser, Parsers...> parser)
  {
    os << "sequence_parser<";
    print_delimited(os, parser);
    os << ">{}";
    return os;
  };

  template<typename T>
  std::ostream&
  operator<<(std::ostream& os, result_parser<T>)
  {
    os << "result(" << T{} << ")";
    return os;
  }

  std::ostream&
  operator<<(std::ostream& os, item_parser)
  {
    return os << "item";
  }

  template<typename T>
  std::ostream&
  operator<<(std::ostream& os, empty_parser<T>)
  {
    return os << "empty(" << T{} << ")";
  }

  template<auto Value>
  std::ostream&
  operator<<(std::ostream& os, hoisted<Value>)
  {
    return os << "hoisted<" << Value << ">{}";
  }

  template<NonstringHoistedList T>
  std::ostream&
  operator<<(std::ostream& os, T)
  {
    constexpr auto values = T::values;
    auto aux = [&]<List U>(auto recur, T xs) -> void {
      if constexpr (NonemptyList<U>) {
        os << "," << head(xs);
        return recur(recur, tail(xs));
      }
    };

    if constexpr (0 == length(T::values)) {
      return os << "hoisted_list<>{}";
    } else {
      os << "hoisted_list<" << head(values);
      u(aux, tail(values));
      return os << ">{}";
    }
  }

  std::ostream&
  operator<<(std::ostream& os, const nothing_s&)
  {
    return os << "nothing";
  }

  template<NonstringList T>
  std::ostream&
  operator<<(std::ostream& os, T xs)
  {
    static_assert(!String<T>);
    constexpr auto aux =
      []<List U>(auto recur, std::ostream& os, U xs) -> std::ostream& {
      if constexpr (Pair<U>) {
        return recur(recur, os << "," << head(xs), tail(xs));
      } else {
        return os << ")";
      }
    };
    if constexpr (Nothing<T>) {
      return os << "nothing";
    } else {
      return aux(aux, os << "list(" << head(xs), tail(xs));
    }
  }

} // namespace ruse::reference

namespace ruse {
  using reference::operator<<;
} // namespace ruse
