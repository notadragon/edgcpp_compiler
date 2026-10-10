//remark: imported from clang:ctor-contracts-no-ctor-aliases.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontract-evaluation-semantic=observe -Xclang -mno-constructor-aliases %libcxx_flags -o %t && %t

// Each assertion is evaluated once for a constructor's contracts, with
// constructor aliases off (the complete variant delegates to the base one):
// only the variant that runs the user-written body evaluates the contracts.
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/dtor-contracts-per-variant.C

static int pres = 0, posts = 0;
bool pre_hit() { ++pres; return true; }
bool post_hit() { ++posts; return true; }

struct S {
  S() pre(pre_hit()) post(post_hit()) {}
};

int main() {
  { S s; }
  if (pres != 1 || posts != 1)
    __builtin_abort();
}

// REQUIRES: contracts-libcxx, native
