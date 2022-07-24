#pragma once

//
// ... Ruse header files
//
#include <ruse/reference/list.hpp>
#include <ruse/reference/nothing.hpp>
#include <ruse/reference/type.hpp>
#include <ruse/reference/pair.hpp>
#include <ruse/reference/tag.hpp>
#include <ruse/reference/hoisted.hpp>
#include <ruse/reference/string.hpp>
#include <ruse/reference/monoid.hpp>
#include <ruse/reference/protocol.hpp>
#include <ruse/reference/parser.hpp>

#include <ruse/protocol_traits.hpp>

namespace ruse {

  using reference::curry;
  using reference::compose;
  using reference::identity;

  using reference::type_s;
  using reference::type;
  using reference::Type;
  using reference::TypeProxy;
  using reference::is_type;
  using reference::type_of;

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
  using reference::PropertyList;
  using reference::ListLike;
  using reference::is_list;
  using reference::is_nonempty_list;
  using reference::is_unitary_list;
  using reference::is_homogeneous_list;
  constexpr auto is_hlist = reference::is_homogeneous_list;
  using reference::is_association_list;
  constexpr auto is_alist = reference::is_association_list;
  using reference::is_homogeneous_association_list;
  using reference::is_property_list;
  constexpr auto is_plist = reference::is_property_list;
  using reference::list;
  using reference::length;
  using reference::reverse;
  using reference::append;
  using reference::head;
  using reference::tail;
  using reference::last;
  using reference::butlast;
  using reference::list_ref;
  using reference::take;
  using reference::drop;
  using reference::dolist;

  using reference::homogeneous_list_ref;
  constexpr auto hlist_ref = reference::homogeneous_list_ref;

  using reference::property_list_ref;
  constexpr auto plist_ref = reference::property_list_ref;

  using reference::operator+;
  using reference::operator*;

  using reference::Nat;
  using reference::nat;

  using reference::Character;
  using reference::is_character;
  using reference::String;
  using reference::is_string;
  using reference::str;
  using reference::hoisted;
  using reference::hoisted_list;
  using reference::Hoisted;
  using reference::HoistedList;
  using reference::is_hoisted;
  using reference::is_hoisted_list;
  using reference::lower;
  using reference::hoist;
  using reference::hoisted_head;
  using reference::hoisted_tail;
  using reference::hoisted_last;
  using reference::hoisted_butlast;
  using reference::hoisted_take;
  using reference::hoisted_drop;
  using reference::hoisted_append;
  using reference::hoisted_nothing;
  using reference::hoisted_cons;
  using reference::operator""_hoist;
  using reference::operator""_s;

  using reference::Tag;
  using reference::Tagged;
  using reference::is_tag;
  using reference::is_tagged;
  using reference::operator""_tag;

  using reference::fmap;
  using reference::letf;
  using reference::fapply;
  using reference::flatmap;
  using reference::letm;
  using reference::flatten;
  using reference::extract;
  using reference::extend;
  using reference::duplicate;
  using reference::zapply;

  using reference::parse_result_value;
  using reference::parse_result_sequence;
  using reference::result;
  using reference::fail;
  using reference::item;
  using reference::empty;
  using reference::conj;
  using reference::disj;
  using reference::alt;
  using reference::sat;
  using reference::seq;
  using reference::equal;
  using reference::word;
  using reference::opt;
  // using reference::many;
  // using reference::many1;

} // end of namespace ruse
