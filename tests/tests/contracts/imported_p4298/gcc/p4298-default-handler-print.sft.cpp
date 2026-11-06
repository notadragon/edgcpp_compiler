//remark: imported from gcc:p4298-default-handler-print.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4298 --contract_evaluation_semantic=noexcept_observe
//use_system_includes: true
//linker_options: -lcontracts
//match_regex: semantic: noexcept_observe
// P4298: the built-in default violation handler names the noexcept semantics.
// A semantic printer switching only on enforce/observe leaves a
// noexcept_observe/noexcept_enforce violation printing "unknown(6)" or
// "unknown(7)".
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4298 -fcontract-evaluation-semantic=noexcept_observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
// { dg-output "semantic: noexcept_observe" }

#include <contracts>

// No user handle_contract_violation -> the default handler runs and prints;
// noexcept_observe continues after it.
int f (int x) pre (x > 0) { return x; }

int main () { f (-1); return 0; }
