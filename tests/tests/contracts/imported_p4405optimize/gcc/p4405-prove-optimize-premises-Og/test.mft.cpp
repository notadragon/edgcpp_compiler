//remark: imported from gcc:p4405-prove-optimize-premises-Og.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=observe -Og
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// D4405's proof optimization level (DECISIONS.md E162-E168, E205, E206):
// p4405-shadow-callee-premises.C in a -Og build, at the default proof
// level, 2.  The callees are optimized at -Og and the shadows at level 2,
// so the premise shadows call premise copies, and the ordinary shadows
// level copies, at level 2: each evaluation is proven as at -O2, the
// callees' premises reaching the premise shadows (h1, h3, h4, h5) without
// being declared inline (p4405-shadow-callee-premises-O1.C).
//
// Mirror: none.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -Og" }
// { dg-additional-options "-fcontract-evaluation-semantic=observe " }

#include "p4405-shadow-callee-premises.C"

//
//
//
//
//
//
