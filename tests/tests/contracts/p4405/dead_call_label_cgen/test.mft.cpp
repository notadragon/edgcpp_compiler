//remark:contracts: P4405 with the C-generating back end: under --contract_analysis_skip_dead_calls, an if substatement that a folded condition never selects is not code never evaluated if a goto or an enclosing switch can enter it at a label inside it (EDG-184, as GCC-992 in our GCC), so the calls there are analyzed; a substatement holding only labels nothing outside it can reach (a nested switch's case labels, the break and continue targets of a loop) is still skipped; the calls after an if statement whose selected substatement cannot complete are analyzed (GCC-994's shapes in our GCC)
//require:DO_IL_LOWERING 1
//type:fn
//source_files:analysis_stub.h
//options:--contracts_p3400 --contracts_p4405 --contract_evaluation_semantic=ignore:--contracts_p3400 --contracts_p4405 --no_contract_analysis_skip_dead_calls --contract_evaluation_semantic=ignore
//match_regex:line ([0-9]+): (error|warning): (contract .*)
// Nothing proves a call's precondition here (EDG-134), so each call
// analyzed is diagnosed.  The first option set skips (the option implied),
// the second does not: there every g(0) is diagnosed.  Under the skip, the
// l* calls are reached through a label and diagnosed; the whole
// substatement holding the label is analyzed, the call before the label
// in l6 included (the skip is a permission, DECISIONS.md E169 (18) in
// notadragon_wg21).  The s* calls are never reached: skipped.
// Mirrors: GCC's p4405-unreachable-O0.C (N4..N7; imported in
// imported_p4405/gcc, where g++ decides) and
// the label shapes of our GCC's GCC-992.
#include "analysis_stub.h"
using namespace std::contracts::labels;

void g(int x) pre<always_prove>(x != 0);

// Reached through a label: analyzed.
void live(int c) { if (c) g(0); }
void l1(int c) { if (c) goto L; if (0) { L: g(0); } }
void l2(int c) { if (c) goto L; if (1) {} else { L: g(0); } }
void l3(int c) { switch (c) { case 0: if (0) { case 1: g(0); } } }
void l4(int c) { switch (c) { case 0: if (0) { default: g(0); } } }
template <int N> void l5(int c) { if (c) goto L; if (N) { L: g(0); } }
template void l5<0>(int);
void l6(int c) { if (c) goto L; if (0) { g(0); L: g(0); } }
void l7(int c) { if (c) goto L; if (0) { while (c) { L: g(0); } } }

// Never reached: skipped.
void s1(int c) { if (0) { switch (c) { case 1: g(0); default: break; } } }
void s2(int c) { if (0) { while (c) { if (c > 1) break; g(0); } } }
void s3(int c) { if (0) { for (;;) { if (c) continue; g(0); } } }
void s4(int c) { switch (c) { case 0: if (0) { switch (c) { case 1: g(0); } } } }

// Our GCC's GCC-994 shapes: the substatement never selected, entered at a
// label, falls through to what follows the if statement.  This skip covers
// only the substatements of an if statement, never what follows one that
// cannot complete (DECISIONS.md E201 (b) in notadragon_wg21), so the calls
// after the if are analyzed: b1..b6, and s5, s6, which are never reached.
void b1(int c) { if (c) goto L; if (1) return; else { L:; } g(0); }
void b2(int c) { if (c) goto L; if (0) { L:; } else return; g(0); }
void b3(int c) { switch (c) { case 0: if (1) return; else { case 1:; } g(0); } }
void b4(int c) { if (c) goto L; if (1) { if (1) return; else { L:; } } g(0); }
void b5(int c) { if (c) goto L; if (0) { L:; } else throw 1; g(0); }
template <int N> void b6(int c) { if (c) goto L; if (N) return; else { L:; } g(0); }
template void b6<1>(int);
void s5(int c) { if (1) return; else { switch (c) { case 1:; } } g(0); }
void s6(int c) { if (1) return; else {} g(0); }
