//remark: imported from gcc:p3400-allowed-mask-no-noexcept.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4298 --contracts_allow_assume --contract_evaluation_semantic=enforce
//match_regex: ", line 48: (?:catastrophic )?error
// P3400: a label whose allowed_semantics deliberately excludes the two
// P4298 noexcept variants must still have that exclusion honoured.
//
// Regression test for the "no restriction" sentinel.  grok_contract must
// store CONTRACT_ALLOWED_MASK against the same full set it started from:
// storing it only when the computed mask differs from
// CES_ALL_ALLOWED_WITH_ASSUME, having started from
// CES_ALL_ALLOWED_WITH_EXTENSIONS, lets a label allowing exactly
// {ignore, observe, enforce, quick_enforce, assume} -- that is,
// WITH_ASSUME precisely -- compare equal and go unstored, whereupon
// make_contract_query's NULL fallback restores the full WITH_EXTENSIONS
// set.  The exclusion is then silently ignored and compute_semantic can
// hand back noexcept_enforce unchallenged.
//
// That is the one mask value such a comparison lets through, and it is
// invisible in every other test, which is why it has one of its own.

// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4298 -fcontracts-allow-assume -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

struct no_noexcept_t
{
  using assertion_control_object = no_noexcept_t;
  static constexpr evaluation_semantic_set allowed_semantics = {
    evaluation_semantic::ignore,
    evaluation_semantic::observe,
    evaluation_semantic::enforce,
    evaluation_semantic::quick_enforce,
    evaluation_semantic::assume
  };
  constexpr evaluation_semantic
  compute_semantic (evaluation_semantic) const
  { return evaluation_semantic::noexcept_enforce; }
};
constexpr no_noexcept_t lbl{};

void f (int x) pre<lbl> (x > 0) { }  // { dg-error "allowed evaluation semantics" }
