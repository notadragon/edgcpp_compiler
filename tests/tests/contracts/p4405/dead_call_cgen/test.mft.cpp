//remark:contracts: P4405 with the C-generating back end: under --contract_analysis_skip_dead_calls, which --contracts_p4405 implies (DECISIONS.md E169 (14) in notadragon_wg21), a call in code never evaluated has no analysis evaluation at the call: an operand of ?: or the right operand of && or || that a condition the front end folds never lets be evaluated, a substatement of an if statement that such a condition never selects, and (as before, whatever the option) a discarded statement; the addresses taken there are still taken; --no_contract_analysis_skip_dead_calls analyzes them, but not the calls of a ?:, && or || that the front end itself folds to a constant (h1b, h2*, h5, h9*), which are no longer in the IL under either setting, nor those of a discarded statement
//require:DO_IL_LOWERING 1
//type:fn
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove:--contracts_p3400 --contracts_p4405 --contract_analysis_skip_dead_calls --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove:--contracts_p3400 --contracts_p4405 --no_contract_analysis_skip_dead_calls --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove:--contracts_p3400 --contracts_p4405 --no_contract_analysis_skip_dead_calls --contract_analysis_skip_dead_calls --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove
//match_regex:line ([0-9]+): (error|warning): (contract .*)
// (No system headers: the C-generating back end cannot find them natively.)
// The first two option sets skip (the option implied, then given), the
// third does not, the fourth gives both (the last wins: skip).  Nothing
// proves a call's precondition here (EDG-134), so each call analyzed is
// diagnosed, the live g4(1) included.  The condition is decided by the
// front end's folding (as the walk of a predicate decides it): a constant,
// integral operators on constants, a constexpr variable of integral type.
// Mirrors: GCC's p4405-analysis-dead-call{,-no-skip}.C (imported in
// imported_p4405/gcc, where g++ decides) and Clang's a1, s1, lam and agg
// (clang/test/Contracts/p4405-analysis-dead-call.cpp).  Differences from
// GCC (DECISIONS.md E169 (18)): the ?:, && and || that the front end folds
// to a constant (h1b, h2*, h5, h9*, t1b, t2*, t5, tn2) are not analyzed
// under the -fno- form either; a1 and s1 are analyzed, as GCC (Clang skips
// them); lam's lambda body is analyzed as a function of its own, as GCC
// (Clang never emits it); agg's dead arm of class type is skipped (Clang
// analyzes it).
#include "analysis_stub.h"
using std::contracts::evaluation_semantic;
using std::contracts::evaluation_semantic_set;
using enum evaluation_semantic;

int g3(int x) pre(x != 0);
int g3(int x) { return 10 / x; }
int g4(int x) pre(x != 0);
int g4(int x) { return 10 / x; }
void ap(int x) pre<always_prove>(x > 0);

constexpr bool k = false;

// Never evaluated: not analyzed under the skip.
int h1() { return 1 ? g4(1) : g3(0); }
int h1b() { return 0 ? g3(0) : 2; }
bool h2() { return false && g3(0); }
bool h2b() { return true || g3(0); }
bool h2c() { return 0 && g3(0); }
bool h2d() { return 1 || g3(0); }
int h3(int y) { if (0) return g3(0); return y; }
int h3b(int y) { if (1) return y; else return g3(0); }
int h5() { return k ? g3(0) : 2; }
int h5b(int y) { if (k) return g3(0); return y; }
int h6() { if constexpr (false) return g3(0); return 1; }
int h6b() { if constexpr (true) return 1; else return g3(0); }
bool h9() { return 1 || 0 && g3(0); }
bool h9b() { return true || (false && g3(0)); }
int h9c(int y) { return y ? (0 ? g3(0) : 1) : 2; }
int h10(int y) { if (1 - 1) { y = g3(0); } return y; }
int h11(int y) { return (k || 0) ? g3(0) : y + g4(y); }
struct S { int v; S(int v) : v(v) {} };
S mk(int);
S agg() { return 1 ? S(1) : mk(g3(0)); }
int lam() { return 0 ? [] { return g3(0); }() : 1; }

// Evaluated, or not known never to be: analyzed.
int live1() { return 0 ? 2 : g3(0); }
bool live2() { return true && g3(0); }
bool live3() { return 0 || g3(0); }
int live4(int y) { if (0) return y; else return g3(0); }
int live5(int y) { if (1) return g3(0); return y; }
int live6(int y) { if (0) return y; return g3(0); }
bool live7() { return 0 && g3(0) || g3(0); }
int h4(int y) { return y > 0 && y < 0 ? g3(0) : y; }
int a1(int y) { return y; g3(0); }
int s1() { switch (1) { case 0: return g3(0); case 1: return 1; } return 0; }

// The address taken in code never evaluated is still taken (a ?: the
// front end folds, 0 ? &ap : nullptr, is a constant: no address taken).
void (*pick(int y))(int);
void (*addr(int y))(int) { return 1 ? pick(y) : &ap; }
void (*addr2(void (*fp)(int)))(int) { if (0) fp = &ap; return fp; }

// Predicates: the nested walk, unchanged (no note for g3's precondition in
// h7; the assertion itself is diagnosed, as every one is here).
int h7(int y) { contract_assert((1 ? g4(1) : g3(0)) == 10); return y; }
int h8(int y) { contract_assert(false && g3(0)); return y; }

// Templates, decided in each instance.
template <typename T> int t1() { return 1 ? g4(1) : g3(0); }
template <typename T> int t1b() { return 0 ? g3(0) : 2; }
template <typename T> bool t2() { return false && g3(0); }
template <typename T> int t3(int y) { if (0) return g3(0); return y; }
template <typename T> int t3b(int y) { if (1) return y; else return g3(0); }
template <typename T> int t5() { return k ? g3(0) : 2; }
template <typename T> int t6() { if constexpr (false) return g3(0); return 1; }
template <int N> int tn1() { return N ? g4(1) : g3(0); }
template <int N> bool tn2() { return N && g3(0); }
template <int N> int tn3(int y) { if (N) return g3(0); return y; }
template <int N> int tn6() { if constexpr (N) return g3(0); return 1; }
template <typename T> int tlive1() { return 0 ? 2 : g3(0); }
template <int N> bool tlive2() { return N && g3(0); }
template <typename T> int th4(int y) { return y > 0 && y < 0 ? g3(0) : y; }

template int t1<int>();
template int t1b<int>();
template bool t2<int>();
template int t3<int>(int);
template int t3b<int>(int);
template int t5<int>();
template int t6<int>();
template int tn1<1>();
template bool tn2<0>();
template int tn3<0>(int);
template int tn6<0>();
template int tlive1<int>();
template bool tlive2<1>();
template int th4<int>(int);
