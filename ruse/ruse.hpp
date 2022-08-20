#pragma once

//
// ... Ruse header files
//
#include <ruse/protocol_traits.hpp>
#include <ruse/reference/functional.hpp>
#include <ruse/reference/hoisted.hpp>
#include <ruse/reference/list.hpp>
#include <ruse/reference/monoid.hpp>
#include <ruse/reference/nothing.hpp>
#include <ruse/reference/pair.hpp>
#include <ruse/reference/parser.hpp>
#include <ruse/reference/protocol.hpp>
#include <ruse/reference/pure.hpp>
#include <ruse/reference/reader.hpp>
#include <ruse/reference/reflection.hpp>
#include <ruse/reference/state.hpp>
#include <ruse/reference/string.hpp>
#include <ruse/reference/tag.hpp>
#include <ruse/reference/template.hpp>
#include <ruse/reference/type.hpp>

namespace ruse {

  using reference::sqr;
  using reference::twc;

  using reference::apply;
  using reference::apply_to;
  using reference::apply_to_list;
  using reference::case_lambda;
  using reference::cut;
  using reference::cute;

  using reference::compose;
  using reference::curry;
  using reference::identity;

  using reference::is_type;
  using reference::type;
  using reference::Type;
  using reference::type_of;
  using reference::TypeProxy;
  using reference::TypeProxyOf;

  using reference::is_nothing;
  using reference::nothing;
  using reference::Nothing;
  using reference::nothing_s;

  using reference::car;
  using reference::cdr;
  using reference::cons;
  using reference::is_pair;
  using reference::pair;
  using reference::Pair;

  using reference::AssociationList;
  using reference::BinaryList;
  using reference::HomogeneousAssociationList;
  using reference::HomogeneousList;
  using reference::is_homogeneous_list;
  using reference::is_list;
  using reference::is_nonempty_list;
  using reference::is_unitary_list;
  using reference::List;
  using reference::ListLike;
  using reference::ListOfLength;
  using reference::ListOfPositiveLength;
  using reference::NonemptyList;
  using reference::PropertyList;
  using reference::TernaryList;
  using reference::UnitaryList;
  constexpr auto is_hlist = reference::is_homogeneous_list;
  using reference::is_vacuous_list;
  constexpr auto is_vlist = reference::is_vacuous_list;
  using reference::is_association_list;
  constexpr auto is_alist = reference::is_association_list;
  using reference::is_homogeneous_association_list;
  using reference::is_property_list;
  constexpr auto is_plist = reference::is_property_list;
  using reference::append;
  using reference::butlast;
  using reference::dolist;
  using reference::drop;
  using reference::head;
  using reference::last;
  using reference::length;
  using reference::list;
  using reference::list_ref;
  using reference::reverse;
  using reference::tail;
  using reference::take;

  using reference::homogeneous_list_ref;
  constexpr auto hlist_ref = reference::homogeneous_list_ref;

  using reference::homogeneous_list_sort;
  constexpr auto hlist_sort = reference::homogeneous_list_sort;

  using reference::property_list_ref;
  constexpr auto plist_ref = reference::property_list_ref;

  using reference::property_list_ref_;
  constexpr auto plist_ref_ = reference::property_list_ref_;

  using reference::vacuous_list_sort;
  constexpr auto vlist_sort = reference::vacuous_list_sort;

  using reference::vacuous_list_filter;
  constexpr auto vlist_filter = reference::vacuous_list_filter;

  using reference::vacuous_list_take_while;
  constexpr auto vlist_take_while = reference::vacuous_list_take_while;

  using reference::vacuous_list_take_until;
  constexpr auto vlist_take_until = reference::vacuous_list_take_until;

  using reference::foldl;

  using reference::operator+;
  using reference::operator*;
  using reference::operator==;

  using reference::add1;
  using reference::Nat;
  using reference::nat;
  using reference::sub1;

  using reference::Character;
  using reference::hoist;
  using reference::hoisted;
  using reference::Hoisted;
  using reference::hoisted_append;
  using reference::hoisted_butlast;
  using reference::hoisted_drop;
  using reference::hoisted_head;
  using reference::hoisted_last;
  using reference::hoisted_list;
  using reference::HoistedList;
  using reference::HoistedString;
  using reference::is_character;
  using reference::is_hoisted;
  using reference::is_string;
  using reference::str;
  using reference::String;

  using reference::hoisted_nothing;
  using reference::hoisted_tail;
  using reference::hoisted_take;
  using reference::HoistedList;
  using reference::is_character;
  using reference::is_hoisted;
  using reference::is_hoisted_list;
  using reference::is_string;
  using reference::lower;
  using reference::NonstringList;
  using reference::str;
  using reference::String;
  using reference::operator""_hoist;
  using reference::operator""_s;

  using reference::is_tag;
  using reference::is_tagged;
  using reference::Tag;
  using reference::Tagged;
  using reference::operator""_tag;

  using reference::is_pure;
  using reference::is_pure_type;
  using reference::pure;
  using reference::Pure;

  using reference::beginm;
  using reference::duplicate;
  using reference::extend;
  using reference::extract;
  using reference::fapply;
  using reference::flatmap;
  using reference::flatten;
  using reference::fmap;
  using reference::letf;
  using reference::letm;
  using reference::zapply;

  using reference::alt;
  using reference::conj;
  using reference::disj;
  using reference::empty;
  using reference::equal;
  using reference::fail;
  using reference::item;
  using reference::opt;
  using reference::parse_result_sequence;
  using reference::parse_result_value;
  using reference::result;
  using reference::sat;
  using reference::seq;
  using reference::word;
  // using reference::many;
  // using reference::many1;

  // reader
  using reference::ask;
  using reference::asks;
  using reference::is_reader;
  using reference::local;
  using reference::make_reader;
  using reference::Reader;
  using reference::ReaderOf;
  using reference::run_reader;

  // state
  using reference::eval_state;
  using reference::exec_state;
  using reference::get_state;
  using reference::make_stateful;
  using reference::modify_state;
  using reference::put_state;
  using reference::run_state;
  using reference::select_state;

  // template
  using reference::Template;
  using reference::TemplateProxy;
  using reference::TemplateSpec;

  // reflection
  using reference::Aggregate;
  using reference::aggregate_member_count;
  using reference::aggregate_member_types;
  using reference::AggregateInitializable;
  using reference::type_name;

} // end of namespace ruse
