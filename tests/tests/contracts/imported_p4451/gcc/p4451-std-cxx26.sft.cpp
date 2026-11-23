//remark: imported from gcc:p4451-std-cxx26.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contract_evaluation_semantic=ignore
//match_regex: ", line 26: (?:catastrophic )?error
// P4451 is part of C++26: -std=c++26 enables -fcontracts-p4451 with no
// contracts option at all, so the feature-test macros take P4451R2's
// values and std::contracts::controls::enforced makes an assertion
// terminating.
// { dg-do compile }
// { dg-options "-std=c++26 -fcontract-evaluation-semantic=ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

#if __cpp_contracts != 202611L || __cpp_lib_contracts != 202611L
#error "P4451's values"
#endif
#if __gcc_contracts_p4451 != 202611L
#error "__gcc_contracts_p4451"
#endif

/* The label is usable, and makes the assertion terminating: the
   precondition is checked in constant evaluation though the configured
   semantic is ignore.  */
constexpr int f (int x) pre<std::contracts::controls::enforced> (x > 0) { return x; }	// { dg-error "contract predicate is false in constant expression" }
constexpr int g (int x) pre (x > 0) { return x; }

constexpr int a = f (1);
constexpr int b = f (-1);
constexpr int c = g (-1);
