#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {

  template<typename T>
  constexpr auto
  pretty_function(T)
  {
    return std::string_view{__PRETTY_FUNCTION__};
  }

  constexpr auto
  strip_garbage(std::string_view str)
  {
    const auto start = str.find('<') + 1;
    const auto stop = str.find_last_of('>');
    return str.substr(start, stop - start);
  }

  template<typename T>
  constexpr auto
  get_type_name(Type<T>)
  {
    return strip_garbage(pretty_function(type<T>));
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

} // end of namespace ruse::reference
