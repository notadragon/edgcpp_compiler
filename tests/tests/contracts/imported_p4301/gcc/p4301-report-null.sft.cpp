//remark: imported from gcc:p4301-report-null.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4301 --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: report=null
// P4301: contract_violation::report() is null for a violation no sanitizer
// produced -- a plain contract_assert here.
//
// Mirror: clang/test/Contracts/Runnable/p4301-report-null.cpp in the
// llvm_llvm-project fork.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4301 -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
#include <contracts>
#include <cstdio>
void handle_contract_violation(const std::contracts::contract_violation& v) {
  std::printf(v.report() == nullptr ? "report=null\n" : "report=nonnull\n");
}
int f(int x) { contract_assert(x > 0); return x; }
int main() { f(0); }
// { dg-output "report=null" }
