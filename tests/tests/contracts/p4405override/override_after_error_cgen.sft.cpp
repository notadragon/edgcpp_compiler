//remark:contracts: P4405 virtual functions with the C-generating back end: override obligations are diagnosed at the end of the translation unit, after an error as well (our GCC's rule since GCC-947): with prove, an unproven precondition at a call is an error and the override obligation another; with diagnose both are warnings
//require:DO_IL_LOWERING 1
//type:fp
//options:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=diagnose:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove;fn
//match_regex:line ([0-9]+): (error|warning): contract assertion cannot be proven.*
// (No system headers: the C-generating back end cannot find them natively.)
// Under the first options two warnings (line 15 at the call, line 14 for
// the override obligation); under the second two errors, the same: the
// error at the call does not hide the obligation of D::f, which would have
// been put out.
int g(int x) pre (x > 0);
struct B { virtual int f(int x) post (r: r > 0); };
struct D : B { int f(int x) override; };
int D::f(int x)
{ return g(x); }
