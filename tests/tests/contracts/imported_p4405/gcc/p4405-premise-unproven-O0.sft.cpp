//remark: imported from gcc:p4405-premise-unproven-O0.C
//type: cp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore -O0 --contracts_prove_optimize=0
//match_regex: Test_name.c:([0-9]+):[0-9]+: (warning|error): (contract .*)
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cp) and matches g++'s diagnostics.
// EDG: adapted -- the three dg-regexp warnings at g1's call are pinned by
// the recording.
// P4405, notadragon_wg21 DECISIONS.md E196 item 1, at proof level 0: only
// folding runs, which takes no premise from an earlier evaluation, so each
// of the three preconditions is warned at the call (a proof shadow, at -O0
// too by default, gives one: p4405-premise-unproven.C).  Accepted: what
// level 0 catches is not the goal (E195's answer).
// Mirror: clang/test/Contracts/p4405-premise-unproven-O0.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontracts-p4405 -O0 -fcontracts-prove-optimize=0" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore" }

#include <contracts>
using namespace std::contracts::labels;

void f (int x) pre<diagnose> (x > 3) pre<diagnose> (x > 2) pre<diagnose> (x > 1);

void g1 (int y)
{
  f (y);
}
// { dg-regexp {[^\n]*p4405-premise-unproven-O0\.C:19:5: warning: contract assertion cannot be proven[^\n]*} }
// { dg-regexp {[^\n]*p4405-premise-unproven-O0\.C:19:5: warning: contract assertion cannot be proven[^\n]*} }
// { dg-regexp {[^\n]*p4405-premise-unproven-O0\.C:19:5: warning: contract assertion cannot be proven[^\n]*} }
