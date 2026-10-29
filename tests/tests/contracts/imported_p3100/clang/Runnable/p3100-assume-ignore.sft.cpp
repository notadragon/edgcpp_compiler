//remark: imported from clang:Runnable/p3100-assume-ignore.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3100 --contract_evaluation_semantic=assume
//use_system_includes: true
//linker_options: -lcontracts
// P3100: selecting the "assume" semantic without -fcontracts-allow-assume
// resolves to "ignore" -- the predicate is not evaluated and no violation is
// reported, so a failing precondition is a no-op.
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3100 -fcontract-evaluation-semantic=assume %libcxx_flags -o %t
// RUN: %t

int side_effect = 0;

bool check(int i) { ++side_effect; return i > 0; }

int f(int i) pre(check(i)) { return i; }

int main() {
  f(-1);              // would violate if enforced
  return side_effect; // "ignore": predicate not evaluated -> 0 -> exit success
}
