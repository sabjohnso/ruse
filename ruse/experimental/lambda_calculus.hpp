#pragma once

//
// ... ruse header files
//
#include <ruse/reference/type.hpp>

namespace ruse::reference
{

  //
  // ... Variables
  //
  template<auto Name>
  struct var{
    static constexpr auto name = Name;
  };

  struct {

    template<auto Name>
    constexpr bool
    operator()(type_s<var<Name>>){ return true; }

    constexpr bool
    operator ()(auto) const {return false;}

  } constexpr is_variable_type{};

  template<typename T>
  concept Variable = is_variable_type(type<T>);

  constexpr auto is_variable = []<typename T>(T){ return Variable<T>; };


  //
  // ... Abstraction
  //
  template<Variable auto Var, auto Body>
  struct fun{
    static constexpr auto var = Var;
    static constexpr auto body = Body;
  };

  struct {

    template<auto Var, auto Body>
    constexpr bool
    operator ()(type_s<fun<Var,Body>>) const { return true; }

    constexpr bool
    operator ()(auto) const { return false; }
  } constexpr is_abstraction_type{};

  template<typename T>
  concept Abstraction = is_abstraction_type(type<T>);

  constexpr auto is_abstraction = []<typename T>(T){ return Abstraction<T>; }


  //
  // ... Application
  //
  template<auto Fun, auto Arg>
  struct app{
    static constexpr auto fun = Fun;
    static constexpr auto arg = Arg;
  };

  struct {
    template<auto Fun, auto Arg>
    constexpr bool
    operator ()(type_s<app<Fun, Arg>>) const { return true; }

    constexpr bool
    operator ()(auto) const { return false; }

  } constexpr is_application_type{};

  template<typename T>
  concept Application = is_application_type(type<T>);


  //
  // ... Expressions
  //
  struct {

    template<Variable T>
    constexpr bool
    operator ()(type_s<T>) const { return true; }

    template<Variable Var, auto Body>
    constexpr bool
    operator ()(type_s<fun<Var, Body>>) const {
      return (*this)(type_of(Body));
    }

    template<auto Fun, auto Arg>
    constexpr bool
    operator ()(type_s<app<Fun, Arg>>) const {
      return (*this)(type_of(Fun)) && (*this)(type_of(Arg));
    }

    constexpr bool
    operator ()(auto) const { return false; }

  } constexpr is_expression_type{};

  template<typename T>
  Expression = (Variable<T> || Abstraction<T> || Application<T>) && is_expression_type(type<T>);

  constexpr auto is_expression = []<typename T>(T){ return Expression<T>; };

} // end of namespace ruse::reference
