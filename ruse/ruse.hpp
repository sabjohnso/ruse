#pragma once

//
// ... Ruse header files
//
#include <ruse/reference/list.hpp>
#include <ruse/reference/nothing.hpp>
#include <ruse/reference/type.hpp>
#include <ruse/reference/pair.hpp>

namespace ruse {

  using reference::type_s;
  using reference::type;
  using reference::Type;
  using reference::TypeProxy;
  using reference::is_type;

  using reference::nothing_s;
  using reference::nothing;
  using reference::Nothing;
  using reference::is_nothing;

  using reference::pair;
  using reference::Pair;
  using reference::is_pair;
  using reference::cons;
  using reference::car;
  using reference::cdr;

  using reference::List;
  using reference::NonemptyList;
  using reference::UnitaryList;
  using reference::HomogeneousList;
  using reference::AssociationList;
  using reference::HomogeneousAssociationList;
  using reference::is_list;
  using reference::is_nonempty_list;
  using reference::is_unitary_list;
  using reference::is_homogeneous_list;
  using reference::is_association_list;
  using reference::is_homogeneous_association_list;
  using reference::list;
  using reference::reverse;
  using reference::head;
  using reference::tail;





} // end of namespace ruse
