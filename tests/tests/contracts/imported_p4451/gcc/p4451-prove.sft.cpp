//remark: imported from gcc:p4451-prove.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p4451 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove -O2
//match_regex: Test_name.c:39:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations, so the test compiles (type cn) and
// matches g++'s diagnostics; -O2 as in the original.
// P4451, D4405: a terminating contract assertion allows prove, as P3400's
// labels::terminating does: with the analysis evaluations configured prove
// its precondition must be proven at each call, and it is still enforced at
// run time.  p4405-paper-analysis-terminating.C under -fcontracts-p4451 alone.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p4451 -fcontracts-p4405 -O2" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore -fcontract-analysis-semantic=prove" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

int table[101];

int lookup (int p)
  pre<terminating> (p >= 0 && p <= 100)	// { dg-message "the predicate of the contract assertion" }
{
  return table[p];
}

int lookup_clamped (int v)
{
  if (v < 0)
    v = 0;
  if (v > 100)
    v = 100;
  return lookup (v);  // OK, proven	{ dg-bogus "cannot be proven" }
}

int lookup_raw (int v)
{
  return lookup (v);  // Error if proven, otherwise enforced	{ dg-error "contract assertion cannot be proven" }
}
