#pragma once

//
// ... ruse header files
//
#include <ruse/ruse.hpp>

namespace ruse::experimental {

  using namespace ruse::reference;

  namespace details {

    template<typename T>
    constexpr void
    methods(Type<T>)
    {}

    struct get_methods_fn
    {
      template<typename T>
      constexpr auto
      operator()(Type<T>) const
      {
        return methods(type<T>);
      }
    };
  } // namespace details

  constexpr auto& get_methods =
    ruse::details::static_const<details::get_methods_fn>;

  template<typename T>
  constexpr auto methods = nothing;

  template<typename Name>
  struct method_name
  {
    constexpr auto
    operator=(auto fun) const
    {
      return tag<Name>{}(fun);
    }
  };

  template<fixed_string Name>
  constexpr method_name<decltype(operator""_hoist<Name>())> method{};

  template<fixed_string MethodName>
  struct send_invocation
  {
    template<typename T>
    constexpr auto
    operator()(T object, auto... args) const
    {
      return plist_ref_(
        tag<decltype(operator""_hoist<MethodName>())>{},
        get_methods(type<T>))(object, args...);
    }
  };

  template<fixed_string MethodName>
  constexpr send_invocation<MethodName> send{};

} // end of namespace ruse::experimental
