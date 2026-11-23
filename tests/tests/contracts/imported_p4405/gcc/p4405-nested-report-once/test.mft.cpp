//remark: imported from gcc:p4405-nested-report-once.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-nested-report-once.json
//options: --c++26 --contracts --contracts_p4405 --contract_configuration_file=p4405-nested-report-once.json --contract_analysis_semantic=prove -O2:--c++26 --contracts --contracts_p4405 --contract_configuration_file=p4405-nested-report-once.json --contract_analysis_semantic=prove -O2 -O0
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// EDG: adapted -- the dg-regexp directives are not run (the recording pins
// every diagnostic), and g++'s warning at a call on its statement's second
// line is on the first: cpfe-cp prints the statement under one #line.
// P4405, DECISIONS.md E169 (9) and (11): a nested diagnose precondition
// in a predicate that is also checked at run time is warned once at its
// call, not twice (the analysis evaluation's nested evaluation and the
// evaluation at the same call in the check give the same warning, E120's
// local, stateless suppression).  Only the analysis and the check of the
// same assertion in the same place are merged: the same nested evaluation
// under several outer evaluations -- dd::g5's precondition in f's
// precondition, analyzed at each call of f -- is warned once per outer.
// Each dg-regexp takes one warning, and one more would be an excess
// message.  At -O2 the outer assertions are proven, conditionally
// (E169 (10)).
//
// `pinned': f2's precondition is checked in f2 (callee side) and analyzed
// at each call of f2; the check's evaluation at dd::g5's call and the
// nested evaluation of the analysis at the caller are not the same
// assertion in the same place, so both warn (today's behaviour; whether
// E169 (9) should merge them is an open question).
//
// Mirror: clang/test/Contracts/p4405-nested-checked.cpp (`report_once')
// in the llvm_llvm-project fork; the several outers and `pinned' are
// gnu_gcc's.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-nested-report-once.json" }
// { dg-additional-options "-fcontract-analysis-semantic=prove" }

namespace dd {
int g5 (int x) pre (x != 0);	// { dg-message "the predicate of the contract assertion" }
int g5 (int) { return 1; }
}

// Checked and analyzed in the same place: once.
int
single (int y)
{
  contract_assert (		// { dg-bogus "cannot be proven" }
    dd::g5 (y) == 1);
  return y;
}
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:37:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }

// Two assertions, each its own pair: once each.
int
two (int y)
{
  contract_assert (		// { dg-bogus "cannot be proven" }
    dd::g5 (y) == 1);
  contract_assert (		// { dg-bogus "cannot be proven" }
    dd::g5 (y + 1) == 1);
  return y;
}
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:47:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:49:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }

// A postcondition, analyzed and checked in p: once.
int
p (int y)
  post (r:			// { dg-bogus "cannot be proven" }
	dd::g5 (r) == 1)
{
  return y;
}
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:59:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }

// f's precondition is only analyzed (preconditions are ignore at run
// time here), at each of three calls: three outers, three warnings.
int f (int y)
  pre (dd::g5 (y) == 1);
int f (int y) { return y; }

int
c1 (int y)
{
  return f (y);			// { dg-bogus "cannot be proven" }
}

int
c2 (int y)
{
  return f (y) + f (y + 1);	// { dg-bogus "cannot be proven" }
}
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:68:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:68:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:68:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }

namespace pinned {
int f2 (int y)
  pre (dd::g5 (y) == 1);
int f2 (int y) { return y; }
}

int
c3 (int y)
{
  return pinned::f2 (y);	// { dg-bogus "cannot be proven" }
}
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:88:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }
// { dg-regexp {[^\n\r]*p4405-nested-report-once\.C:88:[0-9]+: warning: contract assertion cannot be proven [^\n\r]*} }
