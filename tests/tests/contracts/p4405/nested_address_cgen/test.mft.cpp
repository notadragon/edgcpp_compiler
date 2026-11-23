//remark:contracts: P4405 with the C-generating back end: where the address of a function whose precondition allows no run-time semantic is taken, that precondition is decided by folding together with the nested analysis evaluations of the preconditions of the calls in its predicate, with nothing known of the arguments (DECISIONS.md E169 in notadragon_wg21): a predicate folded to true whose calls carry a nested prove precondition not proven is unproven, with a note for each such precondition after the precondition's ("... cannot be proven" / "... is always false"); a nested diagnose precondition is warned at its call for each address taken, ahead of the error; a call the predicate never evaluates (the right operand of true ||, an unevaluated operand) has nothing nested; with --no_contracts_prove_fold nothing is folded
//require:DO_IL_LOWERING 1
//type:fn
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=quick_enforce --contract_analysis_semantic=prove:--contracts_p3400 --contracts_p4405 --no_contracts_prove_fold --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove
//match_regex:line ([0-9]+): (error|warning): (contract .*)
//match_regex:(precondition of a call in the predicate (cannot be proven|is always false) at line [0-9]+)
// (No system headers: the C-generating back end cannot find them natively.)
// Under the first options constant evaluation folds g(0) == 1 to true
// (g's precondition is ignore there), so a2's predicate is true while g's
// precondition at g(0) is always false; under the second constant
// evaluation checks it, so g(0) == 1 does not fold.
// Mirrors: GCC's p4405-nested-address.C (imported in imported_p4405/gcc),
// Clang's p4405-nested-address.cpp (a6, warned for each address taken).
#include "analysis_stub.h"
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;

constexpr int g(int x) pre(x != 0) { return 1; }
int h(int x) pre(x != 0);
int gd(int x) pre<always_diagnose>(x != 0);

void a1(int x) pre<always_prove>(g(1) == 1);      // Nothing.
void a2(int x) pre<always_prove>(g(0) == 1);      // g's always false.
void a3(int x) pre<always_prove>(g(x) == 1);      // g's cannot be proven.
void a4(int x) pre<always_prove>(true || g(0) == 1);   // Nothing.
void a5(int x) pre<always_prove>(sizeof(g(0)) == sizeof(int));  // Nothing.
void a6(int x) pre<always_prove>(gd(x) == 1);     // gd's warned.
void a7(int x) pre<always_prove>((h(x), true));   // True, but h's noted.
void a8(int x) pre<always_diagnose>((h(1), true));     // Nothing.

void take() {
  void (*p1)(int) = &a1;
  void (*p2)(int) = &a2;
  void (*p3)(int) = &a3;
  void (*p4)(int) = &a4;
  void (*p5)(int) = &a5;
  void (*p6)(int) = &a6;
  void (*p7)(int) = &a6;         // gd's warned again.
  void (*p8)(int) = &a7;
  void (*p9)(int) = &a8;
  (void)p1; (void)p2; (void)p3; (void)p4; (void)p5; (void)p6; (void)p7;
  (void)p8; (void)p9;
}
