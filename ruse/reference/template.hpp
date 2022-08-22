#pragma once

//
// ... ruse header files
//
#include <ruse/reference/nat.hpp>
#include <ruse/reference/type.hpp>
#include <ruse/reference/utility.hpp>

namespace ruse::reference {

  /**
   * @brief A data struct template whose instances ack as proxies for the
   * template template parameter or a partial application of the template
   * template parameter over the type template parameters.
   */
  template<template<typename...> class K, typename... Ts>
  struct Template
  {
    template<typename T>
    constexpr auto
    operator()(Type<T>) const
    {
      return type<K<T, Ts...>>;
    }

    friend constexpr bool
    operator==(Template, Template)
    {
      return true;
    }

    template<typename T>
    friend constexpr bool
    operator==(Template, T)
    {
      return false;
    }

    template<typename T>
    friend constexpr bool
    operator!=(Template, T x)
    {
      return !(Template{} == x);
    }
  };

  /**
   * @brief A proxy for the the partial application of the template template
   * parameter over the type template parameters.
   */
  template<template<typename...> class K, typename... Ts>
  constexpr Template<K, Ts...> tmplt{};

  template<template<typename...> class K, typename... Ts>
  constexpr Template<K, Ts...> template_{};

  /**
   * @brief Return a proxy to the template of the inputs class
   */
  constexpr auto template_of =
    []<template<typename...> class K, typename T, typename... Ts>(K<T, Ts...>)
  {
    return tmplt<K, Ts...>;
  };

  /**
   * @brief Return true if the input is a trype proxy for a template_proxy type.
   * Otherwise, return false.
   */
  struct is_template_partial_application_proxy_Type
  {
    template<template<typename...> class K, typename... Ts>
    constexpr bool
    operator()(Type<Template<K, Ts...>>) const
    {
      return true;
    }

    constexpr bool operator()(auto) const { return false; }
  } constexpr is_template_partial_application_proxy_type{};

  /**
   * @brief Return true if the input is a trype proxy for a template_proxy type
   * lacking any partial application. Otherwise, return false.
   */
  struct is_template_proxy_Type
  {
    template<template<typename...> class K>
    constexpr bool
    operator()(Type<Template<K>>) const
    {
      return true;
    }

    constexpr bool operator()(auto) const { return false; }
  } constexpr is_template_proxy_type{};

  /**
   * @brief  A concept for template proxies
   */
  template<typename T>
  concept TemplateProxy = is_template_proxy_type(type<T>);

  /**
   * @brief Return true if the input is a template proxy.  Otherwise, return
   * `false`.
   */
  constexpr auto is_template_proxy = []<typename T>(T) {
    return TemplateProxy<T>;
  };

  /**
   * @brief Return a proxy to the template of the input type type proxy.
   */
  constexpr auto template_of_type = curry(
    nat<1>,
    []<template<typename...> class K, typename T, typename... Ts>(
      Type<K<T, Ts...>>) { return tmplt<K, Ts...>; });

  /**
   * @brief Return `true` if the input is a type proxy for a specialialization
   * of the template specifield in the template parameter.  Otherwise return
   * `false`.
   */
  template<template<typename...> class K>
  struct conceptified_Template
  {
    template<typename... Ts>
    constexpr bool
    operator()(Type<K<Ts...>>) const
    {
      return true;
    }
    constexpr bool operator()(auto) const { return false; }
  };

  /**
   * @brief Return `true` if the input is a type proxy for a specialialization
   * of the template specifield in the template parameter.  Otherwise return
   * `false`.
   */
  template<template<typename...> class K>
  constexpr conceptified_Template<K> is_specialization_of{};

  /**
   * @brief A concept for the specialization of templates
   */
  template<typename T, typename TemplateProxy>
  concept TemplateSpec = is_template_proxy_type(type<TemplateProxy>) and
    (template_of_type(type<T>) == TemplateProxy{});

  constexpr auto template_swap =
    [
    ]<template<typename...> class K1,
      template<typename...>
      class K2,
      typename T>(Template<K1>, K2<T>)
  {
    return K1<T>{};
  };

} // end of namespace ruse::reference
