//remark:contracts: P4405 with the C-generating back end: an erroneous contract assertion -- its predicate has an error, or it is a postcondition that odr-uses a value parameter that is not const -- has had its error, and has no analysis evaluation (no "cannot be proven" at the assertion, at a call or at an address taken), and an erroneous postcondition is not compared with a redeclaration's, as in our GCC (GCC-938)
//require:DO_IL_LOWERING 1
//type:fn
//options:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove
//match_regex:line ([0-9]+): error: .*
// (No system headers: the C-generating back end cannot find them natively.)
// One error each at lines 10, 11, 13, 14, 16 and 17, nothing more (EDG-145:
// no "cannot be proven" for f's, g's at both uses, k's or m's, and no
// "mismatched contract condition" for m's redeclaration).
int f(int x) post (r: r > y) { return 1; }
int g(int x) pre (x > nope) { return 1; }
int h(int x) { return g(x) + (&g)(x); }
int k(int x) post (r: r > x) { return x; }
int m(int x) post (r: r > x);
int m(const int x) post (r: r > x) { return x; }
struct B { virtual int f(int x) post (r: r > z); };
struct D : B { int f(int x) override pre (x > w); };
int D::f(int x) { return x; }
