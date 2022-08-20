#pragma once

//
// ... ruse header files
//
#include <ruse/reference/import.hpp>
#include <ruse/reference/template.hpp>
#include <ruse/reference/type.hpp>

namespace ruse::reference {

  /**
   * @brief A concept for types with a member typedef named `name_type`
   */
  template<typename T>
  concept HasNameType = requires
  {
    typename T::name_type;
  };

  /**
   * @brief A concept for `HasNameType` types where `name_type` is an empty
   * type.
   */
  template<typename T>
  concept HasEmptyNameType = HasNameType<T> && Empty<typename T::name_type>;

  /**
   * @brief A concept for types with a member typedef named `value_type`
   */
  template<typename T>
  concept HasValueType = requires
  {
    typename T::value_type;
  };

  /**
   * @brief A concept tor `HasValueType` with a data member named `value` with
   * type `value_type`.
   */
  template<typename T>
  concept HasValue = HasValueType<T> && requires(T x)
  {
    {
      x.value
      } -> convertible_to<typename T::value_type>;
  };

  /**
   * @brief A concept for type that wrap a value.
   */
  template<typename T>
  concept ValueWrapper = HasValue<T> &&
                         sizeof(T) == sizeof(typename T::value_type);

  template<typename T>
  concept Tagged = ValueWrapper<T> && HasEmptyNameType<T>;

  constexpr auto is_tagged_type = []<typename T>(T) {
    if constexpr (TypeProxy<T>) {
      return Tagged<typename T::type>;
    } else {
      return false;
    }
  };

  constexpr auto is_tagged = []<typename T>(T) { return Tagged<T>; };

  template<typename T>
  concept Tag = Empty<T> && HasEmptyNameType<T>;

  constexpr auto is_tag = []<typename T>(T) { return Tag<T>; };

  template<typename T, Empty Name>
  struct tagged
  {
    using value_type = T;
    using name_type = Name;
    static constexpr name_type name{};

    value_type value;

    constexpr auto
    operator<=>(const tagged&) const = default;
  };

  template<Empty Name>
  struct tag
  {
    using name_type = Name;
    static constexpr name_type name{};

    template<typename T>
    constexpr Tagged auto
    operator()(T x) const
    {
      return tagged<T, Name>{.value = x};
    }

    friend constexpr bool
    operator==(tag, tag)
    {
      return true;
    }

    template<typename U>
    friend constexpr bool
    operator==(tag, U)
    {
      return false;
    }

    template<typename U>
    friend constexpr bool
    operator!=(tag, U x)
    {
      return !(tag{} == x);
    }
  };

  constexpr auto name_type = []<TypeProxy T>(T) {
    return type<typename T::type::name_type>;
  };

  template<ValueWrapper T>
  constexpr auto
  get_pure(Type<T>)
  {
    return
      []<typename U>(U x) { return template_of_type(type<T>)(type<U>)(x); };
  }

  template<ValueWrapper T>
  constexpr auto
  get_fmap(Type<T>)
  {
    return [](auto f, ValueWrapper auto x) {
      constexpr auto pure = get_pure(type<T>);
      return pure(f(x.value));
    };
  }

  template<ValueWrapper T>
  constexpr auto
  get_flatten(Type<T>)
  {
    return [](ValueWrapper auto x) -> ValueWrapper auto { return x.value; };
  }

  template<ValueWrapper T>
  constexpr auto
  get_extract(Type<T>)
  {
    return [](ValueWrapper auto x) { return x.value; };
  }

  template<ValueWrapper T>
  constexpr auto
  get_extend(Type<T>)
  {
    return [](auto f, ValueWrapper auto wx) -> ValueWrapper auto
    {
      return template_of_type(type<T>)(type<decltype(f(wx))>)(f(wx));
    };
  }

} // end of namespace ruse::reference
