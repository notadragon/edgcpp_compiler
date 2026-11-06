//remark: imported from gcc:p4298-flag.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4298 --contract_evaluation_semantic=noexcept_enforce
// P4298: noexcept_enforce/noexcept_observe are only valid evaluation
// semantics when -fcontracts-p4298 is enabled.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4298 -fcontract-evaluation-semantic=noexcept_enforce" }
int f(int x) pre(x > 0) { return x; }
int main() { return f(1); }
