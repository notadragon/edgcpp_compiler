//remark: imported from gcc:p4405-paper-analysis-terminating.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:51:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// D4405R0, "Analyzing Assertions With Restricted Semantics": terminating
// allows prove, so with the analysis evaluations configured prove its
// precondition must be proven at each call -- the clamped call is, the raw
// one is an error -- and it is still enforced at run time
// (p4405-paper-analysis-terminating-run.C).  The paper's illustration.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore -fcontract-analysis-semantic=prove" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>  // `terminating`, `contract_violation`
#include <cstdio>     // `std::printf`, `std::fflush`

using std::contracts::labels::terminating;

// A violation handler that reports each violation; with *enforce* the
// program then terminates.
void handle_contract_violation(const std::contracts::contract_violation& v)
{
  std::printf("violation: %s\n", v.comment());
  std::fflush(stdout);
}

int table[101];

int lookup(int p)
  pre<terminating>(p >= 0 && p <= 100)	// { dg-message "the predicate of the contract assertion" }
{
  return table[p];
}

int lookup_clamped(int v)
{
  if (v < 0)
    v = 0;
  if (v > 100)
    v = 100;
  return lookup(v);  // OK, proven	{ dg-bogus "cannot be proven" }
}

int lookup_raw(int v)
{
  return lookup(v);  // Error if proven, otherwise enforced	{ dg-error "contract assertion cannot be proven" }
}
