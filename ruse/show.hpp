#pragma once

//
// ... Standard header files
//
#include <string>
#include <regex>
#include <iostream>

//
// ... ruse header files
//
#include <ruse/ruse.hpp>


namespace ruse
{
  template<reference::Vacuous T>
  std::ostream&
  operator<<(std::ostream& os, T x){
    std::string str = [](auto){ return __PRETTY_FUNCTION__; }(x);
    std::smatch sm;
    if(std::regex_match(str, sm, std::regex(".*::(.*)\\]"))){
      return os << sm[1];
    } else {
      throw std::logic_error("Yikes");
    }
    return os << str;
  }

  template<auto ... Values, template< auto ...> class K>
  void
  print_delimited(std::ostream& os, K<Values...>, auto delimiter){
    constexpr auto values = list(Values ...);
    if constexpr (is_nonempty_list(values)){
      os << head(values);
      dolist(tail(values), [&](auto value){
        os << delimiter << value;
      });
    }
  }

  template<auto Parser, auto ... Parsers>
  std::ostream&
  operator<<(std::ostream& os, reference::sequence_parser<Parser, Parsers ...> parser){
    os << "sequence_parser<";
    print_delimited(os, parser);
    os << ">{}";
    return os;
  };

  template<typename T>
  std::ostream&
  operator<<(std::ostream& os, reference::result_parser<T>){
    os << "result(" << T{} << ")";
    return os;
  }

  std::ostream&
  operator<<(std::ostream& os, reference::item_parser){
    return os << "item";
  }

  template<typename T>
  std::ostream&
  operator<<(std::ostream&os, reference::empty_parser<T>){
    return os << "empty(" << T{} <<  ")";
  }

  template<auto Value>
  std::ostream&
  operator <<(std::ostream& os, hoisted<Value>){
    return os << "hoisted<" << Value << ">{}";
  }


  template<auto ... Values>
  std::ostream&
  operator <<(std::ostream& os, hoisted_list<Values...>){
    constexpr auto values = list(Values ...);
    auto aux = [&]<List T> (auto recur, T xs) -> void {
      if constexpr (NonemptyList<T>){
        os << "," << head(xs);
        return recur(recur, tail(xs));
      }
    };

    if constexpr (0 == sizeof ... (Values)){
      return os << "hoisted_list<>{}";
    } else {
      os << "hoisted_list<" << head(values);
      reference::u(aux, tail(values));
      return os << ">{}";
    }
  }


  template<List T>
  std::ostream&
  operator <<(std::ostream& os, T xs){
    constexpr auto aux = []<List U>(auto recur, std::ostream& os, U xs) -> std::ostream&{
      if constexpr(Pair<U>){
        return recur(recur, os << "," << head(xs), tail(xs));
      } else {
        return os << ")";
      }
    };
    if constexpr (Nothing<T>){
      return os << "nothing";
    } else {
      return aux(aux, os << "list(" << head(xs), tail(xs));
    }
  }

} // end of namespace ruse
