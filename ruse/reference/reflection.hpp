#pragma once

//
// ... ruse header files
//
#include <ruse/reference/functional.hpp>
#include <ruse/reference/import.hpp>
#include <ruse/reference/string.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {

  template<typename T>
  concept Enum = is_enum_v<T>;

  template<typename T>
  constexpr auto
  pretty_function(T)
  {
    return string_view{__PRETTY_FUNCTION__};
  }

  constexpr auto
  strip_garbage(string_view str)
  {
    const auto start = str.find_last_of('<') + 1;
    const auto stop = str.find_last_of('>');
    return str.substr(start, stop - start);
  }

  template<typename T>
  constexpr auto
  get_type_name(Type<T>)
  {
    return strip_garbage(pretty_function(type<T>));
  }

  template<Enum auto x>
  constexpr auto
  get_enum_name(hoisted<x>)
  {
    return strip_garbage(pretty_function(hoisted<x>{}));
  };

  constexpr integer
  string_view_count(string_view pattern, string_view str)
  {
    const auto recur =
      [=](auto recur, string_view str, integer accum) -> integer {
      const auto pos = str.find(pattern);
      return pos == string_view::npos
               ? accum
               : recur(
                   recur,
                   str.substr(pos + std::size(pattern), string_view::npos),
                   accum + 1);
    };
    return recur(recur, str, 0);
  }

  constexpr integer
  string_view_find_nth_from_back(
    string_view pattern,
    integer n,
    string_view str)
  {
    const auto recur = [=](
                         auto recur,
                         string_view str,
                         integer m,
                         integer accum_from_back) -> integer {
      return m ? [=](auto pos) {
        return recur(
          recur,
          str.substr(0, pos - std::size(pattern)),
          m - 1,
          accum_from_back + std::size(str) + std::size(pattern) - pos -1);
      }(str.find_last_of(pattern))
        : accum_from_back;
    };

    return std::size(str) + 1 - recur(recur, str, n, 0);
  }

  constexpr bool
  enum_name_has_namespace(string_view enum_name)
  {
    return string_view_count(string_view{"::"}, enum_name) > 1;
  }

  constexpr auto
  strip_enum_namespace(string_view enum_name)
  {
    return enum_name_has_namespace(enum_name)
             ? enum_name.substr(
                 string_view_find_nth_from_back(
                   string_view{"::"}, 2, enum_name),
                 string_view::npos)
             : enum_name;
  }

  template<Enum auto x>
  constexpr auto
  get_short_enum_name(hoisted<x>)
  {
    return strip_enum_namespace(get_enum_name(hoisted<x>{}));
  }

  constexpr string_view
  strip_qualification(string_view str)
  {
    return str.substr(
      str.find_last_of(string_view("::")) + 1, string_view::npos);
  }

  template<Enum auto x>
  constexpr auto
  get_unqualified_enum_name(hoisted<x>)
  {
    return strip_qualification(get_enum_name(hoisted<x>{}));
  }

  template<typename T>
  constexpr auto type_name = get_type_name(type<T>);

  template<typename T>
  concept Aggregate = is_aggregate_v<T>;

  template<typename T, typename... Args>
  concept AggregateInitializable = Aggregate<T> && requires
  {
    T{std::declval<Args>()...};
  };

  struct anything
  {
    template<typename T>
    constexpr operator T() const
    {
      return {};
    }
  };

  constexpr auto count_aggregate_members = []<typename T>(Type<T>) {
    constexpr auto recur = []<typename... Args>(auto recur, Args... args)
    {
      if constexpr (AggregateInitializable<T, Args...>) {
        return recur(recur, anything{}, args...);
      } else {
        return sizeof...(Args) - 1;
      }
    };
    return recur(recur);
  };

  template<Aggregate T>
  constexpr auto aggregate_member_count = count_aggregate_members(type<T>);

  template<typename T>
  constexpr auto
  get_aggregate_member_types(Type<T>)
  {
    constexpr T example{};
    constexpr integer n = aggregate_member_count<T>;
    if constexpr (n == 0) {
      return nothing;
    } else if constexpr (n == 1) {
      return decltype([=] {
        auto [x1] = example;
        return list(type<decltype(x1)>);
      }()){};

    } else if constexpr (n == 2) {
      return decltype([=] {
        auto [x1, x2] = example;
        return list(type<decltype(x1)>, type<decltype(x2)>);
      }()){};

    } else if constexpr (n == 3) {
      return decltype([=] {
        auto [x1, x2, x3] = example;
        return list(type<decltype(x1)>, type<decltype(x2)>, type<decltype(x3)>);
      }()){};

    } else if constexpr (n == 4) {
      return decltype([=] {
        auto [x1, x2, x3, x4] = example;
        return list(
          type<decltype(x1)>,
          type<decltype(x2)>,
          type<decltype(x3)>,
          type<decltype(x4)>);
      }()){};

    } else if constexpr (n == 5) {
      return decltype([=] {
        auto [x1, x2, x3, x4, x5] = example;
        return list(
          type<decltype(x1)>,
          type<decltype(x2)>,
          type<decltype(x3)>,
          type<decltype(x4)>,
          type<decltype(x5)>);
      }()){};

    } else if constexpr (n == 6) {
      return decltype([=] {
        auto [x1, x2, x3, x4, x5, x6] = example;
        return list(
          type<decltype(x1)>,
          type<decltype(x2)>,
          type<decltype(x3)>,
          type<decltype(x4)>,
          type<decltype(x5)>,
          type<decltype(x6)>);
      }()){};
    } else {
      return nothing;
    }
  }

  template<Aggregate T>
  constexpr auto aggregate_member_types = get_aggregate_member_types(type<T>);

  template<Character Char, integer N>
  struct Name
  {
    using character_type = Char;
    static constexpr auto extent = N;

    constexpr Name(const Char* input)
    {
      for (integer i = 0; i < N; ++i) {
        value[i] = input[i];
      }
    }

    Char value[N];
  };

  template<typename Char, auto N>
  Name(const Char (&)[N]) -> Name<Char, N>;

  template<class T, Name name_>
  struct named
  {
    static constexpr auto name = name_;
    using value_type = T;
    T value{};

    constexpr named() = default;

    constexpr named(value_type value)
      : value(value)
    {}

    constexpr operator const value_type&() const& { return value; }
    constexpr
    operator value_type&&() &&
    {
      return std::move(value);
    }
    operator value_type&() & { return value; };
  };

  constexpr auto is_named_type = case_lambda(
    []<class T, Name name_>(Type<named<T, name_>>) { return true; },
    [](auto) { return false; });

  template<typename T>
  concept Named = is_named_type(type<T>);

  template<typename T>
  concept NamedAggregate = Aggregate<T> and foldl(
    []<class U>(bool accum, Type<U>) { return accum and Named<U>; },
    true,
    aggregate_member_types<T>);

} // end of namespace ruse::reference
