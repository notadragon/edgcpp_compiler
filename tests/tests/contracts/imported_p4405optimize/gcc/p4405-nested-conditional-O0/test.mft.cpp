//remark: imported from gcc:p4405-nested-conditional-O0.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-nested-conditional.json
//options: --c++26 --contracts --contracts_p4405 -fcontracts-prove-assume --contract_evaluation_semantic=observe --contract_configuration_file=p4405-nested-conditional.json -O0:--c++26 --contracts --contracts_p4405 -fcontracts-prove-assume --contract_evaluation_semantic=observe --contract_configuration_file=p4405-nested-conditional.json -O0 -O2
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// P4405, DECISIONS.md E169: p4405-nested-conditional.C in a -O0 build,
// which proves at the default proof level, 2: the nested precondition and
// the assertion holding it are proven conditionally in the proof shadow,
// as at -O2.  -O0 drops the assumptions, so nothing is assumed.
//
// Mirror: clang/test/Contracts/p4405-nested-conditional.cpp in the
// llvm_llvm-project fork (its -O0 run).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -O0" }
// { dg-additional-options "-fcontracts-prove-assume" }
// { dg-additional-options "-fcontract-evaluation-semantic=observe" }
// { dg-additional-options "" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-nested-conditional.json" }

#include "p4405-nested-conditional.C"

//
//
//
//
