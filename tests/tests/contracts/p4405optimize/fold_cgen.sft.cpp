//remark:contracts: P4405 with the C-generating back end, whose only proving strategy is folding: folding is on at every proof optimization level, so --contracts_prove_optimize=0 to 3 and follow give the same diagnostics (an override obligation folded to true is settled, another is not); --no_contracts_prove_fold still turns folding off
//require:DO_IL_LOWERING 1
//type:fn
//options:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --contracts_prove_optimize=0:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --contracts_prove_optimize=1:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --contracts_prove_optimize=2:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --contracts_prove_optimize=3:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --contracts_prove_optimize=follow:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove --contracts_prove_optimize=3 --no_contracts_prove_fold
//match_regex:line ([0-9]+): (error|warning): contract assertion cannot be proven.*
// (No system headers: the C-generating back end cannot find them natively.)
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/p4405-prove-optimize-0.C and
// p4405-prove-optimize-no-fold.C (gnu_gcc).  Under the first five options
// an error for D::f only (B::f's postcondition); under the last also for
// D::g, whose obligation (B::g's postcondition) folds to true.
struct B {
  virtual int f(int x) post (r: r > 0);
  virtual int g(int x) post (r: sizeof(int) > 1);
};
struct D : B {
  int f(int x) override;
  int g(int x) override;
};
int D::f(int x) { return x + 1; }       // B::f's post: not folded.
int D::g(int x) { return x; }           // B::g's post: folded.
