//remark: imported from gcc:p3400-compute-semantic-assume-once.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3400 --contracts_p3100 --contract_evaluation_semantic=assume
//match_regex: ", line 28: (?:catastrophic )?error
// With the configured semantic assume and no -fcontracts-allow-assume, a
// label whose compute_semantic returns assume (not allowed) is diagnosed
// once for one assertion, though the runtime and the constant-evaluation
// semantics are both resolved (GCC-645).  turns a second
// copy into "compilation terminated", an excess error.
//
// Mirror: clang/test/Contracts/p3100-label-assume-constexpr-no-flag.cpp in
// the llvm_llvm-project fork, whose -verify counts it once.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3400 -fcontracts-p3100 -fcontract-evaluation-semantic=assume" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

using std::contracts::evaluation_semantic;

struct compute_assume_t {
  using assertion_control_object = compute_assume_t;
  constexpr evaluation_semantic compute_semantic (evaluation_semantic) const
  { return evaluation_semantic::assume; }
};

int d (int x) pre<compute_assume_t{}> (x > 0) { return x; }  // { dg-error "compute_semantic. result is not in the allowed" }
