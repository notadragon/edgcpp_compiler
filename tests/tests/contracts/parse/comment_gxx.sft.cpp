//remark:contracts: through edg-gxx the comment of a contract is g++'s printed expression, also for a predicate with macros or blank lines
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=observe
#include <contracts>
#include <cstdio>
// Mirror of our GCC's g++.dg/contracts/cpp26/contract-comment-macro.C and
// contract-comment-blank-line.C, and of Clang's
// clang/test/Contracts/contract-comment-{macro,blank-line}.cpp, for a few
// of their shapes.  cpfe-cp puts the assertions out as written for g++ to
// check, but the generated C++ does not keep the original's columns, so
// edg_gxx.sh passes -fcontract-comment-from-expression: every comment is
// g++'s printed expression of the regenerated predicate, not the front
// end's (comment_blank_line and comment_macro pin those) nor the source
// text GCC and Clang give.
#define NEG(a) ((a) < 0)
#define MY_PRE(c) pre(c)
#define CHK(c) contract_assert((c) > 0)

void handle_contract_violation(const std::contracts::contract_violation& v) {
  std::printf("%s\n", v.comment());
}

int begins_in_fn(int x) pre(NEG(x) && x > 9) { return x; }
int argument(int x) MY_PRE(x < 0) { return x; }
int pre_spaces(int x) pre(x
  
  > 0) { return x; }

int main() {
  begins_in_fn(5);
  argument(5);
  pre_spaces(-1);
  int x = 5;
  CHK(-x);
}
